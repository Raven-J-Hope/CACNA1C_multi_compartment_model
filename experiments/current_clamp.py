#!/usr/bin/env python3

#software and package versions used:
#Python version: 3.12.2
#NEURON version: NEURON -- VERSION 9.0.1

import os
import sys
import json
import numpy as np
import matplotlib.pyplot as plt
from neuron import h

from cells import WTCell, Cav12_50Cell

h.load_file("stdrun.hoc")

#labels and colour scheme
WT_LABEL = "WT"
CAV12_50_LABEL = "Cav1.2 50%"
WT_COLOR = "black"
CAV12_50_COLOR = "#ffa6b2"

print("Python version:", sys.version)
print("NEURON version:", h.nrnversion())

#make and set dir & paths to compiled mod files
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

    cai_soma = np.array(cell.cai_soma_vec) if cell.cai_soma_vec is not None else None
    cai_prox = np.array(cell.cai_prox_vec) if cell.cai_prox_vec is not None else None
    cai_dist = np.array(cell.cai_dist_vec) if cell.cai_dist_vec is not None else None
    cai_spine = np.array(cell.cai_spine_vec) if cell.cai_spine_vec is not None else None
    cai_ais = np.array(cell.cai_ais_vec) if getattr(cell, "cai_ais_vec", None) is not None else None

    ica_soma = np.array(cell.ica_soma_vec) if getattr(cell, "ica_soma_vec", None) is not None else None
    ik_soma = np.array(cell.ik_soma_vec) if getattr(cell, "ik_soma_vec", None) is not None else None
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
    ina_soma = np.array(cell.ina_soma_vec) if getattr(cell, "ina_soma_vec", None) is not None else None

    cav21_ica_soma = np.array(cell.cav21_ica_soma_vec) if getattr(cell, "cav21_ica_soma_vec", None) is not None else None
    cav22_ica_soma = np.array(cell.cav22_ica_soma_vec) if getattr(cell, "cav22_ica_soma_vec", None) is not None else None
    cav12_ica_soma = np.array(cell.cav12_ica_soma_vec) if getattr(cell, "cav12_ica_soma_vec", None) is not None else None
    cav13_ica_soma = np.array(cell.cav13_ica_soma_vec) if getattr(cell, "cav13_ica_soma_vec", None) is not None else None

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
        "bk_Cav22_ik_soma": bk_Cav22_ik_soma,
        "bk_Cav12_ik_soma": bk_Cav12_ik_soma,
        "bk_Cav21_ik_soma": bk_Cav21_ik_soma,
        "bk_total_soma": bk_total_soma,
        "sk_ik_soma": sk_ik_soma,
        "ina_soma": ina_soma,
        "cav21_ica_soma": cav21_ica_soma,
        "cav22_ica_soma": cav22_ica_soma,
        "cav12_ica_soma": cav12_ica_soma,
        "cav13_ica_soma": cav13_ica_soma,
    }


def peak_abs(x):
    return None if x is None else float(np.max(np.abs(x)))


def run_current_clamp():
    h.celsius = 34.0

    #WT
    wt = WTCell()
    wt.add_current_clamp(delay=100.0, dur=300.0, amp=0.3)
    wt.setup_recording()
    wt_data = run_sim(wt, tstop=500.0, v_init=-70.0, dt=0.025)

    #50%
    het = Cav12_50Cell()
    het.add_current_clamp(delay=100.0, dur=300.0, amp=0.3)
    het.setup_recording()
    het_data = run_sim(het, tstop=500.0, v_init=-70.0, dt=0.025)

    #save metadata
    run_meta = {
        "python_version": sys.version,
        "neuron_version": h.nrnversion(),
        "dt_ms": float(h.dt),
        "tstop_ms": 500.0,
        "v_init_mV": -70.0,
        "celsius_C": float(h.celsius),
        "morphology": {
            "soma": {"L": float(wt.soma.L), "diam": float(wt.soma.diam), "nseg": int(wt.soma.nseg)},
            "dend_prox": {"L": float(wt.dend_prox.L), "diam": float(wt.dend_prox.diam), "nseg": int(wt.dend_prox.nseg)},
            "dend_dist": {"L": float(wt.dend_dist.L), "diam": float(wt.dend_dist.diam), "nseg": int(wt.dend_dist.nseg)},
            "axon": {"L": float(wt.axon.L), "diam": float(wt.axon.diam), "nseg": int(wt.axon.nseg)},
        },
        "Ra_ohmcm": float(wt.soma.Ra),
        "cm_uFcm2": float(wt.soma.cm),
        "soma_psection": str(wt.soma.psection()),
    }
    save_run_report(os.path.join(OUT_DIR, "ic_run_report_baseline.json"), run_meta)

    run_meta_50 = dict(run_meta)
    run_meta_50["model"] = "Cav12_50"
    save_run_report(os.path.join(OUT_DIR, "ic_run_report_cav12_50.json"), run_meta_50)

    #basic diagnostics
    print("\n--- CURRENT DIAGNOSTICS (soma, peak |current|) ---")
    for label, data in [(WT_LABEL, wt_data), (CAV12_50_LABEL, het_data)]:
        print(f"{label}: ica_soma =", peak_abs(data['ica_soma']), "mA/cm2")
        print(f"{label}: ik_soma =", peak_abs(data['ik_soma']), "mA/cm2")
        print(f"{label}: BK_Cav22_ik =", peak_abs(data['bk_Cav22_ik_soma']), "mA/cm2")
        print(f"{label}: BK_Cav12_ik =", peak_abs(data['bk_Cav12_ik_soma']), "mA/cm2")
        print(f"{label}: BK_Cav21_ik =", peak_abs(data['bk_Cav21_ik_soma']), "mA/cm2")
        print(f"{label}: BK_total_ik =", peak_abs(data['bk_total_soma']), "mA/cm2")
        print(f"{label}: SK_ik =", peak_abs(data['sk_ik_soma']), "mA/cm2")
        print(f"{label}: ina_soma =", peak_abs(data['ina_soma']), "mA/cm2")
        print(f"{label}: Cav1.2 source current =", peak_abs(data['cav12_ica_soma']), "mA/cm2")
        print(f"{label}: Cav1.3 source current =", peak_abs(data['cav13_ica_soma']), "mA/cm2")
        print(f"{label}: Cav2.1 source current =", peak_abs(data['cav21_ica_soma']), "mA/cm2")
        print(f"{label}: Cav22 source current =", peak_abs(data['cav22_ica_soma']), "mA/cm2")

    #basic plots
    plt.figure()
    plt.plot(wt_data["t"], wt_data["vs"], color=WT_COLOR, label=WT_LABEL)
    plt.plot(het_data["t"], het_data["vs"], color=CAV12_50_COLOR, label=CAV12_50_LABEL)
    plt.xlabel("Time (ms)")
    plt.ylabel("Membrane potential (mV)")
    plt.title("Baseline vs reduced Cav1.2")
    plt.legend()
    plt.tight_layout()
    savefig("ic_vm_soma_WT_vs_Cav12_50.png")
    plt.show()

    if wt_data["cai_soma"] is not None and het_data["cai_soma"] is not None:
        plt.figure()
        plt.plot(wt_data["t"], wt_data["cai_soma"], color=WT_COLOR, label=WT_LABEL)
        plt.plot(het_data["t"], het_data["cai_soma"], color=CAV12_50_COLOR, label=CAV12_50_LABEL)
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Intracellular Ca at soma")
        plt.legend()
        plt.tight_layout()
        savefig("ic_cai_soma_WT_vs_Cav12_50.png")
        plt.show()

    if wt_data["bk_total_soma"] is not None and het_data["bk_total_soma"] is not None:
        plt.figure()
        plt.plot(wt_data["t"], wt_data["bk_total_soma"], color=WT_COLOR, label=WT_LABEL)
        plt.plot(het_data["t"], het_data["bk_total_soma"], color=CAV12_50_COLOR, label=CAV12_50_LABEL)
        plt.xlabel("Time (ms)")
        plt.ylabel("Total BK current density (mA/cm2)")
        plt.title("Total BK recruitment")
        plt.legend()
        plt.tight_layout()
        savefig("ic_BK_total_WT_vs_Cav12_50.png")
        plt.show()

    if wt_data["cav13_ica_soma"] is not None and het_data["cav13_ica_soma"] is not None:
        plt.figure()
        plt.plot(wt_data["t"], wt_data["cav13_ica_soma"], color=WT_COLOR, label=WT_LABEL)
        plt.plot(het_data["t"], het_data["cav13_ica_soma"], color=CAV12_50_COLOR, label=CAV12_50_LABEL)
        plt.xlabel("Time (ms)")
        plt.ylabel("Current density (mA/cm2)")
        plt.title("Cav1.3 source current")
        plt.legend()
        plt.tight_layout()
        savefig("ic_Cav13_source_current_WT_vs_Cav12_50.png")
        plt.show()

    return wt_data, het_data


if __name__ == "__main__":
    run_current_clamp()