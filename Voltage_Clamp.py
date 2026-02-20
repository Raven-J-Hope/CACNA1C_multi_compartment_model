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

import sys  #check versions being used
print("Python version:", sys.version) #if python or neuron version used are different from above and code not running
print("NEURON version:", h.nrnversion()) #switch to aforementioned versions as first troubleshooting step

#make and set dir & paths to compiled mod files
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "outputs")
FIG_DIR = os.path.join(OUT_DIR, "vc_figures")
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

    #baseline/rest estimate, the mean before step (e.g., 50–90 ms)
    w_rest = (t >= 50) & (t <= 90)
    v_rest = float(np.mean(v[w_rest]))

    ahp = v_min - v_rest
    return v_peak, t_peak, v_min, t_min, ahp

#define cell morphology & biophysics
class DGGranuleLikeCell:
    def __init__(self, name="dgcell"):
        self.name = name
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
        return [self.soma, self.dend_prox, self.dend_dist, self.axon] + self.spine_necks + self.spines

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
                    seg.pas.g = 3e-5
                    seg.pas.e = -70.0
            if has_mech(head, "pas"):
                for seg in head:
                    seg.pas.g = 1e-4
                    seg.pas.e = -70.0

            #give spines Ca mechs
            try_insert(neck, "Caold")
            try_insert(head, "Caold")
            try_insert(neck, "Cabuffer")
            try_insert(head, "Cabuffer")
            try_insert(neck, "Cav12")
            try_insert(head, "Cav12")
            try_insert(neck, "BK")
            try_insert(head, "BK")
            try_insert(neck, "SK2")
            try_insert(head, "SK2")
            try_insert(neck, "HCN")
            try_insert(head, "HCN")
            try_insert(neck, "Kv42")
            try_insert(head, "Kv42")
            try_insert(neck, "Kv42b")
            try_insert(head, "Kv42b")
            try_insert(neck, "Kv11")
            try_insert(head, "Kv11")
            try_insert(neck, "Kir21")
            try_insert(head, "Kir21")
            try_insert(neck, "Kv14")
            try_insert(head, "Kv14")
            try_insert(neck, "Kv21")
            try_insert(head, "Kv21")
            try_insert(neck, "Kv33")
            try_insert(head, "Kv33")
            try_insert(neck, "Kv34")
            try_insert(head, "Kv34")
            try_insert(neck, "Kv723")
            try_insert(head, "Kv723")
            try_insert(neck, "ichan3")
            try_insert(head, "ichan3")
            try_insert(neck, "na8st")
            try_insert(head, "na8st")
            try_insert(neck, "Cav22")
            try_insert(head, "Cav22")
            try_insert(neck, "Cav32")
            try_insert(head, "Cav32")

            if has_mech(head, "Cav12"):
                for seg in head:
                    seg.Cav12.gbar = 5e-5 * 3.0   #(spine head > soma)
            if has_mech(neck, "Cav12"):
                for seg in neck:
                    seg.Cav12.gbar = 5e-5 * 1.0

            if has_mech(head, "BK"):
                for seg in head:
                    seg.BK.gakbar = 1e-4
                    seg.BK.gabkbar = 1e-4
            if has_mech(neck, "BK"):
                for seg in neck:
                    seg.BK.gakbar = 1e-4
                    seg.BK.gabkbar = 1e-4

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
                    seg.Kv42.gkbar = 1.5e-4
            if has_mech(neck, "Kv42"):
                for seg in neck:
                    seg.Kv42.gkbar = 1.5e-4

            if has_mech(head, "Kv42b"):
                for seg in head:
                    seg.Kv42b.gkbar = 1.5e-4
            if has_mech(neck, "Kv42b"):
                for seg in neck:
                    seg.Kv42b.gkbar = 1.5e-4

            if has_mech(head, "Kv11"):
                for seg in head:
                    seg.Kv11.gkbar = 1e-5
            if has_mech(neck, "Kv11"):
                for seg in neck:
                    seg.Kv11.gkbar = 1e-5

            if has_mech(head, "Kir21"):
                for seg in head:
                    seg.Kir21.gkbar = 1.5e-4
            if has_mech(neck, "Kir21"):
                for seg in neck:
                    seg.Kir21.gkbar = 1.5e-4

            if has_mech(head, "Kv14"):
                for seg in head:
                    seg.Kv14.gkbar = 1e-5
            if has_mech(neck, "Kv14"):
                for seg in neck:
                    seg.Kv14.gkbar = 1e-5

            if has_mech(head, "Kv21"):
                for seg in head:
                    seg.Kv21.gkbar = 1e-5
            if has_mech(neck, "Kv21"):
                for seg in neck:
                    seg.Kv21.gkbar = 1e-5

            if has_mech(head, "Kv33"):
                for seg in head:
                    seg.Kv33.gkbar = 8e-4
            if has_mech(neck, "Kv33"):
                for seg in neck:
                    seg.Kv33.gkbar = 8e-4

            if has_mech(head, "Kv34"):
                for seg in head:
                    seg.Kv34.gkbar = 1.3e-3
            if has_mech(neck, "Kv34"):
                for seg in neck:
                    seg.Kv34.gkbar = 1.3e-3

            if has_mech(head, "Kv723"):
                for seg in head:
                    seg.Kv723.gkbar = 2e-3
            if has_mech(neck, "Kv723"):
                for seg in neck:
                    seg.Kv723.gkbar = 2e-3

            if has_mech(head, "ichan3"):
                for seg in head:
                    seg.ichan3.gnabar = 5e-3
                    seg.ichan3.gkfbar = 5e-4
                    seg.ichan3.gksbar = 5e-4
                    seg.ichan3.gkabar = 5e-4
            if has_mech(neck, "ichan3"):
                for seg in neck:
                    seg.ichan3.gnabar = 5e-3
                    seg.ichan3.gkfbar = 5e-4
                    seg.ichan3.gksbar = 5e-4
                    seg.ichan3.gkabar = 5e-4

            if has_mech(head, "na8st"):
                for seg in head:
                    seg.na8st.gbar = 5e-6
            if has_mech(neck, "na8st"):
                for seg in neck:
                    seg.na8st.gbar = 5e-6

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
                    seg.Caold.gtcabar = 1e-5  #still need tune?
                    seg.Caold.gncabar = 1e-5
                    seg.Caold.glcabar = 1e-5

            if has_mech(neck, "Caold"):
                for seg in neck:
                    seg.Caold.gtcabar = 1e-5
                    seg.Caold.gncabar = 1e-5
                    seg.Caold.glcabar = 1e-5

            #Cabuffer param - sets buffering strength/kinetics
            if has_mech(head, "Cabuffer"):
                for seg in head:
                    seg.Cabuffer.tau = 9.0  #ms
                    seg.Cabuffer.brat = 1.0 #buffer ratio factor

            if has_mech(neck, "Cabuffer"):
                for seg in neck:
                    seg.Cabuffer.tau = 9.0
                    seg.Cabuffer.brat = 1.0

            self.spine_necks.append(neck)
            self.spines.append(head)

    def _set_geometry(self):
        self.soma.L = 20.0
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
                    seg.pas.g = 3e-5
                    seg.pas.e = -70.0

        #Hodgkin-Huxley(hh)-style mechanism
        for sec in [self.soma, self.ais, self.axon, self.dend_prox, self.dend_dist]:
            try_insert(sec, "hh")

        if has_mech(self.soma, "hh"):
            for seg in self.soma:
                seg.hh.gnabar = 0.12
                seg.hh.gkbar = 0.036
                seg.hh.gl = 5e-5
                seg.hh.el = -70.0

        if has_mech(self.ais, "hh"):
            for seg in self.ais:
                seg.hh.gnabar = 0.30   #ais Na high vs soma/axon
                seg.hh.gkbar = 0.05    #moderate K
                seg.hh.gl = 5e-5
                seg.hh.el = -70.0


        if has_mech(self.axon, "hh"):
            for seg in self.axon:
                seg.hh.gnabar = 0.18
                seg.hh.gkbar = 0.05
                seg.hh.gl = 5e-5
                seg.hh.el = -70.0

        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "hh"):
                for seg in sec:
                    seg.hh.gnabar = 0.03
                    seg.hh.gkbar = 0.01
                    seg.hh.gl = 5e-5
                    seg.hh.el = -70.0

        #adds mechanisms from Beining 2017
        for sec in [self.soma, self.dend_prox, self.dend_dist]:
            try_insert(sec, "Caold")
            try_insert(sec, "Cabuffer")
            try_insert(sec, "Cav12")
            try_insert(sec, "Cav22")
            try_insert(sec, "Cav32")
            try_insert(sec, "BK")
            try_insert(sec, "SK2")
            try_insert(sec, "HCN")
            try_insert(sec, "Kv42")
            try_insert(sec, "Kv11")
            try_insert(sec, "ichan3")
            try_insert(sec, "Kir21")
            try_insert(sec, "Kv14")
            try_insert(sec, "Kv21")
            try_insert(sec, "Kv33")
            try_insert(sec, "Kv34")
            try_insert(sec, "Kv42b")
            try_insert(sec, "Kv723")
            try_insert(sec, "na8st")

        self._set_channel_densities_default()
#set baseline conductances
    def _set_channel_densities_default(self):
       for sec in [self.soma, self.dend_prox, self.dend_dist]:
            for seg in sec:
                if sec is self.soma:
                    scale = 1.0
                elif sec is self.dend_prox:
                    scale = 1.8
                else:
                    scale = 2.5

                #Cav12 baseline
                if has_mech(sec, "Cav12"):
                    seg.Cav12.gbar = 5e-5 * scale

                #Cav22
                if has_mech(sec, "Cav22"):
                    seg.Cav22.gbar = 1e-5 * scale

                #Cav32
                if has_mech(sec, "Cav32"):
                    seg.Cav32.gbar = 1e-5 * scale

                #BK
                if has_mech(sec, "BK"):
                    seg.BK.gakbar = 1e-4 * scale
                    seg.BK.gabkbar = 1e-4 * scale

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
                    seg.Kv21.gkbar = 1e-5 * scale

                #Kv33 & Kv34
                if has_mech(sec, "Kv33"):
                    seg.Kv33.gkbar = 8e-4 * scale
                if has_mech(sec, "Kv34"):
                    seg.Kv34.gkbar = 1.3e-3 * scale

                #Kv723 (KCNQ/M-current style)
                if has_mech(sec, "Kv723"):
                    seg.Kv723.gkbar = 2e-3 * scale

                #Kir21
                if has_mech(sec, "Kir21"):
                    seg.Kir21.gkbar = 1.5e-4 * scale

                #ichan3 (multi-current Na/K mechanism)
                if has_mech(sec, "ichan3"):
                    seg.ichan3.gnabar = 5e-3 * scale
                    seg.ichan3.gkfbar = 5e-4 * scale
                    seg.ichan3.gksbar = 5e-4 * scale
                    seg.ichan3.gkabar = 5e-4 * scale

                #na8st (Na channel)
                if has_mech(sec, "na8st"):
                    seg.na8st.gbar =  5e-6 * scale

                #Caold conductances
                if has_mech(sec, "Caold"):
                    seg.Caold.gtcabar = 1e-5 * scale
                    seg.Caold.gncabar = 1e-5 * scale
                    seg.Caold.glcabar = 1e-5 * scale

                #Cabuffer params - sets buffering strength/kinetics
                if has_mech(sec, "Cabuffer"):
                    seg.Cabuffer.tau = 9.0  #ms
                    seg.Cabuffer.brat = 1.0  #buffer ratio factor

    def scale_cav12(self, factor: float):
        for sec in [self.soma, self.dend_prox, self.dend_dist]:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar *= factor

    def add_voltage_clamp(self, hold=-70.0, step=-50.0, delay=100.0, dur=300.0, sec=None,
                          loc=0.5):  #voltage clamp
        sec = self.soma if sec is None else sec
        self.vclamp = h.SEClamp(sec(loc))  #use single electrode clamp, is in nA
        self.vclamp.dur1 = delay
        self.vclamp.amp1 = hold
        self.vclamp.dur2 = dur
        self.vclamp.amp2 = step
        self.vclamp.dur3 = 0.0
        self.vclamp.amp3 = hold
        self.vclamp.rs = 0.01  #series resistance in megaohm, small = stronger clamp

    def setup_recording(self):
        #time
        self.t_vec = h.Vector()
        self.t_vec.record(h._ref_t)

        #voltages
        self.vsoma_vec = h.Vector()
        self.vsoma_vec.record(self.soma(0.5)._ref_v)

        self.vprox_vec = h.Vector()
        self.vprox_vec.record(self.dend_prox(0.5)._ref_v)

        self.vdend_vec = h.Vector()
        self.vdend_vec.record(self.dend_dist(0.9)._ref_v)

        self.vspine_vec = h.Vector()
        self.vspine_vec.record(self.spines[0](0.5)._ref_v)  #first spine head

        #calcium
        try:
            _ = self.soma(0.5)._ref_cai

            self.cai_soma_vec = h.Vector()
            self.cai_soma_vec.record(self.soma(0.5)._ref_cai)

            self.cai_prox_vec = h.Vector()
            self.cai_prox_vec.record(self.dend_prox(0.5)._ref_cai)

            self.cai_dist_vec = h.Vector()
            self.cai_dist_vec.record(self.dend_dist(0.9)._ref_cai)

            self.cai_spine_vec = h.Vector()
            self.cai_spine_vec.record(self.dend_dist(self.spine_xs[0])._ref_cai) #now records spine ca from attachment site on dendrite
        except Exception as e:
            print("Calcium recording set up failed becasue:", e)
            self.cai_soma_vec = None
            self.cai_prox_vec = None
            self.cai_dist_vec = None
            self.cai_spine_vec = None


        self.iclamp_vec = None
        try:
            if hasattr(self, "vclamp") and self.vclamp is not None:
                self.iclamp_vec = h.Vector()
                self.iclamp_vec.record(self.vclamp._ref_i)
        except Exception:
            self.iclamp_vec = None

def run_sim(cell: DGGranuleLikeCell, tstop=500.0, v_init=-70.0, dt=0.025):
    h.dt = dt
    h.tstop = tstop
    h.finitialize(v_init)
    h.continuerun(tstop)

    t = np.array(cell.t_vec)
    vs = np.array(cell.vsoma_vec)
    vd = np.array(cell.vdend_vec)
    vp = np.array(cell.vprox_vec)
    vsp = np.array(cell.vspine_vec)
    cai_soma = np.array(cell.cai_soma_vec) if cell.cai_soma_vec is not None else None
    cai_prox = np.array(cell.cai_prox_vec) if cell.cai_prox_vec is not None else None
    cai_dist = np.array(cell.cai_dist_vec) if cell.cai_dist_vec is not None else None
    cai_spine = np.array(cell.cai_spine_vec) if cell.cai_spine_vec is not None else None
    return t, vs, vp, vd, vsp, cai_soma, cai_prox, cai_dist, cai_spine


if __name__ == "__main__":
    #baseline aka WT
    cell = DGGranuleLikeCell()
    #mechanism sanity checks - soma sanity
    for mech in ["BK", "Cav12", "Cav22", "Cav32", "SK2", "HCN", "Cabuffer", "Caold", "Kv42", "Kv11", "ichan3", "Kir21",
                 "Kv14", "Kv21", "Kv33", "Kv34", "Kv42b", "Kv723", "na8st"]:
        print(f"Has {mech} on soma?", h.ismembrane(mech, sec=cell.soma))

    #mechanism sanity checks - prox dend sanity
    for mech in ["BK", "Cav12", "Cav22", "Cav32", "SK2", "HCN", "Cabuffer", "Caold", "Kv42", "Kv11", "ichan3", "Kir21",
                 "Kv14", "Kv21", "Kv33", "Kv34", "Kv42b", "Kv723", "na8st"]:
        print(f"Has {mech} on prox dend?", h.ismembrane(mech, sec=cell.dend_prox))

    #mechanism sanity checks - dist dend sanity
    for mech in ["BK", "Cav12", "Cav22", "Cav32", "SK2", "HCN", "Cabuffer", "Caold", "Kv42", "Kv11", "ichan3", "Kir21",
                 "Kv14", "Kv21", "Kv33", "Kv34", "Kv42b", "Kv723", "na8st"]:
        print(f"Has {mech} on dist dend?", h.ismembrane(mech, sec=cell.dend_dist))

    #mechanism sanity checks - spine head sanity
    for mech in ["Cav12", "Cabuffer", "Caold"]:
        print(f"Has {mech} on spine head[0]?", h.ismembrane(mech, sec=cell.spines[0]))

    #print density mechanism names & parameters
    print("SOMA PSECTION (look at density_mechs!)")
    print(cell.soma.psection())

    #make NEURON shape plot schematic of topology so can see where everything connects - methods figure
  #  h.define_shape()
   # ps = h.PlotShape(True)  #show diameters
   # ps.exec_menu("View = plot")  #open shape window
   # h.topology()  #keep printing the tree in the console
    #opens but freezes, check the neuron doccumentation on 28/01 to fix
    #commented out for now - need check cell builder function to make more realistic morpho
    #then shape again

    cell.add_voltage_clamp(hold=-70.0, step=-50.0, delay=100, dur=300)
    cell.setup_recording()
    t0, vs0, vp0, vd0, vsp0, cai0_soma, cai0_prox, cai0_dist, cai0_spine = run_sim(cell, tstop=500, v_init=-70, dt=0.025)
    I0 = np.array(cell.iclamp_vec) if cell.iclamp_vec is not None else None  #adds current arrays so can plot, in nA
    print("lens:", len(t0), len(vs0), len(vp0), len(vd0), len(vsp0))
    print("Peak cai base soma/prox/dist/spine:", #prints ca peak in each compartmnet
          float(np.max(cai0_soma)),
          float(np.max(cai0_prox)),
          float(np.max(cai0_dist)),
          float(np.max(cai0_spine)))   #why is the spine ca so high? how did i even
                                       #rec from where spine attach looks much more reasonable now - check data for sensibility

    #50% Cav1.2 - mimic +/-
    cell2 = DGGranuleLikeCell()
    cell2.scale_cav12(0.5)
    cell2.add_voltage_clamp(hold=-70.0, step=-50.0, delay=100, dur=300)
    cell2.setup_recording()
    t1, vs1, vp1, vd1, vsp1, cai1_soma, cai1_prox, cai1_dist, cai1_spine = run_sim(cell2, tstop=500, v_init=-70, dt=0.025)
    I1 = np.array(cell2.iclamp_vec) if cell2.iclamp_vec is not None else None #in nA
    print("lens:", len(t1), len(vs1), len(vp1), len(vd1), len(vsp1))
    print("Peak cai 50% soma/prox/dist/spine:",  #prints ca peak in each compartmnet
          float(np.max(cai1_soma)),
          float(np.max(cai1_prox)),
          float(np.max(cai1_dist)),
          float(np.max(cai1_spine)))


    vpeak0, tpeak0, vmin0, tmin0, ahp0 = ahp_depth(t0, vs0)
    vpeak1, tpeak1, vmin1, tmin1, ahp1 = ahp_depth(t1, vs1)

    print("Percent Δ peak cai (50% vs WT):",
          100 * (cai1_soma.max() / cai0_soma.max() - 1),
          100 * (cai1_prox.max() / cai0_prox.max() - 1),
          100 * (cai1_dist.max() / cai0_dist.max() - 1),
          100 * (cai1_spine.max() / cai0_spine.max() - 1))

    print(f"AHP baseline: {ahp0:.3f} mV (trough {vmin0:.2f} mV at {tmin0:.2f} ms)")
    print(f"AHP Cav12 50%: {ahp1:.3f} mV (trough {vmin1:.2f} mV at {tmin1:.2f} ms)")
    print(f"ΔAHP (50% - base): {ahp1 - ahp0:+.3f} mV")

#plots - REMEMBER TO REMOVE TITLES AND WHATNOT BEFORE PUT IN DISS!

    #plot voltage clamp current comparison
    if I0 is not None and I1 is not None:
        plt.figure()
        plt.plot(t0, I0, label="baseline clamp current")
        plt.plot(t1, I1, label="Cav12 50% clamp current")
        plt.xlabel("Time (ms)")
        plt.ylabel("Clamp current nA")
        plt.title("Voltage clamp current (baseline vs Cav12 50%)")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("Clamp current not recorded (check vclamp + iclamp_vec)")

    #step I–V
    steps = np.arange(-90, 11, 10)  # mV: -90, -80, ... , +10
    hold = -70.0
    delay = 100.0
    dur = 200.0
    tstop = 350.0

    Ipeak_base = []
    Iss_base = []

    for vstep in steps:
        cell = DGGranuleLikeCell()
        cell.add_voltage_clamp(hold=hold, step=float(vstep), delay=delay, dur=dur)
        cell.setup_recording()

        t, vs, vp, vd, vsp, cai_soma, cai_prox, cai_dist, cai_spine = run_sim(cell, tstop=tstop, v_init=hold, dt=0.025)
        I = np.array(cell.iclamp_vec) if cell.iclamp_vec is not None else None

        #measure the peak and steady-state clamp current during the step
        w_peak = (t >= delay) & (t <= delay + 10)  #first 10 ms of step
        w_ss = (t >= delay + dur - 10) & (t <= delay + dur)  #last 10 ms of step

        Ipeak_base.append(float(np.min(I[w_peak])))  #remembre inward is negative
        Iss_base.append(float(np.mean(I[w_ss])))

    #plot I–V curves
    plt.figure()
    plt.plot(steps, Ipeak_base, marker="o")
    plt.xlabel("Command voltage (mV)")
    plt.ylabel("Peak clamp current (nA)")
    plt.title("Voltage clamp I–V (for peak current)")
    plt.tight_layout()
    plt.show()

    plt.figure()
    plt.plot(steps, Iss_base, marker="o")
    plt.xlabel("Command voltage (mV)")
    plt.ylabel("Steady-state clamp current (nA)")
    plt.title("Voltage clamp I–V (for steady-state current)")
    plt.tight_layout()
    plt.show()

#####plots below are from current clamp script, leaving here for now in case need/easy edit later
    #plot base soma AP
    plt.figure()
    plt.plot(t0, vs0, label="baseline soma")
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("Baseline soma action potential")
    plt.legend()
    plt.tight_layout()
    plt.show()
    #remember add save plt before do final

    #plot Cav1.2 50% soma AP
    plt.figure()
    plt.plot(t1, vs1, label="Cav1.2 50% soma")
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("Cav1.2 50% soma action potential")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #plot Vm soma comparison
    plt.figure()
    plt.plot(t0, vs0, label="soma baseline")
    plt.plot(t1, vs1, label="soma Cav12 50%")
    plt.xlabel("Time (ms)")
    plt.ylabel("Membrane potential (mV)")
    plt.title("Baseline vs reduced Cav1.2")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #plot base Vm with all compartments
    plt.figure()
    plt.plot(t0, vs0, label="soma")
    plt.plot(t0, vp0, label="prox dend (0.5)")
    plt.plot(t0, vd0, label="dist dend (0.9)")
    plt.plot(t0, vsp0, label="spine head[0]")
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("Baseline Vm across compartments")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #plot CaV1.2 50% Vm with all compartments
    plt.figure()
    plt.plot(t1, vs1, label="soma")
    plt.plot(t1, vp1, label="prox dend (0.5)")
    plt.plot(t1, vd1, label="dist dend (0.9)")
    plt.plot(t1, vsp1, label="spine head[0]")
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("50% Vm across compartments")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #plot baseline cai across compartments
    if (cai0_soma is not None and cai0_prox is not None and cai0_dist is not None and cai0_spine is not None):
        plt.figure()
        plt.plot(t0, cai0_soma, label="soma")
        plt.plot(t0, cai0_prox, label="prox dend (0.5)")
        plt.plot(t0, cai0_dist, label="dist dend (0.9)")
        plt.plot(t0, cai0_spine, label="spine head[0]")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Baseline cai across compartments")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("baseline cai missing for one or more compartments")

    # plot 50% cai across compartments
    if (cai1_soma is not None and cai1_prox is not None and cai1_dist is not None and cai1_spine is not None):
        plt.figure()
        plt.plot(t1, cai1_soma, label="soma")
        plt.plot(t1, cai1_prox, label="prox dend (0.5)")
        plt.plot(t1, cai1_dist, label="dist dend (0.9)")
        plt.plot(t1, cai1_spine, label="spine head[0]")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("50% cai across compartments")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("50% cai missing for one or more compartments")

    #plot cai comparison in soma
    if cai0_soma is not None and cai1_soma is not None:
        plt.figure()
        plt.plot(t0, cai0_soma, label="baseline")
        plt.plot(t1, cai1_soma, label="Cav12 50%")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Baseline vs reduced Cav1.2 intracellular Ca at soma")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("missing soma cai")

    #plot cai comparison in proximal dendrite
    if cai0_prox is not None and cai1_prox is not None:
        plt.figure()
        plt.plot(t0, cai0_prox, label="baseline")
        plt.plot(t1, cai1_prox, label="Cav12 50%")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Baseline vs reduced Cav1.2 intracellular Ca at prox dend")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("missing prox cai")

    #plot cai comparison in distal dendrite
    if cai0_dist is not None and cai1_dist is not None:
        plt.figure()
        plt.plot(t0, cai0_dist, label="baseline")
        plt.plot(t1, cai1_dist, label="Cav12 50%")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Baseline vs reduced Cav1.2 intracellular Ca at dist dend")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("missing dist cai")

    #plot cai comparison ate the spine-site
    if cai0_spine is not None and cai1_spine is not None:
        plt.figure()
        plt.plot(t0, cai0_spine, label="baseline")
        plt.plot(t1, cai1_spine, label="Cav12 50%")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Baseline vs reduced Cav1.2 intracellular Ca at spine-site")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("missing spine-site cai")

    #quantify Ca difference during stimulus window --in soma
    if cai0_soma is not None and cai1_soma is not None: #made soma-specifci
        win = (t0 >= 100) & (t0 <= 400)

        peak0 = float(np.max(cai0_soma[win]))
        peak1 = float(np.max(cai1_soma[win]))

        auc0 = float(np.trapz(cai0_soma[win], t0[win]))
        auc1 = float(np.trapz(cai1_soma[win], t0[win]))

        print(f"Peak cai baseline: {peak0:.6f} mM")
        print(f"Peak cai Cav12 50%: {peak1:.6f} mM  (Δ {peak1 - peak0:+.6f}, {100 * (peak1 / peak0 - 1):+.1f}%)")
        print(f"AUC cai baseline:  {auc0:.6f} mM·ms")
        print(f"AUC cai Cav12 50%: {auc1:.6f} mM·ms (Δ {auc1 - auc0:+.6f}, {100 * (auc1 / auc0 - 1):+.1f}%)")
    else:
        print("Skipping Ca quantification because cai is None.")



