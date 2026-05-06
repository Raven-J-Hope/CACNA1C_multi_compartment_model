#!/usr/bin/env python3

#the ic script is better commented b/c it was made first
#cells here are the same, biopys, mechs etc copied over

import os
import sys
import csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
from neuron import h

h.load_file("stdrun.hoc")

# -----------------------------
# Conditions
# -----------------------------
WT_BK_SPLIT = {"BK_Cav22": 1/3, "BK_Cav12": 1/3, "BK_Cav21": 1/3}
CAV12_50_BK_SPLIT = {"BK_Cav22": 5/12, "BK_Cav12": 1/6, "BK_Cav21": 5/12}
CAV12_50_REMOVE_BK_SPLIT = {"BK_Cav22": 2/5, "BK_Cav12": 1/5, "BK_Cav21": 2/5}
CAV12_50_REMOVE_BK_TOTAL_SCALE = 5/6

BK_EFFECTIVE_GAIN = {
    "BK_Cav12": 3.822917700564498e-07,
    "BK_Cav21": 6.977400649452255e-08,
    "BK_Cav22": 3.4477273335658608e-06,
}


CONDITIONS = [
    dict(label="WT", prefix="WT", color="black",
         bk_split=WT_BK_SPLIT, bk_total_scale=1.0, cav12_scale=1.0),
    dict(label="Cav1.2 50% BK_Cav1.2 removed", prefix="CAV12_50_REMOVE", color="#8c52ff",
         bk_split=CAV12_50_REMOVE_BK_SPLIT, bk_total_scale=CAV12_50_REMOVE_BK_TOTAL_SCALE, cav12_scale=0.5),
    dict(label="Cav1.2 50% redist", prefix="CAV12_50_REDIST", color="#ffa6b2",
         bk_split=CAV12_50_BK_SPLIT, bk_total_scale=1.0, cav12_scale=0.5),
]

BK_DIAGNOSTIC_CONDITIONS = [
    dict(label="ONLY_BK_Cav12", prefix="ONLY_BK12", color="blue",
         bk_split={"BK_Cav22": 0.0, "BK_Cav12": 1.0, "BK_Cav21": 0.0},
         bk_total_scale=1.0, cav12_scale=1.0),

    dict(label="ONLY_BK_Cav21", prefix="ONLY_BK21", color="orange",
         bk_split={"BK_Cav22": 0.0, "BK_Cav12": 0.0, "BK_Cav21": 1.0},
         bk_total_scale=1.0, cav12_scale=1.0),

    dict(label="ONLY_BK_Cav22", prefix="ONLY_BK22", color="green",
         bk_split={"BK_Cav22": 1.0, "BK_Cav12": 0.0, "BK_Cav21": 0.0},
         bk_total_scale=1.0, cav12_scale=1.0),
]

print("Python version:", sys.version)
print("NEURON version:", h.nrnversion())

# -----------------------------
# Paths
# -----------------------------
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
OUT_DIR = os.path.join(BASE_DIR, "submission outputs")
FIG_DIR = os.path.join(OUT_DIR, "vc_figures")
os.makedirs(FIG_DIR, exist_ok=True)

MOD_DIR = "/home/raven/PycharmProjects/Masters/Mod_Files"
DLL_PATH = os.path.join(MOD_DIR, "x86_64", "libnrnmech.so")

if os.path.exists(DLL_PATH):
    h.nrn_load_dll(DLL_PATH)
    print("Loaded mechanisms:", DLL_PATH)
else:
    raise RuntimeError(f"Compiled mechanisms not found at: {DLL_PATH}")


# -----------------------------
# Helpers
# -----------------------------
def try_insert(sec, mech):
    try:
        sec.insert(mech)
        return True
    except Exception:
        return False


def has_mech(sec, mech):
    try:
        return bool(h.ismembrane(mech, sec=sec))
    except Exception:
        return False


def set_mech(sec, mech, **params):
    if not has_mech(sec, mech):
        return
    for seg in sec:
        m = getattr(seg, mech)
        for k, v in params.items():
            if hasattr(m, k):
                setattr(m, k, v)


def set_cadepk(sec, value):
    if not has_mech(sec, "CadepK"):
        return
    for seg in sec:
        for attr in ["gbar", "gkbar", "gcakbar"]:
            if hasattr(seg.CadepK, attr):
                setattr(seg.CadepK, attr, value)
                break


def functionalise_bk_split(target_split, effective_gain):
    """
    Converts desired functional current fractions into conductance fractions.

    If one BK subtype is intrinsically stronger, this gives it less conductance
    so that the final measured current contribution better matches target_split.
    """
    raw = {}

    for k, frac in target_split.items():
        gain = effective_gain.get(k, 1.0)

        if gain <= 0:
            raw[k] = 0.0
        else:
            raw[k] = frac / gain

    total = sum(raw.values())

    if total <= 0:
        raise ValueError("Functional BK split failed: total adjusted split <= 0")

    return {k: v / total for k, v in raw.items()}

def validate_bk_split(split):
    s = sum(split.values())
    if abs(s - 1.0) > 1e-9:
        raise ValueError(f"BK split must sum to 1.0, got {s}")


def apply_bk_split_to_section(sec, total_bk_gakbar, total_bk_gabkbar, split):
    validate_bk_split(split)
    split = functionalise_bk_split(split, BK_EFFECTIVE_GAIN)
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


def savefig(name):
    plt.savefig(os.path.join(FIG_DIR, name), dpi=300, bbox_inches="tight")
    plt.close()

def metric_ylim(rows, metrics, pad_frac=0.10, force_zero_min=False):
    vals = []

    for metric in metrics:
        for r in rows:
            if metric in r:
                v = r[metric]
                if np.isfinite(v):
                    vals.append(float(v))

    if len(vals) == 0:
        return None

    vals = np.array(vals, dtype=float)
    ymin = float(np.nanmin(vals))
    ymax = float(np.nanmax(vals))

    if force_zero_min:
        ymin = 0.0

    yr = ymax - ymin
    pad = yr * pad_frac if yr > 0 else max(abs(ymax) * pad_frac, 1e-9)

    return ymin - pad, ymax + pad


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

def set_clean_scaled_yaxis(ax, ymin, ymax, exponent, tick_step):
    """
    Sets clean ticks in scaled units.

    Example:
    exponent=-7, tick_step=0.25 gives ticks:
    0, 0.25, 0.50, 0.75 ... ×10⁻⁷
    """
    scale = 10.0 ** exponent

    tick_min = np.floor(ymin / scale / tick_step) * tick_step
    tick_max = np.ceil(ymax / scale / tick_step) * tick_step

    ticks_scaled = np.arange(tick_min, tick_max + tick_step * 0.5, tick_step)
    ticks_actual = ticks_scaled * scale

    ax.set_ylim(ticks_actual[0], ticks_actual[-1])
    ax.set_yticks(ticks_actual)

    format_scientific_yaxis(ax, exponent=exponent)

def set_fixed_scaled_yaxis(ax, ticks_scaled, exponent, ylim_scaled=None):

    scale = 10.0 ** exponent
    ticks_actual = np.array(ticks_scaled, dtype=float) * scale
    ax.set_yticks(ticks_actual)

    if ylim_scaled is None:
        ax.set_ylim(np.min(ticks_actual), np.max(ticks_actual))
    else:
        ax.set_ylim(ylim_scaled[0] * scale, ylim_scaled[1] * scale)

    format_scientific_yaxis(ax, exponent=exponent)

# -----------------------------
# Reduced cell
# -----------------------------
class DGGranuleLikeCell:
    def __init__(self, name="dgcell", bk_split=None, bk_total_scale=1.0):
        self.name = name
        self.bk_split = WT_BK_SPLIT if bk_split is None else bk_split
        self.bk_total_scale = bk_total_scale
        validate_bk_split(self.bk_split)

        self.spines = []
        self.spine_necks = []
        self.spine_xs = []

        self.soma = h.Section(name=f"{name}.soma")
        self.dend_prox = h.Section(name=f"{name}.dend_prox")
        self.dend_dist = h.Section(name=f"{name}.dend_dist")
        self.ais = h.Section(name=f"{name}.ais")
        self.axon = h.Section(name=f"{name}.axon")

        self.ais.connect(self.soma(0))
        self.axon.connect(self.ais(1))
        self.dend_prox.connect(self.soma(1))
        self.dend_dist.connect(self.dend_prox(1))

        self._set_geometry()
        self.add_spines_to_distal_dend(n_spines=10)
        self._set_biophysics()

    def all_secs(self):
        return [self.soma, self.dend_prox, self.dend_dist, self.ais, self.axon] + self.spine_necks + self.spines

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
        self.soma.diam = 20.0
        self.soma.L = self.soma.diam
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

        self.axon.L = 300.0
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
                    seg.pas.g = 0.00018
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
                seg.ichan3.gksbar = 0.006
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
                            setattr(seg.CadepK, attr, 0.00)
                            break

        for sec in ([self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]
                    + self.spine_necks + self.spines):
            if has_mech(sec, "Caold"):
                for seg in sec:
                    seg.Caold.gtcabar = 0.0
                    seg.Caold.gncabar = 0.0
                    seg.Caold.glcabar = 0.0

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
                seg.SK2.gkbar = 1.2e-9
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

            # Soma
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

            # Prox/dist dendrites
            # so give BK_Cav12 the dendritic BK pool
            for sec in [self.dend_prox, self.dend_dist]:
                if has_mech(sec, "BK_Cav12"):
                    apply_bk_split_to_section(
                        sec,
                        total_bk_gakbar=5.0e-7 * self.bk_total_scale,
                        total_bk_gabkbar=5.0e-6 * self.bk_total_scale,
                        split={"BK_Cav22": 0.0, "BK_Cav12": 1.0, "BK_Cav21": 0.0},
                    )

            # Spines
            for sec in self.spine_necks + self.spines:
                if has_mech(sec, "BK_Cav12"):
                    apply_bk_split_to_section(
                        sec,
                        total_bk_gakbar=1.0e-7 * self.bk_total_scale,
                        total_bk_gabkbar=1.0e-6 * self.bk_total_scale,
                        split={"BK_Cav22": 0.0, "BK_Cav12": 1.0, "BK_Cav21": 0.0},
                    )

            # Axon
            # Renormalise the split across BK_Cav21 and BK_Cav22 only.
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

            # Set potassium reversal potential
            for sec in self.all_secs():
                for seg in sec:
                    if hasattr(seg, "ek"):
                        seg.ek = -90.0

def scale_cav12_everywhere(cell, factor):
    for sec in cell.all_secs():
        if has_mech(sec, "Cav12"):
            for seg in sec:
                seg.Cav12.gbar *= factor


# -----------------------------
# Recording helpers
# -----------------------------
def record_ref(ref, sec):
    vec = h.Vector()
    vec.record(ref, sec=sec)
    return vec


def try_record_segment_ref(seg, ref_name, sec):
    try:
        return record_ref(getattr(seg, ref_name), sec)
    except Exception:
        return None


def try_record_mech_ref(seg, mech_name, ref_name, sec):
    try:
        return record_ref(getattr(getattr(seg, mech_name), ref_name), sec)
    except Exception:
        return None


def arr(vec, n=None):
    if vec is None:
        return np.zeros(n, dtype=float) if n is not None else None
    return np.array(vec, dtype=float)


def peak(x, mask, mode="max"):
    x = np.asarray(x, dtype=float)
    vals = x[mask]
    vals = vals[np.isfinite(vals)]
    if len(vals) == 0:
        return np.nan
    if mode == "mean":
        return float(np.mean(vals))
    if mode == "min":
        return float(np.min(vals))
    return float(np.max(vals))

def try_record_mech_any_ref(seg, mech_name, ref_names, sec):
    for ref_name in ref_names:
        try:
            return record_ref(getattr(getattr(seg, mech_name), ref_name), sec)
        except Exception:
            pass
    return None


# -----------------------------
# Voltage clamp
# -----------------------------
def make_condition_cell(cond):
    cell = DGGranuleLikeCell(
        name=cond["prefix"],
        bk_split=cond["bk_split"],
        bk_total_scale=cond["bk_total_scale"],
    )
    if cond["cav12_scale"] != 1.0:
        scale_cav12_everywhere(cell, cond["cav12_scale"])
    return cell


def run_soma_voltage_step(cell, v_hold=-70.0, v_step=0.0, hold_ms=100.0,
                          step_ms=300.0, tail_ms=100.0, dt=0.025, rs=0.01):
    tstop = hold_ms + step_ms + tail_ms
    seg = cell.soma(0.5)

    clamp = h.SEClamp(seg)
    clamp.dur1 = hold_ms
    clamp.amp1 = v_hold
    clamp.dur2 = step_ms
    clamp.amp2 = v_step
    clamp.dur3 = tail_ms
    clamp.amp3 = v_hold
    clamp.rs = rs

    t_vec = record_ref(h._ref_t, cell.soma)
    v_vec = record_ref(seg._ref_v, cell.soma)
    clamp_i_vec = record_ref(clamp._ref_i, cell.soma)

    #re indiv cav
    cav12_vec = try_record_segment_ref(seg, "_ref_ilca", cell.soma)
    cav21_vec = try_record_segment_ref(seg, "_ref_ipca", cell.soma)
    cav22_vec = try_record_segment_ref(seg, "_ref_inca", cell.soma)

    # Total Ca estimate is sum any available calcium ion currents.
    ca_vecs = []
    for ref_name in ["_ref_ilca", "_ref_ica", "_ref_inca", "_ref_ipca", "_ref_ilca13"]:
        vec = try_record_segment_ref(seg, ref_name, cell.soma)
        if vec is not None:
            ca_vecs.append(vec)

    bk12_vec = try_record_mech_ref(seg, "BK_Cav12", "_ref_ik", cell.soma)
    bk21_vec = try_record_mech_ref(seg, "BK_Cav21", "_ref_ik", cell.soma)
    bk22_vec = try_record_mech_ref(seg, "BK_Cav22", "_ref_ik", cell.soma)
    sk2_vec = try_record_mech_ref(seg, "SK2", "_ref_ik", cell.soma)
    cai_vec = try_record_segment_ref(seg, "_ref_cai", cell.soma)

    h.dt = dt
    h.tstop = tstop
    try:
        h.cvode.active(0)
    except Exception:
        pass

    h.finitialize(v_hold)
    h.frecord_init()
    h.continuerun(tstop)

    t = np.array(t_vec, dtype=float)
    n = len(t)

    total_ca_raw = np.zeros(n)
    for vec in ca_vecs:
        total_ca_raw += arr(vec, n)

    out = {
        "t": t,
        "v": arr(v_vec, n),
        "clamp_i": arr(clamp_i_vec, n),
        "cav12": arr(cav12_vec, n),  # inward negative / downward
        "cav21": arr(cav21_vec, n),
        "cav22": arr(cav22_vec, n),
        "total_ca": total_ca_raw,  # inward negative / downward
        "bk12": arr(bk12_vec, n),          # outward positive
        "bk21": arr(bk21_vec, n),
        "bk22": arr(bk22_vec, n),
        "sk2": arr(sk2_vec, n),
        "cai": arr(cai_vec, n),
    }
    out["bk_total"] = out["bk12"] + out["bk21"] + out["bk22"]

    cell._last_vclamp = clamp
    return out


def run_iv_for_condition(cond, v_steps, v_hold=-70.0, hold_ms=100.0,
                         step_ms=200.0, tail_ms=100.0, dt=0.025, rs=0.01):
    rows = []

    for v_step in v_steps:
        cell = make_condition_cell(cond)
        tr = run_soma_voltage_step(
            cell, v_hold=v_hold, v_step=float(v_step),
            hold_ms=hold_ms, step_ms=step_ms, tail_ms=tail_ms, dt=dt, rs=rs
        )

        t = tr["t"]
        step_mask = (t >= hold_ms + 5.0) & (t <= hold_ms + step_ms)
        late_mask = (t >= hold_ms + step_ms - 20.0) & (t <= hold_ms + step_ms)

        row = {
            "condition": cond["label"],
            "v_step_mV": float(v_step),
            # Ca currents: inward negative, so use min
            "cav12_peak": peak(tr["cav12"], step_mask, mode="min"),
            "cav12_late": peak(tr["cav12"], late_mask, mode="mean"),
            "cav21_peak": peak(tr["cav21"], step_mask, mode="min"),
            "cav21_late": peak(tr["cav21"], late_mask, mode="mean"),
            "cav22_peak": peak(tr["cav22"], step_mask, mode="min"),
            "cav22_late": peak(tr["cav22"], late_mask, mode="mean"),
            "total_ca_peak": peak(tr["total_ca"], step_mask, mode="min"),
            "total_ca_late": peak(tr["total_ca"], late_mask, mode="mean"),
            "bk_total_peak": peak(tr["bk_total"], step_mask),
            "bk_total_late": peak(tr["bk_total"], late_mask, mode="mean"),
            "bk12_peak": peak(tr["bk12"], step_mask),
            "bk12_late": peak(tr["bk12"], late_mask, mode="mean"),
            "bk21_peak": peak(tr["bk21"], step_mask),
            "bk21_late": peak(tr["bk21"], late_mask, mode="mean"),
            "bk22_peak": peak(tr["bk22"], step_mask),
            "bk22_late": peak(tr["bk22"], late_mask, mode="mean"),
            "sk2_peak": peak(tr["sk2"], step_mask),
            "sk2_late": peak(tr["sk2"], late_mask, mode="mean"),
            "actual_vm_mean_late": peak(tr["v"], late_mask, mode="mean"),
            "clamp_i_peak_abs": peak(np.abs(tr["clamp_i"]), step_mask),
        }
        rows.append(row)

        print(
            f"{cond['label']} | V={v_step:>6.1f} mV"
            f" | Cav12={row['cav12_peak']:.6g}"
            f" | totalCa={row['total_ca_peak']:.6g}"
            f" | totalBK={row['bk_total_peak']:.6g}"
        )

    return rows


# -----------------------------
# Plotting
# -----------------------------
def save_rows_csv(rows, path):
    if not rows:
        return
    with open(path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)


def rows_for(rows, label):
    return [r for r in rows if r["condition"] == label]


def plot_iv_metric(
    rows,
    metric,
    ylabel,
    title,
    filename,
    y_limits=None,
    sci_exponent=None,
    y_tick_step=None,
    ticks_scaled=None,
    ylim_scaled=None,
):
    fig, ax = plt.subplots(figsize=(7, 5))

    for cond in CONDITIONS:
        rs = rows_for(rows, cond["label"])
        x = np.array([r["v_step_mV"] for r in rs], dtype=float)
        y = np.array([r[metric] for r in rs], dtype=float)

        order = np.argsort(x)

        ax.plot(
            x[order],
            y[order],
            marker="o",
            linewidth=1.5,
            color=cond["color"],
            label=cond["label"],
        )

    ax.set_xlabel("Command voltage (mV)")
    ax.set_ylabel(ylabel)
    ax.set_title(title)

    if ticks_scaled is not None and sci_exponent is not None:
        set_fixed_scaled_yaxis(
            ax,
            ticks_scaled=ticks_scaled,
            exponent=sci_exponent,
            ylim_scaled=ylim_scaled,
        )
    elif y_limits is not None and sci_exponent is not None and y_tick_step is not None:
        set_clean_scaled_yaxis(
            ax,
            ymin=y_limits[0],
            ymax=y_limits[1],
            exponent=sci_exponent,
            tick_step=y_tick_step,
        )
    else:
        if y_limits is not None:
            ax.set_ylim(y_limits)

        format_scientific_yaxis(ax, exponent=sci_exponent)

    ax.legend(frameon=False)
    fig.tight_layout()
    savefig(filename)

def plot_bk_subtypes_separate_iv(rows, use_late=True):
    """
    Makes one graph per BK subtype using automatic y-axis scaling.
    """

    if use_late:
        metrics = [
            ("bk12_late", "BK_Cav12"),
            ("bk21_late", "BK_Cav21"),
            ("bk22_late", "BK_Cav22"),
        ]
        tag = "late"
        ylabel = "Late BK current density (mA/cm²)"
    else:
        metrics = [
            ("bk12_peak", "BK_Cav12"),
            ("bk21_peak", "BK_Cav21"),
            ("bk22_peak", "BK_Cav22"),
        ]
        tag = "peak"
        ylabel = "Peak BK current density (mA/cm²)"

    for metric, subtype_label in metrics:
        fig, ax = plt.subplots(figsize=(7, 5))

        for cond in CONDITIONS:
            rs = rows_for(rows, cond["label"])
            x = np.array([r["v_step_mV"] for r in rs], dtype=float)
            y = np.array([r[metric] for r in rs], dtype=float)
            order = np.argsort(x)

            ax.plot(
                x[order],
                y[order],
                marker="o",
                linewidth=1.5,
                color=cond["color"],
                label=cond["label"],
            )

        ax.set_xlabel("Command voltage (mV)")
        ax.set_ylabel(ylabel)
        ax.set_title(f"{tag.capitalize()} {subtype_label} I–V")

        # auto scientific notation
        format_scientific_yaxis(ax, exponent=None)

        ax.legend(frameon=False)
        fig.tight_layout()

        savefig(f"BK_{subtype_label}_{tag}_IV.png")


def plot_bk_sum_check(rows, use_late=True):
    """
    Checks whether BK_Cav12 + BK_Cav21 + BK_Cav22 equals total BK.
    Uses automatic y-axis scaling.
    """

    if use_late:
        bk12_key = "bk12_late"
        bk21_key = "bk21_late"
        bk22_key = "bk22_late"
        total_key = "bk_total_late"
        tag = "late"
    else:
        bk12_key = "bk12_peak"
        bk21_key = "bk21_peak"
        bk22_key = "bk22_peak"
        total_key = "bk_total_peak"
        tag = "peak"

    fig, ax = plt.subplots(figsize=(7, 5))

    for cond in CONDITIONS:
        rs = rows_for(rows, cond["label"])

        x = np.array([r["v_step_mV"] for r in rs], dtype=float)
        total_bk = np.array([r[total_key] for r in rs], dtype=float)
        summed_bk = np.array([
            r[bk12_key] + r[bk21_key] + r[bk22_key]
            for r in rs
        ], dtype=float)

        order = np.argsort(x)

        ax.plot(
            x[order],
            total_bk[order],
            marker="o",
            linewidth=1.5,
            color=cond["color"],
            label=f"{cond['label']} total BK",
        )

        ax.plot(
            x[order],
            summed_bk[order],
            linestyle="--",
            linewidth=1.5,
            color=cond["color"],
            label=f"{cond['label']} subtype sum",
        )

    ax.set_xlabel("Command voltage (mV)")
    ax.set_ylabel("BK current density (mA/cm²)")
    ax.set_title(f"{tag.capitalize()} BK total vs subtype-sum check")

    # auto scientific notation
    format_scientific_yaxis(ax, exponent=None)

    ax.legend(frameon=False, fontsize=8)
    fig.tight_layout()

    savefig(f"BK_{tag}_total_vs_subtype_sum_CHECK.png")

# -----------------------------
# Main
# -----------------------------
if __name__ == "__main__":
    h.celsius = 34.0

    v_steps = np.arange(-80.0, 41.0, 10.0)
    vc_params = dict(v_hold=-70.0, hold_ms=100.0, step_ms=200.0,
                     tail_ms=100.0, dt=0.025, rs=0.01)

    print("\nRunning voltage-clamp I–V curves...")
    print("Voltage steps:", v_steps)

    all_rows = []
    for cond in CONDITIONS:
        print(f"\n--- {cond['label']} ---")
        all_rows.extend(run_iv_for_condition(cond, v_steps=v_steps, **vc_params))

#    csv_path = os.path.join(OUT_DIR, "vc_iv_results.csv")
#    save_rows_csv(all_rows, csv_path)
#    print(f"\nSaved CSV: {csv_path}")

    csv_path = os.path.join(OUT_DIR, "VC_raw_data.csv")
    save_rows_csv(all_rows, csv_path)
    print(f"\nSaved VC raw data: {csv_path}")

    plot_iv_metric(
        all_rows,
        metric="cav12_late",
        ylabel="Late Cav1.2 current density (mA/cm²)",
        title="Late Cav1.2 I–V",
        filename="01b_Cav12_late_IV.png",
    )

    plot_iv_metric(
        all_rows,
        metric="cav21_late",
        ylabel="Late Cav2.1 current density (mA/cm²)",
        title="Late Cav2.1 I–V",
        filename="08_Cav21_late_IV.png",
    )


    plot_iv_metric(
        all_rows,
        metric="cav22_late",
        ylabel="Late Cav2.2 current density (mA/cm²)",
        title="Late Cav2.2 I–V",
        filename="10_Cav22_late_IV.png",
    )


    plot_iv_metric(
        all_rows,
        metric="total_ca_late",
        ylabel="Late total Ca²⁺ current density (mA/cm²)",
        title="Late total Ca²⁺ I–V",
        filename="02b_total_Ca_late_IV.png",
    )

    plot_iv_metric(
        all_rows,
        metric="total_ca_peak",
        ylabel="Peak total Ca²⁺ current density (mA/cm²)",
        title="Peak total Ca²⁺ I–V",
        filename="02_total_Ca_peak_IV.png",
        sci_exponent=-3,
        ticks_scaled=[-2.5, -2.0, -1.5, -1.0, -0.5, 0.0],
        ylim_scaled=(-2.6, 0.1),
    )

    plot_iv_metric(
        all_rows,
        metric="cav12_peak",
        ylabel="Peak Cav1.2 current density (mA/cm²)",
        title="Peak Cav1.2 I–V",
        filename="01_Cav12_peak_IV.png",
        sci_exponent=-4,
        ticks_scaled=[-4, -3, -2, -1, 0],
        ylim_scaled=(-4.2, 0.2),
    )

    plot_iv_metric(
        all_rows,
        metric="cav21_peak",
        ylabel="Peak Cav2.1 current density (mA/cm²)",
        title="Peak Cav2.1 I–V",
        filename="07_Cav21_peak_IV.png",
        sci_exponent=-4,
        ticks_scaled=[-4, -3, -2, -1, 0],
        ylim_scaled=(-4.2, 0.2),
    )

    plot_iv_metric(
        all_rows,
        metric="cav22_peak",
        ylabel="Peak Cav2.2 current density (mA/cm²)",
        title="Peak Cav2.2 I–V",
        filename="09_Cav22_peak_IV.png",
        sci_exponent=-4,
        ticks_scaled=[-4, -3, -2, -1, 0],
        ylim_scaled=(-4.2, 0.2),
    )

    plot_iv_metric(
        all_rows,
        metric="bk_total_late",
        ylabel="Late total BK current density (mA/cm²)",
        title="Late total BK I–V",
        filename="06_total_BK_late_IV.png",
        sci_exponent=-7,
        ticks_scaled=[0.0, 0.5, 1.0, 1.5, 2.0],
        ylim_scaled=(-0.05, 2.05),
    )


    def plot_bk_subtypes_separate_iv(rows, use_late=True):
        """
        Makes one graph per BK subtype.
        """

        if use_late:
            metrics = [
                ("bk12_late", "BK_Cav12"),
                ("bk21_late", "BK_Cav21"),
                ("bk22_late", "BK_Cav22"),
            ]
            tag = "late"
            ylabel = "Late BK current density (mA/cm²)"
        else:
            metrics = [
                ("bk12_peak", "BK_Cav12"),
                ("bk21_peak", "BK_Cav21"),
                ("bk22_peak", "BK_Cav22"),
            ]
            tag = "peak"
            ylabel = "Peak BK current density (mA/cm²)"

        for metric, subtype_label in metrics:
            fig, ax = plt.subplots(figsize=(7, 5))

            for cond in CONDITIONS:
                rs = rows_for(rows, cond["label"])
                x = np.array([r["v_step_mV"] for r in rs], dtype=float)
                y = np.array([r[metric] for r in rs], dtype=float)

                order = np.argsort(x)

                ax.plot(
                    x[order],
                    y[order],
                    marker="o",
                    linewidth=1.5,
                    color=cond["color"],
                    label=cond["label"],
                )

            ax.set_xlabel("Command voltage (mV)")
            ax.set_ylabel(ylabel)
            ax.set_title(f"{tag.capitalize()} {subtype_label} I–V")

            if use_late:
                if subtype_label == "BK_Cav12":
                    set_fixed_scaled_yaxis(
                        ax,
                        ticks_scaled=[0, 1, 2, 3, 4, 5, 6, 7],
                        exponent=-8,
                        ylim_scaled=(-0.05, 7.05),
                    )
                elif subtype_label == "BK_Cav21":
                    set_fixed_scaled_yaxis(
                        ax,
                        ticks_scaled=[0, 1, 2, 3, 4, 5, 6, 7],
                        exponent=-7,
                        ylim_scaled=(-0.05, 7.05),
                    )
                elif subtype_label == "BK_Cav22":
                    set_fixed_scaled_yaxis(
                        ax,
                        ticks_scaled=[0, 1, 2, 3, 4, 5, 6, 7],
                        exponent=-9,
                        ylim_scaled=(-0.05, 7.05),
                    )
            else:
                format_scientific_yaxis(ax, exponent=None)

            ax.legend(frameon=False)
            fig.tight_layout()

            savefig(f"BK_{subtype_label}_{tag}_IV.png")


    plot_bk_subtypes_separate_iv(all_rows, use_late=True)

    plot_iv_metric(
        all_rows,
        metric="sk2_peak",
        ylabel="Peak SK2 current density (mA/cm²)",
        title="Peak SK2 I–V",
        filename="07_SK2_peak_IV.png",
    )

    plot_iv_metric(
        all_rows,
        metric="sk2_late",
        ylabel="Late SK2 current density (mA/cm²)",
        title="Late SK2 I–V",
        filename="08_SK2_late_IV.png",
    )

    print(f"Saved figures in: {FIG_DIR}")
    print("Done.")
