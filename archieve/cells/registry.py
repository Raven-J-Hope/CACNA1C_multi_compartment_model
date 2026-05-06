#!/usr/bin/env python3

from .wt_cell import WTCell
from .cav12_50_cell import Cav12_50Cell

CELL_REGISTRY = {
    "rat_dggc": {
        "label": "Rat dentate gyrus granule cell",
        "variants": {
            "wt": {
                "label": "WT",
                "class": WTCell,
                "description": "Wild-type rat dentate gyrus granule cell model.",
            },
            "cav12_50": {
                "label": "50% Cav1.2",
                "class": Cav12_50Cell,
                "description": "Rat dentate gyrus granule cell with 50% Cav1.2 conductance and altered BK split.",
            },
        },
    },
}


def get_cell_type_keys():
    return list(CELL_REGISTRY.keys())


def get_cell_type_label(cell_type_key: str):
    return CELL_REGISTRY[cell_type_key]["label"]


def get_variant_keys(cell_type_key: str):
    return list(CELL_REGISTRY[cell_type_key]["variants"].keys())


def get_variant_label(cell_type_key: str, variant_key: str):
    return CELL_REGISTRY[cell_type_key]["variants"][variant_key]["label"]


def get_cell_class(cell_type_key: str, variant_key: str):
    return CELL_REGISTRY[cell_type_key]["variants"][variant_key]["class"]


def get_variant_description(cell_type_key: str, variant_key: str):
    return CELL_REGISTRY[cell_type_key]["variants"][variant_key].get("description", "")