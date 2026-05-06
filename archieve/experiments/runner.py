from experiments.current_clamp import run_current_clamp
from experiments.voltage_clamp import run_voltage_clamp


def run_experiment(
    protocol_name,
    cell_class,
    protocol_params=None,
    save_report=False,
    report_name=None,
    make_basic_plots=False,
    plot_prefix=None,
):
    protocol_params = protocol_params or {}

    if protocol_name == "current_clamp":
        return run_current_clamp(
            cell_class=cell_class,
            save_report=save_report,
            report_name=report_name or "ic_run_report.json",
            make_basic_plots=make_basic_plots,
            plot_prefix=plot_prefix or "ic",
            **protocol_params,
        )

    if protocol_name == "voltage_clamp":
        return run_voltage_clamp(
            cell_class=cell_class,
            save_report=save_report,
            report_name=report_name or "vc_run_report.json",
            make_basic_plots=make_basic_plots,
            plot_prefix=plot_prefix or "vc",
            **protocol_params,
        )

    raise ValueError(f"Unknown protocol_name: {protocol_name}")