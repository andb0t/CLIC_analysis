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
Set up the proxy:
```shell
dirac_init
dirac_proxy
```
If you have changed any additional libraries in the meantime, upload libraries to EOS space to be used by grid execution:
```shell
cd grid
python upload_custom_libs.py
```
Chose the file ID for samples and download the file list, then submit jobs:
```shell
dirac-dms-find-lfns ProdID=3249 | grep dst > file_lists/qq_ln_samples.txt
python submit_marlin.py file_lists/qq_ln_samples.txt
```
Monitor jobs with the [Job Minitor](https://voilcdiracwebapp.cern.ch/DIRAC/?view=tabs&theme=Grey&url_state=1|*DIRAC.JobMonitor.classes.JobMonitor:,)

### Conversion to CSV
```shell
cd analysis
python convert_root_files.py qq_ln_samples
```

### CSV analysis
Execution of `plot_data.py` with hydrogen or via
```shell
python plot_data.py
```


## Files
- Locate files on this [website](https://twiki.cern.ch/twiki/bin/view/CLIC/MonteCarloSamplesForTheHiggsPaper)
### 1.4 TeV

#### Signal
* ee->qqlv: 3249

#### Background
* ee -> qqqqll: 
    * 2166
    * 5572
* ee -> qqll:
    * 2301
    * 2351
    * 2645
    * 3246
    * 4266
    * 4269
    * 4272
    * 4275
* ee -> hvv, h->ZZ : 2717
* h->ZZ: ?


## Documentation
### References
- [Philipp's talk](https://indico.cern.ch/event/633975/contributions/2680919/attachments/1516016/2365932/clicdp_collaboration_meeting_august_2017_roloff.pdf)
- [Jiayin Gu's talk, referenced by Philipp](https://indico.cern.ch/event/633975/contributions/2689107/attachments/1514499/2363003/eex6.pdf)
