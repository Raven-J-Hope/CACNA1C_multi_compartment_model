from cells import WTCell, Cav12_50Cell

CELL_CLASS_MAP = {
    "WTCell": WTCell,
    "Cav12_50Cell": Cav12_50Cell,
}


def iter_compartment_sections(cell, compartment_name):
    if compartment_name == "soma":
        return [cell.soma]
    if compartment_name == "ais":
        return [cell.ais]
    if compartment_name == "dend_prox":
        return [cell.dend_prox]
    if compartment_name == "dend_dist":
        return [cell.dend_dist]
    if compartment_name == "spine_head":
        return list(getattr(cell, "spines", []))
    if compartment_name == "spine_neck":
        return list(getattr(cell, "spine_necks", []))
    return []


def set_param_on_section(sec, mechanism_name, param_name, value):
    for seg in sec:
        mech = getattr(seg, mechanism_name, None)
        if mech is not None and hasattr(mech, param_name):
            setattr(mech, param_name, value)


def apply_overrides(cell, overrides):
    for compartment_name, params in overrides.items():
        sections = iter_compartment_sections(cell, compartment_name)
        for full_name, value in params.items():
            mechanism_name, param_name = full_name.split(".", 1)
            for sec in sections:
                set_param_on_section(sec, mechanism_name, param_name, value)


def build_cell_from_config(config):
    cell_class_name = config["preset"]["cell_class"]
    cell_cls = CELL_CLASS_MAP[cell_class_name]
    cell = cell_cls()

    preset_overrides = config["preset"].get("overrides", {})
    apply_overrides(cell, preset_overrides)

    user_overrides = config.get("parameter_overrides", {})
    apply_overrides(cell, user_overrides)

    return cell