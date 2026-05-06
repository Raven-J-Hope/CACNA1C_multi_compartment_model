#!/usr/bin/env python3

from registry import get_cell_class
from experiments import run_current_clamp, run_voltage_clamp


def main():
    cell_type_key = "rat_dggc"

    print("Running WT current clamp...")
    run_current_clamp(cell_class=get_cell_class(cell_type_key, "wt"))

    print("Running 50% CaV1.2 current clamp...")
    run_current_clamp(cell_class=get_cell_class(cell_type_key, "cav12_50"))

    print("Running WT voltage clamp...")
    run_voltage_clamp(cell_class=get_cell_class(cell_type_key, "wt"))

    print("Running 50% CaV1.2 voltage clamp...")
    run_voltage_clamp(cell_class=get_cell_class(cell_type_key, "cav12_50"))


if __name__ == "__main__":
    main()