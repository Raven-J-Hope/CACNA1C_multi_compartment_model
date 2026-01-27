#!/usr/bin/env python3
import os
import numpy as np
import matplotlib.pyplot as plt
from neuron import h

h.load_file("stdrun.hoc")

#set dir & paths to compiled mod files
MOD_DIR = "/home/raven/PycharmProjects/Masters/Mod_Files_Beining_2017"
DLL_PATH = os.path.join(MOD_DIR, "x86_64", "libnrnmech.so")

if os.path.exists(DLL_PATH):
    h.nrn_load_dll(DLL_PATH)
    print("Loaded mechanisms:", DLL_PATH)
else:
    print("WARNING: compiled mechanisms not found at:", DLL_PATH)

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
        #create compartments - "nseg" i.e., segments
        self.soma = h.Section(name=f"{name}.soma")
        self.dend_prox = h.Section(name=f"{name}.dend_prox")
        self.dend_dist = h.Section(name=f"{name}.dend_dist")
        self.axon = h.Section(name=f"{name}.axon")

        #connects topology
        self.dend_prox.connect(self.soma(1))
        self.dend_dist.connect(self.dend_prox(1))
        self.axon.connect(self.soma(0))

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
            x = 0.1 + 0.8 * (i / max(1, n_spines - 1))  # ~0.1 to ~0.9
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
                    seg.pas.g = 1e-4
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

            self.spine_necks.append(neck)
            self.spines.append(head)

    def _set_geometry(self):
        self.soma.L = 20.0
        self.soma.diam = 20.0
        self.soma.nseg = 1

        self.dend_prox.L = 150.0
        self.dend_prox.diam = 2.0
        self.dend_prox.nseg = 9

        self.dend_dist.L = 200.0
        self.dend_dist.diam = 1.2
        self.dend_dist.nseg = 11

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
                    seg.pas.g = 1e-4
                    seg.pas.e = -70.0

        #Hodgkin-Huxley(hh)-style mechanism
        for sec in [self.soma, self.axon, self.dend_prox, self.dend_dist]:
            try_insert(sec, "hh")

        if has_mech(self.soma, "hh"):
            for seg in self.soma:
                seg.hh.gnabar = 0.12
                seg.hh.gkbar = 0.036
                seg.hh.gl = 0.0003
                seg.hh.el = -54.3

        if has_mech(self.axon, "hh"):
            for seg in self.axon:
                seg.hh.gnabar = 0.18
                seg.hh.gkbar = 0.05
                seg.hh.gl = 0.0003
                seg.hh.el = -54.3

        for sec in [self.dend_prox, self.dend_dist]:
            if has_mech(sec, "hh"):
                for seg in sec:
                    seg.hh.gnabar = 0.03
                    seg.hh.gkbar = 0.01
                    seg.hh.gl = 0.0003
                    seg.hh.el = -54.3

        #sets mechanisms from Beining 2017
        for sec in [self.soma, self.dend_prox, self.dend_dist]:
            try_insert(sec, "Caold")
            try_insert(sec, "Cabuffer")
            try_insert(sec, "Cav12")
            try_insert(sec, "Cav22")
            try_insert(sec, "Cav32")
            try_insert(sec, "BK")
            try_insert(sec, "SK2")
            try_insert(sec, "HCN")

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

                #BK
                if has_mech(sec, "BK"):
                    seg.BK.gakbar = 1e-4 * scale
                    seg.BK.gabkbar = 1e-4 * scale

                #SK2
                if has_mech(sec, "SK2"):
                    seg.SK2.gkbar = 1e-4 * scale

                #Caold conductances
                if has_mech(sec, "Caold"):
                    seg.Caold.gtcabar = 1e-5 * scale
                    seg.Caold.gncabar = 1e-5 * scale
                    seg.Caold.glcabar = 1e-5 * scale

    def scale_cav12(self, factor: float):
        for sec in [self.soma, self.dend_prox, self.dend_dist]:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar *= factor

    def add_current_clamp(self, delay=100.0, dur=300.0, amp=0.2, sec=None, loc=0.5):
        sec = self.soma if sec is None else sec
        self.iclamp = h.IClamp(sec(loc))
        self.iclamp.delay = delay
        self.iclamp.dur = dur   #dur = duration
        self.iclamp.amp = amp  #unit = nA

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
        self.vspine_vec.record(self.spines[0](0.5)._ref_v)  # first spine head

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
            self.cai_spine_vec.record(self.spines[0](0.5)._ref_cai)
        except Exception as e:
            print("Calcium recording set up failed becasue:", e)
            self.cai_soma_vec = None
            self.cai_prox_vec = None
            self.cai_dist_vec = None
            self.cai_spine_vec = None

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
    cai = np.array(cell.cai_soma_vec) if cell.cai_soma_vec is not None else None
    return t, vs, vp, vd, vsp, cai


if __name__ == "__main__":
    #baseline aka WT
    cell = DGGranuleLikeCell()
    print("Has Cav12 on prox dend?", h.ismembrane("Cav12", sec=cell.dend_prox)) #sanity check if cav in each compartment
    print("Has Cav12 on dist dend?", h.ismembrane("Cav12", sec=cell.dend_dist))
    print("Has Caold on prox dend?", h.ismembrane("Caold", sec=cell.dend_prox))
    print("Has Caold on dist dend?", h.ismembrane("Caold", sec=cell.dend_dist))
    cell.add_current_clamp(delay=100, dur=300, amp=0.2)
    cell.setup_recording()
    t0, vs0, vp0, vd0, vsp0, cai0_soma, cai0_prox, cai0_dist, cai0_spine = run_sim(cell, tstop=500, v_init=-70, dt=0.025)
    print("lens:", len(t0), len(vs0), len(vp0), len(vd0), len(vsp0))
    print("Peak cai soma/prox/dist/spine:", #prints ca peak in each compartmnet
          float(np.max(cai0_soma)),
          float(np.max(cai0_prox)),
          float(np.max(cai0_dist)),
          float(np.max(cai0_spine)))   ##why is the spine ca so high? how did i even

    #50% Cav1.2 - mimic +/-
    cell2 = DGGranuleLikeCell()
    cell2.scale_cav12(0.5)
    cell2.add_current_clamp(delay=100, dur=300, amp=0.2)
    cell2.setup_recording()
    t1, vs1, vp1, vd1, vsp1, cai1 = run_sim(cell2, tstop=500, v_init=-70, dt=0.025)

    vpeak0, tpeak0, vmin0, tmin0, ahp0 = ahp_depth(t0, vs0)
    vpeak1, tpeak1, vmin1, tmin1, ahp1 = ahp_depth(t1, vs1)

    print(f"AHP baseline: {ahp0:.3f} mV (trough {vmin0:.2f} mV at {tmin0:.2f} ms)")
    print(f"AHP Cav12 50%: {ahp1:.3f} mV (trough {vmin1:.2f} mV at {tmin1:.2f} ms)")
    print(f"ΔAHP (50% - base): {ahp1 - ahp0:+.3f} mV")

    #plot base soma
    plt.figure()
    plt.plot(t0, vs0, label="baseline soma")
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("Baseline soma action potential")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #plot Cav12 50% soma AP
    plt.figure()
    plt.plot(t1, vs1, label="Cav12 50% soma")
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.title("Cav12 50% soma action potential")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #plot base with all compartments
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

    #plot comparison
    plt.figure()
    plt.plot(t0, vs0, label="soma baseline")
    plt.plot(t1, vs1, label="soma Cav12 50%")
    plt.xlabel("Time (ms)")
    plt.ylabel("Membrane potential (mV)")
    plt.title("Baseline vs reduced Cav1.2 (Cav12)")
    plt.legend()
    plt.tight_layout()
    plt.show()

    #cai comparison
    if cai0 is not None and cai1 is not None:
        plt.figure()
        plt.plot(t0, cai0, label="cai baseline")
        plt.plot(t1, cai1, label="cai Cav12 50%")
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.title("Baseline vs reduced Cav1.2 intracellular Ca at soma")
        plt.legend()
        plt.tight_layout()
        plt.show()
    else:
        print("cai not recorded (check Ca mechanisms / recording).")

    #quantify Ca difference during stimulus window
    win = (t0 >= 100) & (t0 <= 400)

    peak0 = float(np.max(cai0[win]))
    peak1 = float(np.max(cai1[win]))

    auc0 = float(np.trapz(cai0[win], t0[win]))
    auc1 = float(np.trapz(cai1[win], t0[win]))

    print(f"Peak cai baseline: {peak0:.6f} mM")
    print(f"Peak cai Cav12 50%: {peak1:.6f} mM  (Δ {peak1 - peak0:+.6f}, {100 * (peak1 / peak0 - 1):+.1f}%)")
    print(f"AUC cai baseline:  {auc0:.6f} mM·ms")
    print(f"AUC cai Cav12 50%: {auc1:.6f} mM·ms (Δ {auc1 - auc0:+.6f}, {100 * (auc1 / auc0 - 1):+.1f}%)")

    #mechanism sanity checks
    for mech in ["BK", "Cav12", "Cav22", "Cav32", "SK2", "HCN", "Cabuffer", "Caold"]:
        print(f"Has {mech} on soma?", h.ismembrane(mech, sec=cell.soma))

    #print density mechanism names & parameters
    print("---- SOMA PSECTION (look at density_mechs) ----")
    print(cell.soma.psection())


