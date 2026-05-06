from models import get_cell_class
from experiments import run_experiment


def run_protocol(config: dict):
    cell_key = config["model_key"]
    protocol = config["protocol"]

    protocol_params = {
        "dt": config.get("dt", 0.025),
        "tstop": config.get("tstop", 500.0),
        "v_init": config.get("v_init", -70.0),
        "channel_overrides": config.get("channel_overrides", {}),
    }

    if protocol == "current_clamp":
        protocol_params.update({
            "delay": config.get("delay", 100.0),
            "dur": config.get("dur", 300.0),
            "amp": config.get("amp", 0.3),
        })
    elif protocol == "voltage_clamp":
        protocol_params.update({
            "hold": config.get("hold", -70.0),
            "step": config.get("step", -50.0),
            "delay": config.get("delay", 100.0),
            "dur": config.get("dur", 300.0),
        })
    else:
        raise ValueError(f"Unknown protocol: {protocol}")

    cell_class = get_cell_class(cell_key)

    return run_experiment(
        protocol_name=protocol,
        cell_class=cell_class,
        protocol_params=protocol_params,
        save_report=config.get("save_json", False),
        report_name=config.get("report_name"),
        make_basic_plots=config.get("make_basic_plots", False),
        plot_prefix=config.get("plot_prefix"),
    )