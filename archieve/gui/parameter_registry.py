from cells.wt_cell import DGGranuleLikeCell

ADVANCED_PARAMETER_SPEC = DGGranuleLikeCell.PARAMETER_SPEC

PRESET_REGISTRY = {
    "wt": {
        "display_name": "WT",
        "cell_class": "WTCell",
        "cell_type": "rat_dg_granule",
        "overrides": {},
    },
    "cav12_50": {
        "display_name": "Cav1.2 50%",
        "cell_class": "Cav12_50Cell",
        "cell_type": "rat_dg_granule",
        "overrides": {},
    },
}