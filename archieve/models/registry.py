from cells import WTCell, Cav12_50Cell

MODEL_REGISTRY = {
    "rat_dggc_wt": WTCell,
    "rat_dggc_cav12_50": Cav12_50Cell,
}


def get_cell_class(model_key: str):
    if model_key not in MODEL_REGISTRY:
        raise ValueError(f"Unknown model_key: {model_key}")
    return MODEL_REGISTRY[model_key]