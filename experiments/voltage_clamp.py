#!/usr/bin/env python3

# software and package versions used:
# Python version: 3.12.2
# NEURON version: NEURON -- VERSION 9.0.1

import os
import sys
import json
import numpy as np
import matplotlib.pyplot as plt
from neuron import h

from cells import WTCell, Cav12_50Cell, WT_BK_SPLIT, CAV12_50_BK_SPLIT

h.load_file("stdrun.hoc")

# labels and colour scheme
WT_LABEL = "WT"
CAV12_50_LABEL = "Cav1.2 50%"
WT_COLOR = "black"
CAV12_50_COLOR = "#ffa6b2"  # "#e16173"

print("Python version:", sys.version)
print("NEURON version:", h.nrnversion())

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_DIR = os.path.join(ROOT_DIR, "outputs")
FIG_DIR = os.path.join(OUT_DIR, "vc_figures")
os.makedirs(FIG_DIR, exist_ok=True)


def save_run_report(path, meta: dict):
    def _json_safe(x):
        if isinstance(x, set):
            return sorted(list(x))
        if isinstance(x, (list, tuple)):
            return [_json_safe(v) for v in x]
        if isinstance(x, dict):
            return {str(k): _json_safe(v) for k, v in x.items()}
        if isinstance(x, (str, int, float, bool)) or x is None:
            return x
        try:
            return float(x)
        except Exception:
            return str(x)

    with open(path, "w") as f:
        json.dump(_json_safe(meta), f, indent=2)


def peak_abs(x):
    return None if x is None else float(np.max(np.abs(x)))


def step_metrics(t, x, step_on=100.0, step_off=400.0, plateau_guard=20.0):
    if x is None:
        return {"peak_abs": None, "mean_abs_plateau": None, "auc_abs_step": None}

    t = np.asarray(t)
    x = np.asarray(x)

    w_step = (t >= step_on) & (t <= step_off)
    w_plat = (t >= step_on + plateau_guard) & (t <= step_off - plateau_guard)

    return {
        "peak_abs": float(np.max(np.abs(x[w_step]))) if np.any(w_step) else None,
        "mean_abs_plateau": float(np.mean(np.abs(x[w_plat]))) if np.any(w_plat) else None,
        "auc_abs_step": float(np.trapz(np.abs(x[w_step]), t[w_step])) if np.any(w_step) else None,
    }


def fmt_metrics(m):
    return (
        f"peak| |={m['peak_abs']:.3e}, mean| |plat={m['mean_abs_plateau']:.3e}, AUC| |={m['auc_abs_step']:.3e}"
        if m["peak_abs"] is not None else "None"
    )


def savefig(name: str):
    plt.savefig(os.path.join(FIG_DIR, name), dpi=300, bbox_inches="tight")


def run_sim(cell, tstop=500.0, v_init=-70.0, dt=0.025):
    h.dt = dt
    h.tstop = tstop
    h.finitialize(v_init)
    h.frecord_init()
    h.continuerun(tstop)

    def arr(v):
        return np.array(v) if v is not None else None

    return (
        np.array(cell.t_vec),
        np.array(cell.vsoma_vec),
        arr(getattr(cell, "vais_vec", None)),
        np.array(cell.vprox_vec),
        np.array(cell.vdend_vec),
        np.array(cell.vspine_vec),

        arr(getattr(cell, "cai_soma_vec", None)),
        arr(getattr(cell, "cai_prox_vec", None)),
        arr(getattr(cell, "cai_dist_vec", None)),
        arr(getattr(cell, "cai_spine_vec", None)),

        arr(getattr(cell, "ica_soma_vec", None)),
        arr(getattr(cell, "ik_soma_vec", None)),
        arr(getattr(cell, "bk_Cav22_ik_soma_vec", None)),
        arr(getattr(cell, "bk_Cav12_ik_soma_vec", None)),
        arr(getattr(cell, "bk_Cav21_ik_soma_vec", None)),
        arr(getattr(cell, "sk_ik_soma_vec", None)),
        arr(getattr(cell, "ina_soma_vec", None)),
        arr(getattr(cell, "cav21_ica_soma_vec", None)),
        arr(getattr(cell, "cav22_ica_soma_vec", None)),
        arr(getattr(cell, "cav12_ica_soma_vec", None)),
        arr(getattr(cell, "cav13_ica_soma_vec", None)),
        arr(getattr(cell, "clamp_i_vec", None)),

        # total Ca by compartment
        arr(getattr(cell, "ica_ais_vec", None)),
        arr(getattr(cell, "ica_prox_vec", None)),
        arr(getattr(cell, "ica_dist_vec", None)),
        arr(getattr(cell, "ica_spine_vec", None)),

        # Cav1.2 by compartment
        arr(getattr(cell, "cav12_ica_ais_vec", None)),
        arr(getattr(cell, "cav12_ica_prox_vec", None)),
        arr(getattr(cell, "cav12_ica_dist_vec", None)),
        arr(getattr(cell, "cav12_ica_spine_vec", None)),

        # Cav1.3 by compartment
        arr(getattr(cell, "cav13_ica_ais_vec", None)),
        arr(getattr(cell, "cav13_ica_prox_vec", None)),
        arr(getattr(cell, "cav13_ica_dist_vec", None)),
        arr(getattr(cell, "cav13_ica_spine_vec", None)),

        # Cav2.1 by compartment
        arr(getattr(cell, "cav21_ica_ais_vec", None)),
        arr(getattr(cell, "cav21_ica_prox_vec", None)),
        arr(getattr(cell, "cav21_ica_dist_vec", None)),
        arr(getattr(cell, "cav21_ica_spine_vec", None)),

        # Cav2.2 by compartment
        arr(getattr(cell, "cav22_ica_ais_vec", None)),
        arr(getattr(cell, "cav22_ica_prox_vec", None)),
        arr(getattr(cell, "cav22_ica_dist_vec", None)),
        arr(getattr(cell, "cav22_ica_spine_vec", None)),
    )


def run_iv_curve(cell_class, steps, hold=-70.0, delay=100.0, dur=200.0, tstop=380.0):
    peak_current = []
    steady_current = []
    peak_ica = []
    peak_bk12 = []
    peak_bk21 = []
    peak_bk22 = []

    for vstep in steps:
        cell = cell_class()
        cell.add_voltage_clamp(hold=hold, step=float(vstep), delay=delay, dur=dur)
        cell.setup_recording()

        (
            t, vs, vais, vp, vd, vsp,
            cai_soma, cai_prox, cai_dist, cai_spine,
            ica_soma, ik_soma, bk_Cav22ik_soma, bk_Cav12ik_soma, bk_Cav21ik_soma,
            skik_soma, ina_soma, cav21_ica_soma, cav22_ica_soma, cav12_ica_soma,
            cav13_ica_soma, I,
            ica_ais, ica_prox, ica_dist, ica_spine,
            cav12_ica_ais, cav12_ica_prox, cav12_ica_dist, cav12_ica_spine,
            cav13_ica_ais, cav13_ica_prox, cav13_ica_dist, cav13_ica_spine,
            cav21_ica_ais, cav21_ica_prox, cav21_ica_dist, cav21_ica_spine,
            cav22_ica_ais, cav22_ica_prox, cav22_ica_dist, cav22_ica_spine
        ) = run_sim(cell, tstop=tstop, v_init=hold, dt=0.025)

        if I is None:
            raise RuntimeError("Voltage clamp current not recorded.")

        w_peak = (t >= delay) & (t <= delay + 10)
        w_ss = (t >= delay + dur - 10) & (t <= delay + dur)

        peak_current.append(float(np.min(I[w_peak])))
        steady_current.append(float(np.mean(I[w_ss])))
        peak_ica.append(float(np.min(ica_soma[w_peak])) if ica_soma is not None else np.nan)
        peak_bk12.append(float(np.max(np.abs(bk_Cav12ik_soma[w_peak]))) if bk_Cav12ik_soma is not None else np.nan)
        peak_bk21.append(float(np.max(np.abs(bk_Cav21ik_soma[w_peak]))) if bk_Cav21ik_soma is not None else np.nan)
        peak_bk22.append(float(np.max(np.abs(bk_Cav22ik_soma[w_peak]))) if bk_Cav22ik_soma is not None else np.nan)

    return {
        "steps": np.array(steps),
        "peak_current": np.array(peak_current),
        "steady_current": np.array(steady_current),
        "peak_ica": np.array(peak_ica),
        "peak_bk12": np.array(peak_bk12),
        "peak_bk21": np.array(peak_bk21),
        "peak_bk22": np.array(peak_bk22),
    }


def main():
    h.celsius = 34.0

    cell = WTCell()
    cell.add_voltage_clamp(hold=-70.0, step=-50.0, delay=100.0, dur=300.0)
    cell.setup_recording()
    (
        t0, vs0, vais0, vp0, vd0, vsp0,
        cai0_soma, cai0_prox, cai0_dist, cai0_spine,
        ica0_soma, ik0_soma, bk_Cav22ik0_soma, bk_Cav12ik0_soma, bk_Cav21ik0_soma,
        skik0_soma, ina0_soma, cav21_ica0_soma, cav22_ica0_soma, cav12_ica0_soma,
        cav13_ica0_soma, I0,
        ica0_ais, ica0_prox, ica0_dist, ica0_spine,
        cav12_ica0_ais, cav12_ica0_prox, cav12_ica0_dist, cav12_ica0_spine,
        cav13_ica0_ais, cav13_ica0_prox, cav13_ica0_dist, cav13_ica0_spine,
        cav21_ica0_ais, cav21_ica0_prox, cav21_ica0_dist, cav21_ica0_spine,
        cav22_ica0_ais, cav22_ica0_prox, cav22_ica0_dist, cav22_ica0_spine
    ) = run_sim(cell, tstop=500.0, v_init=-70.0, dt=0.025)

    cell2 = Cav12_50Cell()
    cell2.add_voltage_clamp(hold=-70.0, step=-50.0, delay=100.0, dur=300.0)
    cell2.setup_recording()
    (
        t1, vs1, vais1, vp1, vd1, vsp1,
        cai1_soma, cai1_prox, cai1_dist, cai1_spine,
        ica1_soma, ik1_soma, bk_Cav22ik1_soma, bk_Cav12ik1_soma, bk_Cav21ik1_soma,
        skik1_soma, ina1_soma, cav21_ica1_soma, cav22_ica1_soma, cav12_ica1_soma,
        cav13_ica1_soma, I1,
        ica1_ais, ica1_prox, ica1_dist, ica1_spine,
        cav12_ica1_ais, cav12_ica1_prox, cav12_ica1_dist, cav12_ica1_spine,
        cav13_ica1_ais, cav13_ica1_prox, cav13_ica1_dist, cav13_ica1_spine,
        cav21_ica1_ais, cav21_ica1_prox, cav21_ica1_dist, cav21_ica1_spine,
        cav22_ica1_ais, cav22_ica1_prox, cav22_ica1_dist, cav22_ica1_spine
    ) = run_sim(cell2, tstop=500.0, v_init=-70.0, dt=0.025)

    run_meta = {
        "python_version": sys.version,
        "neuron_version": h.nrnversion(),
        "dt_ms": float(h.dt),
        "tstop_ms": 500.0,
        "v_init_mV": -70.0,
        "celsius_C": float(h.celsius),
        "morphology": {
            "soma": {"L": float(cell.soma.L), "diam": float(cell.soma.diam), "nseg": int(cell.soma.nseg)},
            "dend_prox": {"L": float(cell.dend_prox.L), "diam": float(cell.dend_prox.diam), "nseg": int(cell.dend_prox.nseg)},
            "dend_dist": {"L": float(cell.dend_dist.L), "diam": float(cell.dend_dist.diam), "nseg": int(cell.dend_dist.nseg)},
            "axon": {"L": float(cell.axon.L), "diam": float(cell.axon.diam), "nseg": int(cell.axon.nseg)},
        },
        "Ra_ohmcm": float(cell.soma.Ra),
        "cm_uFcm2": float(cell.soma.cm),
        "soma_psection": str(cell.soma.psection()),
    }
    save_run_report(os.path.join(OUT_DIR, "vc_run_report_WT.json"), run_meta)

    run_meta_50 = dict(run_meta)
    run_meta_50["model"] = "Cav12_50"
    save_run_report(os.path.join(OUT_DIR, "vc_run_report_cav12_50.json"), run_meta_50)

    print("\n--- CURRENT DIAGNOSTICS (soma, peak |current|) --")
    for label, ica, ik, bk22, bk12, bk21, sk, ina, cav21, cav22, cav12, cav13 in [
        (WT_LABEL, ica0_soma, ik0_soma, bk_Cav22ik0_soma, bk_Cav12ik0_soma, bk_Cav21ik0_soma, skik0_soma, ina0_soma, cav21_ica0_soma, cav22_ica0_soma, cav12_ica0_soma, cav13_ica0_soma),
        (CAV12_50_LABEL, ica1_soma, ik1_soma, bk_Cav22ik1_soma, bk_Cav12ik1_soma, bk_Cav21ik1_soma, skik1_soma, ina1_soma, cav21_ica1_soma, cav22_ica1_soma, cav12_ica1_soma, cav13_ica1_soma),
    ]:
        print(f"{label}: ica_soma =", peak_abs(ica), "mA/cm2")
        print(f"{label}: ik_soma  =", peak_abs(ik), "mA/cm2")
        print(f"{label}: BK_Cav22_ik =", peak_abs(bk22), "mA/cm2")
        print(f"{label}: BK_Cav12_ik =", peak_abs(bk12), "mA/cm2")
        print(f"{label}: BK_Cav21_ik =", peak_abs(bk21), "mA/cm2")
        print(f"{label}: SK_ik =", peak_abs(sk), "mA/cm2")
        print(f"{label}: ina_soma =", peak_abs(ina), "mA/cm2")
        print(f"{label}: Cav2.1 source current =", peak_abs(cav21), "mA/cm2")
        print(f"{label}: Cav22 source current =", peak_abs(cav22), "mA/cm2")
        print(f"{label}: Cav1.2 source current =", peak_abs(cav12), "mA/cm2")
        print(f"{label}: Cav1.3 source current =", peak_abs(cav13), "mA/cm2")

    step_on = 100.0
    step_off = 400.0
    print("\n--- STEP METRICS (VC, 100–400 ms) ---")
    for name, wt, het in [
        ("Clamp current", step_metrics(t0, I0, step_on, step_off), step_metrics(t1, I1, step_on, step_off)),
        ("Total Ca current", step_metrics(t0, ica0_soma, step_on, step_off), step_metrics(t1, ica1_soma, step_on, step_off)),
        ("BK_Cav22", step_metrics(t0, bk_Cav22ik0_soma, step_on, step_off), step_metrics(t1, bk_Cav22ik1_soma, step_on, step_off)),
        ("BK_Cav12", step_metrics(t0, bk_Cav12ik0_soma, step_on, step_off), step_metrics(t1, bk_Cav12ik1_soma, step_on, step_off)),
        ("BK_Cav21", step_metrics(t0, bk_Cav21ik0_soma, step_on, step_off), step_metrics(t1, bk_Cav21ik1_soma, step_on, step_off)),
        ("SK", step_metrics(t0, skik0_soma, step_on, step_off), step_metrics(t1, skik1_soma, step_on, step_off)),
        ("Cav1.3", step_metrics(t0, cav13_ica0_soma, step_on, step_off), step_metrics(t1, cav13_ica1_soma, step_on, step_off)),
    ]:
        print(f"WT {name}: {fmt_metrics(wt)}")
        print(f"{CAV12_50_LABEL} {name}: {fmt_metrics(het)}")

    plt.figure()
    plt.plot(t0, I0, color=WT_COLOR, label=WT_LABEL)
    plt.plot(t1, I1, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
    plt.xlabel("Time (ms)")
    plt.ylabel("Clamp current (nA)")
    plt.title("Voltage-clamp current")
    plt.legend()
    plt.tight_layout()
    savefig("vc_clamp_current_WT_vs_Cav12_50.png")
    plt.show()

    plt.figure()
    plt.plot(t0, vs0, color=WT_COLOR, label=WT_LABEL)
    plt.plot(t1, vs1, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("Soma Vm during voltage clamp")
    plt.legend()
    plt.tight_layout()
    savefig("vc_soma_vm_WT_vs_Cav12_50.png")
    plt.show()

    for title, y0, y1, fname in [
        ("Soma intracellular Ca", cai0_soma, cai1_soma, "vc_cai_soma_WT_vs_Cav12_50.png"),
        ("Proximal dendrite intracellular Ca", cai0_prox, cai1_prox, "vc_cai_prox_WT_vs_Cav12_50.png"),
        ("Distal dendrite intracellular Ca", cai0_dist, cai1_dist, "vc_cai_dist_WT_vs_Cav12_50.png"),
        ("Spine intracellular Ca", cai0_spine, cai1_spine, "vc_cai_spine_WT_vs_Cav12_50.png"),
    ]:
        if y0 is not None and y1 is not None:
            plt.figure()
            plt.plot(t0, y0, color=WT_COLOR, label=WT_LABEL)
            plt.plot(t1, y1, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
            plt.xlabel("Time (ms)")
            plt.ylabel("cai (mM)")
            plt.title(title)
            plt.legend()
            plt.tight_layout()
            savefig(fname)
            plt.show()

    plot_specs = [
        (ica0_soma, ica1_soma, "Total calcium current", "Current density (mA/cm2)", "vc_total_ca_current_WT_vs_Cav12_50.png"),
        (ik0_soma, ik1_soma, "Total potassium current", "Current density (mA/cm2)", "vc_total_k_current_WT_vs_Cav12_50.png"),
        (bk_Cav22ik0_soma, bk_Cav22ik1_soma, "BK_Cav22 current", "Current density (mA/cm2)", "vc_BK_Cav22_current_WT_vs_Cav12_50.png"),
        (bk_Cav12ik0_soma, bk_Cav12ik1_soma, "BK_Cav12 current", "Current density (mA/cm2)", "vc_BK_Cav12_current_WT_vs_Cav12_50.png"),
        (bk_Cav21ik0_soma, bk_Cav21ik1_soma, "BK_Cav21 current", "Current density (mA/cm2)", "vc_BK_Cav21_current_WT_vs_Cav12_50.png"),
        (skik0_soma, skik1_soma, "SK current", "Current density (mA/cm2)", "vc_SK_current_WT_vs_Cav12_50.png"),
        (cav12_ica0_soma, cav12_ica1_soma, "Cav1.2 source current", "Current density (mA/cm2)", "vc_Cav12_source_current_WT_vs_Cav12_50.png"),
        (cav13_ica0_soma, cav13_ica1_soma, "Cav1.3 source current", "Current density (mA/cm2)", "vc_Cav13_source_current_WT_vs_Cav12_50.png"),
        (cav21_ica0_soma, cav21_ica1_soma, "Cav2.1 source current", "Current density (mA/cm2)", "vc_Cav21_source_current_WT_vs_Cav12_50.png"),
        (cav22_ica0_soma, cav22_ica1_soma, "Cav22 source current", "Current density (mA/cm2)", "vc_Cav22_source_current_WT_vs_Cav12_50.png"),
    ]

    for y0, y1, title, ylabel, fname in plot_specs:
        if y0 is not None and y1 is not None:
            plt.figure()
            plt.plot(t0, y0, color=WT_COLOR, label=WT_LABEL)
            plt.plot(t1, y1, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
            plt.xlabel("Time (ms)")
            plt.ylabel(ylabel)
            plt.title(title)
            plt.legend()
            plt.tight_layout()
            savefig(fname)
            plt.show()

    steps = np.arange(-90, 11, 10)
    iv_wt = run_iv_curve(WTCell, steps=steps)
    iv_50 = run_iv_curve(Cav12_50Cell, steps=steps)

    for ykey, ylabel, title, fname in [
        ("peak_current", "Peak clamp current (nA)", "Voltage-clamp I–V: peak current", "vc_iv_peak_current.png"),
        ("steady_current", "Steady-state clamp current (nA)", "Voltage-clamp I–V: steady-state current", "vc_iv_steady_current.png"),
        ("peak_ica", "Peak total Ca current (mA/cm2)", "Voltage-clamp I–V: peak Ca current", "vc_iv_peak_ca_current.png"),
        ("peak_bk12", "Peak BK_Cav12 current (mA/cm2)", "Voltage-clamp I–V: peak BK_Cav12", "vc_iv_peak_BK_Cav12.png"),
        ("peak_bk21", "Peak BK_Cav21 current (mA/cm2)", "Voltage-clamp I–V: peak BK_Cav21", "vc_iv_peak_BK_Cav21.png"),
        ("peak_bk22", "Peak BK_Cav22 current (mA/cm2)", "Voltage-clamp I–V: peak BK_Cav22", "vc_iv_peak_BK_Cav22.png"),
    ]:
        plt.figure()
        plt.plot(iv_wt["steps"], iv_wt[ykey], marker="o", color=WT_COLOR, label=WT_LABEL)
        plt.plot(iv_50["steps"], iv_50[ykey], marker="o", color=CAV12_50_COLOR, label=CAV12_50_LABEL)
        plt.xlabel("Command voltage (mV)")
        plt.ylabel(ylabel)
        plt.title(title)
        plt.legend()
        plt.tight_layout()
        savefig(fname)
        plt.show()

    dvdt0 = np.gradient(vs0, t0)
    dvdt1 = np.gradient(vs1, t1)

    plt.figure()
    plt.plot(vs0, dvdt0, color=WT_COLOR, label=WT_LABEL)
    plt.plot(vs1, dvdt1, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
    plt.xlabel("V (mV)")
    plt.ylabel("dV/dt (mV/ms)")
    plt.title("Phase plane soma")
    plt.legend()
    plt.tight_layout()
    savefig("vc_phase_plane_soma_WT_vs_Cav12_50.png")
    plt.show()

    # 6-panel voltage clamp Ca current fig
    fig, axes = plt.subplots(2, 3, figsize=(15, 9), sharex=True, sharey=False)

    ax = axes[0, 0]
    ax.plot(t0, ica0_soma, color=WT_COLOR, label=WT_LABEL)
    ax.plot(t1, ica1_soma, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
    ax.set_ylabel("Current density (mA/cm2)")

    ax = axes[0, 1]
    ax.plot(t0, cav12_ica0_soma, color=WT_COLOR)
    ax.plot(t1, cav12_ica1_soma, color=CAV12_50_COLOR)

    ax = axes[0, 2]
    ax.plot(t0, cav13_ica0_soma, color=WT_COLOR)
    ax.plot(t1, cav13_ica1_soma, color=CAV12_50_COLOR)

    ax = axes[1, 0]
    ax.plot(t0, cav21_ica0_soma, color=WT_COLOR)
    ax.plot(t1, cav21_ica1_soma, color=CAV12_50_COLOR)
    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Current density (mA/cm2)")

    ax = axes[1, 1]
    ax.plot(t0, cav22_ica0_soma, color=WT_COLOR)
    ax.plot(t1, cav22_ica1_soma, color=CAV12_50_COLOR)
    ax.set_xlabel("Time (ms)")

    ax = axes[1, 2]
    if all(x is not None for x in [ica0_soma, cav12_ica0_soma, cav13_ica0_soma, cav21_ica0_soma, cav22_ica0_soma]):
        cav32_est0 = ica0_soma - cav12_ica0_soma - cav13_ica0_soma - cav21_ica0_soma - cav22_ica0_soma
        ax.plot(t0, cav32_est0, color=WT_COLOR)
    if all(x is not None for x in [ica1_soma, cav12_ica1_soma, cav13_ica1_soma, cav21_ica1_soma, cav22_ica1_soma]):
        cav32_est1 = ica1_soma - cav12_ica1_soma - cav13_ica1_soma - cav21_ica1_soma - cav22_ica1_soma
        ax.plot(t1, cav32_est1, color=CAV12_50_COLOR)
    ax.set_xlabel("Time (ms)")

    handles, labels = axes[0, 0].get_legend_handles_labels()
    fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

    fig.tight_layout(rect=[0, 0.05, 1, 1])
    plt.savefig(
        os.path.join(FIG_DIR, "vc_6panel_total_Cav12_Cav13_Cav21_Cav22_Cav32est_WT_vs_Cav12_50.png"),
        dpi=300,
        bbox_inches="tight"
    )
    plt.show()

    # 4-panel total Ca current by compartment
    for panel_data, fname in [
        (
            (ica0_ais, ica1_ais, ica0_prox, ica1_prox, ica0_dist, ica1_dist, ica0_spine, ica1_spine),
            "vc_totalCa_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"
        ),
        (
            (cav12_ica0_ais, cav12_ica1_ais, cav12_ica0_prox, cav12_ica1_prox, cav12_ica0_dist, cav12_ica1_dist, cav12_ica0_spine, cav12_ica1_spine),
            "vc_Cav12_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"
        ),
        (
            (cav13_ica0_ais, cav13_ica1_ais, cav13_ica0_prox, cav13_ica1_prox, cav13_ica0_dist, cav13_ica1_dist, cav13_ica0_spine, cav13_ica1_spine),
            "vc_Cav13_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"
        ),
        (
            (cav21_ica0_ais, cav21_ica1_ais, cav21_ica0_prox, cav21_ica1_prox, cav21_ica0_dist, cav21_ica1_dist, cav21_ica0_spine, cav21_ica1_spine),
            "vc_Cav21_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"
        ),
        (
            (cav22_ica0_ais, cav22_ica1_ais, cav22_ica0_prox, cav22_ica1_prox, cav22_ica0_dist, cav22_ica1_dist, cav22_ica0_spine, cav22_ica1_spine),
            "vc_Cav22_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"
        ),
    ]:
        a0, a1, p0, p1, d0, d1, s0, s1 = panel_data

        fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True, sharey=False)

        ax = axes[0, 0]
        ax.plot(t0, a0, color=WT_COLOR, label=WT_LABEL)
        ax.plot(t1, a1, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
        ax.set_ylabel("Current density (mA/cm2)")

        ax = axes[0, 1]
        ax.plot(t0, p0, color=WT_COLOR)
        ax.plot(t1, p1, color=CAV12_50_COLOR)

        ax = axes[1, 0]
        ax.plot(t0, d0, color=WT_COLOR)
        ax.plot(t1, d1, color=CAV12_50_COLOR)
        ax.set_xlabel("Time (ms)")
        ax.set_ylabel("Current density (mA/cm2)")

        ax = axes[1, 1]
        ax.plot(t0, s0, color=WT_COLOR)
        ax.plot(t1, s1, color=CAV12_50_COLOR)
        ax.set_xlabel("Time (ms)")

        handles, labels = axes[0, 0].get_legend_handles_labels()
        fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

        fig.tight_layout(rect=[0, 0.05, 1, 1])
        plt.savefig(os.path.join(FIG_DIR, fname), dpi=300, bbox_inches="tight")
        plt.show()


if __name__ == "__main__":
    main()