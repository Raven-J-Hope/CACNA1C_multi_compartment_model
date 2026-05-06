#!/usr/bin/env python3

PLOT_DEFINITIONS = {
    "IC": {
        "Vm soma": {
            "key": "vm_soma",
            "requires": ["t", "vs"],
            "title": "Soma membrane potential",
            "ylabel": "Vm (mV)",
        },
        "Vm compartments": {
            "key": "vm_compartments",
            "requires": ["t", "vs", "vais", "vp", "vd", "vsp"],
            "title": "Membrane potential by compartment",
            "ylabel": "Vm (mV)",
        },
        "cai soma": {
            "key": "cai_soma",
            "requires": ["t", "cai_soma"],
            "title": "Soma intracellular calcium",
            "ylabel": "cai (mM)",
        },
        "cai compartments": {
            "key": "cai_compartments",
            "requires": ["t", "cai_soma", "cai_ais", "cai_prox", "cai_dist", "cai_spine"],
            "title": "Intracellular calcium by compartment",
            "ylabel": "cai (mM)",
        },
        "Total BK current": {
            "key": "bk_total",
            "requires": ["t", "bk_total_soma"],
            "title": "Total BK current",
            "ylabel": "Current density (mA/cm2)",
        },
        "BK components": {
            "key": "bk_components",
            "requires": ["t", "bk_Cav12_ik_soma", "bk_Cav21_ik_soma", "bk_Cav22_ik_soma"],
            "title": "BK component currents",
            "ylabel": "Current density (mA/cm2)",
        },
        "Cav currents": {
            "key": "cav_components",
            "requires": ["t", "cav12_ica_soma", "cav13_ica_soma", "cav21_ica_soma", "cav22_ica_soma"],
            "title": "Calcium source currents",
            "ylabel": "Current density (mA/cm2)",
        },
        "AP + total BK": {
            "key": "ap_bk_total",
            "requires": ["t", "vs", "bk_total_soma"],
            "title": "AP with total BK current",
            "ylabel_left": "Vm (mV)",
            "ylabel_right": "Current density (mA/cm2)",
        },
        "AP + cai": {
            "key": "ap_cai",
            "requires": ["t", "vs", "cai_soma"],
            "title": "AP with soma calcium",
            "ylabel_left": "Vm (mV)",
            "ylabel_right": "cai (mM)",
        },
        "Phase plane": {
            "key": "phase_plane",
            "requires": ["t", "vs"],
            "title": "Phase plane",
            "xlabel": "V (mV)",
            "ylabel": "dV/dt (mV/ms)",
        },
    },
    "VC": {
        "Clamp current": {
            "key": "clamp_current",
            "requires": ["t", "clamp_i"],
            "title": "Clamp current",
            "ylabel": "Current (nA)",
        },
        "Vm soma": {
            "key": "vm_soma",
            "requires": ["t", "vs"],
            "title": "Soma membrane potential",
            "ylabel": "Vm (mV)",
        },
        "Total Ca current": {
            "key": "ica_total",
            "requires": ["t", "ica_soma"],
            "title": "Total calcium current",
            "ylabel": "Current density (mA/cm2)",
        },
        "Total K current": {
            "key": "ik_total",
            "requires": ["t", "ik_soma"],
            "title": "Total potassium current",
            "ylabel": "Current density (mA/cm2)",
        },
        "BK components": {
            "key": "bk_components",
            "requires": ["t", "bk_Cav12_ik_soma", "bk_Cav21_ik_soma", "bk_Cav22_ik_soma"],
            "title": "BK component currents",
            "ylabel": "Current density (mA/cm2)",
        },
        "Cav currents": {
            "key": "cav_components",
            "requires": ["t", "cav12_ica_soma", "cav13_ica_soma", "cav21_ica_soma", "cav22_ica_soma"],
            "title": "Calcium source currents",
            "ylabel": "Current density (mA/cm2)",
        },
        "cai soma": {
            "key": "cai_soma",
            "requires": ["t", "cai_soma"],
            "title": "Soma intracellular calcium",
            "ylabel": "cai (mM)",
        },
        "Phase plane": {
            "key": "phase_plane",
            "requires": ["t", "vs"],
            "title": "Phase plane",
            "xlabel": "V (mV)",
            "ylabel": "dV/dt (mV/ms)",
        },
    },
}