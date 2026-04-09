#Masters modelling project

This repository contains a multi-compartment dentate gyrus granule cell model built in Python using NEURON.

It includes:

- a **WT cell** model
- a **Cav1.2 50% cell** model derived from the WT cell
- **current clamp** experiments
- **voltage clamp** experiments
- a **run_all.py** script to run the full workflow

---

## This repository contains: 

- `cells/` containing the cell definitions
- `experiments/` containing the experiment scripts
- `Mod_Files/` containing the NEURON `.mod` mechanism files
- `outputs/` stores generated figures and JSON run reports
- `run_all.py` runs the experiments together


## Repository structure


Project_Folder/
`
├── cells/
│   ├── __init__.py
│   ├── wt_cell.py
│   └── cav12_50_cell.py
├── experiments/
│   ├── __init__.py
│   ├── current_clamp.py
│   └── voltage_clamp.py
├── Mod_Files/
│   ├── *.mod
│   └── x86_64/              #will be created after compilation on Linux/macOS
├── outputs/
│   ├── ic_figures/
│   ├── vc_figures/
│   └── *.json
├── run_all.py
└── README.md `


##Step 1: download the repository

**Option A:** download ZIP from GitHub
Open the GitHub repository page
Click Code
Click Download ZIP
Extract the ZIP to a folder on your computer

**Option B:** clone with git

Open a terminal and run:

git clone https://github.com/USERNAME/REPOSITORY.git
cd REPOSITORY

Replace:

USERNAME with the GitHub username
REPOSITORY with the repository name

## Step 2: check the folder structure

After downloading or cloning, the extracted folder should contain:

your_project_folder/
├── cells/
├── experiments/
├── Mod_Files/
├── run_all.py
└── README.md

It is important that

cells/
experiments/
Mod_Files/
run_all.py

must all stay together in the same top-level folder.

## Step 3: if you don't already have it, install Python

This project was developed with:

Python 3.12.2
NEURON 9.0.1

A similar recent Python 3 version should usually work, but matching the original version is safest.

## Step 4: install the required Python packages

**Required packages:**

numpy
matplotlib
neuron

Install them with pip.

Linux
pip install numpy matplotlib neuron
macOS
pip3 install numpy matplotlib neuron
Windows
pip install numpy matplotlib neuron

If you are using a virtual environment or conda environment, activate it first.

## Step 5: install NEURON

If NEURON is not already installed, install it via pip:

Linux
pip install neuron
macOS
pip3 install neuron
Windows
pip install neuron

Then test it:

Linux
python -c "from neuron import h; print(h.nrnversion())"
macOS
python3 -c "from neuron import h; print(h.nrnversion())"
Windows
python -c "from neuron import h; print(h.nrnversion())"


## Step 6: compile the .mod files

Before running the model, the mechanism files in Mod_Files/ must be compiled.

Run this from the top-level project folder.

**Linux**
cd Mod_Files
nrnivmodl


**macOS**
cd Mod_Files
nrnivmodl


**Windows**

Open NEURON Command Prompt, then run:

cd C:\path\to\your_project_folder\Mod_Files
mknrndll

After successful compilation:

on Linux/macOS you should see Mod_Files/x86_64/libnrnmech.so
on Windows NEURON will create the compiled mechanism library in its Windows-compatible form


## Step 7: make sure you run from the correct folder

Before running any script, move into the top-level project folder.

I.e.,

**Linux/macOS**
cd /path/to/your_project_folder

**Windows**
cd C:\path\to\your_project_folder

This matters because the code expects imports like cells and experiments to be found from the top level.

**Step 8:** run the full project

To run everything:

Linux
python run_all.py

macOS
python3 run_all.py

Windows
python run_all.py

This will run:

current clamp
voltage clamp

and write outputs into the outputs/ folder.

Run only the current clamp experiment with:

python -m experiments.current_clamp

Run only the voltage clamp experiment with:

python -m experiments.voltage_clamp

Using python -m ... is usually safer than running the file directly because it preserves package imports properly.

## Where the outputs go

When the scripts run successfully, outputs are written into:

Current clamp figures
outputs/ic_figures/

Voltage clamp figures
outputs/vc_figures/

Run reports
outputs/*.json

If those output folders do not exist yet, the scripts create them automatically.

