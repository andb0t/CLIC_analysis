# CLIC analysis


## Purpose
This code is written to investigate the ee -> WW -> qqln process at 1.4 and 3 TeV

### Physics motivation
* important for EFT analyses
* most sensitive WW final state to extract the triple gauge couplings by measuring the WW charge from the leptons

### Analysis goals
* channel specific:
  * e: measure WW charge from leptons
  * &mu;: electron bremsstrahlung recovery

### Analysis strategy
* W reconstruction
  * at 1.4 TeV: reconstruct two jets
  * at 3 TeV: reconstruct one jet

## Usage
### Modifying ntuple_maker
Modify code in `ntuple_maker`, then
```shell
cd ntuple_maker
mkdir build && cd "$_"
cmakenewclic
make install  # creates *.so object in ../lib
```

### Processing the ntuples locally
```shell
cd grid
Marlin job_files/ntuple_maker.xml  # just ntuple_maker
Marlin job_files/full_ntuple_maker.xml  # entire chain
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
Chose the file ID for samples and download the file list:
```shell
python create_file_lists.py --ID 3249
```
Then (re-)submit the jobs either all at once or singular files by temporarily modifying this script:
```shell
python submit_marlin.py  # submit all in file_lists dir
```
Monitor jobs with the [Job Minitor](https://voilcdiracwebapp.cern.ch/DIRAC/?view=tabs&theme=Grey&url_state=1|*DIRAC.JobMonitor.classes.JobMonitor:,)

### Conversion to CSV
```shell
python convert_root_files.py --input 3249  # a single dataset
python convert_root_files.py --all  # all available datasets
```

### CSV analysis
Execution of `plot_data.py` with hydrogen or via
```shell
python plot_data.py
```


## Files
- Locate files on this [website](https://twiki.cern.ch/twiki/bin/view/CLIC/MonteCarloSamplesForTheHiggsPaper)
- Chosen files are listed in `grid/create_file_lists.py`


## Documentation
### References
- [Philipp's talk](https://indico.cern.ch/event/633975/contributions/2680919/attachments/1516016/2365932/clicdp_collaboration_meeting_august_2017_roloff.pdf)
- [Jiayin Gu's talk, referenced by Philipp](https://indico.cern.ch/event/633975/contributions/2689107/attachments/1514499/2363003/eex6.pdf)
- [Sascha Dreyer's talk about jets](https://indico.cern.ch/event/666225/attachments/1528721/2391477/summerstudentPres.pdf)

## Todo
* no DH, KT only R > 0.7
* use valencia (VLC) algorithm
	* see Sascha's talk 
	* chosen by Sacha: R = 0.8, β = 1.0 and γ = 0.7 (page 12) with R (jet size), β (clustering order) and γ (jet shrinking for forward jets)
* plot invariant masses for signal and bkg


