import os
import matplotlib.pyplot as plt


def savefig(fig_dir, name: str):
    plt.savefig(os.path.join(fig_dir, name), dpi=300, bbox_inches="tight")


def plot_ic_basic(data, fig_dir, plot_prefix="ic"):
    #soma Vm
    plt.figure()
    plt.plot(data["t"], data["vs"])
    plt.xlabel("Time (ms)")
    plt.ylabel("Membrane potential (mV)")
    plt.tight_layout()
    savefig(fig_dir, f"{plot_prefix}_vm_soma.png")
    plt.show()

    #soma cai
    if data.get("cai_soma") is not None:
        plt.figure()
        plt.plot(data["t"], data["cai_soma"])
        plt.xlabel("Time (ms)")
        plt.ylabel("cai (mM)")
        plt.tight_layout()
        savefig(fig_dir, f"{plot_prefix}_cai_soma.png")
        plt.show()

    #total BK
    if data.get("bk_total_soma") is not None:
        plt.figure()
        plt.plot(data["t"], data["bk_total_soma"])
        plt.xlabel("Time (ms)")
        plt.ylabel("Total BK current density (mA/cm2)")
        plt.tight_layout()
        savefig(fig_dir, f"{plot_prefix}_bk_total.png")
        plt.show()