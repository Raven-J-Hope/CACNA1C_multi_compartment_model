import os
import matplotlib.pyplot as plt


def savefig(fig_dir, name: str):
    plt.savefig(os.path.join(fig_dir, name), dpi=300, bbox_inches="tight")


def plot_vc_basic(data, fig_dir, plot_prefix="vc"):
    #clamp current
    if data.get("clamp_i") is not None:
        plt.figure()
        plt.plot(data["t"], data["clamp_i"])
        plt.xlabel("Time (ms)")
        plt.ylabel("Clamp current (nA)")
        plt.tight_layout()
        savefig(fig_dir, f"{plot_prefix}_clamp_current.png")
        plt.show()

    #soma Vm
    plt.figure()
    plt.plot(data["t"], data["vs"])
    plt.xlabel("Time (ms)")
    plt.ylabel("Vm (mV)")
    plt.tight_layout()
    savefig(fig_dir, f"{plot_prefix}_soma_vm.png")
    plt.show()

    #total Ca current
    if data.get("ica_soma") is not None:
        plt.figure()
        plt.plot(data["t"], data["ica_soma"])
        plt.xlabel("Time (ms)")
        plt.ylabel("Total calcium current (mA/cm2)")
        plt.tight_layout()
        savefig(fig_dir, f"{plot_prefix}_total_ca_current.png")
        plt.show()