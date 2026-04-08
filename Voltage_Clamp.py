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

# BK coupling splits
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

# labels and colour scheme
WT_LABEL = "WT"
CAV12_50_LABEL = "Cav1.2 50%"
WT_COLOR = "black"
CAV12_50_COLOR = "#ffa6b2" #"#e16173"

print("Python version:", sys.version)
print("NEURON version:", h.nrnversion())

OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "outputs")
FIG_DIR = os.path.join(OUT_DIR, "vc_figures")
os.makedirs(FIG_DIR, exist_ok=True)

MOD_DIR = "/home/raven/PycharmProjects/Masters/Mod_Files"
DLL_PATH = os.path.join(MOD_DIR, "x86_64", "libnrnmech.so")

if os.path.exists(DLL_PATH):
    h.nrn_load_dll(DLL_PATH)
    print("Loaded mechanisms:", DLL_PATH)
else:
    raise RuntimeError(f"Compiled mechanisms not found at: {DLL_PATH}")


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


def apply_bk_split_to_section(sec, total_bk_gakbar: float, total_bk_gabkbar: float, split: dict):
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


class DGGranuleLikeCell:
    def __init__(self, name="dgcell", bk_split=None):
        self.name = name
        self.bk_split = WT_BK_SPLIT if bk_split is None else bk_split
        validate_bk_split(self.bk_split)

        self.spines = []
        self.spine_necks = []
        self.spine_xs = []

        self.soma = h.Section(name=f"{name}.soma")
        self.dend_prox = h.Section(name=f"{name}.dend_prox")
        self.dend_dist = h.Section(name=f"{name}.dend_dist")
        self.axon = h.Section(name=f"{name}.axon")
        self.ais = h.Section(name=f"{name}.ais")

        self.dend_prox.connect(self.soma(1))
        self.dend_dist.connect(self.dend_prox(1))
        self.ais.connect(self.soma(0))
        self.axon.connect(self.ais(1))

        self._set_geometry()
        self._set_biophysics()
        self.add_spines_to_distal_dend(n_spines=10)

        self.vclamp = None
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

            x = 0.1 + 0.8 * (i / max(1, n_spines - 1))
            self.spine_xs.append(x)
            neck.connect(self.dend_dist(x))
            head.connect(neck(1))

            neck.L, neck.diam, neck.nseg = 1.0, 0.2, 1
            head.L, head.diam, head.nseg = 0.5, 0.5, 1

            try_insert(neck, "pas")
            try_insert(head, "pas")
            if has_mech(neck, "pas"):
                for seg in neck:
                    seg.pas.g = 0.00039
                    seg.pas.e = -70.0
            if has_mech(head, "pas"):
                for seg in head:
                    seg.pas.g = 0.00039
                    seg.pas.e = -70.0

            for mech in [
                "Caold", "Cabuffer", "Cav12", "Cav13", "BK_Cav22", "BK_Cav12", "BK_Cav21",
                "SK2", "HCN", "Kv42", "Kv42b", "Kv11", "Kir21", "Kv14", "Kv21",
                "Kv33", "Kv34", "Kv723", "ichan3", "na8st", "Cav22", "Cav32", "Cav2_1"
            ]:
                try_insert(neck, mech)
                try_insert(head, mech)

            if has_mech(head, "Cav12"):
                for seg in head:
                    seg.Cav12.gbar = 1e-7
            if has_mech(neck, "Cav12"):
                for seg in neck:
                    seg.Cav12.gbar = 1e-7

            if has_mech(head, "Cav13"):
                for seg in head:
                    seg.Cav13.gbar = 1e-9
            if has_mech(neck, "Cav13"):
                for seg in neck:
                    seg.Cav13.gbar = 1e-9

            apply_bk_split_to_section(head, total_bk_gakbar=1e-4, total_bk_gabkbar=1e-4, split=self.bk_split)
            apply_bk_split_to_section(neck, total_bk_gakbar=1e-4, total_bk_gabkbar=1e-4, split=self.bk_split)

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

            for mech_name, gval in [
                ("Kv42", 1.5e-4), ("Kv42b", 1.5e-4), ("Kv11", 1e-5), ("Kir21", 1.5e-4),
                ("Kv14", 1e-5), ("Kv21", 3e-5), ("Kv33", 2e-2), ("Kv34", 2e-3), ("Kv723", 2e-9)
            ]:
                if has_mech(head, mech_name):
                    for seg in head:
                        getattr(seg, mech_name).gkbar = gval
                if has_mech(neck, mech_name):
                    for seg in neck:
                        getattr(seg, mech_name).gkbar = gval

            if has_mech(head, "ichan3"):
                for seg in head:
                    seg.ichan3.gnabar = 5e-2
                    seg.ichan3.gkfbar = 5e-4
                    seg.ichan3.gksbar = 5e-4
                    seg.ichan3.gkabar = 5e-4
            if has_mech(neck, "ichan3"):
                for seg in neck:
                    seg.ichan3.gnabar = 5e-2
                    seg.ichan3.gkfbar = 5e-4
                    seg.ichan3.gksbar = 5e-4
                    seg.ichan3.gkabar = 5e-4

            if has_mech(head, "na8st"):
                for seg in head:
                    seg.na8st.gbar = 1e-6
            if has_mech(neck, "na8st"):
                for seg in neck:
                    seg.na8st.gbar = 1e-6

            if has_mech(head, "Cav22"):
                for seg in head:
                    seg.Cav22.gbar = 1e-5
            if has_mech(neck, "Cav22"):
                for seg in neck:
                    seg.Cav22.gbar = 1e-5

            if has_mech(head, "Cav32"):
                for seg in head:
                    seg.Cav32.gbar = 1e-5
            if has_mech(neck, "Cav32"):
                for seg in neck:
                    seg.Cav32.gbar = 1e-5

            if has_mech(head, "Caold"):
                for seg in head:
                    seg.Caold.gtcabar = 1e-6
                    seg.Caold.gncabar = 1e-6
                    seg.Caold.glcabar = 1e-6
            if has_mech(neck, "Caold"):
                for seg in neck:
                    seg.Caold.gtcabar = 1e-6
                    seg.Caold.gncabar = 1e-6
                    seg.Caold.glcabar = 1e-6

            if has_mech(head, "Cabuffer"):
                for seg in head:
                    seg.Cabuffer.tau = 8.0
                    seg.Cabuffer.brat = 1.0
            if has_mech(neck, "Cabuffer"):
                for seg in neck:
                    seg.Cabuffer.tau = 8.0
                    seg.Cabuffer.brat = 1.0

            if has_mech(head, "Cav2_1"):
                for seg in head:
                    seg.Cav2_1.pcabar = 1e-5
            if has_mech(neck, "Cav2_1"):
                for seg in neck:
                    seg.Cav2_1.pcabar = 1e-5

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
            sec.Ra = 100.0
            sec.cm = 1.0

    def _set_biophysics(self):
        for sec in self.all_secs():
            try_insert(sec, "pas")
            if has_mech(sec, "pas"):
                for seg in sec:
                    seg.pas.g = 0.00039
                    seg.pas.e = -70.0

        if has_mech(self.ais, "pas"):
            for seg in self.ais:
                seg.pas.g = 0.00039
                seg.pas.e = -70.0

        for sec in [self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]:
            try_insert(sec, "na8st")
            try_insert(sec, "ichan3")

        if has_mech(self.soma, "na8st"):
            for seg in self.soma:
                seg.na8st.gbar = 0.000001
        if has_mech(self.soma, "ichan3"):
            for seg in self.soma:
                seg.ichan3.gnabar = 5e-2
                seg.ichan3.gkfbar = 5e-4
                seg.ichan3.gksbar = 5e-4
                seg.ichan3.gkabar = 5e-4

        if has_mech(self.ais, "na8st"):
            for seg in self.ais:
                seg.na8st.gbar = 0.000001 * 5.0
        if has_mech(self.ais, "ichan3"):
            for seg in self.ais:
                seg.ichan3.gnabar = 5e-2 * 2.0
                seg.ichan3.gkfbar = 5e-4
                seg.ichan3.gksbar = 5e-4
                seg.ichan3.gkabar = 5e-4

        if has_mech(self.axon, "na8st"):
            for seg in self.axon:
                seg.na8st.gbar = 0.0000001 * 3.0
        if has_mech(self.axon, "ichan3"):
            for seg in self.axon:
                seg.ichan3.gnabar = 5e-2
                seg.ichan3.gkfbar = 5e-4
                seg.ichan3.gksbar = 5e-4
                seg.ichan3.gkabar = 5e-4

        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "na8st"):
                for seg in sec:
                    seg.na8st.gbar = 0.000001 * 0.3
            if has_mech(sec, "ichan3"):
                for seg in sec:
                    seg.ichan3.gnabar = 5e-2 * 0.3
                    seg.ichan3.gkfbar = 5e-4 * 0.3
                    seg.ichan3.gksbar = 5e-4 * 0.3
                    seg.ichan3.gkabar = 5e-4 * 0.3

        for sec in [self.soma, self.ais, self.dend_prox, self.dend_dist]:
            for mech in [
                "Caold", "Cabuffer", "Cav12", "Cav13", "Cav22", "Cav32", "BK_Cav22", "BK_Cav12", "BK_Cav21",
                "SK2", "HCN", "Kv42", "Kv11", "Kir21", "Kv14", "Kv21", "Kv33", "Kv34", "Kv42b", "Kv723", "Cav2_1"
            ]:
                try_insert(sec, mech)

        self._set_channel_densities_default()

    def _set_channel_densities_default(self):
        for sec in [self.soma, self.ais, self.dend_prox, self.dend_dist]:
            for seg in sec:
                if sec is self.soma:
                    scale = 1.0
                elif sec is self.dend_prox:
                    scale = 1.8
                else:
                    scale = 2.5

                if has_mech(sec, "Cav12"):
                    seg.Cav12.gbar = 1e-7 * scale
                if has_mech(sec, "Cav13"):
                    seg.Cav13.gbar = 1e-9 * scale
                if has_mech(sec, "Cav22"):
                    seg.Cav22.gbar = 1e-5 * scale
                if has_mech(sec, "Cav32"):
                    seg.Cav32.gbar = 1e-5 * scale

                total_bk_gakbar = 1e-4 * scale
                total_bk_gabkbar = 1e-4 * scale
                if has_mech(sec, "BK_Cav22") and has_mech(sec, "BK_Cav12") and has_mech(sec, "BK_Cav21"):
                    seg.BK_Cav22.gakbar = total_bk_gakbar * self.bk_split["BK_Cav22"]
                    seg.BK_Cav22.gabkbar = total_bk_gabkbar * self.bk_split["BK_Cav22"]
                    seg.BK_Cav12.gakbar = total_bk_gakbar * self.bk_split["BK_Cav12"]
                    seg.BK_Cav12.gabkbar = total_bk_gabkbar * self.bk_split["BK_Cav12"]
                    seg.BK_Cav21.gakbar = total_bk_gakbar * self.bk_split["BK_Cav21"]
                    seg.BK_Cav21.gabkbar = total_bk_gabkbar * self.bk_split["BK_Cav21"]

                if has_mech(sec, "SK2"):
                    seg.SK2.gkbar = 5e-6 * scale
                if has_mech(sec, "HCN"):
                    seg.HCN.gbar = 1e-4 * scale
                if has_mech(sec, "Kv42"):
                    seg.Kv42.gkbar = 1.5e-4 * scale
                if has_mech(sec, "Kv42b"):
                    seg.Kv42b.gkbar = 1.5e-4 * scale
                if has_mech(sec, "Kv11"):
                    seg.Kv11.gkbar = 1e-5 * scale
                if has_mech(sec, "Kv14"):
                    seg.Kv14.gkbar = 1e-5 * scale
                if has_mech(sec, "Kv21"):
                    seg.Kv21.gkbar = 3e-5 * scale
                if has_mech(sec, "Kv33"):
                    seg.Kv33.gkbar = 2e-2 * scale
                if has_mech(sec, "Kv34"):
                    seg.Kv34.gkbar = 2e-3 * scale
                if has_mech(sec, "Kv723"):
                    seg.Kv723.gkbar = 2e-9 * scale
                if has_mech(sec, "Kir21"):
                    seg.Kir21.gkbar = 1.5e-4 * scale
                if has_mech(sec, "Caold"):
                    seg.Caold.gtcabar = 1e-6 * scale
                    seg.Caold.gncabar = 1e-6 * scale
                    seg.Caold.glcabar = 1e-6 * scale
                if has_mech(sec, "Cabuffer"):
                    seg.Cabuffer.tau = 8.0
                    seg.Cabuffer.brat = 1.0
                if has_mech(sec, "Cav2_1"):
                    seg.Cav2_1.pcabar = 1e-5 * scale
                    seg.Cav2_1.vshift = 0.0

    def scale_cav12(self, factor: float):
        for sec in [self.soma, self.dend_prox, self.dend_dist] + self.spine_necks + self.spines:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar *= factor

    def add_voltage_clamp(self, hold=-70.0, step=-50.0, delay=100.0, dur=300.0, sec=None, loc=0.5):
        sec = self.soma if sec is None else sec
        self.vclamp = h.SEClamp(sec(loc)) #use single electrode clamp, is in nA
        self.vclamp.dur1 = delay
        self.vclamp.amp1 = hold
        self.vclamp.dur2 = dur
        self.vclamp.amp2 = step
        self.vclamp.dur3 = 50.0
        self.vclamp.amp3 = hold
        self.vclamp.rs = 0.01

    def setup_recording(self):
        self.t_vec = h.Vector()
        self.t_vec.record(h._ref_t)

        self.vsoma_vec = h.Vector()
        self.vsoma_vec.record(self.soma(0.5)._ref_v)

        self.vais_vec = h.Vector()
        self.vais_vec.record(self.ais(0.5)._ref_v)

        self.vprox_vec = h.Vector()
        self.vprox_vec.record(self.dend_prox(0.5)._ref_v)

        self.vdend_vec = h.Vector()
        self.vdend_vec.record(self.dend_dist(0.9)._ref_v)

        self.vspine_vec = h.Vector()
        self.vspine_vec.record(self.spines[0](0.5)._ref_v)

        try:
            _ = self.soma(0.5)._ref_cai
            self.cai_soma_vec = h.Vector()
            self.cai_soma_vec.record(self.soma(0.5)._ref_cai)

            self.cai_prox_vec = h.Vector()
            self.cai_prox_vec.record(self.dend_prox(0.5)._ref_cai)

            self.cai_dist_vec = h.Vector()
            self.cai_dist_vec.record(self.dend_dist(0.9)._ref_cai)

            self.cai_spine_vec = h.Vector()
            self.cai_spine_vec.record(self.spines[0](0.5)._ref_cai)
        except Exception as e:
            print("Calcium recording set up failed because:", e)
            self.cai_soma_vec = None
            self.cai_prox_vec = None
            self.cai_dist_vec = None
            self.cai_spine_vec = None

        self.ica_soma_vec = h.Vector()
        self.ica_soma_vec.record(self.soma(0.5)._ref_ica)

        self.ik_soma_vec = h.Vector()
        self.ik_soma_vec.record(self.soma(0.5)._ref_ik)

        self.ina_soma_vec = h.Vector()
        self.ina_soma_vec.record(self.soma(0.5)._ref_ina)

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

        self.sk_ik_soma_vec = None
        if has_mech(self.soma, "SK2"):
            self.sk_ik_soma_vec = h.Vector()
            self.sk_ik_soma_vec.record(self.soma(0.5).SK2._ref_ik)

        self.cav21_ica_soma_vec = None
        if has_mech(self.soma, "Cav2_1"):
            self.cav21_ica_soma_vec = h.Vector()
            self.cav21_ica_soma_vec.record(self.soma(0.5).Cav2_1._ref_ipca)

        self.cav22_ica_soma_vec = None
        if has_mech(self.soma, "Cav22"):
            self.cav22_ica_soma_vec = h.Vector()
            self.cav22_ica_soma_vec.record(self.soma(0.5)._ref_inca)

        self.clamp_i_vec = None
        if self.vclamp is not None:
            self.clamp_i_vec = h.Vector()
            self.clamp_i_vec.record(self.vclamp._ref_i)

        self.cav12_ica_soma_vec = None
        if has_mech(self.soma, "Cav12"):
            self.cav12_ica_soma_vec = h.Vector()
            self.cav12_ica_soma_vec.record(self.soma(0.5)._ref_ilca)

        self.cav13_ica_soma_vec = None
        if has_mech(self.soma, "Cav13"):
            self.cav13_ica_soma_vec = h.Vector()
            self.cav13_ica_soma_vec.record(self.soma(0.5)._ref_ilca13)


        #Total calcium current by compartment

        self.ica_ais_vec = h.Vector()
        self.ica_ais_vec.record(self.ais(0.5)._ref_ica)

        self.ica_prox_vec = h.Vector()
        self.ica_prox_vec.record(self.dend_prox(0.5)._ref_ica)

        self.ica_dist_vec = h.Vector()
        self.ica_dist_vec.record(self.dend_dist(0.9)._ref_ica)

        self.ica_spine_vec = h.Vector()
        self.ica_spine_vec.record(self.spines[0](0.5)._ref_ica)


        #Cav1.2 source current by compartment

        self.cav12_ica_ais_vec = None
        self.cav12_ica_prox_vec = None
        self.cav12_ica_dist_vec = None
        self.cav12_ica_spine_vec = None

        if has_mech(self.ais, "Cav12"):
            self.cav12_ica_ais_vec = h.Vector()
            self.cav12_ica_ais_vec.record(self.ais(0.5)._ref_ilca)

        if has_mech(self.dend_prox, "Cav12"):
            self.cav12_ica_prox_vec = h.Vector()
            self.cav12_ica_prox_vec.record(self.dend_prox(0.5)._ref_ilca)

        if has_mech(self.dend_dist, "Cav12"):
            self.cav12_ica_dist_vec = h.Vector()
            self.cav12_ica_dist_vec.record(self.dend_dist(0.9)._ref_ilca)

        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav12"):
            self.cav12_ica_spine_vec = h.Vector()
            self.cav12_ica_spine_vec.record(self.spines[0](0.5)._ref_ilca)


        #Cav1.3 source current by compartment

        self.cav13_ica_ais_vec = None
        self.cav13_ica_prox_vec = None
        self.cav13_ica_dist_vec = None
        self.cav13_ica_spine_vec = None

        if has_mech(self.ais, "Cav13"):
            self.cav13_ica_ais_vec = h.Vector()
            self.cav13_ica_ais_vec.record(self.ais(0.5)._ref_ilca13)

        if has_mech(self.dend_prox, "Cav13"):
            self.cav13_ica_prox_vec = h.Vector()
            self.cav13_ica_prox_vec.record(self.dend_prox(0.5)._ref_ilca13)

        if has_mech(self.dend_dist, "Cav13"):
            self.cav13_ica_dist_vec = h.Vector()
            self.cav13_ica_dist_vec.record(self.dend_dist(0.9)._ref_ilca13)

        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav13"):
            self.cav13_ica_spine_vec = h.Vector()
            self.cav13_ica_spine_vec.record(self.spines[0](0.5)._ref_ilca13)


        #Cav2.1 source current by compartment

        self.cav21_ica_ais_vec = None
        self.cav21_ica_prox_vec = None
        self.cav21_ica_dist_vec = None
        self.cav21_ica_spine_vec = None

        if has_mech(self.ais, "Cav2_1"):
            self.cav21_ica_ais_vec = h.Vector()
            self.cav21_ica_ais_vec.record(self.ais(0.5).Cav2_1._ref_ipca)

        if has_mech(self.dend_prox, "Cav2_1"):
            self.cav21_ica_prox_vec = h.Vector()
            self.cav21_ica_prox_vec.record(self.dend_prox(0.5).Cav2_1._ref_ipca)

        if has_mech(self.dend_dist, "Cav2_1"):
            self.cav21_ica_dist_vec = h.Vector()
            self.cav21_ica_dist_vec.record(self.dend_dist(0.9).Cav2_1._ref_ipca)

        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav2_1"):
            self.cav21_ica_spine_vec = h.Vector()
            self.cav21_ica_spine_vec.record(self.spines[0](0.5).Cav2_1._ref_ipca)


        #Cav2.2 source current by compartment

        self.cav22_ica_ais_vec = None
        self.cav22_ica_prox_vec = None
        self.cav22_ica_dist_vec = None
        self.cav22_ica_spine_vec = None

        if has_mech(self.ais, "Cav22"):
            self.cav22_ica_ais_vec = h.Vector()
            self.cav22_ica_ais_vec.record(self.ais(0.5)._ref_inca)

        if has_mech(self.dend_prox, "Cav22"):
            self.cav22_ica_prox_vec = h.Vector()
            self.cav22_ica_prox_vec.record(self.dend_prox(0.5)._ref_inca)

        if has_mech(self.dend_dist, "Cav22"):
            self.cav22_ica_dist_vec = h.Vector()
            self.cav22_ica_dist_vec.record(self.dend_dist(0.9)._ref_inca)

        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav22"):
            self.cav22_ica_spine_vec = h.Vector()
            self.cav22_ica_spine_vec.record(self.spines[0](0.5)._ref_inca)


def run_sim(cell: DGGranuleLikeCell, tstop=500.0, v_init=-70.0, dt=0.025):
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

        #total Ca by compartment
        arr(getattr(cell, "ica_ais_vec", None)),
        arr(getattr(cell, "ica_prox_vec", None)),
        arr(getattr(cell, "ica_dist_vec", None)),
        arr(getattr(cell, "ica_spine_vec", None)),

        #Cav1.2 by compartment
        arr(getattr(cell, "cav12_ica_ais_vec", None)),
        arr(getattr(cell, "cav12_ica_prox_vec", None)),
        arr(getattr(cell, "cav12_ica_dist_vec", None)),
        arr(getattr(cell, "cav12_ica_spine_vec", None)),

        #Cav1.3 by compartment
        arr(getattr(cell, "cav13_ica_ais_vec", None)),
        arr(getattr(cell, "cav13_ica_prox_vec", None)),
        arr(getattr(cell, "cav13_ica_dist_vec", None)),
        arr(getattr(cell, "cav13_ica_spine_vec", None)),

        #Cav2.1 by compartment
        arr(getattr(cell, "cav21_ica_ais_vec", None)),
        arr(getattr(cell, "cav21_ica_prox_vec", None)),
        arr(getattr(cell, "cav21_ica_dist_vec", None)),
        arr(getattr(cell, "cav21_ica_spine_vec", None)),

        #Cav2.2 by compartment
        arr(getattr(cell, "cav22_ica_ais_vec", None)),
        arr(getattr(cell, "cav22_ica_prox_vec", None)),
        arr(getattr(cell, "cav22_ica_dist_vec", None)),
        arr(getattr(cell, "cav22_ica_spine_vec", None)),
    )


def run_iv_curve(bk_split, cav12_factor, steps, hold=-70.0, delay=100.0, dur=200.0, tstop=380.0):
    peak_current = []
    steady_current = []
    peak_ica = []
    peak_bk12 = []
    peak_bk21 = []
    peak_bk22 = []

    for vstep in steps:
        cell = DGGranuleLikeCell(bk_split=bk_split)
        if cav12_factor != 1.0:
            cell.scale_cav12(cav12_factor)
        cell.add_voltage_clamp(hold=hold, step=float(vstep), delay=delay, dur=dur)
        cell.setup_recording()
        t, vs, vais, vp, vd, vsp, cai_soma, cai_prox, cai_dist, cai_spine, \
            ica_soma, ik_soma, bk_Cav22ik_soma, bk_Cav12ik_soma, bk_Cav21ik_soma, skik_soma, ina_soma, \
            cav21_ica_soma, cav22_ica_soma, cav12_ica_soma, cav13_ica_soma, I, \
            ica_ais, ica_prox, ica_dist, ica_spine, \
            cav12_ica_ais, cav12_ica_prox, cav12_ica_dist, cav12_ica_spine, \
            cav13_ica_ais, cav13_ica_prox, cav13_ica_dist, cav13_ica_spine, \
            cav21_ica_ais, cav21_ica_prox, cav21_ica_dist, cav21_ica_spine, \
            cav22_ica_ais, cav22_ica_prox, cav22_ica_dist, cav22_ica_spine = run_sim(
            cell, tstop=tstop, v_init=hold, dt=0.025
        )

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


if __name__ == "__main__":
    h.celsius = 34.0

    cell = DGGranuleLikeCell(bk_split=WT_BK_SPLIT)
    cell.add_voltage_clamp(hold=-70.0, step=-50.0, delay=100.0, dur=300.0)
    cell.setup_recording()
    t0, vs0, vais0, vp0, vd0, vsp0, cai0_soma, cai0_prox, cai0_dist, cai0_spine, \
        ica0_soma, ik0_soma, bk_Cav22ik0_soma, bk_Cav12ik0_soma, bk_Cav21ik0_soma, skik0_soma, ina0_soma, \
        cav21_ica0_soma, cav22_ica0_soma, cav12_ica0_soma, cav13_ica0_soma, I0, \
        ica0_ais, ica0_prox, ica0_dist, ica0_spine, \
        cav12_ica0_ais, cav12_ica0_prox, cav12_ica0_dist, cav12_ica0_spine, \
        cav13_ica0_ais, cav13_ica0_prox, cav13_ica0_dist, cav13_ica0_spine, \
        cav21_ica0_ais, cav21_ica0_prox, cav21_ica0_dist, cav21_ica0_spine, \
        cav22_ica0_ais, cav22_ica0_prox, cav22_ica0_dist, cav22_ica0_spine = run_sim(
        cell, tstop=500.0, v_init=-70.0, dt=0.025
    )

    cell2 = DGGranuleLikeCell(bk_split=CAV12_50_BK_SPLIT)
    cell2.scale_cav12(0.5)
    cell2.add_voltage_clamp(hold=-70.0, step=-50.0, delay=100.0, dur=300.0)
    cell2.setup_recording()
    t1, vs1, vais1, vp1, vd1, vsp1, cai1_soma, cai1_prox, cai1_dist, cai1_spine, \
        ica1_soma, ik1_soma, bk_Cav22ik1_soma, bk_Cav12ik1_soma, bk_Cav21ik1_soma, skik1_soma, ina1_soma, \
        cav21_ica1_soma, cav22_ica1_soma, cav12_ica1_soma, cav13_ica1_soma, I1, \
        ica1_ais, ica1_prox, ica1_dist, ica1_spine, \
        cav12_ica1_ais, cav12_ica1_prox, cav12_ica1_dist, cav12_ica1_spine, \
        cav13_ica1_ais, cav13_ica1_prox, cav13_ica1_dist, cav13_ica1_spine, \
        cav21_ica1_ais, cav21_ica1_prox, cav21_ica1_dist, cav21_ica1_spine, \
        cav22_ica1_ais, cav22_ica1_prox, cav22_ica1_dist, cav22_ica1_spine = run_sim(
        cell2, tstop=500.0, v_init=-70.0, dt=0.025
    )

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
    iv_wt = run_iv_curve(WT_BK_SPLIT, cav12_factor=1.0, steps=steps)
    iv_50 = run_iv_curve(CAV12_50_BK_SPLIT, cav12_factor=0.5, steps=steps)

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

plt.figure()
plt.plot(t0, cav12_ica0_soma, color=WT_COLOR, label=WT_LABEL)
plt.plot(t1, cav12_ica1_soma, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
plt.xlabel("Time (ms)")
plt.ylabel("Current density (mA/cm2)")
plt.title("Cav1.2 source current")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(FIG_DIR, "vc_Cav12_source_current_WT_vs_Cav12_50.png"), dpi=300)
plt.show()

plt.figure()
plt.plot(t0, cav13_ica0_soma, color=WT_COLOR, label=WT_LABEL)
plt.plot(t1, cav13_ica1_soma, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
plt.xlabel("Time (ms)")
plt.ylabel("Current density (mA/cm2)")
plt.title("Cav1.3 source current")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(FIG_DIR, "vc_Cav13_source_current_WT_vs_Cav12_50.png"), dpi=300)
plt.show()

plt.figure()
plt.plot(t0, cav22_ica0_soma, color=WT_COLOR, label=WT_LABEL)
plt.plot(t1, cav22_ica1_soma, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
plt.xlabel("Time (ms)")
plt.ylabel("Current density (mA/cm2)")
plt.title("Cav22 source current")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(FIG_DIR, "vc_Cav22_source_current_WT_vs_Cav12_50.png"), dpi=300)
plt.show()

plt.figure()
plt.plot(t0, cav21_ica0_soma, color=WT_COLOR, label=WT_LABEL)
plt.plot(t1, cav21_ica1_soma, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
plt.xlabel("Time (ms)")
plt.ylabel("Current density (mA/cm2)")
plt.title("Cav2.1 source current")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(FIG_DIR, "vc_Cav21_source_current_WT_vs_Cav12_50.png"), dpi=300)
plt.show()

#6-panel voltage clamp Ca current fig

fig, axes = plt.subplots(2, 3, figsize=(15, 9), sharex=True, sharey=False)

#top left total Ca current
ax = axes[0, 0]
ax.plot(t0, ica0_soma, color=WT_COLOR, label=WT_LABEL)
ax.plot(t1, ica1_soma, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
ax.set_ylabel("Current density (mA/cm2)")

#top middle Cav1.2 source current
ax = axes[0, 1]
ax.plot(t0, cav12_ica0_soma, color=WT_COLOR)
ax.plot(t1, cav12_ica1_soma, color=CAV12_50_COLOR)

#top right Cav1.3 source current
ax = axes[0, 2]
ax.plot(t0, cav13_ica0_soma, color=WT_COLOR)
ax.plot(t1, cav13_ica1_soma, color=CAV12_50_COLOR)

#bottom left Cav2.1 source current
ax = axes[1, 0]
ax.plot(t0, cav21_ica0_soma, color=WT_COLOR)
ax.plot(t1, cav21_ica1_soma, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Current density (mA/cm2)")

#bottom middle Cav22 source current
ax = axes[1, 1]
ax.plot(t0, cav22_ica0_soma, color=WT_COLOR)
ax.plot(t1, cav22_ica1_soma, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")

#bottom right Cav32 estimate from residual
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



#4-panel total Ca current by compartment
fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True, sharey=False)

# AIS
ax = axes[0, 0]
ax.plot(t0, ica0_ais, color=WT_COLOR, label=WT_LABEL)
ax.plot(t1, ica1_ais, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
ax.set_ylabel("Current density (mA/cm2)")

# prox dend
ax = axes[0, 1]
ax.plot(t0, ica0_prox, color=WT_COLOR)
ax.plot(t1, ica1_prox, color=CAV12_50_COLOR)

# dist dend
ax = axes[1, 0]
ax.plot(t0, ica0_dist, color=WT_COLOR)
ax.plot(t1, ica1_dist, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Current density (mA/cm2)")

# spine
ax = axes[1, 1]
ax.plot(t0, ica0_spine, color=WT_COLOR)
ax.plot(t1, ica1_spine, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")

handles, labels = axes[0, 0].get_legend_handles_labels()
fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

fig.tight_layout(rect=[0, 0.05, 1, 1])
plt.savefig(
    os.path.join(FIG_DIR, "vc_totalCa_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"),
    dpi=300,
    bbox_inches="tight"
)
plt.show()

#4-panel Cav1.2 source current by compartment
fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True, sharey=False)

# AIS
ax = axes[0, 0]
ax.plot(t0, cav12_ica0_ais, color=WT_COLOR, label=WT_LABEL)
ax.plot(t1, cav12_ica1_ais, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
ax.set_ylabel("Current density (mA/cm2)")

# prox dend
ax = axes[0, 1]
ax.plot(t0, cav12_ica0_prox, color=WT_COLOR)
ax.plot(t1, cav12_ica1_prox, color=CAV12_50_COLOR)

# dist dend
ax = axes[1, 0]
ax.plot(t0, cav12_ica0_dist, color=WT_COLOR)
ax.plot(t1, cav12_ica1_dist, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Current density (mA/cm2)")

# spine
ax = axes[1, 1]
ax.plot(t0, cav12_ica0_spine, color=WT_COLOR)
ax.plot(t1, cav12_ica1_spine, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")

handles, labels = axes[0, 0].get_legend_handles_labels()
fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

fig.tight_layout(rect=[0, 0.05, 1, 1])
plt.savefig(
    os.path.join(FIG_DIR, "vc_Cav12_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"),
    dpi=300,
    bbox_inches="tight"
)
plt.show()

#4-panel Cav1.3 source current by compartment
fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True, sharey=False)

# AIS
ax = axes[0, 0]
ax.plot(t0, cav13_ica0_ais, color=WT_COLOR, label=WT_LABEL)
ax.plot(t1, cav13_ica1_ais, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
ax.set_ylabel("Current density (mA/cm2)")

# prox dend
ax = axes[0, 1]
ax.plot(t0, cav13_ica0_prox, color=WT_COLOR)
ax.plot(t1, cav13_ica1_prox, color=CAV12_50_COLOR)

# dist dend
ax = axes[1, 0]
ax.plot(t0, cav13_ica0_dist, color=WT_COLOR)
ax.plot(t1, cav13_ica1_dist, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Current density (mA/cm2)")

# spine
ax = axes[1, 1]
ax.plot(t0, cav13_ica0_spine, color=WT_COLOR)
ax.plot(t1, cav13_ica1_spine, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")

handles, labels = axes[0, 0].get_legend_handles_labels()
fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

fig.tight_layout(rect=[0, 0.05, 1, 1])
plt.savefig(
    os.path.join(FIG_DIR, "vc_Cav13_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"),
    dpi=300,
    bbox_inches="tight"
)
plt.show()

#4-panel Cav2.1 source current by compartment
fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True, sharey=False)

# AIS
ax = axes[0, 0]
ax.plot(t0, cav21_ica0_ais, color=WT_COLOR, label=WT_LABEL)
ax.plot(t1, cav21_ica1_ais, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
ax.set_ylabel("Current density (mA/cm2)")

# prox dend
ax = axes[0, 1]
ax.plot(t0, cav21_ica0_prox, color=WT_COLOR)
ax.plot(t1, cav21_ica1_prox, color=CAV12_50_COLOR)

# dist dend
ax = axes[1, 0]
ax.plot(t0, cav21_ica0_dist, color=WT_COLOR)
ax.plot(t1, cav21_ica1_dist, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Current density (mA/cm2)")

# spine
ax = axes[1, 1]
ax.plot(t0, cav21_ica0_spine, color=WT_COLOR)
ax.plot(t1, cav21_ica1_spine, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")

handles, labels = axes[0, 0].get_legend_handles_labels()
fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

fig.tight_layout(rect=[0, 0.05, 1, 1])
plt.savefig(
    os.path.join(FIG_DIR, "vc_Cav21_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"),
    dpi=300,
    bbox_inches="tight"
)
plt.show()

#4-panel Cav2.2 source current by compartment
fig, axes = plt.subplots(2, 2, figsize=(12, 9), sharex=True, sharey=False)

# AIS
ax = axes[0, 0]
ax.plot(t0, cav22_ica0_ais, color=WT_COLOR, label=WT_LABEL)
ax.plot(t1, cav22_ica1_ais, color=CAV12_50_COLOR, label=CAV12_50_LABEL)
ax.set_ylabel("Current density (mA/cm2)")

# prox dend
ax = axes[0, 1]
ax.plot(t0, cav22_ica0_prox, color=WT_COLOR)
ax.plot(t1, cav22_ica1_prox, color=CAV12_50_COLOR)

#dist dend
ax = axes[1, 0]
ax.plot(t0, cav22_ica0_dist, color=WT_COLOR)
ax.plot(t1, cav22_ica1_dist, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Current density (mA/cm2)")

#spine
ax = axes[1, 1]
ax.plot(t0, cav22_ica0_spine, color=WT_COLOR)
ax.plot(t1, cav22_ica1_spine, color=CAV12_50_COLOR)
ax.set_xlabel("Time (ms)")

handles, labels = axes[0, 0].get_legend_handles_labels()
fig.legend(handles, labels, loc="lower center", ncol=2, frameon=True)

fig.tight_layout(rect=[0, 0.05, 1, 1])
plt.savefig(
    os.path.join(FIG_DIR, "vc_Cav22_4panel_AIS_prox_dist_spine_WT_vs_Cav12_50.png"),
    dpi=300,
    bbox_inches="tight"
)
plt.show()