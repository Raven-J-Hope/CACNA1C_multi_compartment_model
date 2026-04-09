#!/usr/bin/env python3

from experiments import run_current_clamp, run_voltage_clamp


def main():
    print("Running current clamp experiment...")
    run_current_clamp()

    print("\nRunning voltage clamp experiment...")
    run_voltage_clamp()

    print("\nAll experiments finished.")


if __name__ == "__main__":
    main()