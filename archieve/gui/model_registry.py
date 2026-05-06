#!/usr/bin/env python3

from cells import WTCell, Cav12_50Cell

MODEL_REGISTRY = {
    "Rat dentate gyrus granule cell": {
        "WT": WTCell,
        "Cav1.2 50%": Cav12_50Cell,
    },
}