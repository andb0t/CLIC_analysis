# CLIC analysis


## Purpose
This code is written to investigate the ee -> WW -> qqln process at 1.5 and 3 TeV

### Physics motivation
- important for EFT analyses
- most sensitive WW final state to extract the triple gauge couplings by measuring the WW charge from the leptons

### Analysis goals
- measure WW charge from leptons


## Usage
### Modifying ntuple_maker
Modify code in `ntuple_maker`, then
```shell
cd ntuple_maker
mkdir build && cd "$_"
cmake ..
make install  # shoud create *.so object in ../lib
```

### Processing the ntuples locally
```shell
cd grid
# Marlin ...
```

### Processing the ntuples on the grid
After modification and compilation of ntuple_maker, upload libraries to EOS space to be used by grid execution:
```shell
cd grid
python upload_custom_libs.py
```
Then submit jobs:
```shell
python submit_marlin.py
```

### Conversion to CSV
```shell
cd analysis
python convert_root_files.py
```

### CSV analysis
Execution of `plot_data.py` with hydrogen or via
```shell
python plot_data.py
```


## Documentation
### References
- [Philipp's talk](https://indico.cern.ch/event/633975/contributions/2680919/attachments/1516016/2365932/clicdp_collaboration_meeting_august_2017_roloff.pdf)
- [Jiayin Gu's talk, referenced by Philipp](https://indico.cern.ch/event/633975/contributions/2689107/attachments/1514499/2363003/eex6.pdf)
