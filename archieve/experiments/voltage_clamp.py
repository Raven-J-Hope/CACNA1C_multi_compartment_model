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

h.load_file("stdrun.hoc")

print("Python version:", sys.version)
print("NEURON version:", h.nrnversion())

# make and set dir & paths
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_DIR = os.path.join(BASE_DIR, "outputs")
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


def savefig(name: str):
    plt.savefig(os.path.join(FIG_DIR, name), dpi=300, bbox_inches="tight")


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


def run_sim(cell, tstop=500.0, v_init=-70.0, dt=0.025):
    h.dt = dt
    h.tstop = tstop
    h.finitialize(v_init)
    h.frecord_init()
    h.continuerun(tstop)

    def arr(v):
        return np.array(v) if v is not None else None

    return {
        "t": np.array(cell.t_vec),
        "vs": np.array(cell.vsoma_vec),
        "vais": arr(getattr(cell, "vais_vec", None)),
        "vp": np.array(cell.vprox_vec),
        "vd": np.array(cell.vdend_vec),
        "vsp": np.array(cell.vspine_vec),

        "cai_soma": arr(getattr(cell, "cai_soma_vec", None)),
        "cai_prox": arr(getattr(cell, "cai_prox_vec", None)),
        "cai_dist": arr(getattr(cell, "cai_dist_vec", None)),
        "cai_spine": arr(getattr(cell, "cai_spine_vec", None)),

        "ica_soma": arr(getattr(cell, "ica_soma_vec", None)),
        "ik_soma": arr(getattr(cell, "ik_soma_vec", None)),
        "ina_soma": arr(getattr(cell, "ina_soma_vec", None)),
        "bk_Cav22_ik_soma": arr(getattr(cell, "bk_Cav22_ik_soma_vec", None)),
        "bk_Cav12_ik_soma": arr(getattr(cell, "bk_Cav12_ik_soma_vec", None)),
        "bk_Cav21_ik_soma": arr(getattr(cell, "bk_Cav21_ik_soma_vec", None)),
        "sk_ik_soma": arr(getattr(cell, "sk_ik_soma_vec", None)),
        "cav21_ica_soma": arr(getattr(cell, "cav21_ica_soma_vec", None)),
        "cav22_ica_soma": arr(getattr(cell, "cav22_ica_soma_vec", None)),
        "cav12_ica_soma": arr(getattr(cell, "cav12_ica_soma_vec", None)),
        "cav13_ica_soma": arr(getattr(cell, "cav13_ica_soma_vec", None)),
        "clamp_i": arr(getattr(cell, "clamp_i_vec", None)),

        "ica_ais": arr(getattr(cell, "ica_ais_vec", None)),
        "ica_prox": arr(getattr(cell, "ica_prox_vec", None)),
        "ica_dist": arr(getattr(cell, "ica_dist_vec", None)),
        "ica_spine": arr(getattr(cell, "ica_spine_vec", None)),

        "cav12_ica_ais": arr(getattr(cell, "cav12_ica_ais_vec", None)),
        "cav12_ica_prox": arr(getattr(cell, "cav12_ica_prox_vec", None)),
        "cav12_ica_dist": arr(getattr(cell, "cav12_ica_dist_vec", None)),
        "cav12_ica_spine": arr(getattr(cell, "cav12_ica_spine_vec", None)),

        "cav13_ica_ais": arr(getattr(cell, "cav13_ica_ais_vec", None)),
        "cav13_ica_prox": arr(getattr(cell, "cav13_ica_prox_vec", None)),
        "cav13_ica_dist": arr(getattr(cell, "cav13_ica_dist_vec", None)),
        "cav13_ica_spine": arr(getattr(cell, "cav13_ica_spine_vec", None)),

        "cav21_ica_ais": arr(getattr(cell, "cav21_ica_ais_vec", None)),
        "cav21_ica_prox": arr(getattr(cell, "cav21_ica_prox_vec", None)),
        "cav21_ica_dist": arr(getattr(cell, "cav21_ica_dist_vec", None)),
        "cav21_ica_spine": arr(getattr(cell, "cav21_ica_spine_vec", None)),

        "cav22_ica_ais": arr(getattr(cell, "cav22_ica_ais_vec", None)),
        "cav22_ica_prox": arr(getattr(cell, "cav22_ica_prox_vec", None)),
        "cav22_ica_dist": arr(getattr(cell, "cav22_ica_dist_vec", None)),
        "cav22_ica_spine": arr(getattr(cell, "cav22_ica_spine_vec", None)),
    }


def run_iv_curve(
    cell_class,
    steps,
    hold=-70.0,
    delay=100.0,
    dur=200.0,
    tstop=380.0,
    dt=0.025,
    channel_overrides=None,
    bk_split_override=None,
):
    peak_current = []
    steady_current = []
    peak_ica = []
    peak_bk12 = []
    peak_bk21 = []
    peak_bk22 = []

    for vstep in steps:
        cell_kwargs = {}
        if channel_overrides is not None:
            cell_kwargs["channel_overrides"] = channel_overrides
        if bk_split_override is not None:
            cell_kwargs["bk_split"] = bk_split_override

        cell = cell_class(**cell_kwargs)
        cell.add_voltage_clamp(hold=hold, step=float(vstep), delay=delay, dur=dur)
        cell.setup_recording()
        data = run_sim(cell, tstop=tstop, v_init=hold, dt=dt)

        I = data["clamp_i"]
        if I is None:
            raise RuntimeError("Voltage clamp current not recorded.")

        t = data["t"]
        w_peak = (t >= delay) & (t <= delay + 10)
        w_ss = (t >= delay + dur - 10) & (t <= delay + dur)

        peak_current.append(float(np.min(I[w_peak])))
        steady_current.append(float(np.mean(I[w_ss])))
        peak_ica.append(float(np.min(data["ica_soma"][w_peak])) if data["ica_soma"] is not None else np.nan)
        peak_bk12.append(float(np.max(np.abs(data["bk_Cav12_ik_soma"][w_peak]))) if data["bk_Cav12_ik_soma"] is not None else np.nan)
        peak_bk21.append(float(np.max(np.abs(data["bk_Cav21_ik_soma"][w_peak]))) if data["bk_Cav21_ik_soma"] is not None else np.nan)
        peak_bk22.append(float(np.max(np.abs(data["bk_Cav22_ik_soma"][w_peak]))) if data["bk_Cav22_ik_soma"] is not None else np.nan)

    return {
        "steps": np.array(steps),
        "peak_current": np.array(peak_current),
        "steady_current": np.array(steady_current),
        "peak_ica": np.array(peak_ica),
        "peak_bk12": np.array(peak_bk12),
        "peak_bk21": np.array(peak_bk21),
        "peak_bk22": np.array(peak_bk22),
    }


def run_voltage_clamp(
    cell_class,
    hold=-70.0,
    step=-50.0,
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
    celsius=34.0,
    channel_overrides=None,
    bk_split_override=None,
    save_report=False,
    report_name="vc_run_report.json",
    make_basic_plots=False,
    plot_prefix="vc",
):
    h.celsius = celsius

    cell_kwargs = {}
    if channel_overrides is not None:
        cell_kwargs["channel_overrides"] = channel_overrides
    if bk_split_override is not None:
        cell_kwargs["bk_split"] = bk_split_override

    cell = cell_class(**cell_kwargs)
    cell.add_voltage_clamp(hold=hold, step=step, delay=delay, dur=dur)
    cell.setup_recording()
    data = run_sim(cell, tstop=tstop, v_init=v_init, dt=dt)

    if save_report:
        run_meta = {
            "python_version": sys.version,
            "neuron_version": h.nrnversion(),
            "dt_ms": float(h.dt),
            "tstop_ms": float(tstop),
            "v_init_mV": float(v_init),
            "celsius_C": float(h.celsius),
            "cell_class": cell_class.__name__,
            "protocol": {
                "type": "voltage_clamp",
                "hold_mV": float(hold),
                "step_mV": float(step),
                "delay_ms": float(delay),
                "dur_ms": float(dur),
            },
            "channel_overrides": channel_overrides,
            "bk_split_override": bk_split_override,
            "morphology": {
                "soma": {"L": float(cell.soma.L), "diam": float(cell.soma.diam), "nseg": int(cell.soma.nseg)},
                "dend_prox": {"L": float(cell.dend_prox.L), "diam": float(cell.dend_prox.diam), "nseg": int(cell.dend_prox.nseg)},
                "dend_dist": {"L": float(cell.dend_dist.L), "diam": float(cell.dend_dist.diam), "nseg": int(cell.dend_dist.nseg)},
                "ais": {"L": float(cell.ais.L), "diam": float(cell.ais.diam), "nseg": int(cell.ais.nseg)},
                "axon": {"L": float(cell.axon.L), "diam": float(cell.axon.diam), "nseg": int(cell.axon.nseg)},
            },
            "Ra_ohmcm": float(cell.soma.Ra),
            "cm_uFcm2": float(cell.soma.cm),
            "soma_psection": str(cell.soma.psection()),
        }
        save_run_report(os.path.join(OUT_DIR, report_name), run_meta)

    print("\n--- CURRENT DIAGNOSTICS (soma, peak |current|) ---")
    print("cell_class =", cell_class.__name__)
    print("ica_soma =", peak_abs(data["ica_soma"]), "mA/cm2")
    print("ik_soma =", peak_abs(data["ik_soma"]), "mA/cm2")
    print("BK_Cav22_ik =", peak_abs(data["bk_Cav22_ik_soma"]), "mA/cm2")
    print("BK_Cav12_ik =", peak_abs(data["bk_Cav12_ik_soma"]), "mA/cm2")
    print("BK_Cav21_ik =", peak_abs(data["bk_Cav21_ik_soma"]), "mA/cm2")
    print("SK_ik =", peak_abs(data["sk_ik_soma"]), "mA/cm2")
    print("ina_soma =", peak_abs(data["ina_soma"]), "mA/cm2")
    print("Cav1.2 source current =", peak_abs(data["cav12_ica_soma"]), "mA/cm2")
    print("Cav1.3 source current =", peak_abs(data["cav13_ica_soma"]), "mA/cm2")
    print("Cav2.1 source current =", peak_abs(data["cav21_ica_soma"]), "mA/cm2")
    print("Cav22 source current =", peak_abs(data["cav22_ica_soma"]), "mA/cm2")
    print("Clamp current =", peak_abs(data["clamp_i"]), "nA")

    print("\n--- STEP METRICS (VC) ---")
    for name, trace in [
        ("Clamp current", data["clamp_i"]),
        ("Total Ca current", data["ica_soma"]),
        ("BK_Cav22", data["bk_Cav22_ik_soma"]),
        ("BK_Cav12", data["bk_Cav12_ik_soma"]),
        ("BK_Cav21", data["bk_Cav21_ik_soma"]),
        ("SK", data["sk_ik_soma"]),
        ("Cav1.3", data["cav13_ica_soma"]),
    ]:
        m = step_metrics(data["t"], trace, step_on=delay, step_off=delay + dur)
        print(name, m)

    if make_basic_plots:
        if data["clamp_i"] is not None:
            plt.figure()
            plt.plot(data["t"], data["clamp_i"])
            plt.xlabel("Time (ms)")
            plt.ylabel("Clamp current (nA)")
            plt.tight_layout()
            savefig(f"{plot_prefix}_clamp_current.png")
            plt.show()

        plt.figure()
        plt.plot(data["t"], data["vs"])
        plt.xlabel("Time (ms)")
        plt.ylabel("Vm (mV)")
        plt.tight_layout()
        savefig(f"{plot_prefix}_soma_vm.png")
        plt.show()

        if data["ica_soma"] is not None:
            plt.figure()
            plt.plot(data["t"], data["ica_soma"])
            plt.xlabel("Time (ms)")
            plt.ylabel("Total calcium current (mA/cm2)")
            plt.tight_layout()
            savefig(f"{plot_prefix}_total_ca_current.png")
            plt.show()

    return data


if __name__ == "__main__":
    from cells import WTCell

    run_voltage_clamp(
        cell_class=WTCell,
        save_report=True,
        report_name="vc_run_report_WT.json",
        make_basic_plots=True,
        plot_prefix="vc_WT",
    )