#!/usr/bin/env python3

# software and package versions used:
# Python version: 3.12.2
# NEURON version: NEURON -- VERSION 9.0.1

import os
import sys
import json
import csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter, MaxNLocator
from neuron import h

h.load_file("stdrun.hoc")

# ------------------------------------------------------------
# sets labels / colours
# ------------------------------------------------------------
WT_BK_SPLIT = {
    "BK_Cav22": 1.0 / 3.0,
    "BK_Cav12": 1.0 / 3.0,
    "BK_Cav21": 1.0 / 3.0,
}

CAV12_50_BK_SPLIT = {
    "BK_Cav22": 5.0 / 12.0,
    "BK_Cav12": 1.0 / 6.0,
    "BK_Cav21": 5.0 / 12.0,
}

CAV12_50_REMOVE_BK_SPLIT = {
    "BK_Cav22": 2.0 / 5.0,
    "BK_Cav12": 1.0 / 5.0,
    "BK_Cav21": 2.0 / 5.0,
}

CAV12_50_REMOVE_BK_TOTAL_SCALE = 5.0 / 6.0

BK_EFFECTIVE_GAIN = {
    "BK_Cav12": 3.822917700564498e-07,
    "BK_Cav21": 6.977400649452255e-08,
    "BK_Cav22": 3.4477273335658608e-06,
}

WT_LABEL                = "WT"
CAV12_50_LABEL          = "Cav1.2 50%"
CAV12_50_REMOVE_LABEL   = "Cav1.2 50% BK_Cav1.2 removed"

WT_COLOR                = "black"
CAV12_50_COLOR          = "#ffa6b2"
CAV12_50_REMOVE_COLOR   = "#8c52ff"

OVERLAY_AP1_COLOR       = "black"
OVERLAY_AP5_COLOR       = "blue"
OVERLAY_LINEWIDTH       = 0.5

print("Python version:", sys.version)
print("NEURON version:", h.nrnversion())

# ------------------------------------------------------------
# set paths
# ------------------------------------------------------------
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "submission outputs")
FIG_DIR = os.path.join(OUT_DIR, "ic_figures")
os.makedirs(FIG_DIR, exist_ok=True)

MOD_DIR  = "/home/raven/PycharmProjects/Masters/Mod_Files"
DLL_PATH = os.path.join(MOD_DIR, "x86_64", "libnrnmech.so")

if os.path.exists(DLL_PATH):
    h.nrn_load_dll(DLL_PATH)
    print("Loaded mechanisms:", DLL_PATH)
else:
    raise RuntimeError(f"Compiled mechanisms not found at: {DLL_PATH}")

# ------------------------------------------------------------
# helpers
# ------------------------------------------------------------
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


def validate_bk_split(split: dict):
    s = sum(split.values())
    if abs(s - 1.0) > 1e-9:
        raise ValueError(f"BK split must sum to 1.0, got {s}")

def effective_bk_gain(mech_name: str) -> float:
    """
    Compensates for different BK_Cav mechanisms producing different current sizes.
    Keeps intended BK split closer to the specified fractions.
    """
    gains = {
        "BK_Cav12": 1.0,
        "BK_Cav21": 0.10,
        "BK_Cav22": 1.0,
    }
    return gains.get(mech_name, 1.0)

def apply_bk_split_to_section(sec, total_bk_gakbar: float, total_bk_gabkbar: float, split: dict):
    validate_bk_split(split)

    for seg in sec:
        if has_mech(sec, "BK_Cav22"):
            gain = effective_bk_gain("BK_Cav22")
            seg.BK_Cav22.gakbar  = total_bk_gakbar  * split["BK_Cav22"] * gain
            seg.BK_Cav22.gabkbar = total_bk_gabkbar * split["BK_Cav22"] * gain

        if has_mech(sec, "BK_Cav12"):
            gain = effective_bk_gain("BK_Cav12")
            seg.BK_Cav12.gakbar  = total_bk_gakbar  * split["BK_Cav12"] * gain
            seg.BK_Cav12.gabkbar = total_bk_gabkbar * split["BK_Cav12"] * gain

        if has_mech(sec, "BK_Cav21"):
            gain = effective_bk_gain("BK_Cav21")
            seg.BK_Cav21.gakbar  = total_bk_gakbar  * split["BK_Cav21"] * gain
            seg.BK_Cav21.gabkbar = total_bk_gabkbar * split["BK_Cav21"] * gain


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
    plt.close()

def format_scientific_yaxis(ax, exponent=None):
    """
    Force y-axis into scientific notation, e.g. ×10⁻⁷.
    """
    formatter = ScalarFormatter(useMathText=True)
    formatter.set_scientific(True)
    formatter.set_useOffset(False)

    if exponent is not None:
        formatter.set_powerlimits((exponent, exponent))
        ax.ticklabel_format(axis="y", style="sci", scilimits=(exponent, exponent))
    else:
        formatter.set_powerlimits((-3, 3))

    ax.yaxis.set_major_formatter(formatter)


def shared_ticks_from_ylim(y_limits, tick_step=None, n_ticks=5):
    """
    Makes identical tick positions for all panels.
    If tick_step is given, uses clean ticks like 0, 0.5, 1.0 etc.
    """
    if y_limits is None or y_limits[0] is None:
        return None

    ymin, ymax = y_limits

    if tick_step is not None:
        return np.arange(ymin, ymax + tick_step * 0.5, tick_step)

    return np.linspace(ymin, ymax, n_ticks)


def apply_shared_scientific_yaxis(ax, y_limits, exponent=None, n_ticks=5, tick_step=None):
    if y_limits is not None and y_limits[0] is not None:
        ax.set_ylim(y_limits)

        if tick_step is not None:
            ymin, ymax = y_limits
            ticks = np.arange(ymin, ymax + tick_step * 0.5, tick_step)
        else:
            ticks = np.linspace(y_limits[0], y_limits[1], n_ticks)

        ax.set_yticks(ticks)

    format_scientific_yaxis(ax, exponent=exponent)

IC_RAW_ROWS = []


def add_ic_sweep_row(
    condition_label,
    out_prefix,
    amp,
    t,
    v,
    spike_times,
    widths,
    amplitudes,
    isi,
    inst_freq_hz,
    firing_rate_hz,
    first_isi_hz,
    sweep_freq_hz,
    freq_bin,
):
    row = {
        "row_type": "sweep",
        "condition": condition_label,
        "prefix": out_prefix,
        "current_nA": float(amp),
        "n_spikes": int(len(spike_times)),
        "firing_rate_hz": float(firing_rate_hz),
        "first_isi_hz": float(first_isi_hz),
        "sweep_freq_hz": float(sweep_freq_hz),
        "frequency_bin": freq_bin,
        "ap1_peak_mV": np.nan,
        "ap1_time_ms": np.nan,
    }

    if len(spike_times) >= 1:
        ts = spike_times[0]
        w = (t >= ts - 1.0) & (t <= ts + 2.0)
        if np.any(w):
            row["ap1_peak_mV"] = float(np.max(v[w]))
            row["ap1_time_ms"] = float(t[w][np.argmax(v[w])])

    for i in range(10):
        row[f"ap{i+1}_amplitude_mV"] = float(amplitudes[i]) if len(amplitudes) > i else np.nan
        row[f"ap{i+1}_third_width_ms"] = float(widths[i]) if len(widths) > i else np.nan
        row[f"isi{i+1}_ms"] = float(isi[i]) if len(isi) > i else np.nan
        row[f"inst_freq_isi{i+1}_hz"] = float(inst_freq_hz[i]) if len(inst_freq_hz) > i else np.nan

    IC_RAW_ROWS.append(row)


def add_ic_binned_summary_rows(condition_label, out_prefix, bin_store):
    for freq_bin, data in bin_store.items():
        if len(data["amplitudes"]) == 0:
            continue

        amp_arr = np.vstack(data["amplitudes"])
        width_arr = np.vstack(data["widths"])
        freq_arr = np.vstack(data["inst_freq_hz"])

        row = {
            "row_type": "binned_mean",
            "condition": condition_label,
            "prefix": out_prefix,
            "current_nA": np.nan,
            "n_spikes": np.nan,
            "firing_rate_hz": np.nan,
            "first_isi_hz": np.nan,
            "sweep_freq_hz": np.nan,
            "frequency_bin": freq_bin,
            "n_sweeps_in_bin": int(len(data["amplitudes"])),
        }

        for i in range(10):
            row[f"ap{i+1}_amplitude_mV"] = float(np.nanmean(amp_arr[:, i])) if amp_arr.shape[1] > i else np.nan
            row[f"ap{i+1}_third_width_ms"] = float(np.nanmean(width_arr[:, i])) if width_arr.shape[1] > i else np.nan
            row[f"inst_freq_isi{i+1}_hz"] = float(np.nanmean(freq_arr[:, i])) if freq_arr.shape[1] > i else np.nan

        IC_RAW_ROWS.append(row)


def add_ic_overlay_summary_rows(traces):
    """
    Adds exact peak values from the 0.50 nA overlay traces.
    Used for Ca/BK 'under the hood' result placeholders.
    """
    signal_keys = [
        "ca_total", "cav12", "cav13", "cav21", "cav22", "cav32",
        "bk_total", "bk12", "bk21", "bk22",
        "cai", "cai_dist", "cai_spine",
    ]

    for tr in traces:
        t = tr["t"]
        w = (t >= 95.0) & (t <= 130.0)

        for key in signal_keys:
            if key not in tr or tr[key] is None:
                continue

            vals = np.asarray(tr[key][w], dtype=float)
            vals = vals[np.isfinite(vals)]

            if len(vals) == 0:
                peak_val = np.nan
                peak_time = np.nan
            else:
                peak_idx = int(np.nanargmax(vals))
                peak_val = float(vals[peak_idx])
                peak_time = float(t[w][peak_idx])

            IC_RAW_ROWS.append({
                "row_type": "overlay_peak_0p50nA",
                "condition": tr["label"],
                "prefix": tr["label"].replace(" ", "_"),
                "current_nA": 0.50,
                "frequency_bin": "",
                "signal": key,
                "peak_value": peak_val,
                "peak_time_ms": peak_time,
            })


def save_ic_raw_data(path):
    if len(IC_RAW_ROWS) == 0:
        print("No IC raw rows to save.")
        return

    fieldnames = sorted(set().union(*(r.keys() for r in IC_RAW_ROWS)))

    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(IC_RAW_ROWS)

    print(f"\nSaved IC raw data: {path}")

# ------------------------------------------------------------
# spike metrics
# ------------------------------------------------------------
def detect_spikes(t, v, threshold=0.0, refractory_ms=2.0, t_start=100.0, t_end=400.0):
    t = np.asarray(t, dtype=float)
    v = np.asarray(v, dtype=float)

    w  = (t >= t_start) & (t <= t_end)
    tt = t[w]
    vv = v[w]

    if len(tt) < 3:
        return np.array([])

    crosses = (vv[:-1] < threshold) & (vv[1:] >= threshold)
    idx     = np.where(crosses)[0]

    spike_times = []
    last_t      = -1e9
    for i in idx:
        ts = float(tt[i + 1])
        if ts - last_t >= refractory_ms:
            spike_times.append(ts)
            last_t = ts

    return np.array(spike_times, dtype=float)


def _interp_crossing_time(t0, v0, t1, v1, level):
    if v1 == v0:
        return float(t1)
    frac = (level - v0) / (v1 - v0)
    frac = np.clip(frac, 0.0, 1.0)
    return float(t0 + frac * (t1 - t0))


def ap_metrics_per_spike(
    t, v,
    threshold=0.0,
    refractory_ms=2.0,
    t_start=100.0,
    t_end=400.0,
    pre_ms=3.0,
    post_ms=15.0,
    width_frac=1.0 / 3.0,
    width_base_mode="local_min",
):
    t = np.asarray(t, dtype=float)
    v = np.asarray(v, dtype=float)

    baseline_mask = (t >= t_start - 20.0) & (t < t_start)
    if np.any(baseline_mask):
        v_baseline = float(np.mean(v[baseline_mask]))
    else:
        v_baseline = float(np.mean(v[t < t_start])) if np.any(t < t_start) else np.nan

    spike_times = detect_spikes(
        t, v,
        threshold=threshold,
        refractory_ms=refractory_ms,
        t_start=t_start,
        t_end=t_end,
    )

    widths     = []
    amplitudes = []

    for ts in spike_times:
        w    = (t >= ts - pre_ms) & (t <= ts + post_ms)
        t_sp = t[w]
        v_sp = v[w]

        if len(t_sp) < 5:
            widths.append(np.nan)
            amplitudes.append(np.nan)
            continue

        i_peak = int(np.argmax(v_sp))
        v_peak = float(v_sp[i_peak])

        if i_peak < 2:
            widths.append(np.nan)
            amplitudes.append(np.nan)
            continue

        if width_base_mode == "local_min":
            v_before = v_sp[:i_peak + 1]
            i_base   = int(np.argmin(v_before))
            v_base   = float(v_sp[i_base])
        elif width_base_mode == "baseline":
            v_base = v_baseline
        elif width_base_mode == "threshold":
            v_base = float(threshold)
        else:
            raise ValueError(f"Unknown width_base_mode: {width_base_mode}")

        amp = v_peak - v_base
        amplitudes.append(v_peak - v_baseline)

        if amp <= 0:
            widths.append(np.nan)
            continue

        v_level = v_base + width_frac * amp

        up_idx   = np.where((v_sp[:i_peak]    < v_level) & (v_sp[1:i_peak + 1]  >= v_level))[0]
        down_idx = np.where((v_sp[i_peak:-1] >= v_level) & (v_sp[i_peak + 1:]    < v_level))[0]

        if len(up_idx) == 0 or len(down_idx) == 0:
            widths.append(np.nan)
            continue

        i_up   = up_idx[-1]
        i_down = i_peak + down_idx[0]

        t_up = _interp_crossing_time(
            t_sp[i_up],   v_sp[i_up],
            t_sp[i_up + 1], v_sp[i_up + 1],
            v_level,
        )
        t_down = _interp_crossing_time(
            t_sp[i_down],   v_sp[i_down],
            t_sp[i_down + 1], v_sp[i_down + 1],
            v_level,
        )

        widths.append(t_down - t_up)

    isi          = np.diff(spike_times) if len(spike_times) >= 2 else np.array([])
    inst_freq_hz = 1000.0 / isi         if len(isi) > 0        else np.array([])

    return (
        np.array(spike_times,  dtype=float),
        np.array(widths,       dtype=float),
        np.array(amplitudes,   dtype=float),
        np.array(isi,          dtype=float),
        np.array(inst_freq_hz, dtype=float),
    )

def first_ahp_depth(t, v, spike_times, t_start=100.0, baseline_pre_ms=20.0,
                    after_start_ms=1.0, after_end_ms=25.0):
    """
    Measure AHP depth after AP1.
    AHP depth = baseline Vm - minimum Vm after AP1.
    a opsitive value means hyperpolarisation below baseline.
    """
    t = np.asarray(t, dtype=float)
    v = np.asarray(v, dtype=float)

    if len(spike_times) < 1:
        return np.nan, np.nan, np.nan

    baseline_mask = (t >= t_start - baseline_pre_ms) & (t < t_start)
    if not np.any(baseline_mask):
        return np.nan, np.nan, np.nan

    v_baseline = float(np.mean(v[baseline_mask]))

    ts = float(spike_times[0])
    ahp_mask = (t >= ts + after_start_ms) & (t <= ts + after_end_ms)

    if not np.any(ahp_mask):
        return np.nan, v_baseline, np.nan

    v_min = float(np.min(v[ahp_mask]))
    ahp_depth = v_baseline - v_min

    return ahp_depth, v_baseline, v_min

def classify_frequency_bin(freq_hz):
    if np.isnan(freq_hz):
        return None
    if 10.0  <= freq_hz <  40.0: return "10-40 Hz"
    if 40.0  <= freq_hz <  80.0: return "40-80 Hz"
    if 80.0  <= freq_hz < 120.0: return "80-120 Hz"
    if freq_hz >= 120.0:          return ">120 Hz"
    return None


def get_sweep_frequency_hz(inst_freq_hz, n_freq_for_bin=3):
    inst_freq_hz = np.asarray(inst_freq_hz, dtype=float)
    if len(inst_freq_hz) == 0:
        return np.nan
    n_use = min(n_freq_for_bin, len(inst_freq_hz))
    return float(np.nanmean(inst_freq_hz[:n_use]))


# ------------------------------------------------------------
# make cell
# ------------------------------------------------------------
class DGGranuleLikeCell:
    def __init__(self, name="dgcell", bk_split=None, bk_total_scale=1.0):
        self.name          = name
        self.bk_split      = WT_BK_SPLIT if bk_split is None else bk_split
        validate_bk_split(self.bk_split)
        self.bk_total_scale = bk_total_scale

        self.spines      = []
        self.spine_necks = []
        self.spine_xs    = []

        self.soma      = h.Section(name=f"{name}.soma")
        self.dend_prox = h.Section(name=f"{name}.dend_prox")
        self.dend_dist = h.Section(name=f"{name}.dend_dist")
        self.ais       = h.Section(name=f"{name}.ais")
        self.axon      = h.Section(name=f"{name}.axon")

        self.ais.connect(self.soma(0))
        self.axon.connect(self.ais(1))
        self.dend_prox.connect(self.soma(1))
        self.dend_dist.connect(self.dend_prox(1))

        self._set_geometry()
        self.add_spines_to_distal_dend(n_spines=10)
        self._set_biophysics()

        self.iclamp     = None
        self.t_vec      = h.Vector()
        self.vsoma_vec  = h.Vector()

    def all_secs(self):
        return ([self.soma, self.dend_prox, self.dend_dist, self.ais, self.axon]
                + self.spine_necks + self.spines)

    def add_spines_to_distal_dend(self, n_spines=10):
        for i in range(n_spines):
            neck = h.Section(name=f"{self.name}.spine_neck[{i}]")
            head = h.Section(name=f"{self.name}.spine_head[{i}]")

            x = 0.1 + 0.8 * (i / max(1, n_spines - 1))
            self.spine_xs.append(x)
            neck.connect(self.dend_dist(x))
            head.connect(neck(1))

            neck.L, neck.diam, neck.nseg = 1.0, 0.2, 1
            head.L, head.diam, head.nseg = 0.5, 0.5, 1

            self.spine_necks.append(neck)
            self.spines.append(head)

    def _set_geometry(self):
        self.soma.diam  = 20.0
        self.soma.L     = self.soma.diam
        self.soma.nseg  = 1

        self.dend_prox.L    = 150.0
        self.dend_prox.diam = 2.0
        self.dend_prox.nseg = 9

        self.dend_dist.L    = 200.0
        self.dend_dist.diam = 1.2
        self.dend_dist.nseg = 11

        self.ais.L    = 30.0
        self.ais.diam = 1.0
        self.ais.nseg = 3

        self.axon.L    = 300.0
        self.axon.diam = 1.0
        self.axon.nseg = 11

        for sec in self.all_secs():
            sec.Ra = 210.0
            sec.cm = 3

        self.soma.cm = 8
        self.ais.cm = 5

    def _insert_passive_everywhere(self):
        for sec in self.all_secs():
            try_insert(sec, "pas")
            if has_mech(sec, "pas"):
                for seg in sec:
                    seg.pas.g = 0.00018 #was 0.00018
                    seg.pas.e = -75.0

    def _insert_list(self, sec, mechs):
        for mech in mechs:
            try_insert(sec, mech)

    def _set_biophysics(self):
        self._insert_passive_everywhere()

        soma_mechs = [
            "na8st", "ichan3", "HCN", "Kv11", "Kv42", "Kv42b", "Kv723", "Kir21",
            "CadepK", "Caold", "Cabuffer", "Cav12", "Cav13", "Cav2_1", "Cav22", "Cav32",
            "BK_Cav12", "BK_Cav21", "BK_Cav22", "SK2",
        ]
        ais_mechs = [
            "na8st", "ichan3", "HCN", "Kv723",
            "Caold", "Cabuffer", "Cav12", "Cav32", "BK_Cav12",
        ]
        axon_mechs = [
            "na8st", "ichan3", "Kv14", "Kv723",
            "Caold", "Cabuffer", "Cav2_1", "Cav22", "BK_Cav21", "BK_Cav22",
        ]
        prox_mechs = [
            "na8st", "ichan3", "HCN", "Kv42", "Kv42b", "Kir21", "CadepK",
            "Caold", "Cabuffer", "Cav12", "Cav13", "Cav32", "BK_Cav12", "SK2",
        ]
        dist_mechs = [
            "na8st", "ichan3", "HCN", "Kv42", "Kv42b", "CadepK",
            "Caold", "Cabuffer", "Cav12", "Cav13", "Cav32", "BK_Cav12", "SK2",
        ]
        spine_neck_mechs = [
            "HCN", "Kv42", "Kv42b",
            "Caold", "Cabuffer", "Cav12", "Cav13", "Cav32", "BK_Cav12", "SK2",
        ]
        spine_head_mechs = [
            "HCN", "Kv42", "Kv42b",
            "Caold", "Cabuffer", "Cav12", "Cav13", "Cav32", "BK_Cav12", "SK2",
        ]

        self._insert_list(self.soma, soma_mechs)
        self._insert_list(self.ais, ais_mechs)
        self._insert_list(self.axon, axon_mechs)
        self._insert_list(self.dend_prox, prox_mechs)
        self._insert_list(self.dend_dist, dist_mechs)

        for neck in self.spine_necks:
            self._insert_list(neck, spine_neck_mechs)
        for head in self.spines:
            self._insert_list(head, spine_head_mechs)

        self._set_channel_densities_default()

    def _set_channel_densities_default(self):
        if has_mech(self.soma, "na8st"):
            for seg in self.soma:
                seg.na8st.gbar = 2.0e-8

        if has_mech(self.ais, "na8st"):
            for seg in self.ais:
                seg.na8st.gbar = 4.0e-8

        if has_mech(self.axon, "na8st"):
            for seg in self.axon:
                seg.na8st.gbar = 1.0e-9

        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "na8st"):
                for seg in sec:
                    seg.na8st.gbar = 0.00

        if has_mech(self.soma, "ichan3"):
            for seg in self.soma:
                seg.ichan3.gnabar = 0.15
                seg.ichan3.gkfbar = 0.007
                seg.ichan3.gksbar = 0.008 * 1.5
                seg.ichan3.gkabar = 0.0031

        if has_mech(self.ais, "ichan3"):
            for seg in self.ais:
                seg.ichan3.gnabar = 0.20
                seg.ichan3.gkfbar = 0.007
                seg.ichan3.gksbar = 0.012 * 1.5
                seg.ichan3.gkabar = 0.0031

        if has_mech(self.axon, "ichan3"):
            for seg in self.axon:
                seg.ichan3.gnabar = 0.02
                seg.ichan3.gkfbar = 0.0015
                seg.ichan3.gkabar = 0.0021

        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "ichan3"):
                for seg in sec:
                    seg.ichan3.gnabar = 0.002
                    seg.ichan3.gkfbar = 0.0012
                    seg.ichan3.gksbar = 0.004
                    seg.ichan3.gkabar = 0.007

        if has_mech(self.soma, "HCN"):
            for seg in self.soma:
                seg.HCN.gbar = 9.0e-5
        if has_mech(self.ais, "HCN"):
            for seg in self.ais:
                seg.HCN.gbar = 9.0e-3
        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "HCN"):
                for seg in sec:
                    seg.HCN.gbar = 1.0e-4
        for sec in self.spine_necks + self.spines:
            if has_mech(sec, "HCN"):
                for seg in sec:
                    seg.HCN.gbar = 1.0e-5

        if has_mech(self.soma, "Kv11"):
            for seg in self.soma:
                seg.Kv11.gkbar = 1.0e-4

        if has_mech(self.axon, "Kv14"):
            for seg in self.axon:
                seg.Kv14.gkbar = 1.0e-5

        if has_mech(self.soma, "Kv42"):
            for seg in self.soma:
                seg.Kv42.gkbar = 1.0e-5
        if has_mech(self.soma, "Kv42b"):
            for seg in self.soma:
                seg.Kv42b.gkbar = 1.0e-5

        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "Kv42"):
                for seg in sec:
                    seg.Kv42.gkbar = 1.0e-5
            if has_mech(sec, "Kv42b"):
                for seg in sec:
                    seg.Kv42b.gkbar = 1.0e-5

        for sec in self.spine_necks + self.spines:
            if has_mech(sec, "Kv42"):
                for seg in sec:
                    seg.Kv42.gkbar = 1.0e-5
            if has_mech(sec, "Kv42b"):
                for seg in sec:
                    seg.Kv42b.gkbar = 1.0e-5

        if has_mech(self.soma, "Kv723"):
            for seg in self.soma:
                seg.Kv723.gkbar = 0.0027
        if has_mech(self.ais, "Kv723"):
            for seg in self.ais:
                seg.Kv723.gkbar = 0.0013
        if has_mech(self.axon, "Kv723"):
            for seg in self.axon:
                seg.Kv723.gkbar = 0.0025

        if has_mech(self.soma, "Kir21"):
            for seg in self.soma:
                seg.Kir21.gkbar = 0.0001
        if has_mech(self.dend_prox, "Kir21"):
            for seg in self.dend_prox:
                seg.Kir21.gkbar = 0.00001

        for sec in [self.soma, self.dend_prox, self.dend_dist]:
            if has_mech(sec, "CadepK"):
                for seg in sec:
                    for attr in ["gbar", "gkbar", "gcakbar"]:
                        if hasattr(seg.CadepK, attr):
                            setattr(seg.CadepK, attr, 0.00)  #zeroed to remove influ, was 5e-05
                            break

        for sec in ([self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]
                    + self.spine_necks + self.spines):
            if has_mech(sec, "Caold"): #zeroed to reduce influ
                for seg in sec:
                    seg.Caold.gtcabar = 0.0  # 1.0e-4
                    seg.Caold.gncabar = 0.0  # 1.0e-4
                    seg.Caold.glcabar = 0.0  # 1.0e-4

        for sec in ([self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]
                    + self.spine_necks + self.spines):
            if has_mech(sec, "Cabuffer"):
                for seg in sec:
                    seg.Cabuffer.tau = 10.0
                    seg.Cabuffer.brat = 0.5

        if has_mech(self.soma, "Cav12"):
            for seg in self.soma:
                seg.Cav12.gbar = 3.5e-5
        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar = 5.0e-9
        for sec in self.spine_necks:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar = 5.0e-10
        for sec in self.spines:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar = 1.0e-8

        if has_mech(self.soma, "Cav13"):
            for seg in self.soma:
                seg.Cav13.gbar = 5.0e-5
        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "Cav13"):
                for seg in sec:
                    seg.Cav13.gbar = 2.0e-8
        for sec in self.spine_necks + self.spines:
            if has_mech(sec, "Cav13"):
                for seg in sec:
                    seg.Cav13.gbar = 5.0e-9

        if has_mech(self.soma, "Cav2_1"):
            for seg in self.soma:
                seg.Cav2_1.pcabar = 5.0e-7
                seg.Cav2_1.vshift = 0.0
        if has_mech(self.axon, "Cav2_1"):
            for seg in self.axon:
                seg.Cav2_1.pcabar = 2.0e-5
                seg.Cav2_1.vshift = 0.0

        if has_mech(self.soma, "Cav22"):
            for seg in self.soma:
                seg.Cav22.gbar = 5.0e-6
        if has_mech(self.axon, "Cav22"):
            for seg in self.axon:
                seg.Cav22.gbar = 2.0e-5

        if has_mech(self.soma, "Cav32"):
            for seg in self.soma:
                seg.Cav32.gbar = 5.0e-5
        if has_mech(self.ais, "Cav32"):
            for seg in self.ais:
                seg.Cav32.gbar = 2.0e-7
        for sec in ([self.dend_prox, self.dend_dist]
                    + self.spine_necks + self.spines):
            if has_mech(sec, "Cav32"):
                for seg in sec:
                    seg.Cav32.gbar = 5.0e-8

        if has_mech(self.soma, "SK2"):
            for seg in self.soma:
                seg.SK2.gkbar = 1.2e-9  # 9.0e-5
        for sec in ([self.dend_prox, self.dend_dist]
                    + self.spine_necks + self.spines):
            if has_mech(sec, "SK2"):
                for seg in sec:
                    seg.SK2.gkbar = 1.0e-8

            # ----------------------------------------------------
            # this creatse BK pool split across BK_Cav22, BK_Cav12, BK_Cav21
            # Uses:
            #   self.bk_split
            #   self.bk_total_scale
            # ----------------------------------------------------

            # Soma has all three BK-Cav mechanisms present
            if (
                    has_mech(self.soma, "BK_Cav22")
                    and has_mech(self.soma, "BK_Cav12")
                    and has_mech(self.soma, "BK_Cav21")
            ):
                apply_bk_split_to_section(
                    self.soma,
                    total_bk_gakbar=5.0e-7 * self.bk_total_scale,
                    total_bk_gabkbar=5.0e-6 * self.bk_total_scale,
                    split=self.bk_split,
                )
#sanity check - remove # to run
            #        print(
            #            "BK check soma:",
            #            "Cav12 gak/gabk =",
            #            self.soma(0.5).BK_Cav12.gakbar,
            #            self.soma(0.5).BK_Cav12.gabkbar,
            #            "| Cav21 gak/gabk =",
            #            self.soma(0.5).BK_Cav21.gakbar,
            #            self.soma(0.5).BK_Cav21.gabkbar,
            #            "| Cav22 gak/gabk =",
            #            self.soma(0.5).BK_Cav22.gakbar,
            #            self.soma(0.5).BK_Cav22.gabkbar,
            #        )

            # Prox/dist dendrites have only BK_Cav12
            # so gives BK_Cav12 the dendritic BK pool
            for sec in [self.dend_prox, self.dend_dist]:
                if has_mech(sec, "BK_Cav12"):
                    apply_bk_split_to_section(
                        sec,
                        total_bk_gakbar=5.0e-7 * self.bk_total_scale,
                        total_bk_gabkbar=5.0e-6 * self.bk_total_scale,
                        split={"BK_Cav22": 0.0, "BK_Cav12": 1.0, "BK_Cav21": 0.0},
                    )

            # Spines has only BK_Cav12
            for sec in self.spine_necks + self.spines:
                if has_mech(sec, "BK_Cav12"):
                    apply_bk_split_to_section(
                        sec,
                        total_bk_gakbar=1.0e-7 * self.bk_total_scale,
                        total_bk_gabkbar=1.0e-6 * self.bk_total_scale,
                        split={"BK_Cav22": 0.0, "BK_Cav12": 1.0, "BK_Cav21": 0.0},
                    )

            # Axon has BK_Cav21 and BK_Cav22
            # Renormalisses the split across BK_Cav21 and BK_Cav22 only.
            if has_mech(self.axon, "BK_Cav21") and has_mech(self.axon, "BK_Cav22"):
                axon_21_22_sum = self.bk_split["BK_Cav21"] + self.bk_split["BK_Cav22"]

                if axon_21_22_sum > 0:
                    axon_split = {
                        "BK_Cav22": self.bk_split["BK_Cav22"] / axon_21_22_sum,
                        "BK_Cav12": 0.0,
                        "BK_Cav21": self.bk_split["BK_Cav21"] / axon_21_22_sum,
                    }
                else:
                    axon_split = {
                        "BK_Cav22": 0.5,
                        "BK_Cav12": 0.0,
                        "BK_Cav21": 0.5,
                    }

                apply_bk_split_to_section(
                    self.axon,
                    total_bk_gakbar=1.0e-3 * self.bk_total_scale,
                    total_bk_gabkbar=1.0e-3 * self.bk_total_scale,
                    split=axon_split,
                )

            # Sets potassium reversal potential
            for sec in self.all_secs():
                for seg in sec:
                    if hasattr(seg, "ek"):
                        seg.ek = -90.0

    def add_current_clamp(self, delay=100.0, dur=300.0, amp=0.3, sec=None, loc=0.5):
        sec = self.soma if sec is None else sec
        self.iclamp       = h.IClamp(sec(loc))
        self.iclamp.delay = delay
        self.iclamp.dur   = dur
        self.iclamp.amp   = amp

    def setup_recording(self):
        self.t_vec = h.Vector()
        self.t_vec.record(h._ref_t)

        self.vsoma_vec = h.Vector()
        self.vsoma_vec.record(self.soma(0.5)._ref_v)


# ------------------------------------------------------------
# runs the sim
# ------------------------------------------------------------
def run_sim(cell, tstop=500.0, v_init=-70.0, dt=0.025):
    h.dt     = dt
    h.tstop  = tstop
    h.finitialize(v_init)
    h.frecord_init()
    h.continuerun(tstop)

    vs = np.array(cell.vsoma_vec, dtype=float)
    t  = np.arange(len(vs), dtype=float) * dt
    return t, vs


# ------------------------------------------------------------
# frequency-bin helpers
# ------------------------------------------------------------
def init_frequency_bin_store(n_ap_plot=5, ap_start_idx=0):
    ap_numbers = np.arange(ap_start_idx + 1, ap_start_idx + n_ap_plot + 1, dtype=int)
    template   = {"widths": [], "amplitudes": [], "inst_freq_hz": [], "ap_numbers": ap_numbers.copy()}
    return {
        "10-40 Hz":  {k: (v.copy() if isinstance(v, np.ndarray) else list(v)) for k, v in template.items()},
        "40-80 Hz":  {k: (v.copy() if isinstance(v, np.ndarray) else list(v)) for k, v in template.items()},
        "80-120 Hz": {k: (v.copy() if isinstance(v, np.ndarray) else list(v)) for k, v in template.items()},
        ">120 Hz":   {k: (v.copy() if isinstance(v, np.ndarray) else list(v)) for k, v in template.items()},
    }


def init_frequency_bin_store_10ap(n_ap_plot=10, ap_start_idx=0):
    return init_frequency_bin_store(n_ap_plot=n_ap_plot, ap_start_idx=ap_start_idx)


def add_sweep_to_bin_store(bin_store, widths, amplitudes, inst_freq_hz,
                            n_ap_plot=5, ap_start_idx=0):
    sweep_freq_hz = get_sweep_frequency_hz(inst_freq_hz)
    sweep_bin     = classify_frequency_bin(sweep_freq_hz)
    if sweep_bin is None:
        return

    width_vec = np.full(n_ap_plot, np.nan)
    amp_vec   = np.full(n_ap_plot, np.nan)
    freq_vec  = np.full(n_ap_plot, np.nan)

    widths_sel = np.asarray(widths[ap_start_idx:ap_start_idx + n_ap_plot],     dtype=float)
    amps_sel   = np.asarray(amplitudes[ap_start_idx:ap_start_idx + n_ap_plot], dtype=float)
    freq_sel   = np.asarray(inst_freq_hz[ap_start_idx:ap_start_idx + n_ap_plot], dtype=float)

    width_vec[:len(widths_sel)] = widths_sel
    amp_vec[:len(amps_sel)]     = amps_sel
    freq_vec[:len(freq_sel)]    = freq_sel

    bin_store[sweep_bin]["widths"].append(width_vec)
    bin_store[sweep_bin]["amplitudes"].append(amp_vec)
    bin_store[sweep_bin]["inst_freq_hz"].append(freq_vec)


# ------------------------------------------------------------
# plot helpers
# ------------------------------------------------------------
def plot_frequency_binned_broadening(bin_store, out_prefix, line_color,
                                      width_label="AP width at 1/3 amplitude (ms)"):
    for freq_bin, data in bin_store.items():
        if len(data["widths"]) == 0:
            continue
        arr       = np.vstack(data["widths"])
        x         = data["ap_numbers"]
        mean_vals = np.nanmean(arr, axis=0)

        plt.figure(figsize=(8, 5))
        plt.plot(x, mean_vals, marker="o", linestyle="None", markersize=6, color=line_color)
        plt.xlim(x[0] - 0.25, x[-1] + 0.25)
        plt.xticks(x)
        plt.xlabel("AP number")
        plt.ylabel(width_label)
        plt.title(f"{out_prefix}: {freq_bin}")
        plt.ylim(1.0, 3.0)
        plt.yticks(np.arange(1.0, 3.01, 0.5))
        plt.tight_layout()
        fname_bin = freq_bin.replace(" ", "_").replace(">", "gt").replace("-", "to")
        savefig(f"{out_prefix}_{fname_bin}.png")


def plot_frequency_binned_amplitude(bin_store, out_prefix, line_color):
    for freq_bin, data in bin_store.items():
        if len(data["amplitudes"]) == 0:
            continue
        arr       = np.vstack(data["amplitudes"])
        x         = data["ap_numbers"]
        mean_vals = np.nanmean(arr, axis=0)

        plt.figure(figsize=(8, 5))
        plt.plot(x, mean_vals, marker="o", linestyle="None", markersize=6, color=line_color)
        plt.xlim(x[0] - 0.25, x[-1] + 0.25)
        plt.xticks(x)
        plt.xlabel("AP number")
        plt.ylabel("AP amplitude (mV)")
        plt.title(f"{out_prefix}: {freq_bin}")
        plt.ylim(70, 110)
        plt.yticks(np.arange(70, 111, 10))
        plt.tight_layout()
        fname_bin = freq_bin.replace(" ", "_").replace(">", "gt").replace("-", "to")
        savefig(f"{out_prefix}_{fname_bin}.png")


def plot_frequency_binned_amplitude_10ap(bin_store, out_prefix, line_color):
    for freq_bin, data in bin_store.items():
        if len(data["amplitudes"]) == 0:
            continue
        arr       = np.vstack(data["amplitudes"])
        x         = data["ap_numbers"]
        mean_vals = np.nanmean(arr, axis=0)

        plt.figure(figsize=(8, 5))
        plt.plot(x, mean_vals, marker="o", linestyle="None", markersize=6, color=line_color)
        plt.xlim(0, 10)
        plt.xticks(np.arange(1, 11, 1))
        plt.xlabel("AP number")
        plt.ylabel("AP amplitude (mV)")
        plt.title(f"{out_prefix}: {freq_bin}")
        plt.ylim(70, 110)
        plt.yticks(np.arange(70, 111, 10))
        plt.tight_layout()
        fname_bin = freq_bin.replace(" ", "_").replace(">", "gt").replace("-", "to")
        savefig(f"{out_prefix}_{fname_bin}.png")


def plot_frequency_binned_instfreq(bin_store, out_prefix, line_color):
    for freq_bin, data in bin_store.items():
        if len(data["inst_freq_hz"]) == 0:
            continue
        arr       = np.vstack(data["inst_freq_hz"])
        x         = data["ap_numbers"]
        mean_vals = np.nanmean(arr, axis=0)

        plt.figure(figsize=(8, 5))
        plt.plot(x, mean_vals, marker="o", linestyle="None", markersize=6, color=line_color)
        plt.xlim(x[0] - 0.25, x[-1] + 0.25)
        plt.xticks(x)
        plt.xlabel("ISI number")
        plt.ylabel("Instantaneous frequency (Hz)")
        plt.title(f"{out_prefix}: {freq_bin}")
        plt.ylim(0, 70)
        plt.yticks(np.arange(0, 71, 10))
        plt.tight_layout()
        fname_bin = freq_bin.replace(" ", "_").replace(">", "gt").replace("-", "to")
        savefig(f"{out_prefix}_{fname_bin}.png")


def plot_first_isi_frequency_vs_current(currents, first_isi_freqs, label_prefix, line_color):
    x = np.asarray(currents,     dtype=float)
    y = np.asarray(first_isi_freqs, dtype=float)

    plt.figure(figsize=(7, 5))
    plt.plot(x, y, marker="o", linestyle="None", markersize=6, color=line_color)
    plt.xlabel("Injected current (nA)")
    plt.ylabel("1st ISI frequency (Hz)")
    plt.title(f"{label_prefix} 1st ISI frequency-current relationship")
    plt.ylim(0, 70)
    plt.yticks(np.arange(0, 71, 50))
    plt.xlim(0.2, 0.9)
    plt.xticks(np.arange(0.5, 0.91, 0.1))
    plt.tight_layout()
    savefig(f"{label_prefix}_1stISI_frequency_vs_current.png")

def plot_mean_firing_rate_vs_current(currents, firing_rates, label_prefix, line_color):
    x = np.asarray(currents, dtype=float)
    y = np.asarray(firing_rates, dtype=float)

    plt.figure(figsize=(7, 5))
    plt.plot(x, y, marker="o", linestyle="None", markersize=6, color=line_color)
    plt.xlabel("Injected current (nA)")
    plt.ylabel("Mean firing rate (Hz)")
    plt.title(f"{label_prefix} F-I curve")
    plt.ylim(0,70)
    plt.yticks(np.arange(0, 71, 10))
    plt.xlim(0.18, 0.92)
    plt.xticks(np.arange(0.2, 0.91, 0.1))
    plt.tight_layout()
    savefig(f"{label_prefix}_FI_curve_mean_firing_rate.png")

def plot_ahp_depth_vs_current(currents, ahp_depths, label_prefix, line_color):
    x = np.asarray(currents, dtype=float)
    y = np.asarray(ahp_depths, dtype=float)

    plt.figure(figsize=(7, 5))
    plt.plot(x, y, marker="o", linestyle="None", markersize=6, color=line_color)
    plt.xlabel("Injected current (nA)")
    plt.ylabel("AP1 AHP depth (mV)")
    plt.title(f"{label_prefix} AP1 AHP depth vs current")
    plt.xlim(0.18, 0.92)
    plt.xticks(np.arange(0.2, 0.91, 0.1))
    plt.tight_layout()
    savefig(f"{label_prefix}_AP1_AHP_depth_vs_current.png")

def plot_example_traces_by_frequency(example_traces, label_prefix, line_color):
    for freq_bin, ex in example_traces.items():
        if ex is None:
            continue
        plt.figure(figsize=(8, 4))
        plt.plot(ex["t"], ex["v"], lw=2, color=line_color)
        plt.xlabel("Time (ms)")
        plt.ylabel("Vm (mV)")
        plt.title(
            f"{label_prefix} example trace: {freq_bin} | "
            f"{ex['current']:.2f} nA | 1st ISI={ex['first_isi_hz']:.1f} Hz"
        )
        plt.tight_layout()
        savefig(f"{label_prefix}_example_trace_"
                f"{freq_bin.replace(' ', '').replace('>', 'gt').replace('-', '_')}.png")


def plot_05nA_specials_condition(ap_numbers, amplitudes, inst_freq_x, inst_freq_hz,
                                  amp_tag, label_prefix, line_color):
    plt.figure(figsize=(7, 5))
    if len(inst_freq_x) > 0 and len(inst_freq_hz) > 0:
        plt.plot(inst_freq_x, inst_freq_hz, marker="o", linestyle="None",
                 markersize=6, color=line_color)
        plt.xlim(0.75, len(inst_freq_hz) + 0.25)
        plt.xticks(np.arange(1, len(inst_freq_hz) + 1, 1))
    plt.xlabel("ISI Number")
    plt.ylabel("Instantaneous frequency (Hz)")
    plt.title(f"{label_prefix} instantaneous frequency vs ISI number: 0.50 nA")
    plt.ylim(0, 70)
    plt.yticks(np.arange(0, 71, 10))
    plt.tight_layout()
    savefig(f"{label_prefix}_instfreq_vs_ISInumber_{amp_tag}.png")

    plt.figure(figsize=(8, 5))
    if len(ap_numbers) > 0 and len(amplitudes) > 0:
        plt.plot(ap_numbers, amplitudes, marker="o", linestyle="None",
                 markersize=6, color=line_color)
        plt.xlim(0, 10)
        plt.xticks(np.arange(1, 11, 1))
    plt.xlabel("AP number")
    plt.ylabel("AP amplitude (mV)")
    plt.title(f"{label_prefix} amplitude vs AP number: 0.50 nA")
    plt.ylim(70, 110)
    plt.yticks(np.arange(70, 111, 10))
    plt.tight_layout()
    savefig(f"{label_prefix}_amplitude_vs_APnumber_{amp_tag}.png")


# ------------------------------------------------------------
# AP overlay helpers
# ------------------------------------------------------------
def extract_aligned_ap_segment(t, v, spike_times, ap_number, pre_ms=3.0, post_ms=8.0):
    if len(spike_times) < ap_number:
        return None, None

    # spike_times are threshold-crossing/onset times
    ts = float(spike_times[ap_number - 1])

    w = (t >= ts - pre_ms) & (t <= ts + post_ms)
    if np.sum(w) < 5:
        return None, None

    t_seg = np.asarray(t[w], dtype=float)
    v_seg = np.asarray(v[w], dtype=float)

    # align to spike onset/threshold crossing, NOT peak
    return t_seg - ts, v_seg


def interp_trace_to_common_timebase(t_seg, v_seg, t_common):
    if t_seg is None or v_seg is None or len(t_seg) < 2:
        return np.full_like(t_common, np.nan, dtype=float)
    order  = np.argsort(t_seg)
    t_seg  = np.asarray(t_seg[order], dtype=float)
    v_seg  = np.asarray(v_seg[order], dtype=float)
    keep   = np.concatenate(([True], np.diff(t_seg) > 0))
    t_seg  = t_seg[keep]
    v_seg  = v_seg[keep]
    if len(t_seg) < 2:
        return np.full_like(t_common, np.nan, dtype=float)
    y       = np.interp(t_common, t_seg, v_seg, left=np.nan, right=np.nan)
    outside = (t_common < t_seg[0]) | (t_common > t_seg[-1])
    y[outside] = np.nan
    return y


def init_overlay_store():
    return {
        "10-40 Hz":  {"AP1": [], "AP5": []},
        "40-80 Hz":  {"AP1": [], "AP5": []},
        "80-120 Hz": {"AP1": [], "AP5": []},
        ">120 Hz":   {"AP1": [], "AP5": []},
    }


def add_trace_to_overlay_store(overlay_store, freq_bin, t, v, spike_times):
    if freq_bin not in overlay_store:
        return
    t_ap1, v_ap1 = extract_aligned_ap_segment(
        t, v, spike_times,
        ap_number=1,
        pre_ms=3.0,
        post_ms=8.0
    )

    t_ap5, v_ap5 = extract_aligned_ap_segment(
        t, v, spike_times,
        ap_number=5,
        pre_ms=3.0,
        post_ms=8.0
    )
    overlay_store[freq_bin]["AP1"].append((t_ap1, v_ap1))
    overlay_store[freq_bin]["AP5"].append((t_ap5, v_ap5))


def plot_single_overlay_ap1_vs_ap5(t, v, spike_times, freq_bin, model_label,
                                    out_prefix, amp_tag):
    t_ap1, v_ap1 = extract_aligned_ap_segment(
        t, v, spike_times,
        ap_number=1,
        pre_ms=3.0,
        post_ms=8.0
    )

    t_ap5, v_ap5 = extract_aligned_ap_segment(
        t, v, spike_times,
        ap_number=5,
        pre_ms=3.0,
        post_ms=8.0
    )
    if t_ap1 is None or t_ap5 is None:
        print(f"Skipping single overlay for {freq_bin}: fewer than 5 usable spikes.")
        return

    plt.figure(figsize=(6, 4))
    plt.plot(t_ap1, v_ap1, color=OVERLAY_AP1_COLOR, lw=OVERLAY_LINEWIDTH, label="AP1")
    plt.plot(t_ap5, v_ap5, color=OVERLAY_AP5_COLOR, lw=OVERLAY_LINEWIDTH, label="AP5")
    plt.xlabel("Time relative to spike onset (ms)")
    plt.ylabel("Vm (mV)")
    plt.title(f"{model_label} | {freq_bin} | AP1 vs AP5")
    plt.xlim(-2.0, 6.0)
    plt.legend(frameon=False)
    plt.tight_layout()
    fname_bin = freq_bin.replace(" ", "_").replace(">", "gt").replace("-", "to")
    savefig(f"{out_prefix}_single_AP1_vs_AP5_{fname_bin}_{amp_tag}.png")


def plot_binned_overlay_ap1_vs_ap5(overlay_store, model_label, out_prefix):
    t_common = np.arange(-3.0, 8.0 + 0.025, 0.025)

    for freq_bin, d in overlay_store.items():
        if len(d["AP1"]) == 0 or len(d["AP5"]) == 0:
            print(f"Skipping binned overlay for {freq_bin}: no stored traces.")
            continue
        ap1_mat  = np.vstack([interp_trace_to_common_timebase(t, v, t_common)
                               for t, v in d["AP1"]])
        ap5_mat  = np.vstack([interp_trace_to_common_timebase(t, v, t_common)
                               for t, v in d["AP5"]])
        ap1_mean = np.nanmean(ap1_mat, axis=0)
        ap5_mean = np.nanmean(ap5_mat, axis=0)

        plt.figure(figsize=(6, 4))
        plt.plot(t_common, ap1_mean, color=OVERLAY_AP1_COLOR,
                 lw=OVERLAY_LINEWIDTH, label="AP1")
        plt.plot(t_common, ap5_mean, color=OVERLAY_AP5_COLOR,
                 lw=OVERLAY_LINEWIDTH, label="AP5")
        plt.xlabel("Time relative to spike onset (ms)")
        plt.ylabel("Vm (mV)")
        plt.title(f"{model_label} binned | {freq_bin} | AP1 vs AP5")
        plt.xlim(-2.0, 6.0)
        plt.legend(frameon=False)
        plt.tight_layout()
        fname_bin = freq_bin.replace(" ", "_").replace(">", "gt").replace("-", "to")
        savefig(f"{out_prefix}_BINNED_AP1_vs_AP5_{fname_bin}.png")

def plot_direct_AP1_AP5_check(t, v, spike_times, widths, amplitudes, out_prefix, amp_tag):
    if len(spike_times) < 5:
        print("Not enough spikes for AP1/AP5 direct check")
        return

    plt.figure(figsize=(6, 4))

    for ap_num, color in [(1, "black"), (5, "blue")]:
        ts = spike_times[ap_num - 1]

        w = (t >= ts - 3.0) & (t <= ts + 8.0)
        tt = t[w]
        vv = v[w]

        t_peak = tt[np.argmax(vv)]
        tt_align = tt - t_peak

        plt.plot(
            tt_align,
            vv,
            color=color,
            lw=1.5,
            label=f"AP{ap_num}: width={widths[ap_num-1]:.3f} ms, amp={amplitudes[ap_num-1]:.1f} mV"
        )

    plt.xlabel("Time relative to AP peak (ms)")
    plt.ylabel("Vm (mV)")
    plt.title(f"{out_prefix} direct AP1 vs AP5 check: {amp_tag}")
    plt.xlim(-0.5, 2.5)
    plt.axhline(-20, linestyle=":", linewidth=1)
    plt.legend(frameon=False)
    plt.tight_layout()
    savefig(f"{out_prefix}_DIRECT_AP1_vs_AP5_check_{amp_tag}.png")


def plot_v_dvdt_phase_plane(t, v, label_prefix, amp_tag, line_color,
                             t_start=None, t_end=None):
    t = np.asarray(t, dtype=float)
    v = np.asarray(v, dtype=float)
    if t_start is not None or t_end is not None:
        mask = np.ones_like(t, dtype=bool)
        if t_start is not None:
            mask &= (t >= t_start)
        if t_end is not None:
            mask &= (t <= t_end)
        t = t[mask]
        v = v[mask]
    if len(t) < 3:
        return
    dvdt = np.gradient(v, t)
    plt.figure(figsize=(6, 5))
    plt.plot(v, dvdt, lw=1.0, color=line_color)
    plt.xlabel("Vm (mV)")
    plt.ylabel("dV/dt (mV/ms)")
    plt.title(f"{label_prefix} phase plane: {amp_tag}")
    plt.tight_layout()
    savefig(f"{label_prefix}_phase_plane_{amp_tag}.png")

def try_record_mech_ref(seg, mech_name, ref_names):
    """
    Tries to record a mechanism-local variable, e.g. seg.Cav22._ref_ica
    Returns an h.Vector or None.
    """
    try:
        mech = getattr(seg, mech_name)
    except Exception:
        return None

    for ref_name in ref_names:
        try:
            vec = h.Vector()
            vec.record(getattr(mech, ref_name))
            return vec
        except Exception:
            continue

    return None


def vec_to_numpy_inward(vec):
    """
    Convert recorded Ca current vector to numpy and flip sign so inward Ca plots upward.
    """
    if vec is None:
        return None
    return -np.array(vec, dtype=float)


def sum_optional_arrays(*arrays):
    valid = [np.asarray(a, dtype=float) for a in arrays if a is not None]
    if len(valid) == 0:
        return None
    out = np.zeros_like(valid[0])
    for a in valid:
        out += a
    return out

# ------------------------------------------------------------
# 3-cell soma overlay plots panelled as WT -> removed -> redist
# ------------------------------------------------------------
def collect_soma_overlay_trace(
    condition_name,
    bk_split,
    bk_total_scale,
    cav12_scale,
    amp=0.50,
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
):
    # ------------------------------------------------------------
    # local helpers for recording current refs
    # ------------------------------------------------------------
    def try_record_mech_ref(seg_obj, mech_name, ref_names):
        """
        Try mechanism-local refs first, e.g. seg.Cav22._ref_ica.
        Then try segment-level refs, e.g. seg._ref_inca.
        """
        try:
            mech = getattr(seg_obj, mech_name)
            for ref_name in ref_names:
                try:
                    vec = h.Vector()
                    vec.record(getattr(mech, ref_name))
                    return vec
                except Exception:
                    continue
        except Exception:
            pass

        for ref_name in ref_names:
            try:
                vec = h.Vector()
                vec.record(getattr(seg_obj, ref_name))
                return vec
            except Exception:
                continue

        return None

    def vec_to_numpy_inward(vec): #so current isnt coming donward and looking weird
        """
        Convert recorded Ca current vector to numpy and flip sign so inward Ca plots upward.
        """
        if vec is None:
            return None
        return -np.array(vec, dtype=float)

    cell = DGGranuleLikeCell(
        name=condition_name.replace(" ", "_"),
        bk_split=bk_split,
        bk_total_scale=bk_total_scale,
    )

    if cav12_scale != 1.0:
        scale_cav12_everywhere(cell, cav12_scale)

    cell.add_current_clamp(delay=delay, dur=dur, amp=float(amp), sec=cell.soma, loc=0.5)

    seg = cell.soma(0.5)
    dist_seg = cell.dend_dist(0.9)
    spine_seg = cell.spines[-1](0.5) if len(cell.spines) > 0 else None

    t_vec = h.Vector()
    v_vec = h.Vector()
    v_dist_vec = h.Vector()
    v_spine_vec = h.Vector()

    cai_vec = h.Vector()
    cai_dist_vec = h.Vector()
    cai_spine_vec = h.Vector()

    cav12_vec = h.Vector()

    t_vec.record(h._ref_t)
    v_vec.record(seg._ref_v)
    v_dist_vec.record(dist_seg._ref_v)

    if spine_seg is not None:
        v_spine_vec.record(spine_seg._ref_v)

    try:
        cai_vec.record(seg._ref_cai)
    except Exception as e:
        print(f"WARNING: could not record soma cai for {condition_name}: {e}")
        cai_vec = None

    try:
        cai_dist_vec.record(dist_seg._ref_cai)
    except Exception as e:
        print(f"WARNING: could not record distal dendrite cai for {condition_name}: {e}")
        cai_dist_vec = None

    try:
        if spine_seg is not None:
            cai_spine_vec.record(spine_seg._ref_cai)
        else:
            cai_spine_vec = None
    except Exception as e:
        print(f"WARNING: could not record distal spine cai for {condition_name}: {e}")
        cai_spine_vec = None

    try:
        # Cav12.mod writes Cav1.2 current to the segment lca ion variable: ilca
        cav12_vec.record(seg._ref_ilca)
    except Exception as e:
        print(f"WARNING: could not record soma Cav1.2 ilca for {condition_name}: {e}")
        cav12_vec = None

    # ------------------------------------------------------------
    # record other soma Ca-current sources
    # ------------------------------------------------------------
    cav13_vec = try_record_mech_ref(
        seg,
        "Cav13",
        ["_ref_ilca13", "_ref_ica"],
    )

    cav21_vec = try_record_mech_ref(
        seg,
        "Cav2_1",
        ["_ref_ipca", "_ref_ica"],
    )

    cav22_vec = try_record_mech_ref(
        seg,
        "Cav22",
        ["_ref_inca", "_ref_ica"],
    )

    cav32_vec = try_record_mech_ref(
        seg,
        "Cav32",
        ["_ref_itca", "_ref_ica"],
    )

    if cav13_vec is None:
        print(f"WARNING: could not record soma Cav1.3 current for {condition_name}")
    if cav21_vec is None:
        print(f"WARNING: could not record soma Cav2.1 current for {condition_name}")
    if cav22_vec is None:
        print(f"WARNING: could not record soma Cav2.2 current for {condition_name}")
    if cav32_vec is None:
        print(f"WARNING: could not record soma Cav3.2 current for {condition_name}")

    bk12_vec = h.Vector() if has_mech(cell.soma, "BK_Cav12") else None
    bk21_vec = h.Vector() if has_mech(cell.soma, "BK_Cav21") else None
    bk22_vec = h.Vector() if has_mech(cell.soma, "BK_Cav22") else None
    sk2_vec = h.Vector() if has_mech(cell.soma, "SK2") else None

    if bk12_vec is not None:
        bk12_vec.record(seg.BK_Cav12._ref_ik)
    if bk21_vec is not None:
        bk21_vec.record(seg.BK_Cav21._ref_ik)
    if bk22_vec is not None:
        bk22_vec.record(seg.BK_Cav22._ref_ik)
    if sk2_vec is not None:
        sk2_vec.record(seg.SK2._ref_ik)

    h.dt = dt
    h.tstop = tstop
    h.finitialize(v_init)
    h.frecord_init()
    h.continuerun(tstop)

    t = np.array(t_vec, dtype=float)
    v = np.array(v_vec, dtype=float)
    v_dist = np.array(v_dist_vec, dtype=float)
    v_spine = np.array(v_spine_vec, dtype=float) if spine_seg is not None else None

    cai = np.array(cai_vec, dtype=float) if cai_vec is not None else None
    cai_dist = np.array(cai_dist_vec, dtype=float) if cai_dist_vec is not None else None
    cai_spine = np.array(cai_spine_vec, dtype=float) if cai_spine_vec is not None else None

    # Cav inward currents are negative, so multiply by -1 to plot upward
    cav12 = -np.array(cav12_vec, dtype=float) if cav12_vec is not None else None
    cav13 = vec_to_numpy_inward(cav13_vec)
    cav21 = vec_to_numpy_inward(cav21_vec)
    cav22 = vec_to_numpy_inward(cav22_vec)
    cav32 = vec_to_numpy_inward(cav32_vec)

    # total Ca current = sum of available Ca sources
    ca_total = np.zeros_like(v)
    any_ca = False

    for arr_ca in [cav12, cav13, cav21, cav22, cav32]:
        if arr_ca is not None:
            ca_total += arr_ca
            any_ca = True

    if not any_ca:
        ca_total = None

    bk12 = np.array(bk12_vec, dtype=float) if bk12_vec is not None else None
    bk21 = np.array(bk21_vec, dtype=float) if bk21_vec is not None else None
    bk22 = np.array(bk22_vec, dtype=float) if bk22_vec is not None else None
    sk2 = np.array(sk2_vec, dtype=float) if sk2_vec is not None else None

    bk_total = np.zeros_like(v)
    if bk12 is not None:
        bk_total += bk12
    if bk21 is not None:
        bk_total += bk21
    if bk22 is not None:
        bk_total += bk22

    print(
        condition_name,
        "Ca peaks | total:",
        np.nanmax(ca_total) if ca_total is not None else None,
        "| Cav12:",
        np.nanmax(cav12) if cav12 is not None else None,
        "| Cav13:",
        np.nanmax(cav13) if cav13 is not None else None,
        "| Cav21:",
        np.nanmax(cav21) if cav21 is not None else None,
        "| Cav22:",
        np.nanmax(cav22) if cav22 is not None else None,
        "| Cav32:",
        np.nanmax(cav32) if cav32 is not None else None,
    )

    return {
        "label": condition_name,
        "t": t,
        "v": v,
        "v_dist": v_dist,
        "v_spine": v_spine,

        "cai": cai,
        "cai_dist": cai_dist,
        "cai_spine": cai_spine,

        "ca_total": ca_total,
        "cav12": cav12,
        "cav13": cav13,
        "cav21": cav21,
        "cav22": cav22,
        "cav32": cav32,

        "bk_total": bk_total,
        "bk12": bk12,
        "bk21": bk21,
        "bk22": bk22,
        "sk2": sk2,
    }

def plot_cai_compartments_all_3cells(
    traces,
    colors,
    filename,
    single_ap_windows=None,
    y_limits=(0.0, 3.0e-3),
    sci_exponent=-3,
    y_tick_step=0.5e-3,
):
    if single_ap_windows is None:
        single_ap_windows = [
            (95, 130),
            (95, 130),
            (95, 130),
        ]

    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=False, sharey=True)

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        c = colors[i]

        if traces[i]["cai"] is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai"][w],
                color=c,
                linestyle="-",
                linewidth=2,
                label="soma",
            )

        if traces[i]["cai_dist"] is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai_dist"][w],
                color=c,
                linestyle="--",
                linewidth=2,
                alpha=0.75,
                label="distal dendrite",
            )

        if traces[i]["cai_spine"] is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai_spine"][w],
                color=c,
                linestyle=":",
                linewidth=2,
                alpha=0.75,
                label="distal spine",
            )

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        apply_shared_scientific_yaxis(
            ax,
            y_limits=y_limits,
            exponent=sci_exponent,
            n_ticks=7,
            tick_step=y_tick_step,
        )

        if i == 0:
            ax.set_ylabel("Ca concentration, cai (mM)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


def finite_concat_overlay(arr_list):
    vals = []
    for a in arr_list:
        if a is None:
            continue
        a = np.asarray(a, dtype=float)
        vals.append(a[np.isfinite(a)])
    if len(vals) == 0:
        return np.array([])
    return np.concatenate(vals)


def panel_limits_overlay(arr_list, pad_frac=0.05, force_zero_min=False):
    vals = finite_concat_overlay(arr_list)
    if len(vals) == 0:
        return None, None

    ymin = float(np.min(vals))
    ymax = float(np.max(vals))

    if force_zero_min:
        ymin = 0.0

    yr = ymax - ymin
    pad = yr * pad_frac if yr > 0 else 1e-9
    return ymin - pad, ymax + pad

def clean_bk_axis_for_signal(traces, signal_key): #for cleaner plots
    """
    Auto-scales each BK signal, but forces clean tick numbers:
    0, 0.5, 1.0, 1.5 with only the ×10^ exponent changing.
    """
    vals = finite_concat_overlay([tr[signal_key] for tr in traces])

    if len(vals) == 0:
        return None, None, None

    ymax = float(np.nanmax(vals))

    if not np.isfinite(ymax) or ymax <= 0:
        return None, None, None

    # choose exponent so signal fits under 1.5 × 10^exponent
    exponent = int(np.ceil(np.log10(ymax / 1.5)))

    y_top = 1.5 * (10.0 ** exponent)
    tick_step = 0.5 * (10.0 ** exponent)

    return (0.0, y_top), exponent, tick_step

def plot_soma_ap_all_3cells(traces, colors, filename):
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    vm_ylim = panel_limits_overlay([tr["v"] for tr in traces])

    for i, ax in enumerate(axes):
        ax.plot(traces[i]["t"], traces[i]["v"], color=colors[i])
        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

    fig.tight_layout()
    savefig(filename)


def plot_ap_with_overlay_all_3cells(
    traces,
    signal_key,
    signal_ylabel,
    colors,
    filename,
    signal_color="grey",
    y_limits=None,
    sci_exponent=None,
    n_y_ticks=5,
    y_tick_step=None,
):
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True)

    vm_ylim = panel_limits_overlay([tr["v"] for tr in traces])

    if y_limits is None:
        sig_ylim = panel_limits_overlay([tr[signal_key] for tr in traces], force_zero_min=False)
    else:
        sig_ylim = y_limits

    for i, ax in enumerate(axes):
        ax.plot(traces[i]["t"], traces[i]["v"], color=colors[i])
        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        ax2 = ax.twinx()
        if traces[i][signal_key] is not None:
            ax2.plot(
                traces[i]["t"],
                traces[i][signal_key],
                color=signal_color,
                linestyle="--",
            )

        if i == 2:
            ax2.set_ylabel(signal_ylabel)

        apply_shared_scientific_yaxis(
            ax2,
            y_limits=sig_ylim,
            exponent=sci_exponent,
            n_ticks=n_y_ticks,
            tick_step=y_tick_step,
        )

    fig.tight_layout()
    savefig(filename)


def plot_single_ap_with_overlay_all_3cells(
    traces,
    signal_key,
    signal_ylabel,
    colors,
    filename,
    single_ap_windows=None,
    signal_color="grey",
    y_limits=None,
    sci_exponent=None,
    n_y_ticks=5,
    y_tick_step=None,
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
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        vm_zoom.append(traces[i]["v"][w])

        if traces[i][signal_key] is not None:
            sig_zoom.append(traces[i][signal_key][w])
        else:
            sig_zoom.append(None)

    vm_ylim = panel_limits_overlay(vm_zoom)

    if y_limits is None:
        sig_ylim = panel_limits_overlay(sig_zoom, force_zero_min=False)
    else:
        sig_ylim = y_limits

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        ax.plot(traces[i]["t"][w], traces[i]["v"][w], color=colors[i])
        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        ax2 = ax.twinx()
        if traces[i][signal_key] is not None:
            ax2.plot(
                traces[i]["t"][w],
                traces[i][signal_key][w],
                color=signal_color,
                linestyle="--",
            )

        if i == 2:
            ax2.set_ylabel(signal_ylabel)

        apply_shared_scientific_yaxis(
            ax2,
            y_limits=sig_ylim,
            exponent=sci_exponent,
            n_ticks=n_y_ticks,
            tick_step=y_tick_step,
        )

    fig.tight_layout()
    savefig(filename)


def plot_compartment_vm_all_3cells(traces, colors, filename):
    """
    Plots soma, distal dendrite, and most distal spine Vm for each condition.
    """
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    vm_ylim = panel_limits_overlay(
        [tr["v"] for tr in traces] +
        [tr["v_dist"] for tr in traces] +
        [tr["v_spine"] for tr in traces if tr["v_spine"] is not None]
    )

    for i, ax in enumerate(axes):
        ax.plot(traces[i]["t"], traces[i]["v"], color=colors[i], lw=1.5, label="soma")
        ax.plot(traces[i]["t"], traces[i]["v_dist"], color="grey", lw=1.2, linestyle="--", label="distal dendrite")

        if traces[i]["v_spine"] is not None:
            ax.plot(traces[i]["t"], traces[i]["v_spine"], color="darkgrey", lw=1.2, linestyle=":", label="distal spine")

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


def plot_single_ap_compartment_vm_all_3cells(
    traces,
    colors,
    filename,
    single_ap_windows=None,
):
    if single_ap_windows is None:
        single_ap_windows = [
            (95, 130),
            (95, 130),
            (95, 130),
        ]

    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    vm_zoom = []

    for i in range(3):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        vm_zoom.append(traces[i]["v"][w])
        vm_zoom.append(traces[i]["v_dist"][w])

        if traces[i]["v_spine"] is not None:
            vm_zoom.append(traces[i]["v_spine"][w])

    vm_ylim = panel_limits_overlay(vm_zoom)

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        ax.plot(traces[i]["t"][w], traces[i]["v"][w], color=colors[i], lw=1.5, label="soma")
        ax.plot(traces[i]["t"][w], traces[i]["v_dist"][w], color="grey", lw=1.2, linestyle="--", label="distal dendrite")

        if traces[i]["v_spine"] is not None:
            ax.plot(traces[i]["t"][w], traces[i]["v_spine"][w], color="darkgrey", lw=1.2, linestyle=":", label="distal spine")

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


def plot_cai_compartments_all_3cells(
    traces,
    colors,
    filename,
    single_ap_windows=None,
):
    if single_ap_windows is None:
        single_ap_windows = [
            (95, 130),
            (95, 130),
            (95, 130),
        ]

    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=False, sharey=True)

    # auto-scale from all plotted cai traces
    cai_zoom = []
    for i in range(3):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        for key in ["cai", "cai_dist", "cai_spine"]:
            if traces[i][key] is not None:
                cai_zoom.append(traces[i][key][w])

    cai_ylim = panel_limits_overlay(cai_zoom, force_zero_min=False)

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        c = colors[i]

        if traces[i]["cai"] is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai"][w],
                color=c,
                linestyle="-",
                linewidth=2,
                label="soma",
            )

        if traces[i]["cai_dist"] is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai_dist"][w],
                color=c,
                linestyle="--",
                linewidth=2,
                alpha=0.75,
                label="distal dendrite",
            )

        if traces[i]["cai_spine"] is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai_spine"][w],
                color=c,
                linestyle=":",
                linewidth=2,
                alpha=0.75,
                label="distal spine",
            )

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if cai_ylim[0] is not None:
            ax.set_ylim(cai_ylim)

        if i == 0:
            ax.set_ylabel("Ca concentration, cai (mM)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


def finite_concat_overlay(arr_list):
    vals = []
    for a in arr_list:
        if a is None:
            continue
        a = np.asarray(a, dtype=float)
        vals.append(a[np.isfinite(a)])
    if len(vals) == 0:
        return np.array([])
    return np.concatenate(vals)


def panel_limits_overlay(arr_list, pad_frac=0.05, force_zero_min=False):
    vals = finite_concat_overlay(arr_list)
    if len(vals) == 0:
        return None, None

    ymin = float(np.min(vals))
    ymax = float(np.max(vals))

    if force_zero_min:
        ymin = 0.0

    yr = ymax - ymin
    pad = yr * pad_frac if yr > 0 else 1e-9
    return ymin - pad, ymax + pad


def plot_soma_ap_all_3cells(traces, colors, filename):
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    vm_ylim = panel_limits_overlay([tr["v"] for tr in traces])

    for i, ax in enumerate(axes):
        ax.plot(traces[i]["t"], traces[i]["v"], color=colors[i])
        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

    fig.tight_layout()
    savefig(filename)


def plot_ap_with_overlay_all_3cells(
    traces,
    signal_key,
    signal_ylabel,
    colors,
    filename,
    signal_color="grey",
    y_limits=None,
    sci_exponent=None,
    n_y_ticks=5,
    y_tick_step=None,
):
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True)

    vm_ylim = panel_limits_overlay([tr["v"] for tr in traces])

    if y_limits is None:
        sig_ylim = panel_limits_overlay([tr[signal_key] for tr in traces], force_zero_min=False)
    else:
        sig_ylim = y_limits

    for i, ax in enumerate(axes):
        ax.plot(traces[i]["t"], traces[i]["v"], color=colors[i])
        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        ax2 = ax.twinx()
        if traces[i][signal_key] is not None:
            ax2.plot(
                traces[i]["t"],
                traces[i][signal_key],
                color=signal_color,
                linestyle="--",
            )

        if i == 2:
            ax2.set_ylabel(signal_ylabel)

        apply_shared_scientific_yaxis(
            ax2,
            y_limits=sig_ylim,
            exponent=sci_exponent,
            n_ticks=n_y_ticks,
            tick_step=y_tick_step,
        )

    fig.tight_layout()
    savefig(filename)


def plot_single_ap_with_overlay_all_3cells(
    traces,
    signal_key,
    signal_ylabel,
    colors,
    filename,
    single_ap_windows=None,
    signal_color="grey",
    y_limits=None,
    sci_exponent=None,
    n_y_ticks=5,
    y_tick_step=None,
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
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        vm_zoom.append(traces[i]["v"][w])

        if traces[i][signal_key] is not None:
            sig_zoom.append(traces[i][signal_key][w])
        else:
            sig_zoom.append(None)

    vm_ylim = panel_limits_overlay(vm_zoom)

    if y_limits is None:
        sig_ylim = panel_limits_overlay(sig_zoom, force_zero_min=False)
    else:
        sig_ylim = y_limits

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        ax.plot(traces[i]["t"][w], traces[i]["v"][w], color=colors[i])
        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if i == 0:
            ax.set_ylabel("Vm (mV)")

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        ax2 = ax.twinx()
        if traces[i][signal_key] is not None:
            ax2.plot(
                traces[i]["t"][w],
                traces[i][signal_key][w],
                color=signal_color,
                linestyle="--",
            )

        if i == 2:
            ax2.set_ylabel(signal_ylabel)

        apply_shared_scientific_yaxis(
            ax2,
            y_limits=sig_ylim,
            exponent=sci_exponent,
            n_ticks=n_y_ticks,
            tick_step=y_tick_step,
        )

    fig.tight_layout()
    savefig(filename)


def plot_compartment_vm_all_3cells(traces, colors, filename):
    """
    Plots soma, distal dendrite, and most distal spine Vm for each condition.
    """
    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    vm_ylim = panel_limits_overlay(
        [tr["v"] for tr in traces] +
        [tr["v_dist"] for tr in traces] +
        [tr["v_spine"] for tr in traces if tr["v_spine"] is not None]
    )

    for i, ax in enumerate(axes):
        ax.plot(traces[i]["t"], traces[i]["v"], color=colors[i], lw=1.5, label="soma")
        ax.plot(traces[i]["t"], traces[i]["v_dist"], color="grey", lw=1.2, linestyle="--", label="distal dendrite")

        if traces[i]["v_spine"] is not None:
            ax.plot(traces[i]["t"], traces[i]["v_spine"], color="darkgrey", lw=1.2, linestyle=":", label="distal spine")

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(0, 500)

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


def plot_single_ap_compartment_vm_all_3cells(
    traces,
    colors,
    filename,
    single_ap_windows=None,
):
    if single_ap_windows is None:
        single_ap_windows = [
            (95, 130),
            (95, 130),
            (95, 130),
        ]

    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    vm_zoom = []

    for i in range(3):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        vm_zoom.append(traces[i]["v"][w])
        vm_zoom.append(traces[i]["v_dist"][w])

        if traces[i]["v_spine"] is not None:
            vm_zoom.append(traces[i]["v_spine"][w])

    vm_ylim = panel_limits_overlay(vm_zoom)

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        ax.plot(traces[i]["t"][w], traces[i]["v"][w], color=colors[i], lw=1.5, label="soma")
        ax.plot(traces[i]["t"][w], traces[i]["v_dist"][w], color="grey", lw=1.2, linestyle="--", label="distal dendrite")

        if traces[i]["v_spine"] is not None:
            ax.plot(traces[i]["t"][w], traces[i]["v_spine"][w], color="darkgrey", lw=1.2, linestyle=":", label="distal spine")

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if vm_ylim[0] is not None:
            ax.set_ylim(vm_ylim)

        if i == 0:
            ax.set_ylabel("Vm (mV)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


def plot_cai_dist_spine_all_3cells(
    traces,
    colors,
    filename,
    single_ap_windows=None,
):
    if single_ap_windows is None:
        single_ap_windows = [
            (95, 130),
            (95, 130),
            (95, 130),
        ]

    fig, axes = plt.subplots(1, 3, figsize=(15, 4), sharex=True, sharey=True)

    #auto-scale from dist dendrite & spine cai traces only
    cai_zoom = []
    for i in range(3):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        for key in ["cai_dist", "cai_spine"]:
            if traces[i].get(key) is not None:
                cai_zoom.append(traces[i][key][w])

    cai_ylim = panel_limits_overlay(cai_zoom, force_zero_min=False)

    for i, ax in enumerate(axes):
        tmin, tmax = single_ap_windows[i]
        w = (traces[i]["t"] >= tmin) & (traces[i]["t"] <= tmax)

        if traces[i].get("cai_dist") is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai_dist"][w],
                color=colors[i],
                linestyle="--",
                label="distal dendrite",
            )

        if traces[i].get("cai_spine") is not None:
            ax.plot(
                traces[i]["t"][w],
                traces[i]["cai_spine"][w],
                color=colors[i],
                linestyle=":",
                label="distal spine",
            )

        ax.set_title(traces[i]["label"])
        ax.set_xlabel("Time (ms)")
        ax.set_xlim(tmin, tmax)

        if cai_ylim[0] is not None:
            ax.set_ylim(cai_ylim)

        if i == 0:
            ax.set_ylabel("Ca concentration, cai (mM)")
            ax.legend(frameon=False)

    fig.tight_layout()
    savefig(filename)


# ------------------------------------------------------------
# Cav1.2 scaling helper
# ------------------------------------------------------------
def scale_cav12_everywhere(cell, factor):
    for sec in ([cell.soma, cell.dend_prox, cell.dend_dist, cell.ais, cell.axon]
                + cell.spine_necks + cell.spines):
        if has_mech(sec, "Cav12"):
            for seg in sec:
                seg.Cav12.gbar *= factor


# ------------------------------------------------------------
# plots
# ------------------------------------------------------------

def make_metrics_soma_overlay_plots(
    plot_amp=0.50,
    delay=100.0,
    dur=500.0,
    tstop=700.0,
    v_init=-70.0,
    dt=0.025,
):
    # order is WT, removed, redist
    traces = [
        collect_soma_overlay_trace(
            condition_name="WT",
            bk_split=WT_BK_SPLIT,
            bk_total_scale=1.0,
            cav12_scale=1.0,
            amp=plot_amp, delay=delay, dur=dur, tstop=tstop, v_init=v_init, dt=dt,
        ),
        collect_soma_overlay_trace(
            condition_name="Cav1.2 50% BK_Cav1.2 removed",
            bk_split=CAV12_50_REMOVE_BK_SPLIT,
            bk_total_scale=CAV12_50_REMOVE_BK_TOTAL_SCALE,
            cav12_scale=0.5,
            amp=plot_amp, delay=delay, dur=dur, tstop=tstop, v_init=v_init, dt=dt,
        ),
        collect_soma_overlay_trace(
            condition_name="Cav1.2 50% redist",
            bk_split=CAV12_50_BK_SPLIT,
            bk_total_scale=1.0,
            cav12_scale=0.5,
            amp=plot_amp, delay=delay, dur=dur, tstop=tstop, v_init=v_init, dt=dt,
        ),
    ]

    colors = [
        WT_COLOR,
        CAV12_50_REMOVE_COLOR,
        CAV12_50_COLOR,
    ]

    add_ic_overlay_summary_rows(traces)

    plot_soma_ap_all_3cells(
        traces, colors,
        filename="01_soma_AP_all_3_cells.png",
    )

    plot_compartment_vm_all_3cells(
        traces,
        colors,
        filename="16_compartment_VM_soma_distdend_distspine_all_3_cells.png",
    )

    plot_single_ap_compartment_vm_all_3cells(
        traces,
        colors,
        filename="17_single_AP_compartment_VM_soma_distdend_distspine_all_3_cells.png",
    )

    plot_single_ap_with_overlay_all_3cells(
        traces,
        signal_key="cai",
        signal_ylabel="cai soma (mM)",
        colors=colors,
        filename="02_single_AP_with_Ca_overlay_soma_all_3_cells.png",
        signal_color="grey",
    )

    plot_ap_with_overlay_all_3cells(
        traces,
        signal_key="cai",
        signal_ylabel="cai soma (mM)",
        colors=colors,
        filename="03_AP_with_Ca_overlay_soma_all_3_cells.png",
        signal_color="grey",
    )

    plot_cai_compartments_all_3cells(
        traces,
        colors,
        filename="19_single_AP_cai_soma_vs_dend_vs_spine_all_3_cells.png",
        single_ap_windows=[
            (95, 130),
            (95, 130),
            (95, 130),
        ],
    )

    plot_cai_dist_spine_all_3cells(
        traces,
        colors,
        filename="20_single_AP_Cai_dist_spine_all_3_cells.png",
    )

    # ------------------------------------------------------------
    # BK overlays: fixed clean axes
    # ------------------------------------------------------------
    bk_axis_settings = {
        "bk_total": {
            "y_limits": (0.0, 1.2e-9),
            "sci_exponent": -9,
            "y_tick_step": 0.4e-9,
            "n_y_ticks": 4,
        },
        "bk12": {
            "y_limits": (0.0, 7.5e-10),
            "sci_exponent": -10,
            "y_tick_step": 1.5e-10,
            "n_y_ticks": 16,
        },
        "bk21": {
            "y_limits": (0.0, 7.5e-11),
            "sci_exponent": -11,
            "y_tick_step": 1.5e-11,
            "n_y_ticks": 16,
        },
        "bk22": {
            "y_limits": (0.0, 7.5e-10),
            "sci_exponent": -10,
            "y_tick_step": 1.5e-10,
            "n_y_ticks": 16,
        },
    }

    bk_plot_specs = [
        (
            "bk_total",
            "Total BK current density (mA/cm2)",
            "04_AP_with_total_BK_overlay_soma_all_3_cells.png",
            "05_single_AP_with_total_BK_overlay_soma_all_3_cells.png",
        ),
        (
            "bk12",
            "BK_Cav12 current density (mA/cm2)",
            "06_AP_with_BK_Cav12_overlay_soma_all_3_cells.png",
            "07_single_AP_with_BK_Cav12_overlay_soma_all_3_cells.png",
        ),
        (
            "bk21",
            "BK_Cav21 current density (mA/cm2)",
            "08_AP_with_BK_Cav21_overlay_soma_all_3_cells.png",
            "09_single_AP_with_BK_Cav21_overlay_soma_all_3_cells.png",
        ),
        (
            "bk22",
            "BK_Cav22 current density (mA/cm2)",
            "10_AP_with_BK_Cav22_overlay_soma_all_3_cells.png",
            "11_single_AP_with_BK_Cav22_overlay_soma_all_3_cells.png",
        ),
    ]

    for signal_key, signal_ylabel, full_filename, single_filename in bk_plot_specs:
        axis = bk_axis_settings[signal_key]

        plot_ap_with_overlay_all_3cells(
            traces,
            signal_key=signal_key,
            signal_ylabel=signal_ylabel,
            colors=colors,
            filename=full_filename,
            signal_color="grey",
            y_limits=axis["y_limits"],
            sci_exponent=axis["sci_exponent"],
            n_y_ticks=axis["n_y_ticks"],
            y_tick_step=axis["y_tick_step"],
        )

        plot_single_ap_with_overlay_all_3cells(
            traces,
            signal_key=signal_key,
            signal_ylabel=signal_ylabel,
            colors=colors,
            filename=single_filename,
            signal_color="grey",
            y_limits=axis["y_limits"],
            sci_exponent=axis["sci_exponent"],
            n_y_ticks=axis["n_y_ticks"],
            y_tick_step=axis["y_tick_step"],
        )

    plot_ap_with_overlay_all_3cells(
        traces,
        signal_key="sk2",
        signal_ylabel="SK2 current density (mA/cm2)",
        colors=colors,
        filename="14_AP_with_SK2_overlay_soma_all_3_cells.png",
        signal_color="grey",
    )

    plot_single_ap_with_overlay_all_3cells(
        traces,
        signal_key="sk2",
        signal_ylabel="SK2 current density (mA/cm2)",
        colors=colors,
        filename="15_single_AP_with_SK2_overlay_soma_all_3_cells.png",
        signal_color="grey",
        y_limits=(0.0, 1.2e-7),
        sci_exponent=-7,
        n_y_ticks=4,
        y_tick_step=0.4e-7,
    )

    plot_ap_with_overlay_all_3cells(
        traces,
        signal_key="cav12",
        signal_ylabel="-Cav1.2 current density (mA/cm2)",
        colors=colors,
        filename="12_AP_with_Cav12_overlay_soma_all_3_cells.png",
        signal_color="grey",
    )

    plot_single_ap_with_overlay_all_3cells(
        traces,
        signal_key="cav12",
        signal_ylabel="-Cav1.2 current density (mA/cm2)",
        colors=colors,
        filename="13_single_AP_with_Cav12_overlay_soma_all_3_cells.png",
        signal_color="grey",
    )


def make_ca_source_overlay_plots(
    plot_amp=0.50,
    delay=100.0,
    dur=300.0,
    tstop=500.0,
    v_init=-70.0,
    dt=0.025,
):
    # order: WT, removed, redist
    traces = [
        collect_soma_overlay_trace(
            condition_name="WT",
            bk_split=WT_BK_SPLIT,
            bk_total_scale=1.0,
            cav12_scale=1.0,
            amp=plot_amp, delay=delay, dur=dur, tstop=tstop, v_init=v_init, dt=dt,
        ),
        collect_soma_overlay_trace(
            condition_name="Cav1.2 50% BK_Cav1.2 removed",
            bk_split=CAV12_50_REMOVE_BK_SPLIT,
            bk_total_scale=CAV12_50_REMOVE_BK_TOTAL_SCALE,
            cav12_scale=0.5,
            amp=plot_amp, delay=delay, dur=dur, tstop=tstop, v_init=v_init, dt=dt,
        ),
        collect_soma_overlay_trace(
            condition_name="Cav1.2 50% redist",
            bk_split=CAV12_50_BK_SPLIT,
            bk_total_scale=1.0,
            cav12_scale=0.5,
            amp=plot_amp, delay=delay, dur=dur, tstop=tstop, v_init=v_init, dt=dt,
        ),
    ]

    colors = [
        WT_COLOR,
        CAV12_50_REMOVE_COLOR,
        CAV12_50_COLOR,
    ]

    plot_specs = [
        ("ca_total", "-Total Ca current density (mA/cm2)", "14_single_AP_with_total_Ca_overlay_soma_all_3_cells.png"),
        ("cav12",    "-Cav1.2 current density (mA/cm2)",   "15_single_AP_with_Cav12_overlay_soma_all_3_cells.png"),
        ("cav13",    "-Cav1.3 current density (mA/cm2)",   "16_single_AP_with_Cav13_overlay_soma_all_3_cells.png"),
        ("cav21",    "-Cav2.1 current density (mA/cm2)",   "17_single_AP_with_Cav21_overlay_soma_all_3_cells.png"),
        ("cav22",    "-Cav2.2 current density (mA/cm2)",   "18_single_AP_with_Cav22_overlay_soma_all_3_cells.png"),
        ("cav32",    "-Cav3.2 current density (mA/cm2)",   "19_single_AP_with_Cav32_overlay_soma_all_3_cells.png"),
    ]

    for signal_key, signal_ylabel, filename in plot_specs:
        plot_single_ap_with_overlay_all_3cells(
            traces,
            signal_key=signal_key,
            signal_ylabel=signal_ylabel,
            colors=colors,
            filename=filename,
            single_ap_windows=[
                (95, 130),
                (95, 130),
                (95, 130),
            ],
            signal_color="grey",
        )
# ------------------------------------------------------------
# Core per-condition sweep loop
# ------------------------------------------------------------
def run_condition(
    condition_label,
    out_prefix,
    condition_color,
    bk_split,
    bk_total_scale,
    cav12_scale,
    run_report_name,
    # simulation parameters (all passed explicitly — no globals)
    currents,
    delay,
    dur,
    tstop,
    v_init,
    dt,
    n_ap_plot,
    ap_start_idx,
    make_single_sweep_plots,
    target_overlays,
):
    """
    Runs a full current-clamp sweep for one model condition.
    All simulation parameters are explicit arguments so this
    function is completely self-contained with no reliance on
    module-level variables.
    """

    print(f"\n{'='*60}")
    print(f"Running condition: {condition_label}")
    print(f"{'='*60}")

    # ── save run report ──────────────────────────────────────
    run_meta = {
        "python_version":           sys.version,
        "neuron_version":           h.nrnversion(),
        "dt_ms":                    dt,
        "tstop_ms":                 tstop,
        "v_init_mV":                v_init,
        "celsius_C":                float(h.celsius),
        "currents_nA":              [float(x) for x in currents],
        "stim_delay_ms":            delay,
        "stim_dur_ms":              dur,
        "width_measurement_fraction": 1.0 / 3.0,
        "model":                    condition_label,
        "cav12_scale_factor":       cav12_scale,
        "bk_split":                 bk_split,
        "bk_total_scale":           bk_total_scale,
        "bk_effective_gain": {
        "BK_Cav12": effective_bk_gain("BK_Cav12"),
        "BK_Cav21": effective_bk_gain("BK_Cav21"),
        "BK_Cav22": effective_bk_gain("BK_Cav22"),
        },
    }
    save_run_report(os.path.join(OUT_DIR, run_report_name), run_meta)

    # ── initialise stores ────────────────────────────────────
    bin_store      = init_frequency_bin_store(n_ap_plot=n_ap_plot,
                                               ap_start_idx=ap_start_idx)
    bin_store_10ap = init_frequency_bin_store_10ap(n_ap_plot=10, ap_start_idx=0)
    overlay_store  = init_overlay_store()

    example_traces = {k: None for k in ["10-40 Hz", "40-80 Hz", "80-120 Hz", ">120 Hz"]}
    all_currents = []
    all_firing_rates = []
    all_first_isi_freqs = []
    all_ahp_depths = []

    # keep track of the last sweep's arrays for the end-of-loop AP1-10 plot
    last_amplitudes = np.array([])
    last_amp        = float(currents[-1])
    last_amp_tag    = f"{last_amp:.2f}nA".replace(".", "p")

    # ── main sweep loop ──────────────────────────────────────
    for amp in currents:
        cell = DGGranuleLikeCell(bk_split=bk_split, bk_total_scale=bk_total_scale)
        if cav12_scale != 1.0:
            scale_cav12_everywhere(cell, cav12_scale)
        cell.add_current_clamp(delay=delay, dur=dur, amp=float(amp))
        cell.setup_recording()

        t, vs = run_sim(cell, tstop=tstop, v_init=v_init, dt=dt)

        # phase-plane diagnostic at key currents
        if np.isclose(amp, 0.50) or np.isclose(amp, 0.55):
            amp_tag_diag = f"{amp:.2f}nA".replace(".", "p")
            plot_v_dvdt_phase_plane(
                t, vs,
                label_prefix=out_prefix,
                amp_tag=amp_tag_diag,
                line_color=condition_color,
                t_start=delay - 20.0,
                t_end=delay + dur,
            )

        spike_times, widths, amplitudes, isi, inst_freq_hz = ap_metrics_per_spike(
            t, vs,
            threshold=0.0,
            refractory_ms=2.0,
            t_start=delay,
            t_end=delay + dur,
            pre_ms=3.0,
            post_ms=15.0,
            width_frac=1.0 / 3.0,
            width_base_mode="baseline",
        )

        ahp_depth, ahp_baseline, ahp_min = first_ahp_depth(
            t, vs,
            spike_times=spike_times,
            t_start=delay,
        )

        if np.isclose(amp, 0.50):
            print("\n--- AP diagnostic at 0.50 nA ---")

            if len(spike_times) >= 5:
                ap_peaks = [
                    np.max(vs[(t > st - 1.0) & (t < st + 2.0)])
                    for st in spike_times[:5]
                ]

                print("AP1-AP5 peaks:", np.round(ap_peaks, 2))
                print("AP1-AP5 widths:", np.round(widths[:5], 3))

            if len(spike_times) >= 2:
                ahp_mask = (t > spike_times[0] + 1.0) & (t < spike_times[1] - 1.0)
                if np.any(ahp_mask):
                    print("AHP trough AP1-AP2:", np.min(vs[ahp_mask]))

        first_isi_hz  = inst_freq_hz[0] if len(inst_freq_hz) > 0 else np.nan
        sweep_freq_hz = get_sweep_frequency_hz(inst_freq_hz)
        freq_bin      = classify_frequency_bin(sweep_freq_hz)

        if freq_bin in ["10-40 Hz", "40-80 Hz"]:
            amp_tag = f"{amp:.2f}nA".replace(".", "p")
            bin_tag = freq_bin.replace(" ", "_").replace("-", "to")

            plot_direct_AP1_AP5_check(
                t=t,
                v=vs,
                spike_times=spike_times,
                widths=widths,
                amplitudes=amplitudes,
                out_prefix=f"{out_prefix}_{bin_tag}",
                amp_tag=amp_tag,
            )

        add_trace_to_overlay_store(overlay_store, freq_bin=freq_bin,
                                   t=t, v=vs, spike_times=spike_times)

        for target_amp, target_bin in target_overlays.items():
            if np.isclose(amp, target_amp) and freq_bin == target_bin:
                plot_single_overlay_ap1_vs_ap5(
                    t=t, v=vs, spike_times=spike_times,
                    freq_bin=freq_bin,
                    model_label=condition_label,
                    out_prefix=out_prefix,
                    amp_tag=f"{amp:.2f}nA".replace(".", "p"),
                )

        firing_rate_hz = len(spike_times) / (dur / 1000.0)

        add_ic_sweep_row(
            condition_label=condition_label,
            out_prefix=out_prefix,
            amp=amp,
            t=t,
            v=vs,
            spike_times=spike_times,
            widths=widths,
            amplitudes=amplitudes,
            isi=isi,
            inst_freq_hz=inst_freq_hz,
            firing_rate_hz=firing_rate_hz,
            first_isi_hz=first_isi_hz,
            sweep_freq_hz=sweep_freq_hz,
            freq_bin=freq_bin,
        )

        all_currents.append(float(amp))
        all_firing_rates.append(float(firing_rate_hz))
        all_first_isi_freqs.append(float(first_isi_hz))
        all_ahp_depths.append(float(ahp_depth))

        add_sweep_to_bin_store(bin_store, widths=widths, amplitudes=amplitudes,
                                inst_freq_hz=inst_freq_hz,
                                n_ap_plot=n_ap_plot, ap_start_idx=ap_start_idx)
        add_sweep_to_bin_store(bin_store_10ap, widths=widths, amplitudes=amplitudes,
                                inst_freq_hz=inst_freq_hz,
                                n_ap_plot=10, ap_start_idx=1)

        if freq_bin is not None and example_traces[freq_bin] is None:
            example_traces[freq_bin] = {
                "t": np.array(t), "v": np.array(vs),
                "current": float(amp),
                "first_isi_hz": float(first_isi_hz),
            }

        amp_tag = f"{amp:.2f}nA".replace(".", "p")

        if make_single_sweep_plots:
            ap_plot_start    = 1
            ap_plot_end      = 5
            widths_plot      = widths[ap_plot_start - 1:ap_plot_end]
            amplitudes_plot  = amplitudes[ap_plot_start - 1:ap_plot_end]
            spike_times_plot = spike_times[ap_plot_start - 1:ap_plot_end]

            isi_start_number  = 1
            isi_end_number    = 10
            isi_plot          = isi[isi_start_number - 1:isi_end_number]
            inst_freq_plot    = inst_freq_hz[isi_start_number - 1:isi_end_number]

            ap_x   = np.arange(ap_plot_start,    ap_plot_start    + len(widths_plot))
            amp_x  = np.arange(ap_plot_start,    ap_plot_start    + len(amplitudes_plot))
            isi_x  = np.arange(isi_start_number, isi_start_number + len(isi_plot))

            # trace
            plt.figure(figsize=(10, 6))
            plt.plot(t, vs, color=condition_color, lw=2)
            if len(spike_times_plot) > 0:
                plt.xlim(max(0.0, spike_times_plot[0] - 20.0),
                         min(t[-1], spike_times_plot[-1] + 25.0))
            plt.xlabel("Time (ms)")
            plt.ylabel("Vm (mV)")
            title_suffix = f"{amp:.2f} nA"
            if freq_bin is not None:
                title_suffix += f" | {freq_bin} | 1st ISI={first_isi_hz:.1f} Hz"
            plt.title(f"{condition_label} soma AP trace (AP1-AP5): {title_suffix}")
            plt.tight_layout()
            savefig(f"{out_prefix}_soma_AP_trace_AP1toAP5_{amp_tag}.png")

            # broadening
            plt.figure(figsize=(8, 5))
            if len(widths_plot) > 0:
                plt.plot(ap_x, widths_plot, marker="o", linestyle="None",
                         markersize=6, color=condition_color, lw=2)
                plt.xlim(ap_plot_start - 0.25, ap_plot_end + 0.25)
                plt.xticks(np.arange(ap_plot_start, ap_plot_end + 1))
            plt.xlabel("Spike number")
            plt.ylabel("AP width at 1/3 amplitude (ms)")
            plt.title(f"{condition_label} AP broadening (AP1-AP5): {amp:.2f} nA")
            plt.ylim(1.0, 3.0)
            plt.yticks(np.arange(1.0, 3.01, 0.5))
            plt.tight_layout()
            savefig(f"{out_prefix}_AP_broadening_AP1toAP5_{amp_tag}.png")

            # amplitude
            plt.figure(figsize=(8, 5))
            if len(amplitudes_plot) > 0:
                plt.plot(amp_x, amplitudes_plot, marker="o", linestyle="None",
                         markersize=6, color=condition_color, lw=2)
                plt.xlim(ap_plot_start - 0.25, ap_plot_end + 0.25)
                plt.xticks(np.arange(ap_plot_start, ap_plot_end + 1))
            plt.xlabel("AP number")
            plt.ylabel("AP amplitude (mV)")
            plt.title(f"{condition_label} AP amplitude (AP1-AP5): {amp:.2f} nA")
            plt.ylim(70, 110)
            plt.yticks(np.arange(70, 111, 10))
            plt.tight_layout()
            savefig(f"{out_prefix}_AP_amplitude_AP1toAP5_{amp_tag}.png")

            # ISI
            plt.figure(figsize=(8, 5))
            if len(isi_plot) > 0:
                plt.plot(isi_x, isi_plot, marker="o", linestyle="None",
                         markersize=6, color=condition_color, lw=2)
                plt.xlim(isi_start_number - 0.25, isi_end_number + 0.25)
                plt.xticks(np.arange(isi_start_number, isi_end_number + 1))
            plt.xlabel("ISI number")
            plt.ylabel("ISI (ms)")
            plt.title(f"{condition_label} ISI (ISI1-ISI10): {amp:.2f} nA")
            plt.tight_layout()
            savefig(f"{out_prefix}_ISI_AP1toAP5_{amp_tag}.png")

            # 0.50 nA specials
            if np.isclose(amp, 0.50):
                amp_x_10       = np.arange(1, 1 + len(amplitudes[:10]))
                inst_freq_x_10 = np.arange(1, 1 + len(inst_freq_hz[:10]))
                plot_05nA_specials_condition(
                    ap_numbers=amp_x_10,
                    amplitudes=amplitudes[:10],
                    inst_freq_x=inst_freq_x_10,
                    inst_freq_hz=inst_freq_hz[:10],
                    amp_tag=amp_tag,
                    label_prefix=out_prefix,
                    line_color=condition_color,
                )

        # stash for the AP1-10 plot after the loop
        last_amplitudes = amplitudes
        last_amp        = float(amp)
        last_amp_tag    = amp_tag

        print(
            f"{condition_label} | {amp:.2f} nA"
            f" | spikes={len(spike_times)}"
            f" | 1stISI_Hz={first_isi_hz:.3f}"
            f" | sweepFreq_Hz={sweep_freq_hz:.3f}"
            f" | bin={freq_bin}"
            f" | mean width={np.nanmean(widths)  if len(widths)      else np.nan:.3f} ms"
            f" | mean amp={np.nanmean(amplitudes) if len(amplitudes)  else np.nan:.3f} mV"
            f" | mean ISI={np.nanmean(isi)        if len(isi)         else np.nan:.3f} ms"
        )

    # ── end of sweep loop — summary plots ────────────────────
    print(f"\n{out_prefix} all_currents: {all_currents}")
    print(f"{out_prefix} all_firing_rates: {all_firing_rates}")

    plot_mean_firing_rate_vs_current(
        all_currents,
        all_firing_rates,
        label_prefix=out_prefix,
        line_color=condition_color,
    )

    plot_first_isi_frequency_vs_current(
        all_currents,
        all_first_isi_freqs,
        label_prefix=out_prefix,
        line_color=condition_color,
    )

    plot_ahp_depth_vs_current(
        all_currents,
        all_ahp_depths,
        label_prefix=out_prefix,
        line_color=condition_color,
    )

    plot_frequency_binned_broadening(
        bin_store,
        out_prefix=f"{out_prefix}_AP_broadening_binned",
        line_color=condition_color,
    )
    plot_frequency_binned_amplitude(
        bin_store,
        out_prefix=f"{out_prefix}_AP_amplitude_binned",
        line_color=condition_color,
    )
    plot_frequency_binned_instfreq(
        bin_store,
        out_prefix=f"{out_prefix}_instfreq_binned",
        line_color=condition_color,
    )
    plot_example_traces_by_frequency(
        example_traces,
        label_prefix=out_prefix,
        line_color=condition_color,
    )
    plot_binned_overlay_ap1_vs_ap5(
        overlay_store,
        model_label=condition_label,
        out_prefix=out_prefix,
    )

    # AP1-10 amplitude plot using last sweep's data
    amplitudes_10 = last_amplitudes[1:10]
    amp_x_10      = np.arange(0, len(amplitudes_10))

    plt.figure(figsize=(8, 5))
    if len(amplitudes_10) > 0:
        plt.plot(amp_x_10, amplitudes_10, marker="o", linestyle="None",
                 markersize=6, color=condition_color, lw=2)
        plt.xlim(0, 11)
        plt.xticks(np.arange(1, 11, 1))
    plt.xlabel("AP number")
    plt.ylabel("AP amplitude (mV)")
    plt.title(f"{condition_label} AP amplitude (AP1-AP10): {last_amp:.2f} nA")
    plt.ylim(70, 110)
    plt.yticks(np.arange(70, 111, 10))
    plt.tight_layout()
    savefig(f"{out_prefix}_AP_amplitude_AP1toAP10_{last_amp_tag}.png")

    plot_frequency_binned_amplitude_10ap(
        bin_store_10ap,
        out_prefix=f"{out_prefix}_AP_amplitude_binned_AP1toAP10",
        line_color=condition_color,
    )

    add_ic_binned_summary_rows(
        condition_label=condition_label,
        out_prefix=out_prefix,
        bin_store=bin_store,
    )

    print(f"\n{condition_label} frequency-bin counts:")
    for k, v in bin_store.items():
        print(f"  {k}: {len(v['widths'])} sweeps")


# ------------------------------------------------------------
# main
# ------------------------------------------------------------
if __name__ == "__main__":
    h.celsius = 34.0


    # ── simulation parameters — defined once, passed everywhere ──
    delay    = 100.0
    dur      = 300.0
    tstop    = 500.0
    v_init   = -70.0
    dt       = 0.025
    currents = np.arange(0.0, 0.91, 0.05)

    n_ap_plot             = 5
    ap_start_idx          = 0
    make_single_sweep_plots = True

    target_overlays = {
        0.30: "10-40 Hz",
        0.50: "40-80 Hz",
        0.70: "80-120 Hz",
        0.90: ">120 Hz",
    }

    # shared kwargs so each condition call is concise
    sim_kwargs = dict(
        currents=currents,
        delay=delay,
        dur=dur,
        tstop=tstop,
        v_init=v_init,
        dt=dt,
        n_ap_plot=n_ap_plot,
        ap_start_idx=ap_start_idx,
        make_single_sweep_plots=make_single_sweep_plots,
        target_overlays=target_overlays,
    )

    print("\nMaking metrics soma AP/Ca/BK overlay plots...")

    make_metrics_soma_overlay_plots(
        plot_amp=0.50,
        delay=delay,
        dur=dur,
        tstop=tstop,
        v_init=v_init,
        dt=dt,
    )

    print("Done making metrics soma AP/Ca/BK overlay plots.")

    print("\nMaking metrics Ca-source overlay plots...")
    make_ca_source_overlay_plots(
        plot_amp=0.50,
        delay=delay,
        dur=dur,
        tstop=tstop,
        v_init=v_init,
        dt=dt,
    )
    print("Done making metrics Ca-source overlay plots.")

    # ── WT ───────────────────────────────────────────────────
    run_condition(
        condition_label  = WT_LABEL,
        out_prefix       = "WT",
        condition_color  = WT_COLOR,
        bk_split         = WT_BK_SPLIT,
        bk_total_scale   = 1.0,
        cav12_scale      = 1.0,
        run_report_name  = "ic_run_report_WT.json",
        **sim_kwargs,
    )

    # ── Cav1.2 50% with redistributed BK pool ───────────────
    run_condition(
        condition_label  = CAV12_50_LABEL,
        out_prefix       = "CAV12_50",
        condition_color  = CAV12_50_COLOR,
        bk_split         = CAV12_50_BK_SPLIT,
        bk_total_scale   = 1.0,
        cav12_scale      = 0.5,
        run_report_name  = "ic_run_report_CAV12_50.json",
        **sim_kwargs,
    )

    # ── Cav1.2 50% with BK_Cav1.2 removed ──────────────────
    run_condition(
        condition_label  = CAV12_50_REMOVE_LABEL,
        out_prefix       = "CAV12_50_REMOVE",
        condition_color  = CAV12_50_REMOVE_COLOR,
        bk_split         = CAV12_50_REMOVE_BK_SPLIT,
        bk_total_scale   = CAV12_50_REMOVE_BK_TOTAL_SCALE,
        cav12_scale      = 0.5,
        run_report_name  = "ic_run_report_CAV12_50_REMOVE.json",
        **sim_kwargs,
    )

    # ── sanity check: fresh WT cell at known stimulus ────────
    print("\n--- Sanity check: fresh WT cell at 0.50 nA ---")
    _diag_cell = DGGranuleLikeCell(bk_split=WT_BK_SPLIT, bk_total_scale=1.0)
    _diag_cell.add_current_clamp(delay=delay, dur=dur, amp=0.50)
    _diag_cell.setup_recording()
    _diag_t, _diag_vs = run_sim(_diag_cell, tstop=tstop, v_init=v_init, dt=dt)

    print("max Vm:", np.max(_diag_vs))
    print("has na8st soma?", has_mech(_diag_cell.soma, "na8st"))
    print("has na8st ais? ", has_mech(_diag_cell.ais,  "na8st"))
    if has_mech(_diag_cell.soma, "na8st"):
        print("soma na8st gbar:", _diag_cell.soma(0.5).na8st.gbar)
    if has_mech(_diag_cell.ais,  "na8st"):
        print("ais  na8st gbar:", _diag_cell.ais(0.5).na8st.gbar)
    print("soma ina:", _diag_cell.soma(0.5).ina)
    print("ais  ina:", _diag_cell.ais(0.5).ina)

    ic_raw_path = os.path.join(OUT_DIR, "IC_raw_data.csv")
    save_ic_raw_data(ic_raw_path)