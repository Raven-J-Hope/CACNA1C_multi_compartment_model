#!/usr/bin/env python3

#software and package versions used:
#Python version: 3.12.2
#NEURON version: NEURON -- VERSION 9.0.1

import os
import numpy as np
import matplotlib.pyplot as plt
from neuron import h
import json  #to save metadata/parameters for diss

h.load_file("stdrun.hoc")

#BK coupling splits
WT_BK_SPLIT = {              #equal distrib
    "BK_Cav22": 1.0 / 3.0,
    "BK_Cav12": 1.0 / 3.0,
    "BK_Cav21": 1.0 / 3.0,
}

CAV12_50_BK_SPLIT = {          #halves BK_Cav12 and redistribs other half equally to cav22 and 21
    "BK_Cav22": 5.0 / 12.0,
    "BK_Cav12": 1.0 / 6.0,
    "BK_Cav21": 5.0 / 12.0,
}

#het protocol 2 halves BK_Cav12 but does NOT redistribute it
CAV12_50_REMOVE_BK_SPLIT = {            #normalised fractions of the remaining BK pool
    "BK_Cav22": 2.0 / 5.0,
    "BK_Cav12": 1.0 / 5.0,
    "BK_Cav21": 2.0 / 5.0,
}

#total BK pool is now 5/6 of WT, because half of the BK_Cav12 third is removed
CAV12_50_REMOVE_BK_TOTAL_SCALE = 5.0 / 6.0


#labels and colour scheme
WT_LABEL = "WT"
CAV12_50_LABEL = "Cav1.2 50%"
CAV12_50_REMOVE_LABEL = "Cav1.2 50% BK_Cav1.2 removed"

WT_COLOR = "black" #make grey
CAV12_50_COLOR = "#ffa6b2" #"#e16173"
CAV12_50_REMOVE_COLOR = "#8c52ff"

import sys  #check versions being used
print("Python version:", sys.version) #if python or neuron version used are different from above and code not running
print("NEURON version:", h.nrnversion()) #switch to aforementioned versions as first troubleshooting step

#make and set dir & paths to compiled mod files
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "new test outputs")
FIG_DIR = os.path.join(OUT_DIR, "ic_figures")
os.makedirs(FIG_DIR, exist_ok=True)  #creates outputs/ and outputs/figures if missing

MOD_DIR = "/home/raven/PycharmProjects/Masters/Mod_Files"
DLL_PATH = os.path.join(MOD_DIR, "x86_64", "libnrnmech.so")

if os.path.exists(DLL_PATH):
    h.nrn_load_dll(DLL_PATH)
    print("Loaded mechanisms:", DLL_PATH)
else:
    raise RuntimeError(f"Compiled mechanisms not found at: {DLL_PATH}")

#sanity savers
def try_insert(sec, mech: str) -> bool:
    try:
        sec.insert(mech)
        return True
    except Exception:
        return False


def has_mech(sec, mech: str) -> bool:
    try:
        return bool(h.ismembrane(mech, sec=sec))
    except Exception:
        return False

#check BK split acc worky
def validate_bk_split(split: dict):
    s = sum(split.values())
    if abs(s - 1.0) > 1e-9:
        raise ValueError(f"BK split must sum to 1.0, got {s}")


def apply_bk_split_to_section(sec, total_bk_gakbar: float, total_bk_gabkbar: float, split: dict):
    """
    Split total BK across BK_Cav22, BK_Cav12, and BK_Cav21.
    """
    validate_bk_split(split)

    for seg in sec:
        if has_mech(sec, "BK_Cav22"):
            seg.BK_Cav22.gakbar = total_bk_gakbar * split["BK_Cav22"]
            seg.BK_Cav22.gabkbar = total_bk_gabkbar * split["BK_Cav22"]

        if has_mech(sec, "BK_Cav12"):
            seg.BK_Cav12.gakbar = total_bk_gakbar * split["BK_Cav12"]
            seg.BK_Cav12.gabkbar = total_bk_gabkbar * split["BK_Cav12"]

        if has_mech(sec, "BK_Cav21"):
            seg.BK_Cav21.gakbar = total_bk_gakbar * split["BK_Cav21"]
            seg.BK_Cav21.gabkbar = total_bk_gabkbar * split["BK_Cav21"]

def rest_stats(t, v, delay, pre_ms=50.0, gap_ms=5.0):
    """
    Robust baseline/rest estimate - the mean/std in a window ending (gap_ms) before delay.
    """
    t = np.asarray(t)
    v = np.asarray(v)
    t1 = delay - gap_ms
    t0 = max(t.min(), t1 - pre_ms)
    w = (t >= t0) & (t <= t1)
    if not np.any(w):
        return float("nan"), float("nan"), (t0, t1)
    return float(np.mean(v[w])), float(np.std(v[w])), (t0, t1)

def ahp_depth(t, v, spike_window=(90, 140)):
    """
    Returns (v_peak, t_peak, v_min_after, t_min_after, ahp_depth_mV)
    AHP depth = v_min_after - v_rest (more negative = deeper)
    """
    t = np.asarray(t)
    v = np.asarray(v)

    w = (t >= spike_window[0]) & (t <= spike_window[1])
    tt = t[w]; vv = v[w]

    i_peak = int(np.argmax(vv))
    v_peak = float(vv[i_peak])
    t_peak = float(tt[i_peak])

    #search for minimum after the peak i.e., AHP trough
    vv_after = vv[i_peak:]
    tt_after = tt[i_peak:]
    i_min = int(np.argmin(vv_after))
    v_min = float(vv_after[i_min])
    t_min = float(tt_after[i_min])

    #baseline/rest estimate, now robust to different delays
    v_rest, _, _ = rest_stats(t, v, delay=spike_window[0], pre_ms=40.0, gap_ms=5.0)

    ahp = v_min - v_rest
    return v_peak, t_peak, v_min, t_min, ahp

def spike_features(t, v, delay, dur, threshold=0.0, refractory_ms=2.0):
    """
    Returns spike features in the stimulus window.
    """
    t = np.asarray(t)
    v = np.asarray(v)

    #robust baseline/rest estimate just before stimulus
    v_rest, v_rest_sd, (t0, t1) = rest_stats(t, v, delay=delay, pre_ms=50.0, gap_ms=5.0)

    #restrict to stimulus window
    w = (t >= delay) & (t <= delay + dur)
    tt = t[w]
    vv = v[w]
    if len(tt) < 2:
        return {
            "v_rest": v_rest, "v_rest_sd": v_rest_sd,
            "n_spikes": 0, "rate_hz": 0.0,
            "v_peak": float("nan"), "t_peak": float("nan"),
            "v_trough": float("nan"), "t_trough": float("nan"),
            "ahp_depth": float("nan"),
            "width_half": float("nan"),
        }

    #spike count as upward crossings & refractory
    crosses = (vv[:-1] < threshold) & (vv[1:] >= threshold)
    idx = np.where(crosses)[0]

    spike_times = []
    last_t = -1e9
    for i in idx:
        tcross = tt[i + 1]
        if (tcross - last_t) >= refractory_ms:
            spike_times.append(tcross)
            last_t = tcross

    n_spikes = len(spike_times)
    rate_hz = n_spikes / (dur / 1000.0)

    #peak & trough after peak
    i_peak = int(np.argmax(vv))
    v_peak = float(vv[i_peak])
    t_peak = float(tt[i_peak])

    vv_after = vv[i_peak:]
    tt_after = tt[i_peak:]
    i_min = int(np.argmin(vv_after))
    v_trough = float(vv_after[i_min])
    t_trough = float(tt_after[i_min])

    #AHP depth relative to baseline
    ahp = v_trough - v_rest

    #width at half amplitude
    v_half = (v_peak + v_trough) / 2.0
    above = vv >= v_half
    ids = np.where(above)[0]
    width_half = float(tt[ids[-1]] - tt[ids[0]]) if len(ids) >= 2 else float("nan")

    return {
        "v_rest": v_rest,
        "v_rest_sd": v_rest_sd,
        "n_spikes": n_spikes,
        "rate_hz": rate_hz,
        "v_peak": v_peak,
        "t_peak": t_peak,
        "v_trough": v_trough,
        "t_trough": t_trough,
        "ahp_depth": ahp,
        "width_half": width_half,
    }

def adaptation_metrics(t, v, delay=100.0, dur=300.0, threshold=0.0, refractory_ms=2.0,
                       early_ms=100.0, late_ms=100.0):
    """
    Returns early/late firing rates + adaptation index from soma Vm.
    early window= [delay, delay+early_ms]
    late window=  [delay+dur-late_ms, delay+dur]
    """
    t = np.asarray(t)
    v = np.asarray(v)

    def spike_times_in_window(t0, t1):
        w = (t >= t0) & (t <= t1)
        tt = t[w]
        vv = v[w]
        if len(tt) < 2:
            return np.array([])

        crosses = (vv[:-1] < threshold) & (vv[1:] >= threshold)
        idx = np.where(crosses)[0]

        st = []
        last_t = -1e9
        for i in idx:
            tcross = tt[i + 1]
            if (tcross - last_t) >= refractory_ms:
                st.append(tcross)
                last_t = tcross
        return np.array(st, dtype=float)

    t_on = delay
    t_off = delay + dur

    early_t0, early_t1 = t_on, min(t_on + early_ms, t_off)
    late_t0, late_t1 = max(t_off - late_ms, t_on), t_off

    st_early = spike_times_in_window(early_t0, early_t1)
    st_late  = spike_times_in_window(late_t0, late_t1)

    fr_early = len(st_early) / ((early_t1 - early_t0) / 1000.0) if (early_t1 > early_t0) else 0.0
    fr_late  = len(st_late)  / ((late_t1  - late_t0)  / 1000.0) if (late_t1  > late_t0) else 0.0

    #adaptation index where 1 = strong (late<<early), 0 = none (late≈early), negative = facilitation
    adapt_index = (fr_early - fr_late) / (fr_early + 1e-12)

    #ISI ratio (last ISI / first ISI) within full step
    st_full = spike_times_in_window(t_on, t_off)
    if len(st_full) >= 3:
        isis = np.diff(st_full)
        isi_ratio = float(isis[-1] / (isis[0] + 1e-12))
    else:
        isi_ratio = float("nan")

    return {
        "fr_early_hz": float(fr_early),
        "fr_late_hz": float(fr_late),
        "adapt_index": float(adapt_index),
        "isi_ratio_last_over_first": isi_ratio,
        "n_spikes": int(len(st_full)),
    }

def list_mech_fields(seg, mech_name: str, max_items=200):
    """
    Print all public fields NEURON exposes for a mechanism on a specific segment.
    """
    mech = getattr(seg, mech_name, None)
    if mech is None:
        print(f"[{mech_name}] not present on seg")
        return
    names = [n for n in dir(mech) if not n.startswith("_")]
    names = sorted(names)
    print(f"[{mech_name}] fields on {seg.sec.name()}({seg.x}):")
    for n in names[:max_items]:
        print(" ", n)
    if len(names) > max_items:
        print(f" ... ({len(names)-max_items} more)")

#----
def savefig(name: str):
    plt.savefig(os.path.join(FIG_DIR, name), dpi=300, bbox_inches="tight")
    plt.close()


def init_frequency_bin_store(n_ap_plot=5, ap_start_idx=1):
    ap_numbers = np.arange(ap_start_idx + 1, ap_start_idx + n_ap_plot + 1, dtype=int)
    return {
        "10-40 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
        "40-80 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
        "80-120 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
        ">120 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
    }


def init_frequency_bin_store_10ap(n_ap_plot=10, ap_start_idx=1):
    ap_numbers = np.arange(ap_start_idx + 1, ap_start_idx + n_ap_plot + 1, dtype=int)
    return {
        "10-40 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
        "40-80 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
        "80-120 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
        ">120 Hz": {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()},
    }


def classify_frequency_bin(freq_hz):
    if np.isnan(freq_hz):
        return None
    if 10.0 <= freq_hz < 40.0:
        return "10-40 Hz"
    elif 40.0 <= freq_hz < 80.0:
        return "40-80 Hz"
    elif 80.0 <= freq_hz < 120.0:
        return "80-120 Hz"
    elif freq_hz >= 120.0:
        return ">120 Hz"
    return None


def get_sweep_frequency_hz(inst_freq_hz, n_freq_for_bin=3):
    inst_freq_hz = np.asarray(inst_freq_hz, dtype=float)
    if len(inst_freq_hz) == 0:
        return np.nan
    n_use = min(n_freq_for_bin, len(inst_freq_hz))
    return float(np.nanmean(inst_freq_hz[:n_use]))


def add_sweep_to_bin_store(
    bin_store,
    widths,
    amplitudes,
    inst_freq_hz,
    n_ap_plot=5,
    ap_start_idx=1
):
    sweep_freq_hz = get_sweep_frequency_hz(inst_freq_hz)
    sweep_bin = classify_frequency_bin(sweep_freq_hz)
    if sweep_bin is None:
        return

    width_vec = np.full(n_ap_plot, np.nan)
    amp_vec = np.full(n_ap_plot, np.nan)
    freq_vec = np.full(n_ap_plot, np.nan)

    widths_sel = np.asarray(widths[ap_start_idx:ap_start_idx + n_ap_plot], dtype=float)
    amps_sel = np.asarray(amplitudes[ap_start_idx:ap_start_idx + n_ap_plot], dtype=float)

    freq_start_idx = max(0, ap_start_idx - 1)
    freq_sel = np.asarray(inst_freq_hz[freq_start_idx:freq_start_idx + n_ap_plot], dtype=float)

    width_vec[:len(widths_sel)] = widths_sel
    amp_vec[:len(amps_sel)] = amps_sel
    freq_vec[:len(freq_sel)] = freq_sel

    bin_store[sweep_bin]["widths"].append(width_vec)
    bin_store[sweep_bin]["amplitudes"].append(amp_vec)
    bin_store[sweep_bin]["inst_freq_hz"].append(freq_vec)


def extract_aligned_ap_segment(t, v, spike_times, ap_number, pre_ms=3.0, post_ms=8.0):
    if len(spike_times) < ap_number:
        return None, None

    ts = float(spike_times[ap_number - 1])
    w = (t >= ts - pre_ms) & (t <= ts + post_ms)

    if np.sum(w) < 5:
        return None, None

    t_seg = np.asarray(t[w], dtype=float) - ts
    v_seg = np.asarray(v[w], dtype=float)
    return t_seg, v_seg


def interp_trace_to_common_timebase(t_seg, v_seg, t_common):
    if t_seg is None or v_seg is None or len(t_seg) < 2:
        return np.full_like(t_common, np.nan, dtype=float)

    order = np.argsort(t_seg)
    t_seg = np.asarray(t_seg[order], dtype=float)
    v_seg = np.asarray(v_seg[order], dtype=float)

    keep = np.concatenate(([True], np.diff(t_seg) > 0))
    t_seg = t_seg[keep]
    v_seg = v_seg[keep]

    if len(t_seg) < 2:
        return np.full_like(t_common, np.nan, dtype=float)

    y = np.interp(t_common, t_seg, v_seg, left=np.nan, right=np.nan)
    outside = (t_common < t_seg[0]) | (t_common > t_seg[-1])
    y[outside] = np.nan
    return y


def init_overlay_store():
    return {
        "40-80 Hz": {"AP2": [], "AP6": []},
        "80-120 Hz": {"AP2": [], "AP6": []},
        ">120 Hz": {"AP2": [], "AP6": []},
    }


def add_trace_to_overlay_store(overlay_store, freq_bin, t, v, spike_times):
    if freq_bin not in overlay_store:
        return

    t_ap2, v_ap2 = extract_aligned_ap_segment(t, v, spike_times, ap_number=2, pre_ms=3.0, post_ms=8.0)
    t_ap6, v_ap6 = extract_aligned_ap_segment(t, v, spike_times, ap_number=6, pre_ms=3.0, post_ms=8.0)

    if t_ap2 is None or t_ap6 is None:
        return

    overlay_store[freq_bin]["AP2"].append((t_ap2, v_ap2))
    overlay_store[freq_bin]["AP6"].append((t_ap6, v_ap6))

#-----


#define cell morphology & biophysics
class DGGranuleLikeCell:
    def __init__(self, name="dgcell", bk_split=None, bk_total_scale=1.0):
        self.name = name
        self.bk_split = WT_BK_SPLIT if bk_split is None else bk_split
        validate_bk_split(self.bk_split)
        self.bk_total_scale = bk_total_scale
        self.spines = []    #add dendritic spines
        self.spine_necks = []
        self.spine_xs = [] #store where spines attach

        #create compartments - "nseg" i.e., segments
        self.soma = h.Section(name=f"{name}.soma")
        self.dend_prox = h.Section(name=f"{name}.dend_prox")
        self.dend_dist = h.Section(name=f"{name}.dend_dist")
        self.axon = h.Section(name=f"{name}.axon")
        self.ais = h.Section(name=f"{name}.ais")  #adds in inbital axon segment
        self.ica_soma_vec = None
        self.ik_soma_vec = None
        self.bk_ik_soma_vec = None

        #connects topology
        #note to sen, how link to morph? e.g., like trees/T2N/beining?
        self.dend_prox.connect(self.soma(1))
        self.dend_dist.connect(self.dend_prox(1))
        self.ais.connect(self.soma(0))
        self.axon.connect(self.ais(1))

        self._set_geometry()
        self._set_biophysics()
        self.add_spines_to_distal_dend(n_spines=10)

        #stim & recordings
        self.iclamp = None
        self.t_vec = h.Vector()
        self.vsoma_vec = h.Vector()
        self.vdend_vec = h.Vector()
        self.cai_soma_vec = None

    def all_secs(self):
        return [self.soma, self.dend_prox, self.dend_dist, self.ais, self.axon] + self.spine_necks + self.spines

    def add_spines_to_distal_dend(self, n_spines=10):
        for i in range(n_spines):
            neck = h.Section(name=f"{self.name}.spine_neck[{i}]")
            head = h.Section(name=f"{self.name}.spine_head[{i}]")

            #attach spine neck to distal dendrite, going along branch
            x = 0.1 + 0.8 * (i / max(1, n_spines - 1))  #~0.1 to ~0.9
            self.spine_xs.append(x)
            neck.connect(self.dend_dist(x))

            #attach spine head to end of spine neck
            head.connect(neck(1))

            #spine geometry
            neck.L, neck.diam, neck.nseg = 1.0, 0.2, 1
            head.L, head.diam, head.nseg = 0.5, 0.5, 1

            #passive membrane
            try_insert(neck, "pas")
            try_insert(head, "pas")
            if has_mech(neck, "pas"):
                for seg in neck:
                    seg.pas.g = 2.5e-05 #was 0.00039
                    seg.pas.e = -70.0 #was -70
            if has_mech(head, "pas"):
                for seg in head:
                    seg.pas.g = 0.00018 #was 0.00039
                    seg.pas.e = -80.0

            #give spines mechs
            try_insert(neck, "Caold")
            try_insert(head, "Caold")
            try_insert(neck, "Cabuffer")
            try_insert(head, "Cabuffer")
            try_insert(neck, "Cav12")
            try_insert(head, "Cav12")
            try_insert(neck, "Cav13")
            try_insert(head, "Cav13")
        #    try_insert(neck, "BK")
        #    try_insert(head, "BK")
        #    try_insert(neck, "BK_Cav22")
        #    try_insert(head, "BK_Cav22")
            try_insert(neck, "BK_Cav12")
            try_insert(head, "BK_Cav12")
        #    try_insert(neck, "BK_Cav21")
        #    try_insert(head, "BK_Cav21")
            try_insert(neck, "SK2")
            try_insert(head, "SK2")
            try_insert(neck, "HCN")
            try_insert(head, "HCN")
            try_insert(neck, "Kv42")
            try_insert(head, "Kv42")
            try_insert(neck, "Kv42b")
            try_insert(head, "Kv42b")
        #    try_insert(neck, "Kv11")
        #    try_insert(head, "Kv11")
        #    try_insert(neck, "Kir21")
        #    try_insert(head, "Kir21")
        #    try_insert(neck, "Kv14")
        #    try_insert(head, "Kv14")
        #    try_insert(neck, "Kv21")
        #    try_insert(head, "Kv21")
        #    try_insert(neck, "Kv33")
        #    try_insert(head, "Kv33")
        #    try_insert(neck, "Kv34")
        #    try_insert(head, "Kv34")
        #    try_insert(neck, "Kv723")
        #    try_insert(head, "Kv723")
            try_insert(neck, "ichan3")
            try_insert(head, "ichan3")
            try_insert(neck, "na8st")
            try_insert(head, "na8st")
        #    try_insert(neck, "Cav22")
        #    try_insert(head, "Cav22")
            try_insert(neck, "Cav32")
            try_insert(head, "Cav32")
        #    try_insert(neck, "Cav2_1")
        #    try_insert(head, "Cav2_1")

            #spine gbar
            if has_mech(head, "Cav12"):
                for seg in head:
                    seg.Cav12.gbar = 1e-5 #* 3.0
            if has_mech(neck, "Cav12"):
                for seg in neck:
                    seg.Cav12.gbar = 1e-5 #* 1.0

            if has_mech(head, "Cav13"):
                for seg in head:
                    seg.Cav13.gbar = 1e-9

            if has_mech(neck, "Cav13"):
                for seg in neck:
                    seg.Cav13.gbar = 1e-9

#            if has_mech(head, "BK"):
#                for seg in head:
#                    seg.BK.gakbar = 1e-4
#                    seg.BK.gabkbar = 1e-4
#            if has_mech(neck, "BK"):
#                for seg in neck:
#                    seg.BK.gakbar = 1e-4
#                    seg.BK.gabkbar = 1e-4

            apply_bk_split_to_section(
                head,
                total_bk_gakbar=3e-2 * self.bk_total_scale,
                total_bk_gabkbar=3e-2 * self.bk_total_scale,
                split=self.bk_split
            )

            apply_bk_split_to_section(
                neck,
                total_bk_gakbar=3e-2 * self.bk_total_scale,
                total_bk_gabkbar=3e-2 * self.bk_total_scale,
                split=self.bk_split
            )

            if has_mech(head, "SK2"):
                for seg in head:
                    seg.SK2.gkbar = 5e-6
            if has_mech(neck, "SK2"):
                for seg in neck:
                    seg.SK2.gkbar = 5e-6

            if has_mech(head, "HCN"):
                for seg in head:
                    seg.HCN.gbar = 1e-4
            if has_mech(neck, "HCN"):
                for seg in neck:
                    seg.HCN.gbar = 1e-4

            if has_mech(head, "Kv42"):
                for seg in head:
                    seg.Kv42.gkbar = 0.0 #1.5e-4
            if has_mech(neck, "Kv42"):
                for seg in neck:
                    seg.Kv42.gkbar = 0.0 #1.5e-4

            if has_mech(head, "Kv42b"):
                for seg in head:
                    seg.Kv42b.gkbar = 0.0 #1.5e-4
            if has_mech(neck, "Kv42b"):
                for seg in neck:
                    seg.Kv42b.gkbar = 0.0 #1.5e-4

            if has_mech(head, "Kv11"):
                for seg in head:
                    seg.Kv11.gkbar = 0.0 #1e-5
            if has_mech(neck, "Kv11"):
                for seg in neck:
                    seg.Kv11.gkbar = 0.0 #1e-5

            if has_mech(head, "Kir21"):
                for seg in head:
                    seg.Kir21.gkbar = 0.0 #1.5e-4
            if has_mech(neck, "Kir21"):
                for seg in neck:
                    seg.Kir21.gkbar = 0.0 #1.5e-4

            if has_mech(head, "Kv14"):
                for seg in head:
                    seg.Kv14.gkbar = 0.0 #1e-5
            if has_mech(neck, "Kv14"):
                for seg in neck:
                    seg.Kv14.gkbar = 0.0 #1e-5

            if has_mech(head, "Kv21"):
                for seg in head:
                    seg.Kv21.gkbar = 0.0 #3e-5
            if has_mech(neck, "Kv21"):
                for seg in neck:
                    seg.Kv21.gkbar = 0.0 #3e-5

            if has_mech(head, "Kv33"):
                for seg in head:
                    seg.Kv33.gkbar = 0.0 #2e-2
            if has_mech(neck, "Kv33"):
                for seg in neck:
                    seg.Kv33.gkbar = 0.0 #2e-2

            if has_mech(head, "Kv34"):
                for seg in head:
                    seg.Kv34.gkbar = 0.0 #2e-3
            if has_mech(neck, "Kv34"):
                for seg in neck:
                    seg.Kv34.gkbar = 0.0 #2e-3

            if has_mech(head, "Kv723"):
                for seg in head:
                    seg.Kv723.gkbar = 0.0 #2e-9
            if has_mech(neck, "Kv723"):
                for seg in neck:
                    seg.Kv723.gkbar = 0.0 #2e-9

            if has_mech(head, "ichan3"):
                for seg in head:
                    seg.ichan3.gnabar = 0.0 #5e-2
                    seg.ichan3.gkfbar = 0.0 #5e-4
                    seg.ichan3.gksbar = 0.0 #5e-4
                    seg.ichan3.gkabar = 0.0 #5e-4
            if has_mech(neck, "ichan3"):
                for seg in neck:
                    seg.ichan3.gnabar = 0.0 #5e-2
                    seg.ichan3.gkfbar = 0.0 #5e-4
                    seg.ichan3.gksbar = 0.0 #5e-4
                    seg.ichan3.gkabar = 0.0 #5e-4

            if has_mech(head, "na8st"):
                for seg in head:
                    seg.na8st.gbar = 0.0 #0.000001
            if has_mech(neck, "na8st"):
                for seg in neck:
                    seg.na8st.gbar = 0.0 #0.000001

            if has_mech(head, "Cav22"):
                for seg in head:
                    seg.Cav22.gbar = 0.0 #1e-5
            if has_mech(neck, "Cav22"):
                for seg in neck:
                    seg.Cav22.gbar = 0.0 #1e-5

            if has_mech(head, "Cav32"):
                for seg in head:
                    seg.Cav32.gbar = 1e-5
            if has_mech(neck, "Cav32"):
                for seg in neck:
                    seg.Cav32.gbar = 1e-5

            if has_mech(head, "Caold"):
                for seg in head:
                    seg.Caold.gtcabar = 1e-6  #tuuuuuuuuuuned
                    seg.Caold.gncabar = 1e-6
                    seg.Caold.glcabar = 1e-6

            if has_mech(neck, "Caold"):
                for seg in neck:
                    seg.Caold.gtcabar = 1e-6
                    seg.Caold.gncabar = 1e-6
                    seg.Caold.glcabar = 1e-6

            #Cabuffer param - sets buffering strength/kinetics
            if has_mech(head, "Cabuffer"):
                for seg in head:
                    seg.Cabuffer.tau = 8.0  #ms
                    seg.Cabuffer.brat = 0.6 #buffer ratio factor

            if has_mech(neck, "Cabuffer"):
                for seg in neck:
                    seg.Cabuffer.tau = 8.0
                    seg.Cabuffer.brat = 0.6

            #Cav2.1permeability baseline
            if has_mech(head, "Cav2_1"):
                for seg in head:
                    seg.Cav2_1.pcabar = 0.0 #1e-5

            if has_mech(neck, "Cav2_1"):
                for seg in neck:
                    seg.Cav2_1.pcabar = 0.0 #1e-5

            self.spine_necks.append(neck)
            self.spines.append(head)

    def _set_geometry(self):
        self.soma.diam = 20.0
        self.soma.L = self.soma.diam  #make the soma spherical i.e., make L = diam
        self.soma.nseg = 1

        self.dend_prox.L = 150.0
        self.dend_prox.diam = 2.0
        self.dend_prox.nseg = 9

        self.dend_dist.L = 200.0
        self.dend_dist.diam = 1.2
        self.dend_dist.nseg = 11

        self.ais.L = 30.0
        self.ais.diam = 1.0
        self.ais.nseg = 3

        self.axon.L = 300.0    #L = length
        self.axon.diam = 1.0   #diam = diameter
        self.axon.nseg = 11    #nseg = compartment

        for sec in self.all_secs():
            sec.Ra = 100.0       #Ra = axial resistivity
            sec.cm = 1.0         #cm = capacitance

    def _set_biophysics(self):
        #set passive membrane
        for sec in self.all_secs():
            try_insert(sec, "pas")
            if has_mech(sec, "pas"):
                for seg in sec:
                    seg.pas.g = 0.00039
                    seg.pas.e = -70.0

        #AIS specific passive override - use to tune/test, wond breaky overall
        if has_mech(self.ais, "pas"):
            for seg in self.ais:
                seg.pas.g = 0.00039
                seg.pas.e = -70.0


        #Hodgkin-Huxley(hh)-style mechanism
        #mammalian spiking - inbuilt NEURON is squid hh
        #uses na8st for fast Na, and ichan3 for additional Na/K current
        #acc na8st is markov

        for sec in [self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]:
            try_insert(sec, "na8st")
            try_insert(sec, "ichan3")

        #soma
        if has_mech(self.soma, "na8st"):
            for seg in self.soma:
                seg.na8st.gbar = 0.000001   #now it spikey woo

        if has_mech(self.soma, "ichan3"):
            for seg in self.soma:
                seg.ichan3.gnabar = 5e-2
                seg.ichan3.gkfbar = 5e-4
                seg.ichan3.gksbar = 5e-4
                seg.ichan3.gkabar = 5e-4

        #AIS
        if has_mech(self.ais, "na8st"):
            for seg in self.ais:
                seg.na8st.gbar = 0.000001 * 5.0 #AIS boost is 5x soma

        if has_mech(self.ais, "ichan3"):
            for seg in self.ais:
                seg.ichan3.gnabar = 5e-2 * 2.0  #AIS boost
                seg.ichan3.gkfbar = 5e-4
                seg.ichan3.gksbar = 5e-4
                seg.ichan3.gkabar = 5e-4

        #axon
        if has_mech(self.axon, "na8st"):
            for seg in self.axon:
                seg.na8st.gbar = 0.0000001 * 3.0

        if has_mech(self.axon, "ichan3"):
            for seg in self.axon:
                seg.ichan3.gnabar = 5e-2
                seg.ichan3.gkfbar = 5e-4
                seg.ichan3.gksbar = 5e-4
                seg.ichan3.gkabar = 5e-4

        #dend
        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "na8st"):
                for seg in sec:
                    seg.na8st.gbar = 0.000001 * 0.3

            if has_mech(sec, "ichan3"):
                for seg in sec:
                    seg.ichan3.gnabar = 5e-2 * 0.3 #was-3
                    seg.ichan3.gkfbar = 5e-4 * 0.3
                    seg.ichan3.gksbar = 5e-4 * 0.3
                    seg.ichan3.gkabar = 5e-4 * 0.3

        #adds mechanisms from Beining 2017
        for sec in [self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]:
            try_insert(sec, "Caold")
            try_insert(sec, "Cabuffer")
            try_insert(sec, "Cav12")
            try_insert(sec, "Cav13")
            try_insert(sec, "Cav22")
            try_insert(sec, "Cav32")
        #    try_insert(sec, "BK")
            try_insert(sec, "BK_Cav22")
            try_insert(sec, "BK_Cav12")
            try_insert(sec, "BK_Cav21")
            try_insert(sec, "SK2")
            try_insert(sec, "HCN")
            try_insert(sec, "Kv42")
            try_insert(sec, "Kv11")
            try_insert(sec, "Kir21")
            try_insert(sec, "Kv14")
            try_insert(sec, "Kv21")
            try_insert(sec, "Kv33")
            try_insert(sec, "Kv34")
            try_insert(sec, "Kv42b")
            try_insert(sec, "Kv723")
            try_insert(sec, "Cav2_1")

        self._set_channel_densities_default()

    #set baseline conductances
    def _set_channel_densities_default(self):
        for sec in [self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]:
            for seg in sec:
                if sec is self.soma:
                    scale = 1.0
                elif sec is self.ais:
                    scale = 1.3
                elif sec is self.axon:
                    scale = 1.1
                elif sec is self.dend_prox:
                    scale = 1.4
                elif sec is self.dend_dist:
                    scale = 1.7

                #Cav12 baseline
                if has_mech(sec, "Cav12"):
                    seg.Cav12.gbar = 1e-5 * scale

                #cav13
                if has_mech(sec, "Cav13"):
                    seg.Cav13.gbar = 1e-9 * scale

                #Cav22
                if has_mech(sec, "Cav22"):
                    seg.Cav22.gbar = 1e-5 * scale

                #Cav32
                if has_mech(sec, "Cav32"):
                    seg.Cav32.gbar = 1e-5 * scale

                #BK
#                if has_mech(sec, "BK"):
#                    seg.BK.gakbar = 1e-4 * scale
#                    seg.BK.gabkbar = 1e-4 * scale

                # coupled BK pool split across BK_Cav22, 21 & 12
                total_bk_gakbar = 3e-2 * scale * self.bk_total_scale #was 1e-4
                total_bk_gabkbar = 3e-2 * scale * self.bk_total_scale

                if has_mech(sec, "BK_Cav22") and has_mech(sec, "BK_Cav12") and has_mech(sec, "BK_Cav21"):
                    seg.BK_Cav22.gakbar = total_bk_gakbar * self.bk_split["BK_Cav22"]
                    seg.BK_Cav22.gabkbar = total_bk_gabkbar * self.bk_split["BK_Cav22"]

                    seg.BK_Cav12.gakbar = total_bk_gakbar * self.bk_split["BK_Cav12"]
                    seg.BK_Cav12.gabkbar = total_bk_gabkbar * self.bk_split["BK_Cav12"]

                    seg.BK_Cav21.gakbar = total_bk_gakbar * self.bk_split["BK_Cav21"]
                    seg.BK_Cav21.gabkbar = total_bk_gabkbar * self.bk_split["BK_Cav21"]

                #SK2
                if has_mech(sec, "SK2"):
                    seg.SK2.gkbar = 5e-6 * scale

                #HCN
                if has_mech(sec, "HCN"):
                    seg.HCN.gbar = 1e-4 * scale

                #Kv42 & Kv42b
                if has_mech(sec, "Kv42"):
                    seg.Kv42.gkbar = 1.5e-4 * scale
                if has_mech(sec, "Kv42b"):
                    seg.Kv42b.gkbar = 1.5e-4 * scale

                #Kv11
                if has_mech(sec, "Kv11"):
                    seg.Kv11.gkbar = 1e-5 * scale

                #Kv14
                if has_mech(sec, "Kv14"):
                    seg.Kv14.gkbar = 1e-5 * scale

                #Kv21
                if has_mech(sec, "Kv21"):
                    seg.Kv21.gkbar = 3e-5 * scale

                #Kv33 & Kv34
                if has_mech(sec, "Kv33"):
                    seg.Kv33.gkbar = 2e-2 * scale
                if has_mech(sec, "Kv34"):
                    seg.Kv34.gkbar = 2e-3 * scale

                #Kv723 (KCNQ/M-current style)
                if has_mech(sec, "Kv723"):
                    seg.Kv723.gkbar = 2e-9 * scale

                #Kir21
                if has_mech(sec, "Kir21"):
                    seg.Kir21.gkbar = 1.5e-4 * scale

                #Caold conductances
                if has_mech(sec, "Caold"):
                    seg.Caold.gtcabar = 1e-6 * scale
                    seg.Caold.gncabar = 1e-6 * scale
                    seg.Caold.glcabar = 1e-6 * scale

                #Cabuffer params - sets buffering strength/kinetics
                if has_mech(sec, "Cabuffer"):
                    seg.Cabuffer.tau = 8.0  #ms
                    seg.Cabuffer.brat = 0.1  #buffer ratio factor

                #Cav2.1 permeability density, cm/s
                if has_mech(sec, "Cav2_1"):
                    seg.Cav2_1.pcabar = 1e-5 * scale  #-6 = like w/ no cav21, -5 big diff & -4 break
                    seg.Cav2_1.vshift = 0.0

    def scale_cav12(self, factor: float):
        for sec in [self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist] +self.spine_necks +self.spines:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar *= factor

    def add_current_clamp(self, delay=100.0, dur=300.0, amp=0.3, sec=None, loc=0.5):
        sec = self.soma if sec is None else sec
        self.iclamp = h.IClamp(sec(loc))
        self.iclamp.delay = delay
        self.iclamp.dur = dur   #dur = duration
        self.iclamp.amp = amp  #unit = nA

    def setup_recording(self):
        # time
        self.t_vec = h.Vector()
        self.t_vec.record(h._ref_t)

        # --------------------------------------------------
        # voltages
        # --------------------------------------------------
        self.vsoma_vec = h.Vector()
        self.vsoma_vec.record(self.soma(0.5)._ref_v)

        self.vais_vec = h.Vector()
        self.vais_vec.record(self.ais(0.5)._ref_v)

        self.vax_vec = h.Vector()
        self.vax_vec.record(self.axon(0.5)._ref_v)

        self.vprox_vec = h.Vector()
        self.vprox_vec.record(self.dend_prox(0.5)._ref_v)

        self.vdend_vec = h.Vector()
        self.vdend_vec.record(self.dend_dist(0.9)._ref_v)

        self.vspine_vec = h.Vector()
        self.vspine_vec.record(self.spines[0](0.5)._ref_v)

        # --------------------------------------------------
        # intracellular calcium
        # --------------------------------------------------
        try:
            _ = self.soma(0.5)._ref_cai

            self.cai_soma_vec = h.Vector()
            self.cai_soma_vec.record(self.soma(0.5)._ref_cai)

            self.cai_ais_vec = h.Vector()
            self.cai_ais_vec.record(self.ais(0.5)._ref_cai)

            self.cai_axon_vec = h.Vector()
            self.cai_axon_vec.record(self.axon(0.5)._ref_cai)

            self.cai_prox_vec = h.Vector()
            self.cai_prox_vec.record(self.dend_prox(0.5)._ref_cai)

            self.cai_dist_vec = h.Vector()
            self.cai_dist_vec.record(self.dend_dist(0.9)._ref_cai)

            self.cai_spine_vec = h.Vector()
            self.cai_spine_vec.record(self.spines[0](0.5)._ref_cai)

            self.cai_neck_vec = h.Vector()
            self.cai_neck_vec.record(self.spine_necks[0](0.5)._ref_cai)

        except Exception as e:
            print("Calcium recording set up failed becasue:", e)
            self.cai_soma_vec = None
            self.cai_ais_vec = None
            self.cai_axon_vec = None
            self.cai_prox_vec = None
            self.cai_dist_vec = None
            self.cai_spine_vec = None
            self.cai_neck_vec = None

        # --------------------------------------------------
        # total ionic currents
        # --------------------------------------------------
        self.ica_soma_vec = h.Vector()
        self.ica_soma_vec.record(self.soma(0.5)._ref_ica)

        self.ica_ais_vec = h.Vector()
        self.ica_ais_vec.record(self.ais(0.5)._ref_ica)

        self.ica_axon_vec = h.Vector()
        self.ica_axon_vec.record(self.axon(0.5)._ref_ica)

        self.ica_prox_vec = h.Vector()
        self.ica_prox_vec.record(self.dend_prox(0.5)._ref_ica)

        self.ica_dist_vec = h.Vector()
        self.ica_dist_vec.record(self.dend_dist(0.9)._ref_ica)

        self.ica_spine_vec = h.Vector()
        self.ica_spine_vec.record(self.spines[0](0.5)._ref_ica)

        self.ik_soma_vec = h.Vector()
        self.ik_soma_vec.record(self.soma(0.5)._ref_ik)

        self.ina_soma_vec = h.Vector()
        self.ina_soma_vec.record(self.soma(0.5)._ref_ina)

        # --------------------------------------------------
        # na8st Markov state proxies
        # --------------------------------------------------
        self.na8st_o_soma_vec = None
        self.na8st_g_soma_vec = None
        self.na8st_i_soma_vecs = None

        self.na8st_o_ais_vec = None
        self.na8st_g_ais_vec = None
        self.na8st_i_ais_vecs = None

        def _record_na8st_states(sec, loc=0.5):
            if not has_mech(sec, "na8st"):
                return None, None, None

            o_vec = h.Vector()
            o_vec.record(sec(loc).na8st._ref_o)

            g_vec = h.Vector()
            g_vec.record(sec(loc).na8st._ref_g)

            i_vecs = []
            for k in range(1, 7):
                v = h.Vector()
                v.record(getattr(sec(loc).na8st, f"_ref_i{k}"))
                i_vecs.append(v)

            return o_vec, g_vec, i_vecs

        self.na8st_o_soma_vec, self.na8st_g_soma_vec, self.na8st_i_soma_vecs = _record_na8st_states(self.soma, 0.5)
        self.na8st_o_ais_vec, self.na8st_g_ais_vec, self.na8st_i_ais_vecs = _record_na8st_states(self.ais, 0.5)

        # --------------------------------------------------
        # BK local Ca at soma
        # --------------------------------------------------
        self.bk_acai22_soma_vec = None
        if has_mech(self.soma, "BK_Cav22"):
            try:
                self.bk_acai22_soma_vec = h.Vector()
                self.bk_acai22_soma_vec.record(self.soma(0.5).BK_Cav22._ref_acai)
            except Exception as e:
                print("FAILED recording BK_Cav22 acai:", e)

        self.bk_acai12_soma_vec = None
        if has_mech(self.soma, "BK_Cav12"):
            try:
                self.bk_acai12_soma_vec = h.Vector()
                self.bk_acai12_soma_vec.record(self.soma(0.5).BK_Cav12._ref_acai)
            except Exception as e:
                print("FAILED recording BK_Cav12 acai:", e)

        self.bk_acai21_soma_vec = None
        if has_mech(self.soma, "BK_Cav21"):
            try:
                self.bk_acai21_soma_vec = h.Vector()
                self.bk_acai21_soma_vec.record(self.soma(0.5).BK_Cav21._ref_acai)
            except Exception as e:
                print("FAILED recording BK_Cav21 acai:", e)

        # --------------------------------------------------
        # BK currents at soma
        # --------------------------------------------------
        self.bk_Cav22_ik_soma_vec = None
        if has_mech(self.soma, "BK_Cav22"):
            self.bk_Cav22_ik_soma_vec = h.Vector()
            self.bk_Cav22_ik_soma_vec.record(self.soma(0.5).BK_Cav22._ref_ik)

        self.bk_Cav12_ik_soma_vec = None
        if has_mech(self.soma, "BK_Cav12"):
            self.bk_Cav12_ik_soma_vec = h.Vector()
            self.bk_Cav12_ik_soma_vec.record(self.soma(0.5).BK_Cav12._ref_ik)

        self.bk_Cav21_ik_soma_vec = None
        if has_mech(self.soma, "BK_Cav21"):
            self.bk_Cav21_ik_soma_vec = h.Vector()
            self.bk_Cav21_ik_soma_vec.record(self.soma(0.5).BK_Cav21._ref_ik)

        # --------------------------------------------------
        # BK currents by compartment
        # --------------------------------------------------
        self.bk_Cav22_ik_ais_vec = None
        self.bk_Cav22_ik_axon_vec = None
        self.bk_Cav22_ik_prox_vec = None
        self.bk_Cav22_ik_dist_vec = None
        self.bk_Cav22_ik_spine_vec = None

        if has_mech(self.ais, "BK_Cav22"):
            self.bk_Cav22_ik_ais_vec = h.Vector()
            self.bk_Cav22_ik_ais_vec.record(self.ais(0.5).BK_Cav22._ref_ik)
        if has_mech(self.axon, "BK_Cav22"):
            self.bk_Cav22_ik_axon_vec = h.Vector()
            self.bk_Cav22_ik_axon_vec.record(self.axon(0.5).BK_Cav22._ref_ik)
        if has_mech(self.dend_prox, "BK_Cav22"):
            self.bk_Cav22_ik_prox_vec = h.Vector()
            self.bk_Cav22_ik_prox_vec.record(self.dend_prox(0.5).BK_Cav22._ref_ik)
        if has_mech(self.dend_dist, "BK_Cav22"):
            self.bk_Cav22_ik_dist_vec = h.Vector()
            self.bk_Cav22_ik_dist_vec.record(self.dend_dist(0.9).BK_Cav22._ref_ik)
        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav22"):
            self.bk_Cav22_ik_spine_vec = h.Vector()
            self.bk_Cav22_ik_spine_vec.record(self.spines[0](0.5).BK_Cav22._ref_ik)

        self.bk_Cav12_ik_ais_vec = None
        self.bk_Cav12_ik_axon_vec = None
        self.bk_Cav12_ik_prox_vec = None
        self.bk_Cav12_ik_dist_vec = None
        self.bk_Cav12_ik_spine_vec = None

        if has_mech(self.ais, "BK_Cav12"):
            self.bk_Cav12_ik_ais_vec = h.Vector()
            self.bk_Cav12_ik_ais_vec.record(self.ais(0.5).BK_Cav12._ref_ik)
        if has_mech(self.axon, "BK_Cav12"):
            self.bk_Cav12_ik_axon_vec = h.Vector()
            self.bk_Cav12_ik_axon_vec.record(self.axon(0.5).BK_Cav12._ref_ik)
        if has_mech(self.dend_prox, "BK_Cav12"):
            self.bk_Cav12_ik_prox_vec = h.Vector()
            self.bk_Cav12_ik_prox_vec.record(self.dend_prox(0.5).BK_Cav12._ref_ik)
        if has_mech(self.dend_dist, "BK_Cav12"):
            self.bk_Cav12_ik_dist_vec = h.Vector()
            self.bk_Cav12_ik_dist_vec.record(self.dend_dist(0.9).BK_Cav12._ref_ik)
        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav12"):
            self.bk_Cav12_ik_spine_vec = h.Vector()
            self.bk_Cav12_ik_spine_vec.record(self.spines[0](0.5).BK_Cav12._ref_ik)

        self.bk_Cav21_ik_ais_vec = None
        self.bk_Cav21_ik_axon_vec = None
        self.bk_Cav21_ik_prox_vec = None
        self.bk_Cav21_ik_dist_vec = None
        self.bk_Cav21_ik_spine_vec = None

        if has_mech(self.ais, "BK_Cav21"):
            self.bk_Cav21_ik_ais_vec = h.Vector()
            self.bk_Cav21_ik_ais_vec.record(self.ais(0.5).BK_Cav21._ref_ik)
        if has_mech(self.axon, "BK_Cav21"):
            self.bk_Cav21_ik_axon_vec = h.Vector()
            self.bk_Cav21_ik_axon_vec.record(self.axon(0.5).BK_Cav21._ref_ik)
        if has_mech(self.dend_prox, "BK_Cav21"):
            self.bk_Cav21_ik_prox_vec = h.Vector()
            self.bk_Cav21_ik_prox_vec.record(self.dend_prox(0.5).BK_Cav21._ref_ik)
        if has_mech(self.dend_dist, "BK_Cav21"):
            self.bk_Cav21_ik_dist_vec = h.Vector()
            self.bk_Cav21_ik_dist_vec.record(self.dend_dist(0.9).BK_Cav21._ref_ik)
        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav21"):
            self.bk_Cav21_ik_spine_vec = h.Vector()
            self.bk_Cav21_ik_spine_vec.record(self.spines[0](0.5).BK_Cav21._ref_ik)

        # --------------------------------------------------
        # BK local acai by compartment
        # --------------------------------------------------
        self.bk_acai22_ais_vec = None
        self.bk_acai22_axon_vec = None
        self.bk_acai22_prox_vec = None
        self.bk_acai22_dist_vec = None
        self.bk_acai22_spine_vec = None

        if has_mech(self.ais, "BK_Cav22"):
            self.bk_acai22_ais_vec = h.Vector()
            self.bk_acai22_ais_vec.record(self.ais(0.5).BK_Cav22._ref_acai)
        if has_mech(self.axon, "BK_Cav22"):
            self.bk_acai22_axon_vec = h.Vector()
            self.bk_acai22_axon_vec.record(self.axon(0.5).BK_Cav22._ref_acai)
        if has_mech(self.dend_prox, "BK_Cav22"):
            self.bk_acai22_prox_vec = h.Vector()
            self.bk_acai22_prox_vec.record(self.dend_prox(0.5).BK_Cav22._ref_acai)
        if has_mech(self.dend_dist, "BK_Cav22"):
            self.bk_acai22_dist_vec = h.Vector()
            self.bk_acai22_dist_vec.record(self.dend_dist(0.9).BK_Cav22._ref_acai)
        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav22"):
            self.bk_acai22_spine_vec = h.Vector()
            self.bk_acai22_spine_vec.record(self.spines[0](0.5).BK_Cav22._ref_acai)

        self.bk_acai12_ais_vec = None
        self.bk_acai12_axon_vec = None
        self.bk_acai12_prox_vec = None
        self.bk_acai12_dist_vec = None
        self.bk_acai12_spine_vec = None

        if has_mech(self.ais, "BK_Cav12"):
            self.bk_acai12_ais_vec = h.Vector()
            self.bk_acai12_ais_vec.record(self.ais(0.5).BK_Cav12._ref_acai)
        if has_mech(self.axon, "BK_Cav12"):
            self.bk_acai12_axon_vec = h.Vector()
            self.bk_acai12_axon_vec.record(self.axon(0.5).BK_Cav12._ref_acai)
        if has_mech(self.dend_prox, "BK_Cav12"):
            self.bk_acai12_prox_vec = h.Vector()
            self.bk_acai12_prox_vec.record(self.dend_prox(0.5).BK_Cav12._ref_acai)
        if has_mech(self.dend_dist, "BK_Cav12"):
            self.bk_acai12_dist_vec = h.Vector()
            self.bk_acai12_dist_vec.record(self.dend_dist(0.9).BK_Cav12._ref_acai)
        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav12"):
            self.bk_acai12_spine_vec = h.Vector()
            self.bk_acai12_spine_vec.record(self.spines[0](0.5).BK_Cav12._ref_acai)

        self.bk_acai21_ais_vec = None
        self.bk_acai21_axon_vec = None
        self.bk_acai21_prox_vec = None
        self.bk_acai21_dist_vec = None
        self.bk_acai21_spine_vec = None

        if has_mech(self.ais, "BK_Cav21"):
            self.bk_acai21_ais_vec = h.Vector()
            self.bk_acai21_ais_vec.record(self.ais(0.5).BK_Cav21._ref_acai)
        if has_mech(self.axon, "BK_Cav21"):
            self.bk_acai21_axon_vec = h.Vector()
            self.bk_acai21_axon_vec.record(self.axon(0.5).BK_Cav21._ref_acai)
        if has_mech(self.dend_prox, "BK_Cav21"):
            self.bk_acai21_prox_vec = h.Vector()
            self.bk_acai21_prox_vec.record(self.dend_prox(0.5).BK_Cav21._ref_acai)
        if has_mech(self.dend_dist, "BK_Cav21"):
            self.bk_acai21_dist_vec = h.Vector()
            self.bk_acai21_dist_vec.record(self.dend_dist(0.9).BK_Cav21._ref_acai)
        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav21"):
            self.bk_acai21_spine_vec = h.Vector()
            self.bk_acai21_spine_vec.record(self.spines[0](0.5).BK_Cav21._ref_acai)

        # --------------------------------------------------
        # SK currents + acai
        # --------------------------------------------------
        self.sk_ik_soma_vec = None
        self.sk_ik_ais_vec = None
        self.sk_ik_axon_vec = None
        self.sk_ik_prox_vec = None
        self.sk_ik_dist_vec = None
        self.sk_ik_spine_vec = None

        self.sk_acai_soma_vec = None
        self.sk_acai_ais_vec = None
        self.sk_acai_axon_vec = None
        self.sk_acai_prox_vec = None
        self.sk_acai_dist_vec = None
        self.sk_acai_spine_vec = None

        if has_mech(self.soma, "SK2"):
            self.sk_ik_soma_vec = h.Vector()
            self.sk_ik_soma_vec.record(self.soma(0.5).SK2._ref_ik)
            self.sk_acai_soma_vec = h.Vector()
            self.sk_acai_soma_vec.record(self.soma(0.5).SK2._ref_acai)

        if has_mech(self.ais, "SK2"):
            self.sk_ik_ais_vec = h.Vector()
            self.sk_ik_ais_vec.record(self.ais(0.5).SK2._ref_ik)
            self.sk_acai_ais_vec = h.Vector()
            self.sk_acai_ais_vec.record(self.ais(0.5).SK2._ref_acai)

        if has_mech(self.axon, "SK2"):
            self.sk_ik_axon_vec = h.Vector()
            self.sk_ik_axon_vec.record(self.axon(0.5).SK2._ref_ik)
            self.sk_acai_axon_vec = h.Vector()
            self.sk_acai_axon_vec.record(self.axon(0.5).SK2._ref_acai)

        if has_mech(self.dend_prox, "SK2"):
            self.sk_ik_prox_vec = h.Vector()
            self.sk_ik_prox_vec.record(self.dend_prox(0.5).SK2._ref_ik)
            self.sk_acai_prox_vec = h.Vector()
            self.sk_acai_prox_vec.record(self.dend_prox(0.5).SK2._ref_acai)

        if has_mech(self.dend_dist, "SK2"):
            self.sk_ik_dist_vec = h.Vector()
            self.sk_ik_dist_vec.record(self.dend_dist(0.9).SK2._ref_ik)
            self.sk_acai_dist_vec = h.Vector()
            self.sk_acai_dist_vec.record(self.dend_dist(0.9).SK2._ref_acai)

        if len(self.spines) > 0 and has_mech(self.spines[0], "SK2"):
            self.sk_ik_spine_vec = h.Vector()
            self.sk_ik_spine_vec.record(self.spines[0](0.5).SK2._ref_ik)
            self.sk_acai_spine_vec = h.Vector()
            self.sk_acai_spine_vec.record(self.spines[0](0.5).SK2._ref_acai)

        # --------------------------------------------------
        # CaV source currents by compartment
        # --------------------------------------------------
        self.cav12_ica_soma_vec = None
        self.cav12_ica_ais_vec = None
        self.cav12_ica_axon_vec = None
        self.cav12_ica_prox_vec = None
        self.cav12_ica_dist_vec = None
        self.cav12_ica_spine_vec = None

        if has_mech(self.soma, "Cav12"):
            self.cav12_ica_soma_vec = h.Vector()
            self.cav12_ica_soma_vec.record(self.soma(0.5)._ref_ilca)
        if has_mech(self.ais, "Cav12"):
            self.cav12_ica_ais_vec = h.Vector()
            self.cav12_ica_ais_vec.record(self.ais(0.5)._ref_ilca)
        if has_mech(self.axon, "Cav12"):
            self.cav12_ica_axon_vec = h.Vector()
            self.cav12_ica_axon_vec.record(self.axon(0.5)._ref_ilca)
        if has_mech(self.dend_prox, "Cav12"):
            self.cav12_ica_prox_vec = h.Vector()
            self.cav12_ica_prox_vec.record(self.dend_prox(0.5)._ref_ilca)
        if has_mech(self.dend_dist, "Cav12"):
            self.cav12_ica_dist_vec = h.Vector()
            self.cav12_ica_dist_vec.record(self.dend_dist(0.9)._ref_ilca)
        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav12"):
            self.cav12_ica_spine_vec = h.Vector()
            self.cav12_ica_spine_vec.record(self.spines[0](0.5)._ref_ilca)

        self.cav13_ica_soma_vec = None
        self.cav13_ica_ais_vec = None
        self.cav13_ica_axon_vec = None
        self.cav13_ica_prox_vec = None
        self.cav13_ica_dist_vec = None
        self.cav13_ica_spine_vec = None

        if has_mech(self.soma, "Cav13"):
            self.cav13_ica_soma_vec = h.Vector()
            self.cav13_ica_soma_vec.record(self.soma(0.5)._ref_ilca13)
        if has_mech(self.ais, "Cav13"):
            self.cav13_ica_ais_vec = h.Vector()
            self.cav13_ica_ais_vec.record(self.ais(0.5)._ref_ilca13)
        if has_mech(self.axon, "Cav13"):
            self.cav13_ica_axon_vec = h.Vector()
            self.cav13_ica_axon_vec.record(self.axon(0.5)._ref_ilca13)
        if has_mech(self.dend_prox, "Cav13"):
            self.cav13_ica_prox_vec = h.Vector()
            self.cav13_ica_prox_vec.record(self.dend_prox(0.5)._ref_ilca13)
        if has_mech(self.dend_dist, "Cav13"):
            self.cav13_ica_dist_vec = h.Vector()
            self.cav13_ica_dist_vec.record(self.dend_dist(0.9)._ref_ilca13)
        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav13"):
            self.cav13_ica_spine_vec = h.Vector()
            self.cav13_ica_spine_vec.record(self.spines[0](0.5)._ref_ilca13)

        self.cav21_ica_soma_vec = None
        self.cav21_ica_ais_vec = None
        self.cav21_ica_axon_vec = None
        self.cav21_ica_prox_vec = None
        self.cav21_ica_dist_vec = None
        self.cav21_ica_spine_vec = None

        if has_mech(self.soma, "Cav2_1"):
            self.cav21_ica_soma_vec = h.Vector()
            self.cav21_ica_soma_vec.record(self.soma(0.5).Cav2_1._ref_ipca)
        if has_mech(self.ais, "Cav2_1"):
            self.cav21_ica_ais_vec = h.Vector()
            self.cav21_ica_ais_vec.record(self.ais(0.5).Cav2_1._ref_ipca)
        if has_mech(self.axon, "Cav2_1"):
            self.cav21_ica_axon_vec = h.Vector()
            self.cav21_ica_axon_vec.record(self.axon(0.5).Cav2_1._ref_ipca)
        if has_mech(self.dend_prox, "Cav2_1"):
            self.cav21_ica_prox_vec = h.Vector()
            self.cav21_ica_prox_vec.record(self.dend_prox(0.5).Cav2_1._ref_ipca)
        if has_mech(self.dend_dist, "Cav2_1"):
            self.cav21_ica_dist_vec = h.Vector()
            self.cav21_ica_dist_vec.record(self.dend_dist(0.9).Cav2_1._ref_ipca)
        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav2_1"):
            self.cav21_ica_spine_vec = h.Vector()
            self.cav21_ica_spine_vec.record(self.spines[0](0.5).Cav2_1._ref_ipca)

        self.cav22_ica_soma_vec = None
        self.cav22_ica_ais_vec = None
        self.cav22_ica_axon_vec = None
        self.cav22_ica_prox_vec = None
        self.cav22_ica_dist_vec = None
        self.cav22_ica_spine_vec = None

        if has_mech(self.soma, "Cav22"):
            self.cav22_ica_soma_vec = h.Vector()
            self.cav22_ica_soma_vec.record(self.soma(0.5)._ref_inca)
        if has_mech(self.ais, "Cav22"):
            self.cav22_ica_ais_vec = h.Vector()
            self.cav22_ica_ais_vec.record(self.ais(0.5)._ref_inca)
        if has_mech(self.axon, "Cav22"):
            self.cav22_ica_axon_vec = h.Vector()
            self.cav22_ica_axon_vec.record(self.axon(0.5)._ref_inca)
        if has_mech(self.dend_prox, "Cav22"):
            self.cav22_ica_prox_vec = h.Vector()
            self.cav22_ica_prox_vec.record(self.dend_prox(0.5)._ref_inca)
        if has_mech(self.dend_dist, "Cav22"):
            self.cav22_ica_dist_vec = h.Vector()
            self.cav22_ica_dist_vec.record(self.dend_dist(0.9)._ref_inca)
        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav22"):
            self.cav22_ica_spine_vec = h.Vector()
            self.cav22_ica_spine_vec.record(self.spines[0](0.5)._ref_inca)

        self.cav32_ica_soma_vec = None
        self.cav32_ica_ais_vec = None
        self.cav32_ica_axon_vec = None
        self.cav32_ica_prox_vec = None
        self.cav32_ica_dist_vec = None
        self.cav32_ica_spine_vec = None

        if has_mech(self.soma, "Cav32"):
            self.cav32_ica_soma_vec = h.Vector()
            self.cav32_ica_soma_vec.record(self.soma(0.5)._ref_ica)
        if has_mech(self.ais, "Cav32"):
            self.cav32_ica_ais_vec = h.Vector()
            self.cav32_ica_ais_vec.record(self.ais(0.5)._ref_ica)
        if has_mech(self.axon, "Cav32"):
            self.cav32_ica_axon_vec = h.Vector()
            self.cav32_ica_axon_vec.record(self.axon(0.5)._ref_ica)
        if has_mech(self.dend_prox, "Cav32"):
            self.cav32_ica_prox_vec = h.Vector()
            self.cav32_ica_prox_vec.record(self.dend_prox(0.5)._ref_ica)
        if has_mech(self.dend_dist, "Cav32"):
            self.cav32_ica_dist_vec = h.Vector()
            self.cav32_ica_dist_vec.record(self.dend_dist(0.9)._ref_ica)
        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav32"):
            self.cav32_ica_spine_vec = h.Vector()
            self.cav32_ica_spine_vec.record(self.spines[0](0.5)._ref_ica)


def run_sim(cell: DGGranuleLikeCell, tstop=500.0, v_init=-70.0, dt=0.025):
    h.dt = dt
    h.tstop = tstop
    h.finitialize(v_init)
    h.frecord_init()  #makes rec vectors start cleanly at initialized state
    h.continuerun(tstop)

    t = np.array(cell.t_vec)

    vs = np.array(cell.vsoma_vec)
    vais = np.array(cell.vais_vec) if getattr(cell, "vais_vec", None) is not None else None
    vax = np.array(cell.vax_vec) if getattr(cell, "vax_vec", None) is not None else None
    vd = np.array(cell.vdend_vec)
    vp = np.array(cell.vprox_vec)
    vsp = np.array(cell.vspine_vec)

    cai_soma = np.array(cell.cai_soma_vec) if getattr(cell, "cai_soma_vec", None) is not None else None
    cai_ais = np.array(cell.cai_ais_vec) if getattr(cell, "cai_ais_vec", None) is not None else None
    cai_axon = np.array(cell.cai_axon_vec) if getattr(cell, "cai_axon_vec", None) is not None else None
    cai_prox = np.array(cell.cai_prox_vec) if getattr(cell, "cai_prox_vec", None) is not None else None
    cai_dist = np.array(cell.cai_dist_vec) if getattr(cell, "cai_dist_vec", None) is not None else None
    cai_spine = np.array(cell.cai_spine_vec) if getattr(cell, "cai_spine_vec", None) is not None else None

    ica_soma = np.array(cell.ica_soma_vec) if getattr(cell, "ica_soma_vec", None) is not None else None
    ica_ais = np.array(cell.ica_ais_vec) if getattr(cell, "ica_ais_vec", None) is not None else None
    ica_axon = np.array(cell.ica_axon_vec) if getattr(cell, "ica_axon_vec", None) is not None else None
    ica_prox = np.array(cell.ica_prox_vec) if getattr(cell, "ica_prox_vec", None) is not None else None
    ica_dist = np.array(cell.ica_dist_vec) if getattr(cell, "ica_dist_vec", None) is not None else None
    ica_spine = np.array(cell.ica_spine_vec) if getattr(cell, "ica_spine_vec", None) is not None else None

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

    na8st_o_soma = np.array(cell.na8st_o_soma_vec) if getattr(cell, "na8st_o_soma_vec", None) is not None else None
    na8st_g_soma = np.array(cell.na8st_g_soma_vec) if getattr(cell, "na8st_g_soma_vec", None) is not None else None
    na8st_o_ais = np.array(cell.na8st_o_ais_vec) if getattr(cell, "na8st_o_ais_vec", None) is not None else None
    na8st_g_ais = np.array(cell.na8st_g_ais_vec) if getattr(cell, "na8st_g_ais_vec", None) is not None else None

    na8st_i_soma = None
    if getattr(cell, "na8st_i_soma_vecs", None) is not None:
        na8st_i_soma = [np.array(v) for v in cell.na8st_i_soma_vecs]

    na8st_i_ais = None
    if getattr(cell, "na8st_i_ais_vecs", None) is not None:
        na8st_i_ais = [np.array(v) for v in cell.na8st_i_ais_vecs]

    na8st_i_total_soma = None
    if na8st_i_soma is not None:
        na8st_i_total_soma = np.sum(np.vstack(na8st_i_soma), axis=0)

    na8st_i_total_ais = None
    if na8st_i_ais is not None:
        na8st_i_total_ais = np.sum(np.vstack(na8st_i_ais), axis=0)

    sk_acai_soma = np.array(cell.sk_acai_soma_vec) if getattr(cell, "sk_acai_soma_vec", None) is not None else None
    sk_ik_ais = np.array(cell.sk_ik_ais_vec) if getattr(cell, "sk_ik_ais_vec", None) is not None else None
    sk_ik_axon = np.array(cell.sk_ik_axon_vec) if getattr(cell, "sk_ik_axon_vec", None) is not None else None
    sk_ik_prox = np.array(cell.sk_ik_prox_vec) if getattr(cell, "sk_ik_prox_vec", None) is not None else None
    sk_ik_dist = np.array(cell.sk_ik_dist_vec) if getattr(cell, "sk_ik_dist_vec", None) is not None else None
    sk_ik_spine = np.array(cell.sk_ik_spine_vec) if getattr(cell, "sk_ik_spine_vec", None) is not None else None

    sk_acai_ais = np.array(cell.sk_acai_ais_vec) if getattr(cell, "sk_acai_ais_vec", None) is not None else None
    sk_acai_axon = np.array(cell.sk_acai_axon_vec) if getattr(cell, "sk_acai_axon_vec", None) is not None else None
    sk_acai_prox = np.array(cell.sk_acai_prox_vec) if getattr(cell, "sk_acai_prox_vec", None) is not None else None
    sk_acai_dist = np.array(cell.sk_acai_dist_vec) if getattr(cell, "sk_acai_dist_vec", None) is not None else None
    sk_acai_spine = np.array(cell.sk_acai_spine_vec) if getattr(cell, "sk_acai_spine_vec", None) is not None else None

    cav12_ica_soma = np.array(cell.cav12_ica_soma_vec) if getattr(cell, "cav12_ica_soma_vec", None) is not None else None
    cav12_ica_ais = np.array(cell.cav12_ica_ais_vec) if getattr(cell, "cav12_ica_ais_vec", None) is not None else None
    cav12_ica_axon = np.array(cell.cav12_ica_axon_vec) if getattr(cell, "cav12_ica_axon_vec", None) is not None else None
    cav12_ica_prox = np.array(cell.cav12_ica_prox_vec) if getattr(cell, "cav12_ica_prox_vec", None) is not None else None
    cav12_ica_dist = np.array(cell.cav12_ica_dist_vec) if getattr(cell, "cav12_ica_dist_vec", None) is not None else None
    cav12_ica_spine = np.array(cell.cav12_ica_spine_vec) if getattr(cell, "cav12_ica_spine_vec", None) is not None else None

    cav13_ica_soma = np.array(cell.cav13_ica_soma_vec) if getattr(cell, "cav13_ica_soma_vec", None) is not None else None
    cav13_ica_ais = np.array(cell.cav13_ica_ais_vec) if getattr(cell, "cav13_ica_ais_vec", None) is not None else None
    cav13_ica_axon = np.array(cell.cav13_ica_axon_vec) if getattr(cell, "cav13_ica_axon_vec", None) is not None else None
    cav13_ica_prox = np.array(cell.cav13_ica_prox_vec) if getattr(cell, "cav13_ica_prox_vec", None) is not None else None
    cav13_ica_dist = np.array(cell.cav13_ica_dist_vec) if getattr(cell, "cav13_ica_dist_vec", None) is not None else None
    cav13_ica_spine = np.array(cell.cav13_ica_spine_vec) if getattr(cell, "cav13_ica_spine_vec", None) is not None else None

    cav21_ica_soma = np.array(cell.cav21_ica_soma_vec) if getattr(cell, "cav21_ica_soma_vec", None) is not None else None
    cav21_ica_ais = np.array(cell.cav21_ica_ais_vec) if getattr(cell, "cav21_ica_ais_vec", None) is not None else None
    cav21_ica_axon = np.array(cell.cav21_ica_axon_vec) if getattr(cell, "cav21_ica_axon_vec", None) is not None else None
    cav21_ica_prox = np.array(cell.cav21_ica_prox_vec) if getattr(cell, "cav21_ica_prox_vec", None) is not None else None
    cav21_ica_dist = np.array(cell.cav21_ica_dist_vec) if getattr(cell, "cav21_ica_dist_vec", None) is not None else None
    cav21_ica_spine = np.array(cell.cav21_ica_spine_vec) if getattr(cell, "cav21_ica_spine_vec", None) is not None else None

    cav22_ica_soma = np.array(cell.cav22_ica_soma_vec) if getattr(cell, "cav22_ica_soma_vec", None) is not None else None
    cav22_ica_ais = np.array(cell.cav22_ica_ais_vec) if getattr(cell, "cav22_ica_ais_vec", None) is not None else None
    cav22_ica_axon = np.array(cell.cav22_ica_axon_vec) if getattr(cell, "cav22_ica_axon_vec", None) is not None else None
    cav22_ica_prox = np.array(cell.cav22_ica_prox_vec) if getattr(cell, "cav22_ica_prox_vec", None) is not None else None
    cav22_ica_dist = np.array(cell.cav22_ica_dist_vec) if getattr(cell, "cav22_ica_dist_vec", None) is not None else None
    cav22_ica_spine = np.array(cell.cav22_ica_spine_vec) if getattr(cell, "cav22_ica_spine_vec", None) is not None else None

    cav32_ica_soma = np.array(cell.cav32_ica_soma_vec) if getattr(cell, "cav32_ica_soma_vec", None) is not None else None
    cav32_ica_ais = np.array(cell.cav32_ica_ais_vec) if getattr(cell, "cav32_ica_ais_vec", None) is not None else None
    cav32_ica_axon = np.array(cell.cav32_ica_axon_vec) if getattr(cell, "cav32_ica_axon_vec", None) is not None else None
    cav32_ica_prox = np.array(cell.cav32_ica_prox_vec) if getattr(cell, "cav32_ica_prox_vec", None) is not None else None
    cav32_ica_dist = np.array(cell.cav32_ica_dist_vec) if getattr(cell, "cav32_ica_dist_vec", None) is not None else None
    cav32_ica_spine = np.array(cell.cav32_ica_spine_vec) if getattr(cell, "cav32_ica_spine_vec", None) is not None else None

    bk_acai12 = np.array(cell.bk_acai12_soma_vec) if getattr(cell, "bk_acai12_soma_vec", None) is not None else None
    bk_acai21 = np.array(cell.bk_acai21_soma_vec) if getattr(cell, "bk_acai21_soma_vec", None) is not None else None
    bk_acai22 = np.array(cell.bk_acai22_soma_vec) if getattr(cell, "bk_acai22_soma_vec", None) is not None else None

    bk_Cav22_ik_ais = np.array(cell.bk_Cav22_ik_ais_vec) if getattr(cell, "bk_Cav22_ik_ais_vec", None) is not None else None
    bk_Cav22_ik_axon = np.array(cell.bk_Cav22_ik_axon_vec) if getattr(cell, "bk_Cav22_ik_axon_vec", None) is not None else None
    bk_Cav22_ik_prox = np.array(cell.bk_Cav22_ik_prox_vec) if getattr(cell, "bk_Cav22_ik_prox_vec", None) is not None else None
    bk_Cav22_ik_dist = np.array(cell.bk_Cav22_ik_dist_vec) if getattr(cell, "bk_Cav22_ik_dist_vec", None) is not None else None
    bk_Cav22_ik_spine = np.array(cell.bk_Cav22_ik_spine_vec) if getattr(cell, "bk_Cav22_ik_spine_vec", None) is not None else None

    bk_Cav12_ik_ais = np.array(cell.bk_Cav12_ik_ais_vec) if getattr(cell, "bk_Cav12_ik_ais_vec", None) is not None else None
    bk_Cav12_ik_axon = np.array(cell.bk_Cav12_ik_axon_vec) if getattr(cell, "bk_Cav12_ik_axon_vec", None) is not None else None
    bk_Cav12_ik_prox = np.array(cell.bk_Cav12_ik_prox_vec) if getattr(cell, "bk_Cav12_ik_prox_vec", None) is not None else None
    bk_Cav12_ik_dist = np.array(cell.bk_Cav12_ik_dist_vec) if getattr(cell, "bk_Cav12_ik_dist_vec", None) is not None else None
    bk_Cav12_ik_spine = np.array(cell.bk_Cav12_ik_spine_vec) if getattr(cell, "bk_Cav12_ik_spine_vec", None) is not None else None

    bk_Cav21_ik_ais = np.array(cell.bk_Cav21_ik_ais_vec) if getattr(cell, "bk_Cav21_ik_ais_vec", None) is not None else None
    bk_Cav21_ik_axon = np.array(cell.bk_Cav21_ik_axon_vec) if getattr(cell, "bk_Cav21_ik_axon_vec", None) is not None else None
    bk_Cav21_ik_prox = np.array(cell.bk_Cav21_ik_prox_vec) if getattr(cell, "bk_Cav21_ik_prox_vec", None) is not None else None
    bk_Cav21_ik_dist = np.array(cell.bk_Cav21_ik_dist_vec) if getattr(cell, "bk_Cav21_ik_dist_vec", None) is not None else None
    bk_Cav21_ik_spine = np.array(cell.bk_Cav21_ik_spine_vec) if getattr(cell, "bk_Cav21_ik_spine_vec", None) is not None else None

    bk_acai22_ais = np.array(cell.bk_acai22_ais_vec) if getattr(cell, "bk_acai22_ais_vec", None) is not None else None
    bk_acai22_axon = np.array(cell.bk_acai22_axon_vec) if getattr(cell, "bk_acai22_axon_vec", None) is not None else None
    bk_acai22_prox = np.array(cell.bk_acai22_prox_vec) if getattr(cell, "bk_acai22_prox_vec", None) is not None else None
    bk_acai22_dist = np.array(cell.bk_acai22_dist_vec) if getattr(cell, "bk_acai22_dist_vec", None) is not None else None
    bk_acai22_spine = np.array(cell.bk_acai22_spine_vec) if getattr(cell, "bk_acai22_spine_vec", None) is not None else None

    bk_acai12_ais = np.array(cell.bk_acai12_ais_vec) if getattr(cell, "bk_acai12_ais_vec", None) is not None else None
    bk_acai12_axon = np.array(cell.bk_acai12_axon_vec) if getattr(cell, "bk_acai12_axon_vec", None) is not None else None
    bk_acai12_prox = np.array(cell.bk_acai12_prox_vec) if getattr(cell, "bk_acai12_prox_vec", None) is not None else None
    bk_acai12_dist = np.array(cell.bk_acai12_dist_vec) if getattr(cell, "bk_acai12_dist_vec", None) is not None else None
    bk_acai12_spine = np.array(cell.bk_acai12_spine_vec) if getattr(cell, "bk_acai12_spine_vec", None) is not None else None

    bk_acai21_ais = np.array(cell.bk_acai21_ais_vec) if getattr(cell, "bk_acai21_ais_vec", None) is not None else None
    bk_acai21_axon = np.array(cell.bk_acai21_axon_vec) if getattr(cell, "bk_acai21_axon_vec", None) is not None else None
    bk_acai21_prox = np.array(cell.bk_acai21_prox_vec) if getattr(cell, "bk_acai21_prox_vec", None) is not None else None
    bk_acai21_dist = np.array(cell.bk_acai21_dist_vec) if getattr(cell, "bk_acai21_dist_vec", None) is not None else None
    bk_acai21_spine = np.array(cell.bk_acai21_spine_vec) if getattr(cell, "bk_acai21_spine_vec", None) is not None else None

    return (
        t, vs, vais, vax, vp, vd, vsp,
        cai_soma, cai_ais, cai_axon, cai_prox, cai_dist, cai_spine,
        ica_soma, ica_ais, ica_axon, ica_prox, ica_dist, ica_spine,
        ik_soma, ina_soma,
        bk_Cav22_ik_soma, bk_Cav12_ik_soma, bk_Cav21_ik_soma,
        bk_total_soma, sk_ik_soma,
        na8st_o_soma, na8st_g_soma, na8st_i_total_soma,
        na8st_o_ais, na8st_g_ais, na8st_i_total_ais,
        sk_acai_soma,
        sk_ik_ais, sk_ik_axon, sk_ik_prox, sk_ik_dist, sk_ik_spine,
        sk_acai_ais, sk_acai_axon, sk_acai_prox, sk_acai_dist, sk_acai_spine,
        cav12_ica_soma, cav12_ica_ais, cav12_ica_axon, cav12_ica_prox, cav12_ica_dist, cav12_ica_spine,
        cav13_ica_soma, cav13_ica_ais, cav13_ica_axon, cav13_ica_prox, cav13_ica_dist, cav13_ica_spine,
        cav21_ica_soma, cav21_ica_ais, cav21_ica_axon, cav21_ica_prox, cav21_ica_dist, cav21_ica_spine,
        cav22_ica_soma, cav22_ica_ais, cav22_ica_axon, cav22_ica_prox, cav22_ica_dist, cav22_ica_spine,
        cav32_ica_soma, cav32_ica_ais, cav32_ica_axon, cav32_ica_prox, cav32_ica_dist, cav32_ica_spine,
        bk_acai12, bk_acai21, bk_acai22,
        bk_Cav22_ik_ais, bk_Cav22_ik_axon, bk_Cav22_ik_prox, bk_Cav22_ik_dist, bk_Cav22_ik_spine,
        bk_Cav12_ik_ais, bk_Cav12_ik_axon, bk_Cav12_ik_prox, bk_Cav12_ik_dist, bk_Cav12_ik_spine,
        bk_Cav21_ik_ais, bk_Cav21_ik_axon, bk_Cav21_ik_prox, bk_Cav21_ik_dist, bk_Cav21_ik_spine,
        bk_acai22_ais, bk_acai22_axon, bk_acai22_prox, bk_acai22_dist, bk_acai22_spine,
        bk_acai12_ais, bk_acai12_axon, bk_acai12_prox, bk_acai12_dist, bk_acai12_spine,
        bk_acai21_ais, bk_acai21_axon, bk_acai21_prox, bk_acai21_dist, bk_acai21_spine
    )

def save_run_report(path, meta: dict):
    def _json_safe(x):
        if isinstance(x, set):
            return sorted(list(x))
        if isinstance(x, (list, tuple)):
            return [_json_safe(v) for v in x]
        if isinstance(x, dict):
            return {str(k): _json_safe(v) for k, v in x.items()}
        return x

    with open(path, "w") as f:
        json.dump(_json_safe(meta), f, indent=2)

def print_key_params(cell, label=""):
    print(f"\n--- PARAM SNAPSHOT {label} ---")
    print("h.celsius =", float(h.celsius))
    print("soma Ra =", float(cell.soma.Ra), "ais Ra =", float(cell.ais.Ra), "dend_dist Ra =", float(cell.dend_dist.Ra))
    if has_mech(cell.soma, "pas"):
        print("soma pas.g =", float(cell.soma(0.5).pas.g), "pas.e =", float(cell.soma(0.5).pas.e))
    if has_mech(cell.soma, "na8st"):
        print("soma na8st.gbar =", float(cell.soma(0.5).na8st.gbar))
    else:
        print("WARNING: na8st not present on soma")
    if has_mech(cell.soma, "SK2"):
        print("soma SK2.gkbar =", float(cell.soma(0.5).SK2.gkbar))
    if has_mech(cell.soma, "BK_Cav22"):
        print("soma BK_Cav22.gakbar =", float(cell.soma(0.5).BK_Cav22.gakbar),
              "gabkbar =", float(cell.soma(0.5).BK_Cav22.gabkbar))
    if has_mech(cell.soma, "BK_Cav12"):
        print("soma BK_Cav12.gakbar =", float(cell.soma(0.5).BK_Cav12.gakbar),
              "gabkbar =", float(cell.soma(0.5).BK_Cav12.gabkbar))
    if has_mech(cell.soma, "BK_Cav21"):
        print("soma BK_Cav21.gakbar =", float(cell.soma(0.5).BK_Cav21.gakbar),
              "gabkbar =", float(cell.soma(0.5).BK_Cav21.gabkbar))
    if has_mech(cell.soma, "Cav12"):
        print("soma Cav12.gbar =", float(cell.soma(0.5).Cav12.gbar))
    if has_mech(cell.soma, "BK_Cav22") and has_mech(cell.soma, "BK_Cav12") and has_mech(cell.soma, "BK_Cav21"):
        bk22 = float(cell.soma(0.5).BK_Cav22.gakbar) #checks split
        bk12 = float(cell.soma(0.5).BK_Cav12.gakbar)
        bk21 = float(cell.soma(0.5).BK_Cav21.gakbar)
        total_bk = bk22 + bk12 + bk21

        print("soma total BK gakbar =", total_bk)
        print("soma BK fractions:")
        print("  BK_Cav22 =", bk22 / total_bk)
        print("  BK_Cav12 =", bk12 / total_bk)
        print("  BK_Cav21 =", bk21 / total_bk)

def ap_widths_per_spike(t, v, frac=0.5, threshold=0.0, refractory_ms=2.0,
                        t_start=100.0, t_end=400.0,
                        pre_ms=3.0, post_ms=15.0):
    """
    Returns AP width (ms) at a chosen fractional level for each spike in [t_start, t_end].

    frac = 0.5 -> half-width
    frac = 1/3 -> one-third width

    Width level is measured at:
        v_level = v_trough + frac * (v_peak - v_trough)
    """
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
        t_after = t_sp[i_peak:]
        i_trough = int(np.argmin(v_after))
        v_trough = float(v_after[i_trough])

        v_level = v_trough + frac * (v_peak - v_trough)

        up_idx = np.where((v_sp[:i_peak] < v_level) & (v_sp[1:i_peak+1] >= v_level))[0]
        down_idx = np.where((v_sp[i_peak:-1] >= v_level) & (v_sp[i_peak+1:] < v_level))[0]

        if len(up_idx) == 0 or len(down_idx) == 0:
            widths.append(np.nan)
        else:
            t_up = float(t_sp[up_idx[-1] + 1])
            t_down = float(t_sp[i_peak + down_idx[0] + 1])
            widths.append(t_down - t_up)

        peaks.append(v_peak)
        troughs.append(v_trough)

    return np.array(spike_times), np.array(widths), np.array(peaks), np.array(troughs)

if __name__ == "__main__":
    #baseline aka WT
    h.celsius = 34.0 #37 in vivo-like, 34 slice-like
    cell = DGGranuleLikeCell(bk_split=WT_BK_SPLIT)
    print_key_params(cell, label="WT")

    if has_mech(cell.soma, "na8st"):
        list_mech_fields(cell.soma(0.5), "na8st")
    else:
        print("na8st not found on soma")

    run_meta = {
        "python_version": __import__("sys").version,
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
        "soma_psection": cell.soma.psection(),  #includes density_mechs & parameter values
    }
    save_run_report(os.path.join(OUT_DIR, "ic_run_report_baseline.json"), run_meta)

    run_meta_50 = dict(run_meta)  #50% meta
    run_meta_50["model"] = "Cav12_50"
    save_run_report(os.path.join(OUT_DIR, "ic_run_report_cav12_50.json"), run_meta_50)

    run_meta_50_remove = dict(run_meta)
    run_meta_50_remove["model"] = "Cav12_50_removeBK12"
    save_run_report(os.path.join(OUT_DIR, "ic_run_report_cav12_50_removeBK12.json"), run_meta_50_remove)

    def savefig(name: str):
        plt.savefig(os.path.join(FIG_DIR, name), dpi=300, bbox_inches="tight")

    #make NEURON shape plot schematic of topology so can see where everything connects - methods figure
#    h.define_shape()
#    ps = h.PlotShape(True)  #show diameters
#    ps.exec_menu("View = plot")  #open shape window
#    h.topology()  #keep printing the tree in the console
    #opens but freezes, check the neuron doccumentation on 28/01 to fix
    #commented out for now - need check cell builder function to make more realistic morpho
    #then shape again

    for Ra in [50, 100, 150, 200, 300]:
        cell = DGGranuleLikeCell()
        for sec in cell.all_secs():
            sec.Ra = Ra
        cell.add_current_clamp(delay=100, dur=300, amp=0.3)
        cell.setup_recording()
        print_key_params(cell, label=f"Ra={Ra}")
        t, vs, vais, vp, vd, vsp, *_ = run_sim(cell)

        #peak separation during step
        w = (t >= 100) & (t <= 400)
        d_soma_dist = float(np.max(np.abs(vs[w] - vd[w])))
        d_ais_soma = float(np.max(np.abs(vais[w] - vs[w])))

        print(f"Ra={Ra:>3} Ω·cm | max|soma-dist|={d_soma_dist:.3f} mV | max|ais-soma|={d_ais_soma:.3f} mV")


    cell.add_current_clamp(delay=100, dur=300, amp=0.3)
    cell.setup_recording()
    t0, vs0, vais0, vax0, vp0, vd0, vsp0, \
        cai0_soma, cai0_ais, cai0_axon, cai0_prox, cai0_dist, cai0_spine, \
        ica0_soma, ica0_ais, ica0_axon, ica0_prox, ica0_dist, ica0_spine, \
        ik0_soma, ina0_soma, \
        bk_Cav22ik0_soma, bk_Cav12ik0_soma, bk_Cav21ik0_soma, bk_total0_soma, skik0_soma, \
        na8st0_o_soma, na8st0_g_soma, na8st0_i_total_soma, \
        na8st0_o_ais, na8st0_g_ais, na8st0_i_total_ais, \
        sk_acai0_soma, \
        skik0_ais, skik0_axon, skik0_prox, skik0_dist, skik0_spine, \
        sk_acai0_ais, sk_acai0_axon, sk_acai0_prox, sk_acai0_dist, sk_acai0_spine, \
        cav12_ica0_soma, cav12_ica0_ais, cav12_ica0_axon, cav12_ica0_prox, cav12_ica0_dist, cav12_ica0_spine, \
        cav13_ica0_soma, cav13_ica0_ais, cav13_ica0_axon, cav13_ica0_prox, cav13_ica0_dist, cav13_ica0_spine, \
        cav21_ica0_soma, cav21_ica0_ais, cav21_ica0_axon, cav21_ica0_prox, cav21_ica0_dist, cav21_ica0_spine, \
        cav22_ica0_soma, cav22_ica0_ais, cav22_ica0_axon, cav22_ica0_prox, cav22_ica0_dist, cav22_ica0_spine, \
        cav32_ica0_soma, cav32_ica0_ais, cav32_ica0_axon, cav32_ica0_prox, cav32_ica0_dist, cav32_ica0_spine, \
        bk_Cav22ik0_ais, bk_Cav22ik0_axon, bk_Cav22ik0_prox, bk_Cav22ik0_dist, bk_Cav22ik0_spine, \
        bk_Cav12ik0_ais, bk_Cav12ik0_axon, bk_Cav12ik0_prox, bk_Cav12ik0_dist, bk_Cav12ik0_spine, \
        bk_Cav21ik0_ais, bk_Cav21ik0_axon, bk_Cav21ik0_prox, bk_Cav21ik0_dist, bk_Cav21ik0_spine, \
        bk_acai22_0, bk_acai12_0, bk_acai21_0, \
        bk_acai22_0_ais, bk_acai22_0_axon, bk_acai22_0_prox, bk_acai22_0_dist, bk_acai22_0_spine, \
        bk_acai12_0_ais, bk_acai12_0_axon, bk_acai12_0_prox, bk_acai12_0_dist, bk_acai12_0_spine, \
        bk_acai21_0_ais, bk_acai21_0_axon, bk_acai21_0_prox, bk_acai21_0_dist, bk_acai21_0_spine = run_sim(
        cell, tstop=500.0, v_init=-70.0, dt=0.025
    )
    print("lens:", len(t0), len(vs0), len(vp0), len(vd0), len(vsp0))
    print("Peak cai base soma/prox/dist/spine:", #prints ca peak in each compartmnet
          float(np.max(cai0_soma)),
          float(np.max(cai0_prox)),
          float(np.max(cai0_dist)),
          float(np.max(cai0_spine)))

    #na8st opening / conductance sanity (during)
    w = (t0 >= 100) & (t0 <= 400)

    print("[na8st] peak o_soma =", float(np.max(na8st0_o_soma[w])) if na8st0_o_soma is not None else None)
    print("[na8st] peak g_soma =", float(np.max(na8st0_g_soma[w])) if na8st0_g_soma is not None else None)

    print("[na] peak |ina_soma| =", float(np.max(np.abs(ina0_soma[w]))), "mA/cm2")
    print("[na] ena_soma =", float(cell.soma(0.5).ena), "mV")
    print("[na] Vmax soma step =", float(np.max(vs0[w])), "mV")

    #soma current-balance sanity (during)
    w = (t0 >= 100) & (t0 <= 400)

    #passive leak current density (mA/cm2) from pas
    ipas0 = None
    if has_mech(cell.soma, "pas"):
        #soNEURON defines i as membrane current density (mA/cm2)
        ipas0 = np.array([cell.soma(0.5).pas.g * (v - cell.soma(0.5).pas.e) for v in vs0])

    print("[pas] soma pas.g =", float(cell.soma(0.5).pas.g) if has_mech(cell.soma, "pas") else None)
    print("[pas] soma pas.e =", float(cell.soma(0.5).pas.e) if has_mech(cell.soma, "pas") else None)
    print("[pas] mean|ipas| step =", float(np.mean(np.abs(ipas0[w]))) if ipas0 is not None else None, "mA/cm2")

    print("[total] mean|ina| step =", float(np.mean(np.abs(ina0_soma[w]))), "mA/cm2")
    print("[total] mean|ik|  step =", float(np.mean(np.abs(ik0_soma[w]))), "mA/cm2")
    print("[total] mean|ica| step =", float(np.mean(np.abs(ica0_soma[w]))), "mA/cm2")


    def report_ik_sources(sec, loc=0.5):
        """
        Prints which inserted density mechanisms on this section expose an 'ik' variable,
        plus conductance parameters if present.
        """
        ps = sec.psection()
        dens = ps.get("density_mechs", {})
        print(f"\n[ik source scan] section={sec.name()}({loc})")
        for mech_name, params in dens.items():
            #many mechanisms expose ik if they WRITE ik
            has_ik = isinstance(params, dict) and ("ik" in params)
            if not has_ik:
                continue

            #show a "gbar-ish" parameter if present
            g_fields = [k for k in params.keys() if ("g" in k and k != "ik")]
            g_preview = {k: params[k][0] for k in g_fields[:6]}  # small preview

            ik0 = params["ik"][0] if isinstance(params["ik"], list) and len(params["ik"]) else params["ik"]
            print(f"  - {mech_name}: ik={ik0}  g_fields={g_preview}")

    #50% Cav1.2 - mimic +/- not protocol 1
    cell2 = DGGranuleLikeCell(bk_split=CAV12_50_BK_SPLIT)
    cell2.scale_cav12(0.5)
    print_key_params(cell2, label="Cav12_50")
    print("[Cav12 gbar check] soma WT:", cell.soma(0.5).Cav12.gbar)
    print("[Cav12 gbar check] soma 50%:", cell2.soma(0.5).Cav12.gbar)
    print("[Cav12 gbar ratio 50%/WT]:", cell2.soma(0.5).Cav12.gbar / cell.soma(0.5).Cav12.gbar) #check 50% scale corrcet
    cell2.add_current_clamp(delay=100, dur=300, amp=0.3)
    cell2.setup_recording()
    t1, vs1, vais1, vax1, vp1, vd1, vsp1, \
        cai1_soma, cai1_ais, cai1_axon, cai1_prox, cai1_dist, cai1_spine, \
        ica1_soma, ica1_ais, ica1_axon, ica1_prox, ica1_dist, ica1_spine, \
        ik1_soma, ina1_soma, \
        bk_Cav22ik1_soma, bk_Cav12ik1_soma, bk_Cav21ik1_soma, bk_total1_soma, skik1_soma, \
        na8st1_o_soma, na8st1_g_soma, na8st1_i_total_soma, \
        na8st1_o_ais, na8st1_g_ais, na8st1_i_total_ais, \
        sk_acai1_soma, \
        skik1_ais, skik1_axon, skik1_prox, skik1_dist, skik1_spine, \
        sk_acai1_ais, sk_acai1_axon, sk_acai1_prox, sk_acai1_dist, sk_acai1_spine, \
        cav12_ica1_soma, cav12_ica1_ais, cav12_ica1_axon, cav12_ica1_prox, cav12_ica1_dist, cav12_ica1_spine, \
        cav13_ica1_soma, cav13_ica1_ais, cav13_ica1_axon, cav13_ica1_prox, cav13_ica1_dist, cav13_ica1_spine, \
        cav21_ica1_soma, cav21_ica1_ais, cav21_ica1_axon, cav21_ica1_prox, cav21_ica1_dist, cav21_ica1_spine, \
        cav22_ica1_soma, cav22_ica1_ais, cav22_ica1_axon, cav22_ica1_prox, cav22_ica1_dist, cav22_ica1_spine, \
        cav32_ica1_soma, cav32_ica1_ais, cav32_ica1_axon, cav32_ica1_prox, cav32_ica1_dist, cav32_ica1_spine, \
        bk_Cav22ik1_ais, bk_Cav22ik1_axon, bk_Cav22ik1_prox, bk_Cav22ik1_dist, bk_Cav22ik1_spine, \
        bk_Cav12ik1_ais, bk_Cav12ik1_axon, bk_Cav12ik1_prox, bk_Cav12ik1_dist, bk_Cav12ik1_spine, \
        bk_Cav21ik1_ais, bk_Cav21ik1_axon, bk_Cav21ik1_prox, bk_Cav21ik1_dist, bk_Cav21ik1_spine, \
        bk_acai22_1, bk_acai12_1, bk_acai21_1, \
        bk_acai22_1_ais, bk_acai22_1_axon, bk_acai22_1_prox, bk_acai22_1_dist, bk_acai22_1_spine, \
        bk_acai12_1_ais, bk_acai12_1_axon, bk_acai12_1_prox, bk_acai12_1_dist, bk_acai12_1_spine, \
        bk_acai21_1_ais, bk_acai21_1_axon, bk_acai21_1_prox, bk_acai21_1_dist, bk_acai21_1_spine = run_sim(
        cell2, tstop=500.0, v_init=-70.0, dt=0.025
    )
    print("lens:", len(t1), len(vs1), len(vp1), len(vd1), len(vsp1))
    print("Peak cai 50% soma/prox/dist/spine:",  #prints ca peak in each compartment
          float(np.max(cai1_soma)),
          float(np.max(cai1_prox)),
          float(np.max(cai1_dist)),
          float(np.max(cai1_spine)))

    # 50% Cav1.2 - BK_Cav12 reduced without redistribution
    cell3 = DGGranuleLikeCell(
        bk_split=CAV12_50_REMOVE_BK_SPLIT,
        bk_total_scale=CAV12_50_REMOVE_BK_TOTAL_SCALE
    )
    cell3.scale_cav12(0.5)
    print_key_params(cell3, label="Cav12_50_removeBK12")
    print("[Cav12 gbar check] soma remove-only:", cell3.soma(0.5).Cav12.gbar)
    print("[BK total scale check] expected =", CAV12_50_REMOVE_BK_TOTAL_SCALE)

    cell3.add_current_clamp(delay=100, dur=300, amp=0.3)
    cell3.setup_recording()
    t2, vs2, vais2, vax2, vp2, vd2, vsp2, \
        cai2_soma, cai2_ais, cai2_axon, cai2_prox, cai2_dist, cai2_spine, \
        ica2_soma, ica2_ais, ica2_axon, ica2_prox, ica2_dist, ica2_spine, \
        ik2_soma, ina2_soma, \
        bk_Cav22ik2_soma, bk_Cav12ik2_soma, bk_Cav21ik2_soma, bk_total2_soma, skik2_soma, \
        na8st2_o_soma, na8st2_g_soma, na8st2_i_total_soma, \
        na8st2_o_ais, na8st2_g_ais, na8st2_i_total_ais, \
        sk_acai2_soma, \
        skik2_ais, skik2_axon, skik2_prox, skik2_dist, skik2_spine, \
        sk_acai2_ais, sk_acai2_axon, sk_acai2_prox, sk_acai2_dist, sk_acai2_spine, \
        cav12_ica2_soma, cav12_ica2_ais, cav12_ica2_axon, cav12_ica2_prox, cav12_ica2_dist, cav12_ica2_spine, \
        cav13_ica2_soma, cav13_ica2_ais, cav13_ica2_axon, cav13_ica2_prox, cav13_ica2_dist, cav13_ica2_spine, \
        cav21_ica2_soma, cav21_ica2_ais, cav21_ica2_axon, cav21_ica2_prox, cav21_ica2_dist, cav21_ica2_spine, \
        cav22_ica2_soma, cav22_ica2_ais, cav22_ica2_axon, cav22_ica2_prox, cav22_ica2_dist, cav22_ica2_spine, \
        cav32_ica2_soma, cav32_ica2_ais, cav32_ica2_axon, cav32_ica2_prox, cav32_ica2_dist, cav32_ica2_spine, \
        bk_Cav22ik2_ais, bk_Cav22ik2_axon, bk_Cav22ik2_prox, bk_Cav22ik2_dist, bk_Cav22ik2_spine, \
        bk_Cav12ik2_ais, bk_Cav12ik2_axon, bk_Cav12ik2_prox, bk_Cav12ik2_dist, bk_Cav12ik2_spine, \
        bk_Cav21ik2_ais, bk_Cav21ik2_axon, bk_Cav21ik2_prox, bk_Cav21ik2_dist, bk_Cav21ik2_spine, \
        bk_acai22_2, bk_acai12_2, bk_acai21_2, \
        bk_acai22_2_ais, bk_acai22_2_axon, bk_acai22_2_prox, bk_acai22_2_dist, bk_acai22_2_spine, \
        bk_acai12_2_ais, bk_acai12_2_axon, bk_acai12_2_prox, bk_acai12_2_dist, bk_acai12_2_spine, \
        bk_acai21_2_ais, bk_acai21_2_axon, bk_acai21_2_prox, bk_acai21_2_dist, bk_acai21_2_spine = run_sim(
        cell3, tstop=500.0, v_init=-70.0, dt=0.025
    )

    print("lens remove-only:", len(t2), len(vs2), len(vp2), len(vd2), len(vsp2))
    print("Peak cai remove-only soma/prox/dist/spine:",
          float(np.max(cai2_soma)),
          float(np.max(cai2_prox)),
          float(np.max(cai2_dist)),
          float(np.max(cai2_spine)))


    #recruitment metrics for soma, peak, mean(abs) plateau, and AUC(abs) during step
    def step_metrics(t, x, step_on=100.0, step_off=400.0, plateau_guard=20.0):
        """
        Returns peak(|x|) during step, mean(|x|) during plateau, AUC(|x|) during step.
        x can be current density (mA/cm2) or any trace.
        """
        if x is None:
            return {"peak_abs": None, "mean_abs_plateau": None, "auc_abs_step": None}

        t = np.asarray(t)
        x = np.asarray(x)

        w_step = (t >= step_on) & (t <= step_off)
        w_plat = (t >= step_on + plateau_guard) & (t <= step_off - plateau_guard)

        peak_abs = float(np.max(np.abs(x[w_step]))) if np.any(w_step) else None
        mean_abs_plateau = float(np.mean(np.abs(x[w_plat]))) if np.any(w_plat) else None
        auc_abs_step = float(np.trapz(np.abs(x[w_step]), t[w_step])) if np.any(w_step) else None  # units: (x * ms)

        return {"peak_abs": peak_abs, "mean_abs_plateau": mean_abs_plateau, "auc_abs_step": auc_abs_step}


    step_on = 100.0
    step_off = 400.0

    wt_cav = step_metrics(t0, ica0_soma, step_on, step_off)
    het_cav = step_metrics(t1, ica1_soma, step_on, step_off)

    wt_bk_Cav22 = step_metrics(t0, bk_Cav22ik0_soma, step_on, step_off)
    het_bk_Cav22 = step_metrics(t1, bk_Cav22ik1_soma, step_on, step_off)

    wt_bk_Cav12 = step_metrics(t0, bk_Cav12ik0_soma, step_on, step_off)
    het_bk_Cav12 = step_metrics(t1, bk_Cav12ik1_soma, step_on, step_off)

    wt_bk_Cav21 = step_metrics(t0, bk_Cav21ik0_soma, step_on, step_off)
    het_bk_Cav21 = step_metrics(t1, bk_Cav21ik1_soma, step_on, step_off)

    wt_sk = step_metrics(t0, skik0_soma, step_on, step_off)
    het_sk = step_metrics(t1, skik1_soma, step_on, step_off)


    def fmt(m):
        return f"peak| |={m['peak_abs']:.3e}, mean| |plat={m['mean_abs_plateau']:.3e}, AUC| |= {m['auc_abs_step']:.3e}" if \
        m["peak_abs"] is not None else "None"


    print("\n--- RECRUITMENT METRICS (soma, step 100–400 ms) ---")
    print("WT  Cav (ica):", fmt(wt_cav))
    print("50% Cav (ica):", fmt(het_cav))
    print("Δ Cav (50%-WT):",
          f"peak {het_cav['peak_abs'] - wt_cav['peak_abs']:+.3e}, "
          f"plat {het_cav['mean_abs_plateau'] - wt_cav['mean_abs_plateau']:+.3e}, "
          f"AUC {het_cav['auc_abs_step'] - wt_cav['auc_abs_step']:+.3e}")

    print("WT  BK_Cav22 (ik):", fmt(wt_bk_Cav22))
    print("50% BK_Cav22 (ik):", fmt(het_bk_Cav22))
    print("Δ BK_Cav22 (50%-WT):",
          f"peak {het_bk_Cav22['peak_abs'] - wt_bk_Cav22['peak_abs']:+.3e}, "
          f"plat {het_bk_Cav22['mean_abs_plateau'] - wt_bk_Cav22['mean_abs_plateau']:+.3e}, "
          f"AUC {het_bk_Cav22['auc_abs_step'] - wt_bk_Cav22['auc_abs_step']:+.3e}")

    print("WT  BK_Cav12 (ik):", fmt(wt_bk_Cav12))
    print("50% BK_Cav12 (ik):", fmt(het_bk_Cav12))
    print("Δ BK_Cav12 (50%-WT):",
          f"peak {het_bk_Cav12['peak_abs'] - wt_bk_Cav12['peak_abs']:+.3e}, "
          f"plat {het_bk_Cav12['mean_abs_plateau'] - wt_bk_Cav12['mean_abs_plateau']:+.3e}, "
          f"AUC {het_bk_Cav12['auc_abs_step'] - wt_bk_Cav12['auc_abs_step']:+.3e}")

    print("WT  BK_Cav21 (ik):", fmt(wt_bk_Cav21))
    print("50% BK_Cav21 (ik):", fmt(het_bk_Cav21))
    print("Δ BK_Cav21 (50%-WT):",
          f"peak {het_bk_Cav21['peak_abs'] - wt_bk_Cav21['peak_abs']:+.3e}, "
          f"plat {het_bk_Cav21['mean_abs_plateau'] - wt_bk_Cav21['mean_abs_plateau']:+.3e}, "
          f"AUC {het_bk_Cav21['auc_abs_step'] - wt_bk_Cav21['auc_abs_step']:+.3e}")

    print("WT  SK (ik):", fmt(wt_sk))
    print("50% SK (ik):", fmt(het_sk))
    print("Δ SK (50%-WT):",
          f"peak {het_sk['peak_abs'] - wt_sk['peak_abs']:+.3e}, "
          f"plat {het_sk['mean_abs_plateau'] - wt_sk['mean_abs_plateau']:+.3e}, "
          f"AUC {het_sk['auc_abs_step'] - wt_sk['auc_abs_step']:+.3e}")

    def peak_abs(x):
        return None if x is None else float(np.max(np.abs(x)))

    print("\n--- CURRENT DIAGNOSTICS (soma, peak |current|) ---")
    print("WT:  ica_soma =", peak_abs(ica0_soma), "mA/cm2")
    print("WT:  ik_soma  =", peak_abs(ik0_soma), "mA/cm2")
    print("WT:  BK_Cav22_ik =", peak_abs(bk_Cav22ik0_soma), "mA/cm2")
    print("WT:  BK_Cav12_ik =", peak_abs(bk_Cav12ik0_soma), "mA/cm2")
    print("WT:  BK_Cav21_ik =", peak_abs(bk_Cav21ik0_soma), "mA/cm2")
    print("WT:  BK_total_ik =", peak_abs(bk_total0_soma), "mA/cm2")
    print("WT:  SK_ik    =", peak_abs(skik0_soma), "mA/cm2")
    print("WT:  ina_soma =", peak_abs(ina0_soma), "mA/cm2")

    print("50%: ica_soma =", peak_abs(ica1_soma), "mA/cm2")
    print("50%: ik_soma  =", peak_abs(ik1_soma), "mA/cm2")
    print("50%: BK_Cav22_ik =", peak_abs(bk_Cav22ik1_soma), "mA/cm2")
    print("50%: BK_Cav12_ik =", peak_abs(bk_Cav12ik1_soma), "mA/cm2")
    print("50%: BK_Cav21_ik =", peak_abs(bk_Cav21ik1_soma), "mA/cm2")
    print("50%: BK_total_ik =", peak_abs(bk_total1_soma), "mA/cm2")
    print("50%: SK_ik    =", peak_abs(skik1_soma), "mA/cm2")
    print("50%: ina_soma =", peak_abs(ina1_soma), "mA/cm2")

    print("WT bk_acai12 vec exists?", cell.bk_acai12_soma_vec is not None)
    print("WT bk_acai21 vec exists?", cell.bk_acai21_soma_vec is not None)
    print("WT bk_acai22 vec exists?", cell.bk_acai22_soma_vec is not None)

    print("50% bk_acai12 vec exists?", cell2.bk_acai12_soma_vec is not None)
    print("50% bk_acai21 vec exists?", cell2.bk_acai21_soma_vec is not None)
    print("50% bk_acai22 vec exists?", cell2.bk_acai22_soma_vec is not None)

    print("WT BK_Cav12 acai max:", float(np.max(bk_acai12_0)) if bk_acai12_0 is not None else None)
    print("WT BK_Cav21 acai max:", float(np.max(bk_acai21_0)) if bk_acai21_0 is not None else None)
    print("WT BK_Cav22 acai max:", float(np.max(bk_acai22_0)) if bk_acai22_0 is not None else None)

    print("50% BK_Cav12 acai max:", float(np.max(bk_acai12_1)) if bk_acai12_1 is not None else None)
    print("50% BK_Cav21 acai max:", float(np.max(bk_acai21_1)) if bk_acai21_1 is not None else None)
    print("50% BK_Cav22 acai max:", float(np.max(bk_acai22_1)) if bk_acai22_1 is not None else None)

    print("WT Cav2.1 ica max:", float(np.max(np.abs(cav21_ica0_soma))) if cav21_ica0_soma is not None else None)
    print("50% Cav2.1 ica max:", float(np.max(np.abs(cav21_ica1_soma))) if cav21_ica1_soma is not None else None)

    print("WT Cav22 ica max:", float(np.max(np.abs(cav22_ica0_soma))) if cav22_ica0_soma is not None else None)
    print("50% Cav22 ica max:", float(np.max(np.abs(cav22_ica1_soma))) if cav22_ica1_soma is not None else None)

    print("WT Cav12 ica max:", float(np.max(np.abs(cav12_ica0_soma))) if cav12_ica0_soma is not None else None)
    print("50% Cav12 ica max:", float(np.max(np.abs(cav12_ica1_soma))) if cav12_ica1_soma is not None else None)

    print("WT Cav13 ica max:", float(np.max(np.abs(cav13_ica0_soma))) if cav13_ica0_soma is not None else None)
    print("50% Cav13 ica max:", float(np.max(np.abs(cav13_ica1_soma))) if cav13_ica1_soma is not None else None)

    #window-average currents during the stimulus, excluding the first 20 ms after onset
    w = (t0 >= 120) & (t0 <= 390)

    print("[SK acai check] WT peak acai (mM):", None if sk_acai0_soma is None else float(np.max(sk_acai0_soma)))
    print("[SK acai check] 50% peak acai (mM):", None if sk_acai1_soma is None else float(np.max(sk_acai1_soma)))

    print("WT max Vsoma:", float(np.max(vs0)))
    print("50% max Vsoma:", float(np.max(vs1)))
    print("WT max SK acai:", float(np.max(sk_acai0_soma)) if sk_acai0_soma is not None else None)
    print("50% max SK acai:", float(np.max(sk_acai1_soma)) if sk_acai1_soma is not None else None)

    print("WT Vmax:", float(np.max(vs0)))
    print("50% Vmax:", float(np.max(vs1)))

    print("WT has SK2 on soma?", h.ismembrane("SK2", sec=cell.soma))
    print("50% has SK2 on soma?", h.ismembrane("SK2", sec=cell2.soma))

    print("WT has SK2 on soma?", h.ismembrane("SK2", sec=cell.soma))
    print("50% has SK2 on soma?", h.ismembrane("SK2", sec=cell2.soma))

    def mean_abs(x):
        return None if x is None else float(np.mean(np.abs(x[w])))

    print("\n--- PLATEAU (120–390 ms) mean |current| ---")
    print("WT: mean|ik_soma| =", mean_abs(ik0_soma), "mA/cm2")
    print("WT: mean|SK_ik|   =", mean_abs(skik0_soma), "mA/cm2")
    print("WT: mean|BK_Cav22_ik|   =", mean_abs(bk_Cav22ik0_soma), "mA/cm2")
    print("WT: mean|BK_Cav12_ik|   =", mean_abs(bk_Cav12ik0_soma), "mA/cm2")
    print("WT: mean|BK_Cav21_ik|   =", mean_abs(bk_Cav21ik0_soma), "mA/cm2")
    print("WT: mean|ica_soma|=", mean_abs(ica0_soma), "mA/cm2")
    print("WT: mean|ina_soma| =", float(np.mean(np.abs(ina0_soma[w]))), "mA/cm2")

    print("50%: mean|ik_soma| =", mean_abs(ik1_soma), "mA/cm2")
    print("50%: mean|SK_ik|   =", mean_abs(skik1_soma), "mA/cm2")
    print("50%: mean|BK_Cav22_ik|   =", mean_abs(bk_Cav22ik1_soma), "mA/cm2")
    print("50%: mean|BK_Cav12_ik|   =", mean_abs(bk_Cav12ik1_soma), "mA/cm2")
    print("50%: mean|BK_Cav21_ik|   =", mean_abs(bk_Cav21ik1_soma), "mA/cm2")
    print("50%: mean|ica_soma|=", mean_abs(ica1_soma), "mA/cm2")
    print("50%: mean|ina_soma| =", float(np.mean(np.abs(ina1_soma[w]))), "mA/cm2")

    vpeak0, tpeak0, vmin0, tmin0, ahp0 = ahp_depth(t0, vs0)
    vpeak1, tpeak1, vmin1, tmin1, ahp1 = ahp_depth(t1, vs1)

    am0 = adaptation_metrics(t0, vs0, delay=100.0, dur=300.0, threshold=0.0)
    am1 = adaptation_metrics(t1, vs1, delay=100.0, dur=300.0, threshold=0.0)

    print("\n--- ADAPTATION (soma) ---")
    print("WT:  early Hz =", am0["fr_early_hz"], " late Hz =", am0["fr_late_hz"],
          " adapt_index =", am0["adapt_index"], " isi_ratio =", am0["isi_ratio_last_over_first"])
    print("50%: early Hz =", am1["fr_early_hz"], " late Hz =", am1["fr_late_hz"],
          " adapt_index =", am1["adapt_index"], " isi_ratio =", am1["isi_ratio_last_over_first"])
    print("Δadapt_index (50%-WT):", am1["adapt_index"] - am0["adapt_index"])

    #Rin diagnostics (MΩ) sanity - check Rin = if no spikey, want ~100MΩ
    delay = float(cell.iclamp.delay)
    dur = float(cell.iclamp.dur)
    amp = float(cell.iclamp.amp)  #nA

    vrest = float(np.mean(vs0[(t0 >= 50) & (t0 <= 90)]))
    vstep = float(np.mean(vs0[(t0 >= delay + 50) & (t0 <= delay + dur - 50)]))  #mid step
    dv = vstep - vrest
    rin_mohm = dv / amp  #mV / nA = MΩ
    print(f"[Rin] vrest={vrest:.2f} mV, vstep={vstep:.2f} mV, ΔV={dv:.2f} mV, Rin≈{rin_mohm:.1f} MΩ")

    print("Percent Δ peak cai (50% vs WT):",
          100 * (cai1_soma.max() / cai0_soma.max() - 1),
          100 * (cai1_prox.max() / cai0_prox.max() - 1),
          100 * (cai1_dist.max() / cai0_dist.max() - 1),
          100 * (cai1_spine.max() / cai0_spine.max() - 1))

    print(f"AHP baseline: {ahp0:.3f} mV (trough {vmin0:.2f} mV at {tmin0:.2f} ms)")
    print(f"AHP Cav12 50%: {ahp1:.3f} mV (trough {vmin1:.2f} mV at {tmin1:.2f} ms)")
    print(f"ΔAHP (50% - base): {ahp1 - ahp0:+.3f} mV")

#prints differences
    delay = 100.0
    dur = 300.0

    for name, vA0, vA1 in [
        ("soma", vs0, vs1),
        ("prox", vp0, vp1),
        ("dist", vd0, vd1),
        ("spine", vsp0, vsp1),
    ]:
        f0 = spike_features(t0, vA0, delay=delay, dur=dur, threshold=0.0)
        f1 = spike_features(t1, vA1, delay=delay, dur=dur, threshold=0.0)

        print(
            f"{name}: "
            f"Δrest={f1['v_rest']-f0['v_rest']:+.3f} mV, "
            f"Δpeak={f1['v_peak']-f0['v_peak']:+.3f} mV, "
            f"ΔAHP={f1['ahp_depth']-f0['ahp_depth']:+.3f} mV, "
            f"Δwidth={f1['width_half']-f0['width_half']:+.3f} ms, "
            f"Δrate={f1['rate_hz']-f0['rate_hz']:+.3f} Hz"
        )


    #quant spike &AHP diff per compartment
    def spike_features_window_metrics(t, v, spike_window=(90, 140)):
        t = np.asarray(t)
        v = np.asarray(v)
        w = (t >= spike_window[0]) & (t <= spike_window[1])
        tt = t[w]
        vv = v[w]

        i_peak = int(np.argmax(vv))
        v_peak = float(vv[i_peak])
        t_peak = float(tt[i_peak])

        vv_after = vv[i_peak:]
        tt_after = tt[i_peak:]
        i_min = int(np.argmin(vv_after))
        v_trough = float(vv_after[i_min])
        t_trough = float(tt_after[i_min])

        #width at half-max
        v_half = (v_peak + v_trough) / 2.0
        above = vv >= v_half
        idx = np.where(above)[0]
        if len(idx) >= 2:
            width = float(tt[idx[-1]] - tt[idx[0]])
        else:
            width = float("nan")

        return v_peak, t_peak, v_trough, t_trough, width


    for name, vA0, vA1 in [
        ("soma", vs0, vs1),
        ("prox", vp0, vp1),
        ("dist", vd0, vd1),
        ("spine", vsp0, vsp1),
    ]:
        pk0, tpk0, tr0, ttr0, w0 = spike_features_window_metrics(t0, vA0)
        pk1, tpk1, tr1, ttr1, w1 = spike_features_window_metrics(t1, vA1)
        print(f"{name}: Δpeak={pk1 - pk0:+.3f} mV, Δtrough={tr1 - tr0:+.3f} mV, Δwidth={w1 - w0:+.3f} ms")


    #AP threshold & max dV/dt at soma
    def ap_threshold_dvdt(t, v, delay, dur, dvdt_thresh=20.0):
        """
        Return (v_thresh, t_thresh, dvdt_max) within the stimulus window.
        Threshold = first time dV/dt crosses dvdt_thresh in mV/ms.
        """
        t = np.asarray(t)
        v = np.asarray(v)

        w = (t >= delay) & (t <= delay + dur)
        tt = t[w]
        vv = v[w]
        if len(tt) < 3:
            return float("nan"), float("nan"), float("nan")

        dvdt = np.gradient(vv, tt)  #mV/ms b/c t is ms
        dvdt_max = float(np.max(dvdt))

        idx = np.where(dvdt >= dvdt_thresh)[0]
        if len(idx) == 0:
            return float("nan"), float("nan"), dvdt_max

        i0 = int(idx[0])
        return float(vv[i0]), float(tt[i0]), dvdt_max


    delay = 100.0
    dur = 300.0

    vth0, tth0, dvdtmax0 = ap_threshold_dvdt(t0, vs0, delay=delay, dur=dur, dvdt_thresh=20.0)
    vth1, tth1, dvdtmax1 = ap_threshold_dvdt(t1, vs1, delay=delay, dur=dur, dvdt_thresh=20.0)

    print(f"[Threshold soma] WT:  Vth={vth0:.2f} mV at {tth0:.2f} ms, max dV/dt={dvdtmax0:.2f} mV/ms")
    print(f"[Threshold soma] 50%: Vth={vth1:.2f} mV at {tth1:.2f} ms, max dV/dt={dvdtmax1:.2f} mV/ms")
    print(f"[ΔThreshold soma] 50%-WT: ΔVth={vth1 - vth0:+.2f} mV, Δ(max dV/dt)={dvdtmax1 - dvdtmax0:+.2f} mV/ms")

    #AP broadening (soma) per-spike
    spk_t0, widths0, peaks0, troughs0 = ap_widths_per_spike(
        t0, vs0, frac=0.5, threshold=0.0, t_start=100.0, t_end=400.0
    )
    spk_t1, widths1, peaks1, troughs1 = ap_widths_per_spike(
        t1, vs1, frac=0.5, threshold=0.0, t_start=100.0, t_end=400.0
    )
    spk_t2, widths2, peaks2, troughs2 = ap_widths_per_spike(
        t2, vs2, frac=0.5, threshold=0.0, t_start=100.0, t_end=400.0
    )

    spk_t0_third, widths_third0, peaks_third0, troughs_third0 = ap_widths_per_spike( #so can rec third widthys
        t0, vs0, frac=1.0 / 3.0, threshold=0.0, t_start=100.0, t_end=400.0
    )
    spk_t1_third, widths_third1, peaks_third1, troughs_third1 = ap_widths_per_spike(
        t1, vs1, frac=1.0 / 3.0, threshold=0.0, t_start=100.0, t_end=400.0
    )
    spk_t2_third, widths_third2, peaks_third2, troughs_third2 = ap_widths_per_spike(
        t2, vs2, frac=1.0 / 3.0, threshold=0.0, t_start=100.0, t_end=400.0
    )

    print("WT AP half-widths (ms):", widths0)
    print("50% AP half-widths (ms):", widths1)
    print("WT mean AP half-width (ms):", float(np.nanmean(widths0)) if len(widths0) else np.nan)
    print("50% mean AP half-width (ms):", float(np.nanmean(widths1)) if len(widths1) else np.nan)
    print("Δ AP half-width (50%-WT) (ms):",
          (float(np.nanmean(widths1)) - float(np.nanmean(widths0)))
          if (len(widths0) and len(widths1)) else np.nan)

    print("WT AP one-third widths (ms):", widths_third0)
    print("50% AP one-third widths (ms):", widths_third1)
    print("WT mean AP one-third width (ms):", float(np.nanmean(widths_third0)) if len(widths_third0) else np.nan)
    print("50% mean AP one-third width (ms):", float(np.nanmean(widths_third1)) if len(widths_third1) else np.nan)
    print("Δ AP one-third width (50%-WT) (ms):",
          (float(np.nanmean(widths_third1)) - float(np.nanmean(widths_third0)))
          if (len(widths_third0) and len(widths_third1)) else np.nan)


    #plot AP tird-width per spike - wooooooo now show boradening more wt vs 50
    n_hw = min(len(widths0), len(widths1))
    if n_hw > 0:
        plt.figure()
        plt.plot(np.arange(1, n_hw + 1), widths_third0[:n_hw], marker="o", color=WT_COLOR, label=WT_LABEL)
        plt.plot(np.arange(1, n_hw + 1), widths_third1[:n_hw], marker="o", color=CAV12_50_COLOR, label=CAV12_50_LABEL)
        plt.xlabel("Spike number")
        plt.ylabel("AP third-width (ms)")
        plt.title("third width Action potential broadening across spike train")
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(FIG_DIR, "AP_third_width_per_spike_WT_vs_50.png"), dpi=300)
        plt.show()
    else:
        print("AP third-width plot skipped no spikes detected in one or both conditions.")

    #plots - REMEMBER TO REMOVE TITLES AND WHATNOT BEFORE PUT IN DISS!

# ============================================================
# Simulation identity:
#   Cell 1 = WT
#   Cell 2 = Cav1.2 50% BK_Cav1.2 redistributed
#   Cell 3 = Cav1.2 50% BK_Cav1.2 removed
#
# Plot/display order:
#   Cell 1, then Cell 3, then Cell 2
#   WT -> removed -> redistributed
# ============================================================

PLOT_SHOW = False

def save_cleanfig(name):
    plt.savefig(os.path.join(FIG_DIR, name), dpi=300, bbox_inches="tight")
    if PLOT_SHOW:
        plt.show()
    plt.close()


# ------------------------------------------------------------
# Display order: WT, removed, redistributed
# ------------------------------------------------------------

COND_LABELS = [
    "Cell 1: WT",
    "Cell 3: Cav1.2 50% BK_Cav1.2 removed",
    "Cell 2: Cav1.2 50% BK_Cav1.2 redist",
]

COND_COLORS = [
    WT_COLOR,
    CAV12_50_REMOVE_COLOR,
    CAV12_50_COLOR,
]

cond_t = [t0, t2, t1]
cond_vs = [vs0, vs2, vs1]

cond_cai_soma = [cai0_soma, cai2_soma, cai1_soma]

cond_bk_total_soma = [
    bk_total0_soma,
    bk_total2_soma,
    bk_total1_soma,
]

cond_bk12_soma = [
    bk_Cav12ik0_soma,
    bk_Cav12ik2_soma,
    bk_Cav12ik1_soma,
]

cond_bk21_soma = [
    bk_Cav21ik0_soma,
    bk_Cav21ik2_soma,
    bk_Cav21ik1_soma,
]

cond_bk22_soma = [
    bk_Cav22ik0_soma,
    bk_Cav22ik2_soma,
    bk_Cav22ik1_soma,
]


def finite_concat(arr_list):
    vals = []
    for a in arr_list:
        if a is not None:
            a = np.asarray(a)
            vals.append(a[np.isfinite(a)])
    if len(vals) == 0:
        return np.array([])
    return np.concatenate(vals)


def panel_limits(arr_list, pad_frac=0.05, force_zero_min=False):
    vals = finite_concat(arr_list)
    if len(vals) == 0:
        return None, None

    ymin = float(np.min(vals))
    ymax = float(np.max(vals))

    if force_zero_min:
        ymin = 0.0

    yr = ymax - ymin
    pad = yr * pad_frac if yr > 0 else 1e-9

    return ymin - pad, ymax + pad


# ============================================================
# GENERIC 3-CELL PLOTTING HELPERS
# ============================================================

def plot_soma_ap_3cells():
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    ylim = panel_limits(cond_vs)

    for i, ax in enumerate(axes):
        ax.plot(cond_t[i], cond_vs[i], color=COND_COLORS[i])
        ax.set_title(COND_LABELS[i])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if ylim[0] is not None:
            ax.set_ylim(ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

    fig.tight_layout()
    save_cleanfig("01_soma_AP_all_3_cells.png")


def plot_ap_with_overlay_3cells(signal_list, signal_ylabel, filename, signal_color="grey"):
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True)

    vm_ylim = panel_limits(cond_vs)
    sig_ylim = panel_limits(signal_list, force_zero_min=False)

    for i, ax in enumerate(axes):
        ax.plot(cond_t[i], cond_vs[i], color=COND_COLORS[i])
        ax.set_title(COND_LABELS[i])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        ax2 = ax.twinx()

        if signal_list[i] is not None:
            ax2.plot(cond_t[i], signal_list[i], color=signal_color, linestyle="--")

        if i == 2:
            ax2.set_ylabel(signal_ylabel)

        if sig_ylim[0] is not None:
            ax2.set_ylim(sig_ylim)

    fig.tight_layout()
    save_cleanfig(filename)


def plot_single_ap_with_overlay_3cells(
    signal_list,
    signal_ylabel,
    filename,
    single_ap_windows=None,
    signal_color="grey"
):
    if single_ap_windows is None:
        single_ap_windows = [
            (95, 130),
            (95, 130),
            (95, 130),
        ]

    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True)

    vm_zoom = []
    sig_zoom = []

    for i in range(3):
        tmin, tmax = single_ap_windows[i]
        w = (cond_t[i] >= tmin) & (cond_t[i] <= tmax)

        vm_zoom.append(cond_vs[i][w])

        if signal_list[i] is not None:
            sig_zoom.append(signal_list[i][w])
        else:
            sig_zoom.append(None)

    vm_ylim = panel_limits(vm_zoom)
    sig_ylim = panel_limits(sig_zoom, force_zero_min=False)

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (cond_t[i] >= tmin) & (cond_t[i] <= tmax)

        ax.plot(cond_t[i][w], cond_vs[i][w], color=COND_COLORS[i])
        ax.set_title(COND_LABELS[i])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        ax2 = ax.twinx()

        if signal_list[i] is not None:
            ax2.plot(cond_t[i][w], signal_list[i][w], color=signal_color, linestyle="--")

        if i == 2:
            ax2.set_ylabel(signal_ylabel)

        if sig_ylim[0] is not None:
            ax2.set_ylim(sig_ylim)

    fig.tight_layout()
    save_cleanfig(filename)


# ============================================================
# REQUIRED SOMA AP / CA / BK PLOTS
# ============================================================

plot_soma_ap_3cells()

plot_ap_with_overlay_3cells(
    cond_cai_soma,
    signal_ylabel="cai soma (mM)",
    filename="02_AP_with_Ca_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_single_ap_with_overlay_3cells(
    cond_cai_soma,
    signal_ylabel="cai soma (mM)",
    filename="03_single_AP_with_Ca_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_ap_with_overlay_3cells(
    cond_bk_total_soma,
    signal_ylabel="Total BK current density (mA/cm2)",
    filename="04_AP_with_total_BK_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_single_ap_with_overlay_3cells(
    cond_bk_total_soma,
    signal_ylabel="Total BK current density (mA/cm2)",
    filename="05_single_AP_with_total_BK_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_ap_with_overlay_3cells(
    cond_bk12_soma,
    signal_ylabel="BK_Cav12 current density (mA/cm2)",
    filename="06_AP_with_BK_Cav12_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_single_ap_with_overlay_3cells(
    cond_bk12_soma,
    signal_ylabel="BK_Cav12 current density (mA/cm2)",
    filename="07_single_AP_with_BK_Cav12_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_ap_with_overlay_3cells(
    cond_bk21_soma,
    signal_ylabel="BK_Cav21 current density (mA/cm2)",
    filename="08_AP_with_BK_Cav21_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_single_ap_with_overlay_3cells(
    cond_bk21_soma,
    signal_ylabel="BK_Cav21 current density (mA/cm2)",
    filename="09_single_AP_with_BK_Cav21_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_ap_with_overlay_3cells(
    cond_bk22_soma,
    signal_ylabel="BK_Cav22 current density (mA/cm2)",
    filename="10_AP_with_BK_Cav22_overlay_soma_all_3_cells.png",
    signal_color="grey"
)

plot_single_ap_with_overlay_3cells(
    cond_bk22_soma,
    signal_ylabel="BK_Cav22 current density (mA/cm2)",
    filename="11_single_AP_with_BK_Cav22_overlay_soma_all_3_cells.png",
    signal_color="grey"
)


# ============================================================
# AP WIDTH / AMPLITUDE / F-I HELPERS
# ============================================================

def detect_spike_times(t, v, threshold=0.0, refractory_ms=2.0, t_start=100.0, t_end=400.0):
    t = np.asarray(t, dtype=float)
    v = np.asarray(v, dtype=float)

    w = (t >= t_start) & (t <= t_end)
    tt = t[w]
    vv = v[w]

    if len(tt) < 3:
        return np.array([], dtype=float)

    crosses = (vv[:-1] < threshold) & (vv[1:] >= threshold)
    idx = np.where(crosses)[0]

    spike_times = []
    last_t = -1e9

    for i in idx:
        ts = float(tt[i + 1])
        if ts - last_t >= refractory_ms:
            spike_times.append(ts)
            last_t = ts

    return np.array(spike_times, dtype=float)


def _interp_crossing_time(t0_, v0_, t1_, v1_, level):
    if v1_ == v0_:
        return float(t1_)

    frac = (level - v0_) / (v1_ - v0_)
    frac = np.clip(frac, 0.0, 1.0)

    return float(t0_ + frac * (t1_ - t0_))


def ap_metrics_per_spike(
    t, v,
    threshold=0.0,
    refractory_ms=2.0,
    t_start=100.0,
    t_end=400.0,
    pre_ms=3.0,
    post_ms=15.0,
    width_frac=1.0 / 3.0
):
    t = np.asarray(t, dtype=float)
    v = np.asarray(v, dtype=float)

    baseline_mask = (t >= t_start - 20.0) & (t < t_start)

    if np.any(baseline_mask):
        v_baseline = float(np.mean(v[baseline_mask]))
    else:
        v_baseline = float(np.mean(v[t < t_start])) if np.any(t < t_start) else np.nan

    spike_times = detect_spike_times(
        t, v,
        threshold=threshold,
        refractory_ms=refractory_ms,
        t_start=t_start,
        t_end=t_end
    )

    widths = []
    amplitudes = []

    for ts in spike_times:
        w = (t >= ts - pre_ms) & (t <= ts + post_ms)
        t_sp = t[w]
        v_sp = v[w]

        if len(t_sp) < 5:
            widths.append(np.nan)
            amplitudes.append(np.nan)
            continue

        i_peak = int(np.argmax(v_sp))
        v_peak = float(v_sp[i_peak])

        amp = v_peak - v_baseline
        amplitudes.append(amp)

        if amp <= 0:
            widths.append(np.nan)
            continue

        v_level = v_baseline + width_frac * amp

        up_idx = np.where((v_sp[:i_peak] < v_level) & (v_sp[1:i_peak + 1] >= v_level))[0]
        down_idx = np.where((v_sp[i_peak:-1] >= v_level) & (v_sp[i_peak + 1:] < v_level))[0]

        if len(up_idx) == 0 or len(down_idx) == 0:
            widths.append(np.nan)
            continue

        i_up = up_idx[-1]
        i_down = i_peak + down_idx[0]

        t_up = _interp_crossing_time(
            t_sp[i_up], v_sp[i_up],
            t_sp[i_up + 1], v_sp[i_up + 1],
            v_level
        )

        t_down = _interp_crossing_time(
            t_sp[i_down], v_sp[i_down],
            t_sp[i_down + 1], v_sp[i_down + 1],
            v_level
        )

        widths.append(t_down - t_up)

    isi = np.diff(spike_times) if len(spike_times) >= 2 else np.array([], dtype=float)
    inst_freq_hz = 1000.0 / isi if len(isi) > 0 else np.array([], dtype=float)

    return (
        np.array(spike_times, dtype=float),
        np.array(widths, dtype=float),
        np.array(amplitudes, dtype=float),
        np.array(isi, dtype=float),
        np.array(inst_freq_hz, dtype=float),
    )


def classify_frequency_bin(freq_hz):
    if np.isnan(freq_hz):
        return None
    if 10.0 <= freq_hz < 40.0:
        return "10-40 Hz"
    elif 40.0 <= freq_hz < 80.0:
        return "40-80 Hz"
    elif 80.0 <= freq_hz < 120.0:
        return "80-120 Hz"
    elif freq_hz >= 120.0:
        return ">120 Hz"
    return None


def get_sweep_frequency_hz(inst_freq_hz, n_freq_for_bin=3):
    inst_freq_hz = np.asarray(inst_freq_hz, dtype=float)

    if len(inst_freq_hz) == 0:
        return np.nan

    n_use = min(n_freq_for_bin, len(inst_freq_hz))

    return float(np.nanmean(inst_freq_hz[:n_use]))


def init_metric_bin_store(n_ap_plot=5, ap_start_idx=0):
    ap_numbers = np.arange(ap_start_idx + 1, ap_start_idx + n_ap_plot + 1, dtype=int)

    return {
        "10-40 Hz": {"values": [], "ap_numbers": ap_numbers.copy()},
        "40-80 Hz": {"values": [], "ap_numbers": ap_numbers.copy()},
        "80-120 Hz": {"values": [], "ap_numbers": ap_numbers.copy()},
        ">120 Hz": {"values": [], "ap_numbers": ap_numbers.copy()},
    }


def add_metric_sweep_to_bin_store(bin_store, metric_values, inst_freq_hz, n_ap_plot=5, ap_start_idx=0):
    sweep_freq_hz = get_sweep_frequency_hz(inst_freq_hz)
    sweep_bin = classify_frequency_bin(sweep_freq_hz)

    if sweep_bin is None:
        return

    vec = np.full(n_ap_plot, np.nan)

    vals_sel = np.asarray(
        metric_values[ap_start_idx:ap_start_idx + n_ap_plot],
        dtype=float
    )

    vec[:len(vals_sel)] = vals_sel

    bin_store[sweep_bin]["values"].append(vec)


def count_spikes_for_fi(t, v, threshold=0.0, refractory_ms=2.0, t_start=100.0, t_end=400.0):
    return len(
        detect_spike_times(
            t, v,
            threshold=threshold,
            refractory_ms=refractory_ms,
            t_start=t_start,
            t_end=t_end
        )
    )


def run_condition_for_fi_and_ap_metrics(
    condition_label,
    condition_color,
    bk_split,
    bk_total_scale,
    cav12_factor,
    currents=np.arange(0.0, 0.71, 0.05),
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
    n_ap_plot=5,
    ap_start_idx=0
):
    firing_rates = []
    width_bin_store = init_metric_bin_store(n_ap_plot=n_ap_plot, ap_start_idx=ap_start_idx)
    amp_bin_store = init_metric_bin_store(n_ap_plot=n_ap_plot, ap_start_idx=ap_start_idx)

    for amp in currents:
        cellx = DGGranuleLikeCell(
            bk_split=bk_split,
            bk_total_scale=bk_total_scale
        )

        if cav12_factor != 1.0:
            cellx.scale_cav12(cav12_factor)

        cellx.add_current_clamp(delay=delay, dur=dur, amp=float(amp))
        cellx.setup_recording()

        result = run_sim(cellx, tstop=tstop, v_init=v_init, dt=dt)

        tx = result[0]
        vsx = result[1]

        spike_times_x, widths_x, amps_x, isi_x, inst_freq_x = ap_metrics_per_spike(
            tx, vsx,
            threshold=0.0,
            refractory_ms=2.0,
            t_start=delay,
            t_end=delay + dur,
            pre_ms=3.0,
            post_ms=15.0,
            width_frac=1.0 / 3.0
        )

        n_spikes = count_spikes_for_fi(
            tx, vsx,
            threshold=0.0,
            refractory_ms=2.0,
            t_start=delay,
            t_end=delay + dur
        )

        rate_hz = n_spikes / (dur / 1000.0)
        firing_rates.append(rate_hz)

        add_metric_sweep_to_bin_store(
            width_bin_store,
            widths_x,
            inst_freq_x,
            n_ap_plot=n_ap_plot,
            ap_start_idx=ap_start_idx
        )

        add_metric_sweep_to_bin_store(
            amp_bin_store,
            amps_x,
            inst_freq_x,
            n_ap_plot=n_ap_plot,
            ap_start_idx=ap_start_idx
        )

        print(
            f"{condition_label} | {amp:.2f} nA | "
            f"spikes={n_spikes} | rate={rate_hz:.2f} Hz"
        )

    return {
        "currents": np.asarray(currents, dtype=float),
        "firing_rates": np.asarray(firing_rates, dtype=float),
        "width_bin_store": width_bin_store,
        "amp_bin_store": amp_bin_store,
        "label": condition_label,
        "color": condition_color,
    }


def plot_fi_curve_3cells(res_cell1, res_cell3, res_cell2):
    plt.figure(figsize=(7, 5))

    # Display order: Cell 1, Cell 3, Cell 2
    for res in [res_cell1, res_cell3, res_cell2]:
        plt.plot(
            res["currents"],
            res["firing_rates"],
            marker="o",
            color=res["color"],
            label=res["label"]
        )

    plt.xlabel("Injected current (nA)")
    plt.ylabel("Firing rate (Hz)")
    plt.legend()
    plt.tight_layout()
    save_cleanfig("14_FI_curve_all_3_cells.png")


def plot_frequency_binned_metric_3cells(
    res_cell1,
    res_cell3,
    res_cell2,
    store_key,
    ylabel,
    out_prefix,
    ylim=None,
    yticks=None
):
    freq_bins = list(res_cell1[store_key].keys())

    for freq_bin in freq_bins:
        has_any = (
            len(res_cell1[store_key][freq_bin]["values"]) > 0 or
            len(res_cell3[store_key][freq_bin]["values"]) > 0 or
            len(res_cell2[store_key][freq_bin]["values"]) > 0
        )

        if not has_any:
            continue

        plt.figure(figsize=(8, 5))

        x = res_cell1[store_key][freq_bin]["ap_numbers"]

        # Display order: Cell 1, Cell 3, Cell 2
        for res in [res_cell1, res_cell3, res_cell2]:
            vals = res[store_key][freq_bin]["values"]

            if len(vals) > 0:
                arr = np.vstack(vals)

                plt.plot(
                    x,
                    np.nanmean(arr, axis=0),
                    marker="o",
                    color=res["color"],
                    label=res["label"]
                )

        plt.xlabel("AP number")
        plt.ylabel(ylabel)
        plt.xticks(x)

        if ylim is not None:
            plt.ylim(*ylim)

        if yticks is not None:
            plt.yticks(yticks)

        plt.legend()
        plt.tight_layout()

        fname_bin = (
            freq_bin
            .replace(" ", "_")
            .replace(">", "gt")
            .replace("-", "to")
        )

        save_cleanfig(f"{out_prefix}_{fname_bin}.png")


# ============================================================
# RUN F-I SERIES + BINNED AP WIDTH / AMPLITUDE
# ============================================================

fi_currents = np.arange(0.0, 0.71, 0.05)

res_cell1 = run_condition_for_fi_and_ap_metrics(
    condition_label="Cell 1: WT",
    condition_color=WT_COLOR,
    bk_split=WT_BK_SPLIT,
    bk_total_scale=1.0,
    cav12_factor=1.0,
    currents=fi_currents,
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
    n_ap_plot=5,
    ap_start_idx=0  # AP1-AP5
)

res_cell2 = run_condition_for_fi_and_ap_metrics(
    condition_label="Cell 2: Cav1.2 50% BK_Cav1.2 redist",
    condition_color=CAV12_50_COLOR,
    bk_split=CAV12_50_BK_SPLIT,
    bk_total_scale=1.0,
    cav12_factor=0.5,
    currents=fi_currents,
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
    n_ap_plot=5,
    ap_start_idx=0  # AP1-AP5
)

res_cell3 = run_condition_for_fi_and_ap_metrics(
    condition_label="Cell 3: Cav1.2 50% BK_Cav1.2 removed",
    condition_color=CAV12_50_REMOVE_COLOR,
    bk_split=CAV12_50_REMOVE_BK_SPLIT,
    bk_total_scale=CAV12_50_REMOVE_BK_TOTAL_SCALE,
    cav12_factor=0.5,
    currents=fi_currents,
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
    n_ap_plot=5,
    ap_start_idx=0  # AP1-AP5
)


# AP 1/3rd widths for AP1-AP5 at:
# 10-40, 40-80, 80-120, >120 Hz
plot_frequency_binned_metric_3cells(
    res_cell1,
    res_cell3,
    res_cell2,
    store_key="width_bin_store",
    ylabel="AP width at 1/3 amplitude (ms)",
    out_prefix="12_AP_1third_width_AP1toAP5",
    ylim=None,
    yticks=None
)


# AP amplitudes for AP1-AP5 at:
# 10-40, 40-80, 80-120, >120 Hz
plot_frequency_binned_metric_3cells(
    res_cell1,
    res_cell3,
    res_cell2,
    store_key="amp_bin_store",
    ylabel="AP amplitude (mV)",
    out_prefix="13_AP_amplitude_AP1toAP5",
    ylim=None,
    yticks=None
)


# F-I curve all 3 cells
plot_fi_curve_3cells(
    res_cell1,
    res_cell3,
    res_cell2
)

print("Done: cleaned plot set saved only.")