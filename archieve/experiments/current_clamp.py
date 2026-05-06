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
FIG_DIR = os.path.join(OUT_DIR, "ic_figures")
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


def run_sim(cell, tstop=500.0, v_init=-70.0, dt=0.025):
    h.dt = dt
    h.tstop = tstop
    h.finitialize(v_init)
    h.frecord_init()
    h.continuerun(tstop)

    t = np.array(cell.t_vec)

    vs = np.array(cell.vsoma_vec)
    vais = np.array(cell.vais_vec) if getattr(cell, "vais_vec", None) is not None else None
    vp = np.array(cell.vprox_vec)
    vd = np.array(cell.vdend_vec)
    vsp = np.array(cell.vspine_vec)

    cai_soma = np.array(cell.cai_soma_vec) if getattr(cell, "cai_soma_vec", None) is not None else None
    cai_prox = np.array(cell.cai_prox_vec) if getattr(cell, "cai_prox_vec", None) is not None else None
    cai_dist = np.array(cell.cai_dist_vec) if getattr(cell, "cai_dist_vec", None) is not None else None
    cai_spine = np.array(cell.cai_spine_vec) if getattr(cell, "cai_spine_vec", None) is not None else None
    cai_ais = np.array(cell.cai_ais_vec) if getattr(cell, "cai_ais_vec", None) is not None else None

    ica_soma = np.array(cell.ica_soma_vec) if getattr(cell, "ica_soma_vec", None) is not None else None
    ik_soma = np.array(cell.ik_soma_vec) if getattr(cell, "ik_soma_vec", None) is not None else None
    ina_soma = np.array(cell.ina_soma_vec) if getattr(cell, "ina_soma_vec", None) is not None else None

    bk_Cav22_ik_soma = np.array(cell.bk_Cav22_ik_soma_vec) if getattr(cell, "bk_Cav22_ik_soma_vec", None) is not None else None
    bk_Cav12_ik_soma = np.array(cell.bk_Cav12_ik_soma_vec) if getattr(cell, "bk_Cav12_ik_soma_vec", None) is not None else None
    bk_Cav21_ik_soma = np.array(cell.bk_Cav21_ik_soma_vec) if getattr(cell, "bk_Cav21_ik_soma_vec", None) is not None else None

    bk_total_soma = None
    if (
        bk_Cav22_ik_soma is not None
        and bk_Cav12_ik_soma is not None
        and bk_Cav21_ik_soma is not None
    ):
        bk_total_soma = bk_Cav22_ik_soma + bk_Cav12_ik_soma + bk_Cav21_ik_soma

    sk_ik_soma = np.array(cell.sk_ik_soma_vec) if getattr(cell, "sk_ik_soma_vec", None) is not None else None

    cav21_ica_soma = np.array(cell.cav21_ica_soma_vec) if getattr(cell, "cav21_ica_soma_vec", None) is not None else None
    cav22_ica_soma = np.array(cell.cav22_ica_soma_vec) if getattr(cell, "cav22_ica_soma_vec", None) is not None else None
    cav12_ica_soma = np.array(cell.cav12_ica_soma_vec) if getattr(cell, "cav12_ica_soma_vec", None) is not None else None
    cav13_ica_soma = np.array(cell.cav13_ica_soma_vec) if getattr(cell, "cav13_ica_soma_vec", None) is not None else None

    bk_acai22_soma = np.array(cell.bk_acai22_soma_vec) if getattr(cell, "bk_acai22_soma_vec", None) is not None else None
    bk_acai12_soma = np.array(cell.bk_acai12_soma_vec) if getattr(cell, "bk_acai12_soma_vec", None) is not None else None
    bk_acai21_soma = np.array(cell.bk_acai21_soma_vec) if getattr(cell, "bk_acai21_soma_vec", None) is not None else None

    return {
        "t": t,
        "vs": vs,
        "vais": vais,
        "vp": vp,
        "vd": vd,
        "vsp": vsp,
        "cai_soma": cai_soma,
        "cai_prox": cai_prox,
        "cai_dist": cai_dist,
        "cai_spine": cai_spine,
        "cai_ais": cai_ais,
        "ica_soma": ica_soma,
        "ik_soma": ik_soma,
        "ina_soma": ina_soma,
        "bk_Cav22_ik_soma": bk_Cav22_ik_soma,
        "bk_Cav12_ik_soma": bk_Cav12_ik_soma,
        "bk_Cav21_ik_soma": bk_Cav21_ik_soma,
        "bk_total_soma": bk_total_soma,
        "sk_ik_soma": sk_ik_soma,
        "cav21_ica_soma": cav21_ica_soma,
        "cav22_ica_soma": cav22_ica_soma,
        "cav12_ica_soma": cav12_ica_soma,
        "cav13_ica_soma": cav13_ica_soma,
        "bk_acai22_soma": bk_acai22_soma,
        "bk_acai12_soma": bk_acai12_soma,
        "bk_acai21_soma": bk_acai21_soma,
    }


def run_current_clamp(
    cell_class,
    delay=100.0,
    dur=300.0,
    amp=0.3,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
    celsius=34.0,
    channel_overrides=None,
    bk_split_override=None,
    save_report=False,
    report_name="ic_run_report.json",
    make_basic_plots=False,
    plot_prefix="ic",
):
    h.celsius = celsius

    cell_kwargs = {}
    if channel_overrides is not None:
        cell_kwargs["channel_overrides"] = channel_overrides
    if bk_split_override is not None:
        cell_kwargs["bk_split"] = bk_split_override

    cell = cell_class(**cell_kwargs)
    cell.add_current_clamp(delay=delay, dur=dur, amp=amp)
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
                "type": "current_clamp",
                "delay_ms": float(delay),
                "dur_ms": float(dur),
                "amp_nA": float(amp),
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
    print("BK_total_ik =", peak_abs(data["bk_total_soma"]), "mA/cm2")
    print("SK_ik =", peak_abs(data["sk_ik_soma"]), "mA/cm2")
    print("ina_soma =", peak_abs(data["ina_soma"]), "mA/cm2")
    print("Cav1.2 source current =", peak_abs(data["cav12_ica_soma"]), "mA/cm2")
    print("Cav1.3 source current =", peak_abs(data["cav13_ica_soma"]), "mA/cm2")
    print("Cav2.1 source current =", peak_abs(data["cav21_ica_soma"]), "mA/cm2")
    print("Cav22 source current =", peak_abs(data["cav22_ica_soma"]), "mA/cm2")

    if make_basic_plots:
        plt.figure()
        plt.plot(data["t"], data["vs"])
        plt.xlabel("Time (ms)")
        plt.ylabel("Membrane potential (mV)")
        plt.tight_layout()
        savefig(f"{plot_prefix}_vm_soma.png")
        plt.show()

        if data["cai_soma"] is not None:
            plt.figure()
            plt.plot(data["t"], data["cai_soma"])
            plt.xlabel("Time (ms)")
            plt.ylabel("cai (mM)")
            plt.tight_layout()
            savefig(f"{plot_prefix}_cai_soma.png")
            plt.show()

        if data["bk_total_soma"] is not None:
            plt.figure()
            plt.plot(data["t"], data["bk_total_soma"])
            plt.xlabel("Time (ms)")
            plt.ylabel("Total BK current density (mA/cm2)")
            plt.tight_layout()
            savefig(f"{plot_prefix}_bk_total.png")
            plt.show()

    return data


if __name__ == "__main__":
    from cells import WTCell

    run_current_clamp(
        cell_class=WTCell,
        save_report=True,
        report_name="ic_run_report_WT.json",
        make_basic_plots=True,
        plot_prefix="ic_WT",
    )