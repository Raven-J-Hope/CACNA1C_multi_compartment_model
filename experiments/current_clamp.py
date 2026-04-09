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


    #plots

    # ============================================================
    # EXTRA IC PLOTS
    # ============================================================

    # helper: convert recorded NEURON vector to numpy array
    def vec_to_np(v):
        return np.array(v) if v is not None else None

    # helper: upward crossing spike times
    def spike_times_upcross(t, v, thr=0.0, refractory_ms=2.0, t0=100.0, t1=400.0):
        t = np.asarray(t)
        v = np.asarray(v)
        w = (t >= t0) & (t <= t1)
        tt = t[w]
        vv = v[w]
        if len(tt) < 2:
            return np.array([])
        crosses = (vv[:-1] < thr) & (vv[1:] >= thr)
        idx = np.where(crosses)[0]

        out = []
        last = -1e9
        for i in idx:
            ts = float(tt[i + 1])
            if ts - last >= refractory_ms:
                out.append(ts)
                last = ts
        return np.array(out)

    # helper: AP widths
    def ap_widths_per_spike(t, v, frac=0.5, threshold=0.0, refractory_ms=2.0,
                            t_start=100.0, t_end=400.0, pre_ms=3.0, post_ms=15.0):
        t = np.asarray(t)
        v = np.asarray(v)

        w = (t >= t_start) & (t <= t_end)
        tt = t[w]
        vv = v[w]

        crosses = (vv[:-1] < threshold) & (vv[1:] >= threshold)
        idx = np.where(crosses)[0]

        spike_times = []
        last_t = -1e9
        for i in idx:
            ts = float(tt[i + 1])
            if ts - last_t >= refractory_ms:
                spike_times.append(ts)
                last_t = ts

        widths = []
        peaks = []
        troughs = []

        for ts in spike_times:
            w_sp = (t >= ts - pre_ms) & (t <= ts + post_ms)
            t_sp = t[w_sp]
            v_sp = v[w_sp]

            if len(t_sp) < 5:
                widths.append(np.nan)
                peaks.append(np.nan)
                troughs.append(np.nan)
                continue

            i_peak = int(np.argmax(v_sp))
            v_peak = float(v_sp[i_peak])

            v_after = v_sp[i_peak:]
            i_trough = int(np.argmin(v_after))
            v_trough = float(v_after[i_trough])

            v_level = v_trough + frac * (v_peak - v_trough)

            up_idx = np.where((v_sp[:i_peak] < v_level) & (v_sp[1:i_peak + 1] >= v_level))[0]
            down_idx = np.where((v_sp[i_peak:-1] >= v_level) & (v_sp[i_peak + 1:] < v_level))[0]

            if len(up_idx) == 0 or len(down_idx) == 0:
                widths.append(np.nan)
            else:
                t_up = float(t_sp[up_idx[-1] + 1])
                t_down = float(t_sp[i_peak + down_idx[0] + 1])
                widths.append(t_down - t_up)

            peaks.append(v_peak)
            troughs.append(v_trough)

        return np.array(spike_times), np.array(widths), np.array(peaks), np.array(troughs)

    # helper: AUC around spikes
    def auc_around_spikes(t, y, spike_ts, pre_ms=5.0, post_ms=20.0):
        if y is None:
            return np.array([])
        t = np.asarray(t)
        y = np.asarray(y)
        aucs = []
        for ts in spike_ts:
            w = (t >= ts - pre_ms) & (t <= ts + post_ms)
            if np.any(w):
                aucs.append(float(np.trapz(y[w], t[w])))
        return np.array(aucs)

    # helper: first AP window
    def first_ap_window(t, v, thr=0.0, t0=100.0, t1=400.0, pre_ms=5.0, post_ms=20.0):
        spk = spike_times_upcross(t, v, thr=thr, refractory_ms=2.0, t0=t0, t1=t1)
        if len(spk) == 0:
            return None
        ts = spk[0]
        return (t >= ts - pre_ms) & (t <= ts + post_ms)

    # helper: safe max abs
    def safe_max_abs(*arrays):
        vals = []
        for a in arrays:
            if a is not None:
                vals.append(float(np.max(np.abs(a))))
        return max(vals) if vals else None

    # helper: safe min/max
    def safe_min(*arrays):
        vals = []
        for a in arrays:
            if a is not None:
                vals.append(float(np.min(a)))
        return min(vals) if vals else None

    def safe_max(*arrays):
        vals = []
        for a in arrays:
            if a is not None:
                vals.append(float(np.max(a)))
        return max(vals) if vals else None

    # ------------------------------------------------------------
    # pull extra traces directly from cell objects
    # ------------------------------------------------------------
    t0 = wt_data["t"]
    t1 = het_data["t"]

    vs0 = wt_data["vs"]
    vs1 = het_data["vs"]

    vais0 = wt_data["vais"]
    vais1 = het_data["vais"]
    vp0 = wt_data["vp"]
    vp1 = het_data["vp"]
    vd0 = wt_data["vd"]
    vd1 = het_data["vd"]
    vsp0 = wt_data["vsp"]
    vsp1 = het_data["vsp"]

    cai0_soma = wt_data["cai_soma"]
    cai1_soma = het_data["cai_soma"]

    # BK local acai
    bk_acai12_0 = vec_to_np(getattr(wt, "bk_acai12_soma_vec", None))
    bk_acai21_0 = vec_to_np(getattr(wt, "bk_acai21_soma_vec", None))
    bk_acai22_0 = vec_to_np(getattr(wt, "bk_acai22_soma_vec", None))

    bk_acai12_1 = vec_to_np(getattr(het, "bk_acai12_soma_vec", None))
    bk_acai21_1 = vec_to_np(getattr(het, "bk_acai21_soma_vec", None))
    bk_acai22_1 = vec_to_np(getattr(het, "bk_acai22_soma_vec", None))

    # summed BK acai
    bk_acai_total_0 = None
    if bk_acai12_0 is not None and bk_acai21_0 is not None and bk_acai22_0 is not None:
        bk_acai_total_0 = bk_acai12_0 + bk_acai21_0 + bk_acai22_0

    bk_acai_total_1 = None
    if bk_acai12_1 is not None and bk_acai21_1 is not None and bk_acai22_1 is not None:
        bk_acai_total_1 = bk_acai12_1 + bk_acai21_1 + bk_acai22_1

    # BK currents by type
    bk12_0 = wt_data["bk_Cav12_ik_soma"]
    bk21_0 = wt_data["bk_Cav21_ik_soma"]
    bk22_0 = wt_data["bk_Cav22_ik_soma"]
    bk12_1 = het_data["bk_Cav12_ik_soma"]
    bk21_1 = het_data["bk_Cav21_ik_soma"]
    bk22_1 = het_data["bk_Cav22_ik_soma"]

    bk_total0 = wt_data["bk_total_soma"]
    bk_total1 = het_data["bk_total_soma"]

    # y-limits shared across comparable plots
    vm_min = safe_min(vs0, vs1, vais0, vais1, vp0, vp1, vd0, vd1, vsp0, vsp1)
    vm_max = safe_max(vs0, vs1, vais0, vais1, vp0, vp1, vd0, vd1, vsp0, vsp1)

    cai_min = safe_min(cai0_soma, cai1_soma)
    cai_max = safe_max(cai0_soma, cai1_soma)

    bk_total_min = safe_min(bk_total0, bk_total1)
    bk_total_max = safe_max(bk_total0, bk_total1)

    bk12_min = safe_min(bk12_0, bk12_1)
    bk12_max = safe_max(bk12_0, bk12_1)
    bk21_min = safe_min(bk21_0, bk21_1)
    bk21_max = safe_max(bk21_0, bk21_1)
    bk22_min = safe_min(bk22_0, bk22_1)
    bk22_max = safe_max(bk22_0, bk22_1)

    bk_acai_total_min = safe_min(bk_acai_total_0, bk_acai_total_1)
    bk_acai_total_max = safe_max(bk_acai_total_0, bk_acai_total_1)

    bk_acai12_min = safe_min(bk_acai12_0, bk_acai12_1)
    bk_acai12_max = safe_max(bk_acai12_0, bk_acai12_1)
    bk_acai21_min = safe_min(bk_acai21_0, bk_acai21_1)
    bk_acai21_max = safe_max(bk_acai21_0, bk_acai21_1)
    bk_acai22_min = safe_min(bk_acai22_0, bk_acai22_1)
    bk_acai22_max = safe_max(bk_acai22_0, bk_acai22_1)

    # ------------------------------------------------------------
    # # AP - soma
    # ------------------------------------------------------------
    plt.figure()
    plt.plot(t0, vs0, color=WT_COLOR)
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.ylim(vm_min, vm_max)
    plt.tight_layout()
    savefig("ic_AP_soma_WT.png")
    plt.show()

    plt.figure()
    plt.plot(t1, vs1, color=CAV12_50_COLOR)
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.ylim(vm_min, vm_max)
    plt.tight_layout()
    savefig("ic_AP_soma_Cav12_50.png")
    plt.show()

    # ------------------------------------------------------------
    # # panelled AP plot for AIS / prox dend / dist dend / spine
    # ------------------------------------------------------------
    fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True)
    axes[0, 0].plot(t0, vais0, color=WT_COLOR)
    axes[0, 0].set_ylabel("Vm (mV)")
    axes[0, 0].set_ylim(vm_min, vm_max)

    axes[0, 1].plot(t0, vp0, color=WT_COLOR)
    axes[0, 1].set_ylim(vm_min, vm_max)

    axes[1, 0].plot(t0, vd0, color=WT_COLOR)
    axes[1, 0].set_xlabel("Time (ms)")
    axes[1, 0].set_ylabel("Vm (mV)")
    axes[1, 0].set_ylim(vm_min, vm_max)

    axes[1, 1].plot(t0, vsp0, color=WT_COLOR)
    axes[1, 1].set_xlabel("Time (ms)")
    axes[1, 1].set_ylim(vm_min, vm_max)

    fig.tight_layout()
    savefig("ic_AP_panel_AIS_prox_dist_spine_WT.png")
    plt.show()

    fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True)
    axes[0, 0].plot(t1, vais1, color=CAV12_50_COLOR)
    axes[0, 0].set_ylabel("Vm (mV)")
    axes[0, 0].set_ylim(vm_min, vm_max)

    axes[0, 1].plot(t1, vp1, color=CAV12_50_COLOR)
    axes[0, 1].set_ylim(vm_min, vm_max)

    axes[1, 0].plot(t1, vd1, color=CAV12_50_COLOR)
    axes[1, 0].set_xlabel("Time (ms)")
    axes[1, 0].set_ylabel("Vm (mV)")
    axes[1, 0].set_ylim(vm_min, vm_max)

    axes[1, 1].plot(t1, vsp1, color=CAV12_50_COLOR)
    axes[1, 1].set_xlabel("Time (ms)")
    axes[1, 1].set_ylim(vm_min, vm_max)

    fig.tight_layout()
    savefig("ic_AP_panel_AIS_prox_dist_spine_Cav12_50.png")
    plt.show()

    # ------------------------------------------------------------
    # # AP half widths across train (soma)
    # ------------------------------------------------------------
    spk_t0, widths0, _, _ = ap_widths_per_spike(t0, vs0, frac=0.5, threshold=0.0, t_start=100.0, t_end=400.0)
    spk_t1, widths1, _, _ = ap_widths_per_spike(t1, vs1, frac=0.5, threshold=0.0, t_start=100.0, t_end=400.0)

    hw_min = safe_min(widths0, widths1)
    hw_max = safe_max(widths0, widths1)

    if len(widths0) > 0:
        plt.figure()
        plt.plot(np.arange(1, len(widths0) + 1), widths0, marker="o", color=WT_COLOR)
        plt.xlabel("Spike number")
        plt.ylabel("AP half-width (ms)")
        plt.ylim(hw_min, hw_max)
        plt.tight_layout()
        savefig("ic_AP_half_widths_WT.png")
        plt.show()

    if len(widths1) > 0:
        plt.figure()
        plt.plot(np.arange(1, len(widths1) + 1), widths1, marker="o", color=CAV12_50_COLOR)
        plt.xlabel("Spike number")
        plt.ylabel("AP half-width (ms)")
        plt.ylim(hw_min, hw_max)
        plt.tight_layout()
        savefig("ic_AP_half_widths_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # AP aligned with cai
    # ------------------------------------------------------------
    if cai0_soma is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t0, vs0, color=WT_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")
        ax1.set_ylim(vm_min, vm_max)

        ax2 = ax1.twinx()
        ax2.plot(t0, cai0_soma, color="blue")
        ax2.set_ylabel("cai (mM)")
        ax2.set_ylim(cai_min, cai_max)

        fig.tight_layout()
        savefig("ic_AP_with_cai_WT.png")
        plt.show()

    if cai1_soma is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t1, vs1, color=CAV12_50_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")
        ax1.set_ylim(vm_min, vm_max)

        ax2 = ax1.twinx()
        ax2.plot(t1, cai1_soma, color="blue")
        ax2.set_ylabel("cai (mM)")
        ax2.set_ylim(cai_min, cai_max)

        fig.tight_layout()
        savefig("ic_AP_with_cai_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # AP aligned with cai for 1 AP
    # ------------------------------------------------------------
    w_ap0 = first_ap_window(t0, vs0, thr=0.0, t0=100.0, t1=400.0, pre_ms=5.0, post_ms=20.0)
    w_ap1 = first_ap_window(t1, vs1, thr=0.0, t0=100.0, t1=400.0, pre_ms=5.0, post_ms=20.0)

    if w_ap0 is not None and cai0_soma is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t0[w_ap0], vs0[w_ap0], color=WT_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")

        ax2 = ax1.twinx()
        ax2.plot(t0[w_ap0], cai0_soma[w_ap0], color="blue")
        ax2.set_ylabel("cai (mM)")

        fig.tight_layout()
        savefig("ic_AP_with_cai_1AP_WT.png")
        plt.show()

    if w_ap1 is not None and cai1_soma is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t1[w_ap1], vs1[w_ap1], color=CAV12_50_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")

        ax2 = ax1.twinx()
        ax2.plot(t1[w_ap1], cai1_soma[w_ap1], color="blue")
        ax2.set_ylabel("cai (mM)")

        fig.tight_layout()
        savefig("ic_AP_with_cai_1AP_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # total BK plot
    # ------------------------------------------------------------
    if bk_total0 is not None:
        plt.figure()
        plt.plot(t0, bk_total0, color=WT_COLOR)
        plt.xlabel("Time (ms)")
        plt.ylabel("Total BK current density (mA/cm2)")
        plt.ylim(bk_total_min, bk_total_max)
        plt.tight_layout()
        savefig("ic_total_BK_WT.png")
        plt.show()

    if bk_total1 is not None:
        plt.figure()
        plt.plot(t1, bk_total1, color=CAV12_50_COLOR)
        plt.xlabel("Time (ms)")
        plt.ylabel("Total BK current density (mA/cm2)")
        plt.ylim(bk_total_min, bk_total_max)
        plt.tight_layout()
        savefig("ic_total_BK_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # BkCavXX plot, panelled with BkCaV12, 21 and 22
    # ------------------------------------------------------------
    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)
    axes[0].plot(t0, bk12_0, color=WT_COLOR)
    axes[0].set_ylabel("BK_Cav12")
    axes[0].set_ylim(bk12_min, bk12_max)

    axes[1].plot(t0, bk21_0, color=WT_COLOR)
    axes[1].set_ylabel("BK_Cav21")
    axes[1].set_ylim(bk21_min, bk21_max)

    axes[2].plot(t0, bk22_0, color=WT_COLOR)
    axes[2].set_ylabel("BK_Cav22")
    axes[2].set_xlabel("Time (ms)")
    axes[2].set_ylim(bk22_min, bk22_max)

    fig.tight_layout()
    savefig("ic_BK_CavXX_panel_WT.png")
    plt.show()

    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)
    axes[0].plot(t1, bk12_1, color=CAV12_50_COLOR)
    axes[0].set_ylabel("BK_Cav12")
    axes[0].set_ylim(bk12_min, bk12_max)

    axes[1].plot(t1, bk21_1, color=CAV12_50_COLOR)
    axes[1].set_ylabel("BK_Cav21")
    axes[1].set_ylim(bk21_min, bk21_max)

    axes[2].plot(t1, bk22_1, color=CAV12_50_COLOR)
    axes[2].set_ylabel("BK_Cav22")
    axes[2].set_xlabel("Time (ms)")
    axes[2].set_ylim(bk22_min, bk22_max)

    fig.tight_layout()
    savefig("ic_BK_CavXX_panel_Cav12_50.png")
    plt.show()

    # ------------------------------------------------------------
    # # total BK acai plot
    # ------------------------------------------------------------
    if bk_acai_total_0 is not None:
        plt.figure()
        plt.plot(t0, bk_acai_total_0, color=WT_COLOR)
        plt.xlabel("Time (ms)")
        plt.ylabel("Summed BK acai (mM)")
        plt.ylim(bk_acai_total_min, bk_acai_total_max)
        plt.tight_layout()
        savefig("ic_total_BK_acai_WT.png")
        plt.show()

    if bk_acai_total_1 is not None:
        plt.figure()
        plt.plot(t1, bk_acai_total_1, color=CAV12_50_COLOR)
        plt.xlabel("Time (ms)")
        plt.ylabel("Summed BK acai (mM)")
        plt.ylim(bk_acai_total_min, bk_acai_total_max)
        plt.tight_layout()
        savefig("ic_total_BK_acai_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # panelled acai BkCavXX plot, panelled with BkCaV12, 21 and 22
    # ------------------------------------------------------------
    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)
    axes[0].plot(t0, bk_acai12_0, color=WT_COLOR)
    axes[0].set_ylabel("BK_Cav12 acai")
    axes[0].set_ylim(bk_acai12_min, bk_acai12_max)

    axes[1].plot(t0, bk_acai21_0, color=WT_COLOR)
    axes[1].set_ylabel("BK_Cav21 acai")
    axes[1].set_ylim(bk_acai21_min, bk_acai21_max)

    axes[2].plot(t0, bk_acai22_0, color=WT_COLOR)
    axes[2].set_ylabel("BK_Cav22 acai")
    axes[2].set_xlabel("Time (ms)")
    axes[2].set_ylim(bk_acai22_min, bk_acai22_max)

    fig.tight_layout()
    savefig("ic_BK_acai_CavXX_panel_WT.png")
    plt.show()

    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)
    axes[0].plot(t1, bk_acai12_1, color=CAV12_50_COLOR)
    axes[0].set_ylabel("BK_Cav12 acai")
    axes[0].set_ylim(bk_acai12_min, bk_acai12_max)

    axes[1].plot(t1, bk_acai21_1, color=CAV12_50_COLOR)
    axes[1].set_ylabel("BK_Cav21 acai")
    axes[1].set_ylim(bk_acai21_min, bk_acai21_max)

    axes[2].plot(t1, bk_acai22_1, color=CAV12_50_COLOR)
    axes[2].set_ylabel("BK_Cav22 acai")
    axes[2].set_xlabel("Time (ms)")
    axes[2].set_ylim(bk_acai22_min, bk_acai22_max)

    fig.tight_layout()
    savefig("ic_BK_acai_CavXX_panel_Cav12_50.png")
    plt.show()

    # ------------------------------------------------------------
    # # total BK current density aligned with AP
    # ------------------------------------------------------------
    if bk_total0 is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t0, vs0, color=WT_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")
        ax1.set_ylim(vm_min, vm_max)

        ax2 = ax1.twinx()
        ax2.plot(t0, bk_total0, color="blue")
        ax2.set_ylabel("Total BK current density (mA/cm2)")
        ax2.set_ylim(bk_total_min, bk_total_max)

        fig.tight_layout()
        savefig("ic_total_BK_with_AP_WT.png")
        plt.show()

    if bk_total1 is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t1, vs1, color=CAV12_50_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")
        ax1.set_ylim(vm_min, vm_max)

        ax2 = ax1.twinx()
        ax2.plot(t1, bk_total1, color="blue")
        ax2.set_ylabel("Total BK current density (mA/cm2)")
        ax2.set_ylim(bk_total_min, bk_total_max)

        fig.tight_layout()
        savefig("ic_total_BK_with_AP_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # total BK current density aligned with AP for 1 AP
    # ------------------------------------------------------------
    if w_ap0 is not None and bk_total0 is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t0[w_ap0], vs0[w_ap0], color=WT_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")

        ax2 = ax1.twinx()
        ax2.plot(t0[w_ap0], bk_total0[w_ap0], color="blue")
        ax2.set_ylabel("Total BK current density (mA/cm2)")

        fig.tight_layout()
        savefig("ic_total_BK_with_AP_1AP_WT.png")
        plt.show()

    if w_ap1 is not None and bk_total1 is not None:
        fig, ax1 = plt.subplots(figsize=(10, 5))
        ax1.plot(t1[w_ap1], vs1[w_ap1], color=CAV12_50_COLOR)
        ax1.set_xlabel("Time (ms)")
        ax1.set_ylabel("Vm (mV)")

        ax2 = ax1.twinx()
        ax2.plot(t1[w_ap1], bk_total1[w_ap1], color="blue")
        ax2.set_ylabel("Total BK current density (mA/cm2)")

        fig.tight_layout()
        savefig("ic_total_BK_with_AP_1AP_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # panelled BkCavXX current density aligned with AP
    # ------------------------------------------------------------
    fig, axes = plt.subplots(3, 1, figsize=(10, 12), sharex=True)
    for ax, bk, label in zip(
        axes,
        [bk12_0, bk21_0, bk22_0],
        ["BK_Cav12", "BK_Cav21", "BK_Cav22"]
    ):
        ax.plot(t0, vs0, color=WT_COLOR)
        ax2 = ax.twinx()
        ax2.plot(t0, bk, color="blue")
        ax.set_ylabel("Vm (mV)")
        ax2.set_ylabel(label)
    axes[-1].set_xlabel("Time (ms)")
    fig.tight_layout()
    savefig("ic_BK_CavXX_with_AP_panel_WT.png")
    plt.show()

    fig, axes = plt.subplots(3, 1, figsize=(10, 12), sharex=True)
    for ax, bk, label in zip(
        axes,
        [bk12_1, bk21_1, bk22_1],
        ["BK_Cav12", "BK_Cav21", "BK_Cav22"]
    ):
        ax.plot(t1, vs1, color=CAV12_50_COLOR)
        ax2 = ax.twinx()
        ax2.plot(t1, bk, color="blue")
        ax.set_ylabel("Vm (mV)")
        ax2.set_ylabel(label)
    axes[-1].set_xlabel("Time (ms)")
    fig.tight_layout()
    savefig("ic_BK_CavXX_with_AP_panel_Cav12_50.png")
    plt.show()

    # ------------------------------------------------------------
    # # panelled BkCavXX current density aligned with AP for 1 AP
    # ------------------------------------------------------------
    if w_ap0 is not None:
        fig, axes = plt.subplots(3, 1, figsize=(10, 12), sharex=True)
        for ax, bk, label in zip(
            axes,
            [bk12_0, bk21_0, bk22_0],
            ["BK_Cav12", "BK_Cav21", "BK_Cav22"]
        ):
            ax.plot(t0[w_ap0], vs0[w_ap0], color=WT_COLOR)
            ax2 = ax.twinx()
            ax2.plot(t0[w_ap0], bk[w_ap0], color="blue")
            ax.set_ylabel("Vm (mV)")
            ax2.set_ylabel(label)
        axes[-1].set_xlabel("Time (ms)")
        fig.tight_layout()
        savefig("ic_BK_CavXX_with_AP_panel_1AP_WT.png")
        plt.show()

    if w_ap1 is not None:
        fig, axes = plt.subplots(3, 1, figsize=(10, 12), sharex=True)
        for ax, bk, label in zip(
            axes,
            [bk12_1, bk21_1, bk22_1],
            ["BK_Cav12", "BK_Cav21", "BK_Cav22"]
        ):
            ax.plot(t1[w_ap1], vs1[w_ap1], color=CAV12_50_COLOR)
            ax2 = ax.twinx()
            ax2.plot(t1[w_ap1], bk[w_ap1], color="blue")
            ax.set_ylabel("Vm (mV)")
            ax2.set_ylabel(label)
        axes[-1].set_xlabel("Time (ms)")
        fig.tight_layout()
        savefig("ic_BK_CavXX_with_AP_panel_1AP_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # total BK recruitment efficiency
    # ------------------------------------------------------------
    spk0 = spike_times_upcross(t0, vs0, thr=0.0, refractory_ms=2.0, t0=100.0, t1=400.0)
    spk1 = spike_times_upcross(t1, vs1, thr=0.0, refractory_ms=2.0, t0=100.0, t1=400.0)

    pre_ms = 5.0
    post_ms = 20.0

    spk0 = spk0[(spk0 - pre_ms >= 100.0) & (spk0 + post_ms <= 400.0)]
    spk1 = spk1[(spk1 - pre_ms >= 100.0) & (spk1 + post_ms <= 400.0)]

    auc_ca0 = auc_around_spikes(t0, cai0_soma, spk0, pre_ms=pre_ms, post_ms=post_ms)
    auc_ca1 = auc_around_spikes(t1, cai1_soma, spk1, pre_ms=pre_ms, post_ms=post_ms)

    auc_bk_total0 = auc_around_spikes(t0, bk_total0, spk0, pre_ms=pre_ms, post_ms=post_ms)
    auc_bk_total1 = auc_around_spikes(t1, bk_total1, spk1, pre_ms=pre_ms, post_ms=post_ms)

    n_eff0 = min(len(auc_bk_total0), len(auc_ca0))
    n_eff1 = min(len(auc_bk_total1), len(auc_ca1))

    ratio_total0 = auc_bk_total0[:n_eff0] / (auc_ca0[:n_eff0] + 1e-12) if n_eff0 > 0 else np.array([])
    ratio_total1 = auc_bk_total1[:n_eff1] / (auc_ca1[:n_eff1] + 1e-12) if n_eff1 > 0 else np.array([])

    eff_total_min = safe_min(ratio_total0, ratio_total1)
    eff_total_max = safe_max(ratio_total0, ratio_total1)

    if len(ratio_total0) > 0:
        plt.figure()
        plt.plot(np.arange(1, len(ratio_total0) + 1), ratio_total0, marker="o", color=WT_COLOR)
        plt.xlabel("Spike number")
        plt.ylabel("Total BK / Ca AUC")
        plt.ylim(eff_total_min, eff_total_max)
        plt.tight_layout()
        savefig("ic_total_BK_recruitment_efficiency_WT.png")
        plt.show()

    if len(ratio_total1) > 0:
        plt.figure()
        plt.plot(np.arange(1, len(ratio_total1) + 1), ratio_total1, marker="o", color=CAV12_50_COLOR)
        plt.xlabel("Spike number")
        plt.ylabel("Total BK / Ca AUC")
        plt.ylim(eff_total_min, eff_total_max)
        plt.tight_layout()
        savefig("ic_total_BK_recruitment_efficiency_Cav12_50.png")
        plt.show()

    # ------------------------------------------------------------
    # # panelled BK recruitment efficiency for BK_Cav12 / 21 / 22
    # ------------------------------------------------------------
    auc_bk12_0 = auc_around_spikes(t0, bk12_0, spk0, pre_ms=pre_ms, post_ms=post_ms)
    auc_bk21_0 = auc_around_spikes(t0, bk21_0, spk0, pre_ms=pre_ms, post_ms=post_ms)
    auc_bk22_0 = auc_around_spikes(t0, bk22_0, spk0, pre_ms=pre_ms, post_ms=post_ms)

    auc_bk12_1 = auc_around_spikes(t1, bk12_1, spk1, pre_ms=pre_ms, post_ms=post_ms)
    auc_bk21_1 = auc_around_spikes(t1, bk21_1, spk1, pre_ms=pre_ms, post_ms=post_ms)
    auc_bk22_1 = auc_around_spikes(t1, bk22_1, spk1, pre_ms=pre_ms, post_ms=post_ms)

    n12_0 = min(len(auc_bk12_0), len(auc_ca0))
    n21_0 = min(len(auc_bk21_0), len(auc_ca0))
    n22_0 = min(len(auc_bk22_0), len(auc_ca0))

    n12_1 = min(len(auc_bk12_1), len(auc_ca1))
    n21_1 = min(len(auc_bk21_1), len(auc_ca1))
    n22_1 = min(len(auc_bk22_1), len(auc_ca1))

    ratio12_0 = auc_bk12_0[:n12_0] / (auc_ca0[:n12_0] + 1e-12) if n12_0 > 0 else np.array([])
    ratio21_0 = auc_bk21_0[:n21_0] / (auc_ca0[:n21_0] + 1e-12) if n21_0 > 0 else np.array([])
    ratio22_0 = auc_bk22_0[:n22_0] / (auc_ca0[:n22_0] + 1e-12) if n22_0 > 0 else np.array([])

    ratio12_1 = auc_bk12_1[:n12_1] / (auc_ca1[:n12_1] + 1e-12) if n12_1 > 0 else np.array([])
    ratio21_1 = auc_bk21_1[:n21_1] / (auc_ca1[:n21_1] + 1e-12) if n21_1 > 0 else np.array([])
    ratio22_1 = auc_bk22_1[:n22_1] / (auc_ca1[:n22_1] + 1e-12) if n22_1 > 0 else np.array([])

    eff_bk_min = safe_min(ratio12_0, ratio21_0, ratio22_0, ratio12_1, ratio21_1, ratio22_1)
    eff_bk_max = safe_max(ratio12_0, ratio21_0, ratio22_0, ratio12_1, ratio21_1, ratio22_1)

    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)
    if len(ratio12_0) > 0:
        axes[0].plot(np.arange(1, len(ratio12_0) + 1), ratio12_0, marker="o", color=WT_COLOR)
    axes[0].set_ylabel("BK_Cav12 / Ca")
    axes[0].set_ylim(eff_bk_min, eff_bk_max)

    if len(ratio21_0) > 0:
        axes[1].plot(np.arange(1, len(ratio21_0) + 1), ratio21_0, marker="o", color=WT_COLOR)
    axes[1].set_ylabel("BK_Cav21 / Ca")
    axes[1].set_ylim(eff_bk_min, eff_bk_max)

    if len(ratio22_0) > 0:
        axes[2].plot(np.arange(1, len(ratio22_0) + 1), ratio22_0, marker="o", color=WT_COLOR)
    axes[2].set_ylabel("BK_Cav22 / Ca")
    axes[2].set_xlabel("Spike number")
    axes[2].set_ylim(eff_bk_min, eff_bk_max)

    fig.tight_layout()
    savefig("ic_BK_recruitment_efficiency_panel_WT.png")
    plt.show()

    fig, axes = plt.subplots(3, 1, figsize=(10, 10), sharex=True)
    if len(ratio12_1) > 0:
        axes[0].plot(np.arange(1, len(ratio12_1) + 1), ratio12_1, marker="o", color=CAV12_50_COLOR)
    axes[0].set_ylabel("BK_Cav12 / Ca")
    axes[0].set_ylim(eff_bk_min, eff_bk_max)

    if len(ratio21_1) > 0:
        axes[1].plot(np.arange(1, len(ratio21_1) + 1), ratio21_1, marker="o", color=CAV12_50_COLOR)
    axes[1].set_ylabel("BK_Cav21 / Ca")
    axes[1].set_ylim(eff_bk_min, eff_bk_max)

    if len(ratio22_1) > 0:
        axes[2].plot(np.arange(1, len(ratio22_1) + 1), ratio22_1, marker="o", color=CAV12_50_COLOR)
    axes[2].set_ylabel("BK_Cav22 / Ca")
    axes[2].set_xlabel("Spike number")
    axes[2].set_ylim(eff_bk_min, eff_bk_max)

    fig.tight_layout()
    savefig("ic_BK_recruitment_efficiency_panel_Cav12_50.png")
    plt.show()



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