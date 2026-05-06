EXPERIMENT_REGISTRY = {
    "current_clamp": {
        "label": "Current clamp",
        "runner": "experiments.current_clamp.run_current_clamp",
        "description": "Runs current-clamp protocols on the selected cell model.",
    },
    "voltage_clamp": {
        "label": "Voltage clamp",
        "runner": "experiments.voltage_clamp.run_voltage_clamp",
        "description": "Runs voltage-clamp protocols on the selected cell model.",
    },
}