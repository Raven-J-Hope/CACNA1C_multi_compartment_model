#!/usr/bin/env python3

import os
from neuron import h

h.load_file("stdrun.hoc")

#make and set dir & paths to compiled mod files
MOD_DIR = "/home/raven/PycharmProjects/Masters/Mod_Files"
DLL_PATH = os.path.join(MOD_DIR, "x86_64", "libnrnmech.so")

if os.path.exists(DLL_PATH):
    h.nrn_load_dll(DLL_PATH)
    print("Loaded mechanisms:", DLL_PATH)
else:
    raise RuntimeError(f"Compiled mechanisms not found at: {DLL_PATH}")

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


#define cell morphology & biophysics
class DGGranuleLikeCell:
    def __init__(self, name="dgcell", bk_split=None):
        self.name = name
        self.bk_split = WT_BK_SPLIT if bk_split is None else bk_split
        validate_bk_split(self.bk_split)
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
                    seg.pas.g = 0.00039
                    seg.pas.e = -70.0
            if has_mech(head, "pas"):
                for seg in head:
                    seg.pas.g = 0.00039
                    seg.pas.e = -70.0

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
            try_insert(neck, "BK_Cav22")
            try_insert(head, "BK_Cav22")
            try_insert(neck, "BK_Cav12")
            try_insert(head, "BK_Cav12")
            try_insert(neck, "BK_Cav21")
            try_insert(head, "BK_Cav21")
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
            try_insert(neck, "Cav2_1")
            try_insert(head, "Cav2_1")

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
                total_bk_gakbar=3e-2,
                total_bk_gabkbar=3e-2,
                split=self.bk_split
            )

            apply_bk_split_to_section(
                neck,
                total_bk_gakbar=3e-2,
                total_bk_gabkbar=3e-2,
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
                    seg.Kv21.gkbar = 3e-5
            if has_mech(neck, "Kv21"):
                for seg in neck:
                    seg.Kv21.gkbar = 3e-5

            if has_mech(head, "Kv33"):
                for seg in head:
                    seg.Kv33.gkbar = 2e-2
            if has_mech(neck, "Kv33"):
                for seg in neck:
                    seg.Kv33.gkbar = 2e-2

            if has_mech(head, "Kv34"):
                for seg in head:
                    seg.Kv34.gkbar = 2e-3
            if has_mech(neck, "Kv34"):
                for seg in neck:
                    seg.Kv34.gkbar = 2e-3

            if has_mech(head, "Kv723"):
                for seg in head:
                    seg.Kv723.gkbar = 2e-9
            if has_mech(neck, "Kv723"):
                for seg in neck:
                    seg.Kv723.gkbar = 2e-9

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
                    seg.na8st.gbar = 0.000001
            if has_mech(neck, "na8st"):
                for seg in neck:
                    seg.na8st.gbar = 0.000001

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
                    seg.Cabuffer.brat = 1.0 #buffer ratio factor

            if has_mech(neck, "Cabuffer"):
                for seg in neck:
                    seg.Cabuffer.tau = 8.0
                    seg.Cabuffer.brat = 1.0

            #Cav2.1permeability baseline
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
            sec.Ra = 300.0       #Ra = axial resistivity
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
        for sec in [self.soma, self.ais, self.dend_prox, self.dend_dist]:
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
        for sec in [self.soma, self.ais, self.dend_prox, self.dend_dist]:
            for seg in sec:
                if sec is self.soma:
                    scale = 1.0
                elif sec is self.dend_prox:
                    scale = 1.8
                else:
                    scale = 2.5

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
                total_bk_gakbar = 3e-2 * scale #was 1e-4
                total_bk_gabkbar = 3e-2 * scale

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
                    seg.Cabuffer.brat = 1.0  #buffer ratio factor

                #Cav2.1 permeability density, cm/s
                if has_mech(sec, "Cav2_1"):
                    seg.Cav2_1.pcabar = 1e-5 * scale  #-6 = like w/ no cav21, -5 big diff & -4 break
                    seg.Cav2_1.vshift = 0.0

    def scale_cav12(self, factor: float):
        for sec in [self.soma, self.ais, self.dend_prox, self.dend_dist] + self.spine_necks + self.spines:
            if has_mech(sec, "Cav12"):
                for seg in sec:
                    seg.Cav12.gbar *= factor

    def add_current_clamp(self, delay=100.0, dur=300.0, amp=0.3, sec=None, loc=0.5):
        sec = self.soma if sec is None else sec
        self.iclamp = h.IClamp(sec(loc))
        self.iclamp.delay = delay
        self.iclamp.dur = dur   #dur = duration
        self.iclamp.amp = amp  #unit = nA

    def add_voltage_clamp(self, hold=-70.0, step=-50.0, delay=100.0, dur=300.0, sec=None, loc=0.5):
        sec = self.soma if sec is None else sec
        self.vclamp = h.SEClamp(sec(loc))  #use single electrode clamp, is in nA
        self.vclamp.dur1 = delay
        self.vclamp.amp1 = hold
        self.vclamp.dur2 = dur
        self.vclamp.amp2 = step
        self.vclamp.dur3 = 50.0
        self.vclamp.amp3 = hold
        self.vclamp.rs = 0.01

    def setup_recording(self):
        #time
        self.t_vec = h.Vector()
        self.t_vec.record(h._ref_t)

        #voltages - soma
        self.vsoma_vec = h.Vector()
        self.vsoma_vec.record(self.soma(0.5)._ref_v)

        #AIS
        self.vais_vec = h.Vector()
        self.vais_vec.record(self.ais(0.5)._ref_v)

        #prox dend
        self.vprox_vec = h.Vector()
        self.vprox_vec.record(self.dend_prox(0.5)._ref_v)

        #dist dend
        self.vdend_vec = h.Vector()
        self.vdend_vec.record(self.dend_dist(0.9)._ref_v)

        #spine
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
            self.cai_spine_vec.record(self.spines[0](0.5)._ref_cai) #rec spines

            self.cai_neck_vec = h.Vector()
            self.cai_neck_vec.record(self.spine_necks[0](0.5)._ref_cai) #rec spine neck

            self.cai_ais_vec = h.Vector()
            self.cai_ais_vec.record(self.ais(0.5)._ref_cai)

        except Exception as e:
            print("Calcium recording set up failed becasue:", e)
            self.cai_soma_vec = None
            self.cai_prox_vec = None
            self.cai_dist_vec = None
            self.cai_spine_vec = None
            self.cai_ais_vec = None

        #BK_Cav22 local Ca
        self.bk_acai22_soma_vec = None
        if has_mech(self.soma, "BK_Cav22"):
            try:
                self.bk_acai22_soma_vec = h.Vector()
                self.bk_acai22_soma_vec.record(self.soma(0.5).BK_Cav22._ref_acai)
                print("Recorded BK_Cav22 acai on soma")
            except Exception as e:
                print("FAILED recording BK_Cav22 acai:", e)

        #BK_Cav12 local Ca
        self.bk_acai12_soma_vec = None
        if has_mech(self.soma, "BK_Cav12"):
            try:
                self.bk_acai12_soma_vec = h.Vector()
                self.bk_acai12_soma_vec.record(self.soma(0.5).BK_Cav12._ref_acai)
                print("Recorded BK_Cav12 acai on soma")
            except Exception as e:
                print("FAILED recording BK_Cav12 acai:", e)

        #BK_Cav21 local Ca
        self.bk_acai21_soma_vec = None
        if has_mech(self.soma, "BK_Cav21"):
            try:
                self.bk_acai21_soma_vec = h.Vector()
                self.bk_acai21_soma_vec.record(self.soma(0.5).BK_Cav21._ref_acai)
                print("Recorded BK_Cav21 acai on soma")
            except Exception as e:
                print("FAILED recording BK_Cav21 acai:", e)

        #total ionic current densities (mA/cm2) at soma(0.5)
        self.ica_soma_vec = h.Vector()
        self.ica_soma_vec.record(self.soma(0.5)._ref_ica)  #all ca

        self.ik_soma_vec = h.Vector()
        self.ik_soma_vec.record(self.soma(0.5)._ref_ik)  #all k

        self.ina_soma_vec = h.Vector()
        self.ina_soma_vec.record(self.soma(0.5)._ref_ina)  #all na

        if has_mech(self.soma, "na8st"):
            self.na8st_m_soma_vec = h.Vector()
            #na8st Markov proxies
            self.na8st_o_soma_vec = None
            self.na8st_g_soma_vec = None
            self.na8st_i_soma_vecs = None  #list of vectors

            self.na8st_o_ais_vec = None
            self.na8st_g_ais_vec = None
            self.na8st_i_ais_vecs = None  #list of vectors

            def _record_na8st_states(sec, loc=0.5):
                """Return (o_vec, g_vec, [i1..i6 vecs]) or (None, None, None) if na8st not present."""
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

            #record soma + AIS
            self.na8st_o_soma_vec, self.na8st_g_soma_vec, self.na8st_i_soma_vecs = _record_na8st_states(self.soma, 0.5)
            self.na8st_o_ais_vec, self.na8st_g_ais_vec, self.na8st_i_ais_vecs = _record_na8st_states(self.ais, 0.5)

        #BK specific current density (mA/cm2)
#        self.bk_ik_soma_vec = None
#        if has_mech(self.soma, "BK"):
#            self.bk_ik_soma_vec = h.Vector()
#            self.bk_ik_soma_vec.record(self.soma(0.5).BK._ref_ik)  #BK

        #BK_Cav22 specific current density (mA/cm2)
        self.bk_Cav22_ik_soma_vec = None
        if has_mech(self.soma, "BK_Cav22"):
            self.bk_Cav22_ik_soma_vec = h.Vector()
            self.bk_Cav22_ik_soma_vec.record(self.soma(0.5).BK_Cav22._ref_ik)  #BK_Cav22

        #BK_Cav12 specific current density (mA/cm2)
        self.bk_Cav12_ik_soma_vec = None
        if has_mech(self.soma, "BK_Cav12"):
            self.bk_Cav12_ik_soma_vec = h.Vector()
            self.bk_Cav12_ik_soma_vec.record(self.soma(0.5).BK_Cav12._ref_ik)  #BK_Cav12

        #BK_Cav21 specific current density (mA/cm2)
        self.bk_Cav21_ik_soma_vec = None
        if has_mech(self.soma, "BK_Cav21"):
            self.bk_Cav21_ik_soma_vec = h.Vector()
            self.bk_Cav21_ik_soma_vec.record(self.soma(0.5).BK_Cav21._ref_ik)  #BK_Cav21

        #BK_Cav22 currents by compartment
        self.bk_Cav22_ik_ais_vec = None
        self.bk_Cav22_ik_prox_vec = None
        self.bk_Cav22_ik_dist_vec = None
        self.bk_Cav22_ik_spine_vec = None

        if has_mech(self.ais, "BK_Cav22"):
            self.bk_Cav22_ik_ais_vec = h.Vector()
            self.bk_Cav22_ik_ais_vec.record(self.ais(0.5).BK_Cav22._ref_ik)

        if has_mech(self.dend_prox, "BK_Cav22"):
            self.bk_Cav22_ik_prox_vec = h.Vector()
            self.bk_Cav22_ik_prox_vec.record(self.dend_prox(0.5).BK_Cav22._ref_ik)

        if has_mech(self.dend_dist, "BK_Cav22"):
            self.bk_Cav22_ik_dist_vec = h.Vector()
            self.bk_Cav22_ik_dist_vec.record(self.dend_dist(0.9).BK_Cav22._ref_ik)

        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav22"):
            self.bk_Cav22_ik_spine_vec = h.Vector()
            self.bk_Cav22_ik_spine_vec.record(self.spines[0](0.5).BK_Cav22._ref_ik)

        #BK_Cav12 currents by compartment
        self.bk_Cav12_ik_ais_vec = None
        self.bk_Cav12_ik_prox_vec = None
        self.bk_Cav12_ik_dist_vec = None
        self.bk_Cav12_ik_spine_vec = None

        if has_mech(self.ais, "BK_Cav12"):
            self.bk_Cav12_ik_ais_vec = h.Vector()
            self.bk_Cav12_ik_ais_vec.record(self.ais(0.5).BK_Cav12._ref_ik)

        if has_mech(self.dend_prox, "BK_Cav12"):
            self.bk_Cav12_ik_prox_vec = h.Vector()
            self.bk_Cav12_ik_prox_vec.record(self.dend_prox(0.5).BK_Cav12._ref_ik)

        if has_mech(self.dend_dist, "BK_Cav12"):
            self.bk_Cav12_ik_dist_vec = h.Vector()
            self.bk_Cav12_ik_dist_vec.record(self.dend_dist(0.9).BK_Cav12._ref_ik)

        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav12"):
            self.bk_Cav12_ik_spine_vec = h.Vector()
            self.bk_Cav12_ik_spine_vec.record(self.spines[0](0.5).BK_Cav12._ref_ik)

        #BK_Cav21 currents by compartment
        self.bk_Cav21_ik_ais_vec = None
        self.bk_Cav21_ik_prox_vec = None
        self.bk_Cav21_ik_dist_vec = None
        self.bk_Cav21_ik_spine_vec = None

        if has_mech(self.ais, "BK_Cav21"):
            self.bk_Cav21_ik_ais_vec = h.Vector()
            self.bk_Cav21_ik_ais_vec.record(self.ais(0.5).BK_Cav21._ref_ik)

        if has_mech(self.dend_prox, "BK_Cav21"):
            self.bk_Cav21_ik_prox_vec = h.Vector()
            self.bk_Cav21_ik_prox_vec.record(self.dend_prox(0.5).BK_Cav21._ref_ik)

        if has_mech(self.dend_dist, "BK_Cav21"):
            self.bk_Cav21_ik_dist_vec = h.Vector()
            self.bk_Cav21_ik_dist_vec.record(self.dend_dist(0.9).BK_Cav21._ref_ik)

        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav21"):
            self.bk_Cav21_ik_spine_vec = h.Vector()
            self.bk_Cav21_ik_spine_vec.record(self.spines[0](0.5).BK_Cav21._ref_ik)

        #BK local acai by compartment: Cav22
        self.bk_acai22_ais_vec = None
        self.bk_acai22_prox_vec = None
        self.bk_acai22_dist_vec = None
        self.bk_acai22_spine_vec = None

        if has_mech(self.ais, "BK_Cav22"):
            self.bk_acai22_ais_vec = h.Vector()
            self.bk_acai22_ais_vec.record(self.ais(0.5).BK_Cav22._ref_acai)

        if has_mech(self.dend_prox, "BK_Cav22"):
            self.bk_acai22_prox_vec = h.Vector()
            self.bk_acai22_prox_vec.record(self.dend_prox(0.5).BK_Cav22._ref_acai)

        if has_mech(self.dend_dist, "BK_Cav22"):
            self.bk_acai22_dist_vec = h.Vector()
            self.bk_acai22_dist_vec.record(self.dend_dist(0.9).BK_Cav22._ref_acai)

        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav22"):
            self.bk_acai22_spine_vec = h.Vector()
            self.bk_acai22_spine_vec.record(self.spines[0](0.5).BK_Cav22._ref_acai)

        #BK local acai by compartment: Cav12
        self.bk_acai12_ais_vec = None
        self.bk_acai12_prox_vec = None
        self.bk_acai12_dist_vec = None
        self.bk_acai12_spine_vec = None

        if has_mech(self.ais, "BK_Cav12"):
            self.bk_acai12_ais_vec = h.Vector()
            self.bk_acai12_ais_vec.record(self.ais(0.5).BK_Cav12._ref_acai)

        if has_mech(self.dend_prox, "BK_Cav12"):
            self.bk_acai12_prox_vec = h.Vector()
            self.bk_acai12_prox_vec.record(self.dend_prox(0.5).BK_Cav12._ref_acai)

        if has_mech(self.dend_dist, "BK_Cav12"):
            self.bk_acai12_dist_vec = h.Vector()
            self.bk_acai12_dist_vec.record(self.dend_dist(0.9).BK_Cav12._ref_acai)

        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav12"):
            self.bk_acai12_spine_vec = h.Vector()
            self.bk_acai12_spine_vec.record(self.spines[0](0.5).BK_Cav12._ref_acai)

        #BK local acai by compartment for Cav21
        self.bk_acai21_ais_vec = None
        self.bk_acai21_prox_vec = None
        self.bk_acai21_dist_vec = None
        self.bk_acai21_spine_vec = None

        if has_mech(self.ais, "BK_Cav21"):
            self.bk_acai21_ais_vec = h.Vector()
            self.bk_acai21_ais_vec.record(self.ais(0.5).BK_Cav21._ref_acai)

        if has_mech(self.dend_prox, "BK_Cav21"):
            self.bk_acai21_prox_vec = h.Vector()
            self.bk_acai21_prox_vec.record(self.dend_prox(0.5).BK_Cav21._ref_acai)

        if has_mech(self.dend_dist, "BK_Cav21"):
            self.bk_acai21_dist_vec = h.Vector()
            self.bk_acai21_dist_vec.record(self.dend_dist(0.9).BK_Cav21._ref_acai)

        if len(self.spines) > 0 and has_mech(self.spines[0], "BK_Cav21"):
            self.bk_acai21_spine_vec = h.Vector()
            self.bk_acai21_spine_vec.record(self.spines[0](0.5).BK_Cav21._ref_acai)

        #SK specific current density (mA/cm2)
        self.sk_ik_soma_vec = None
        if has_mech(self.soma, "SK2"):
            self.sk_ik_soma_vec = h.Vector()
            self.sk_ik_soma_vec.record(self.soma(0.5).SK2._ref_ik)

        #SK acai, used by SK2 kinetics (mM) from mod as ca driver
        self.sk_acai_soma_vec = None
        if has_mech(self.soma, "SK2"):
            self.sk_acai_soma_vec = h.Vector()
            self.sk_acai_soma_vec.record(self.soma(0.5).SK2._ref_acai)

        #Cav2.1-specific current density (mA/cm2) at soma(0.5)
        self.cav21_ica_soma_vec = None
        if has_mech(self.soma, "Cav2_1"):
             self.cav21_ica_soma_vec = h.Vector()
             self.cav21_ica_soma_vec.record(self.soma(0.5).Cav2_1._ref_ipca)

        #Cav22-specific current density (mA/cm2) at soma(0.5)
        self.cav22_ica_soma_vec = None
        if has_mech(self.soma, "Cav22"):
            self.cav22_ica_soma_vec = h.Vector()
            self.cav22_ica_soma_vec.record(self.soma(0.5)._ref_inca)

        #Cav12-specific current density (mA/cm2) at soma(0.5)
        self.cav12_ica_soma_vec = None
        if has_mech(self.soma, "Cav12"):
            self.cav12_ica_soma_vec = h.Vector()
            self.cav12_ica_soma_vec.record(self.soma(0.5)._ref_ilca)

        #Cav1.3-specific current density (mA/cm2) at soma(0.5)
        self.cav13_ica_soma_vec = None
        if has_mech(self.soma, "Cav13"):
            self.cav13_ica_soma_vec = h.Vector()
            self.cav13_ica_soma_vec.record(self.soma(0.5)._ref_ilca13)

        #VC clamp current
        self.clamp_i_vec = None
        if self.vclamp is not None:
            self.clamp_i_vec = h.Vector()
            self.clamp_i_vec.record(self.vclamp._ref_i)

        #Total calcium current by compartment
        self.ica_ais_vec = None
        self.ica_prox_vec = None
        self.ica_dist_vec = None
        self.ica_spine_vec = None

        self.ica_ais_vec = h.Vector()
        self.ica_ais_vec.record(self.ais(0.5)._ref_ica)

        self.ica_prox_vec = h.Vector()
        self.ica_prox_vec.record(self.dend_prox(0.5)._ref_ica)

        self.ica_dist_vec = h.Vector()
        self.ica_dist_vec.record(self.dend_dist(0.9)._ref_ica)

        self.ica_spine_vec = h.Vector()
        self.ica_spine_vec.record(self.spines[0](0.5)._ref_ica)

        #Cav1.2-specific current density (mA/cm2) by compartment
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

        #Cav1.3-specific current density (mA/cm2) by compartment
        self.cav13_ica_ais_vec = None
        if has_mech(self.ais, "Cav13"):
            self.cav13_ica_ais_vec = h.Vector()
            self.cav13_ica_ais_vec.record(self.ais(0.5)._ref_ilca13)

        self.cav13_ica_prox_vec = None
        if has_mech(self.dend_prox, "Cav13"):
            self.cav13_ica_prox_vec = h.Vector()
            self.cav13_ica_prox_vec.record(self.dend_prox(0.5)._ref_ilca13)

        self.cav13_ica_dist_vec = None
        if has_mech(self.dend_dist, "Cav13"):
            self.cav13_ica_dist_vec = h.Vector()
            self.cav13_ica_dist_vec.record(self.dend_dist(0.9)._ref_ilca13)

        self.cav13_ica_spine_vec = None
        if len(self.spines) > 0 and has_mech(self.spines[0], "Cav13"):
            self.cav13_ica_spine_vec = h.Vector()
            self.cav13_ica_spine_vec.record(self.spines[0](0.5)._ref_ilca13)

        #Cav2.1-specific current density (mA/cm2) by compartment
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

        #Cav2.2-specific current density (mA/cm2) by compartment
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


WTCell = DGGranuleLikeCell