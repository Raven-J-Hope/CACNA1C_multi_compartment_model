import json
import sys

from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QApplication,
    QCheckBox,
    QComboBox,
    QDoubleSpinBox,
    QFormLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QMainWindow,
    QMessageBox,
    QPushButton,
    QPlainTextEdit,
    QSizePolicy,
    QTabWidget,
    QVBoxLayout,
    QWidget,
)

from .protocol_runner import run_protocol
from .parameter_registry import ADVANCED_PARAMETER_SPEC


CELL_TYPE_OPTIONS = {
    "Rat dentate gyrus granule cell": {
        "WT": "rat_dggc_wt",
        "50% CaV1.2": "rat_dggc_cav12_50",
    }
}


class NeuronModelGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Neuron Model GUI")
        self.resize(1200, 850)

        self.advanced_inputs = {}

        self.central = QWidget()
        self.setCentralWidget(self.central)

        self.main_layout = QVBoxLayout(self.central)

        self._build_top_row()
        self._build_protocol_row()
        self._build_common_group()
        self._build_ic_group()
        self._build_vc_group()
        self._build_advanced_group()
        self._build_buttons()
        self._build_output()

        self._refresh_dynamics()
        self._toggle_protocol_groups()
        self._toggle_advanced_panel()

    def _build_top_row(self):
        row = QHBoxLayout()

        self.cell_type_label = QLabel("Cell type")
        self.cell_type_combo = QComboBox()
        self.cell_type_combo.addItems(CELL_TYPE_OPTIONS.keys())
        self.cell_type_combo.currentTextChanged.connect(self._refresh_dynamics)

        self.cell_dynamics_label = QLabel("Cell dynamics")
        self.cell_dynamics_combo = QComboBox()

        self.advanced_checkbox = QCheckBox("Show advanced channel settings")
        self.advanced_checkbox.setChecked(False)
        self.advanced_checkbox.toggled.connect(self._toggle_advanced_panel)

        row.addWidget(self.cell_type_label)
        row.addWidget(self.cell_type_combo, 1)
        row.addWidget(self.cell_dynamics_label)
        row.addWidget(self.cell_dynamics_combo, 1)
        row.addWidget(self.advanced_checkbox)

        self.main_layout.addLayout(row)

    def _build_protocol_row(self):
        self.protocol_label = QLabel("Experimental protocol")
        self.protocol_combo = QComboBox()
        self.protocol_combo.addItems(["IC", "VC"])
        self.protocol_combo.currentTextChanged.connect(self._toggle_protocol_groups)

        self.main_layout.addWidget(self.protocol_label)
        self.main_layout.addWidget(self.protocol_combo)

    def _build_common_group(self):
        self.common_group = QGroupBox("Common settings")
        layout = QFormLayout()

        self.dt_spin = self._make_spinbox(0.0001, 1000.0, 0.025, 4)
        self.tstop_spin = self._make_spinbox(0.1, 100000.0, 500.0, 3)
        self.v_init_spin = self._make_spinbox(-200.0, 200.0, -70.0, 3)

        layout.addRow("dt (ms)", self.dt_spin)
        layout.addRow("tstop (ms)", self.tstop_spin)
        layout.addRow("v_init (mV)", self.v_init_spin)

        self.common_group.setLayout(layout)
        self.main_layout.addWidget(self.common_group)

    def _build_ic_group(self):
        self.ic_group = QGroupBox("Current clamp settings")
        layout = QFormLayout()

        self.ic_delay_spin = self._make_spinbox(0.0, 100000.0, 100.0, 3)
        self.ic_dur_spin = self._make_spinbox(0.0, 100000.0, 300.0, 3)
        self.ic_amp_spin = self._make_spinbox(-1000.0, 1000.0, 0.3, 4)

        layout.addRow("delay (ms)", self.ic_delay_spin)
        layout.addRow("duration (ms)", self.ic_dur_spin)
        layout.addRow("amplitude (nA)", self.ic_amp_spin)

        self.ic_group.setLayout(layout)
        self.main_layout.addWidget(self.ic_group)

    def _build_vc_group(self):
        self.vc_group = QGroupBox("Voltage clamp settings")
        layout = QFormLayout()

        self.vc_hold_spin = self._make_spinbox(-200.0, 200.0, -70.0, 3)
        self.vc_step_spin = self._make_spinbox(-200.0, 200.0, -50.0, 3)
        self.vc_delay_spin = self._make_spinbox(0.0, 100000.0, 100.0, 3)
        self.vc_dur_spin = self._make_spinbox(0.0, 100000.0, 300.0, 3)

        layout.addRow("hold (mV)", self.vc_hold_spin)
        layout.addRow("step (mV)", self.vc_step_spin)
        layout.addRow("delay (ms)", self.vc_delay_spin)
        layout.addRow("duration (ms)", self.vc_dur_spin)

        self.vc_group.setLayout(layout)
        self.main_layout.addWidget(self.vc_group)

    def _build_advanced_group(self):
        self.advanced_group = QGroupBox("Advanced channel settings")
        outer_layout = QVBoxLayout()

        self.advanced_tabs = QTabWidget()
        self._populate_advanced_tabs()

        outer_layout.addWidget(self.advanced_tabs)
        self.advanced_group.setLayout(outer_layout)

        self.main_layout.addWidget(self.advanced_group)

    def _populate_advanced_tabs(self):
        self.advanced_inputs = {}
        self.advanced_tabs.clear()

        for compartment_name, param_map in ADVANCED_PARAMETER_SPEC.items():
            tab = QWidget()
            form = QFormLayout(tab)

            self.advanced_inputs[compartment_name] = {}

            for param_name, default_value in param_map.items():
                spin = self._make_adaptive_spinbox(default_value)
                self.advanced_inputs[compartment_name][param_name] = spin
                form.addRow(param_name, spin)

            self.advanced_tabs.addTab(tab, compartment_name)

        # BK split tab
        self.bk_split_tab = QWidget()
        bk_form = QFormLayout(self.bk_split_tab)

        self.bk_split_inputs = {}

        self.bk_split_inputs["BK_Cav12"] = self._make_fraction_spinbox(1.0 / 3.0)
        self.bk_split_inputs["BK_Cav21"] = self._make_fraction_spinbox(1.0 / 3.0)
        self.bk_split_inputs["BK_Cav22"] = self._make_fraction_spinbox(1.0 / 3.0)

        bk_form.addRow("BK_Cav12", self.bk_split_inputs["BK_Cav12"])
        bk_form.addRow("BK_Cav21", self.bk_split_inputs["BK_Cav21"])
        bk_form.addRow("BK_Cav22", self.bk_split_inputs["BK_Cav22"])

        self.advanced_tabs.addTab(self.bk_split_tab, "BK_split")

    def _build_buttons(self):
        row = QHBoxLayout()

        self.run_button = QPushButton("Run protocol")
        self.run_button.clicked.connect(self._run_protocol)

        self.show_config_button = QPushButton("Show current config")
        self.show_config_button.clicked.connect(self._show_config)

        row.addWidget(self.run_button)
        row.addWidget(self.show_config_button)

        self.main_layout.addLayout(row)

    def _build_output(self):
        self.output_box = QPlainTextEdit()
        self.output_box.setReadOnly(True)
        self.output_box.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.main_layout.addWidget(self.output_box)

    def _make_spinbox(self, min_val, max_val, value, decimals):
        box = QDoubleSpinBox()
        box.setRange(min_val, max_val)
        box.setDecimals(decimals)
        box.setValue(value)
        box.setSingleStep(0.1)
        box.setAlignment(Qt.AlignLeft)
        return box

    def _make_adaptive_spinbox(self, value):
        box = QDoubleSpinBox()
        box.setDecimals(10)
        box.setRange(-1e9, 1e9)
        box.setValue(float(value))
        box.setSingleStep(max(abs(float(value)) / 10.0, 1e-9))
        box.setAlignment(Qt.AlignLeft)
        return box

    def _make_fraction_spinbox(self, value):
        box = QDoubleSpinBox()
        box.setDecimals(6)
        box.setRange(0.0, 1.0)
        box.setValue(float(value))
        box.setSingleStep(0.01)
        box.setAlignment(Qt.AlignLeft)
        return box

    def _refresh_dynamics(self):
        cell_type = self.cell_type_combo.currentText()
        current = self.cell_dynamics_combo.currentText()

        self.cell_dynamics_combo.blockSignals(True)
        self.cell_dynamics_combo.clear()
        self.cell_dynamics_combo.addItems(CELL_TYPE_OPTIONS[cell_type].keys())

        if current in CELL_TYPE_OPTIONS[cell_type]:
            self.cell_dynamics_combo.setCurrentText(current)
        else:
            self.cell_dynamics_combo.setCurrentIndex(0)

        self.cell_dynamics_combo.blockSignals(False)

        # update BK split defaults to match selected dynamics
        dynamics = self.cell_dynamics_combo.currentText()
        if dynamics == "WT":
            self.bk_split_inputs["BK_Cav12"].setValue(1.0 / 3.0)
            self.bk_split_inputs["BK_Cav21"].setValue(1.0 / 3.0)
            self.bk_split_inputs["BK_Cav22"].setValue(1.0 / 3.0)
        elif dynamics == "50% CaV1.2":
            self.bk_split_inputs["BK_Cav12"].setValue(1.0 / 6.0)
            self.bk_split_inputs["BK_Cav21"].setValue(5.0 / 12.0)
            self.bk_split_inputs["BK_Cav22"].setValue(5.0 / 12.0)

    def _toggle_protocol_groups(self):
        protocol = self.protocol_combo.currentText()
        self.ic_group.setVisible(protocol == "IC")
        self.vc_group.setVisible(protocol == "VC")

    def _toggle_advanced_panel(self):
        self.advanced_group.setVisible(self.advanced_checkbox.isChecked())

    def _collect_channel_overrides(self):
        if not self.advanced_checkbox.isChecked():
            return {}

        overrides = {}
        for compartment_name, widgets in self.advanced_inputs.items():
            overrides[compartment_name] = {}
            for param_name, widget in widgets.items():
                overrides[compartment_name][param_name] = widget.value()

        return overrides

    def _collect_bk_split_override(self):
        if not self.advanced_checkbox.isChecked():
            return None

        bk_split = {
            "BK_Cav12": self.bk_split_inputs["BK_Cav12"].value(),
            "BK_Cav21": self.bk_split_inputs["BK_Cav21"].value(),
            "BK_Cav22": self.bk_split_inputs["BK_Cav22"].value(),
        }

        total = sum(bk_split.values())
        if abs(total - 1.0) > 1e-6:
            raise ValueError(f"BK split must sum to 1.0, got {total:.6f}")

        return bk_split

    def _collect_config(self):
        cell_type = self.cell_type_combo.currentText()
        dynamics = self.cell_dynamics_combo.currentText()
        model_key = CELL_TYPE_OPTIONS[cell_type][dynamics]

        config = {
            "model_key": model_key,
            "protocol": "current_clamp" if self.protocol_combo.currentText() == "IC" else "voltage_clamp",
            "dt": self.dt_spin.value(),
            "tstop": self.tstop_spin.value(),
            "v_init": self.v_init_spin.value(),
            "make_basic_plots": True,
            "save_json": True,
            "channel_overrides": self._collect_channel_overrides(),
            "bk_split_override": self._collect_bk_split_override(),
        }

        if config["protocol"] == "current_clamp":
            config.update({
                "delay": self.ic_delay_spin.value(),
                "dur": self.ic_dur_spin.value(),
                "amp": self.ic_amp_spin.value(),
            })
        else:
            config.update({
                "hold": self.vc_hold_spin.value(),
                "step": self.vc_step_spin.value(),
                "delay": self.vc_delay_spin.value(),
                "dur": self.vc_dur_spin.value(),
            })

        return config

    def _show_config(self):
        config = self._collect_config()
        self.output_box.setPlainText(json.dumps(config, indent=2))

    def _run_protocol(self):
        try:
            config = self._collect_config()
            self.output_box.setPlainText("Running...\n")
            QApplication.processEvents()

            result = run_protocol(config)
            self.output_box.setPlainText(json.dumps(result, indent=2, default=str))

        except Exception as e:
            QMessageBox.critical(self, "Run error", str(e))


def launch_gui():
    app = QApplication.instance()
    owns_app = app is None

    if owns_app:
        app = QApplication(sys.argv)

    window = NeuronModelGUI()
    window.show()

    if owns_app:
        sys.exit(app.exec())