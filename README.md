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

#### W reconstruction
* at 1.4 TeV: reconstruct two jets
  * Valencia algorithm with R = 0.8 (jet size), β = 1.0 (clustering order) and γ = 0.7 (jet shrinking for forward jets), see [page 12](https://indico.cern.ch/event/666225/attachments/1528721/2391477/summerstudentPres.pdf)
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
Some valuable documentation:
* [EVENT::ReconstructedParticle class](http://lcio.desy.de/v02-09/doc/doxygen_api/html/classEVENT_1_1ReconstructedParticle.html).


### Processing the ntuples locally
```shell
cd grid
Marlin marlin/ntuple_maker.xml  # just ntuple_maker
Marlin marlin/full_ntuple_maker.xml  # entire chain
```
Then inspect the output with
```
root example_data/output.root
anajob example_data/output.slcio
dumpevent example_data/output.slcio 1 > example_data/dump.txt
```


### Processing the ntuples on the grid
Set up the proxy:
```shell
dirac_init  # sources dirac bashrc. Warning: has its own python version incompatible with PyRoot!
dirac_proxy
```
If you have changed any additional libraries in the meantime, upload libraries to EOS space to be used by grid execution:
```shell
cd grid
./upload_custom_libs.py
```
Chose the file ID for samples and download the file list:
```shell
./create_file_lists.py --ID 3249
```
Then (re-)submit the jobs either all at once or singular files by temporarily modifying this script:
```shell
./submit_marlin.py show  # show info about processed data files
./submit_marlin.py resubmit  # resubmit missing data files
./submit_marlin.py all  # submit all in file_lists dir
```
Monitor jobs with the [Job Minitor](https://voilcdiracwebapp.cern.ch/DIRAC/?view=tabs&theme=Grey&url_state=1|*DIRAC.JobMonitor.classes.JobMonitor:,)

To inspect the content of the EOS file storage and remove some files:
```
dirac-dms-remove-files /ilc/user/a/amaier/files/output_3246/output_3246_batch_35.root  # remove single files
./manage_root_files.py  # show available files
./manage_root_files.py --remove FILE_ID
```


### CSV analysis
Adapt `convert_root_files.py` to get the branches you'd like to save. Then
```shell
# set_python2  # resets the python path to python version with PyRoot
cd analysis
./convert_root_files.py  # show available files
./convert_root_files.py --input 3249  # a single dataset
./convert_root_files.py --all  # all available datasets
./convert_root_files.py --help  # for more options
```
Execution of `plot_data.py` with hydrogen or via
```shell
python3 plot_data.py
```


## Files
- Locate files on this [website](https://twiki.cern.ch/twiki/bin/view/CLIC/MonteCarloSamplesForTheHiggsPaper)
- Chosen files are listed in `grid/create_file_lists.py`


## Documentation

### LateX
The directory `doc` contains files to present the status of the analysis.

### References
- [Philipp's talk](https://indico.cern.ch/event/633975/contributions/2680919/attachments/1516016/2365932/clicdp_collaboration_meeting_august_2017_roloff.pdf)
- [Jiayin Gu's talk, referenced by Philipp](https://indico.cern.ch/event/633975/contributions/2689107/attachments/1514499/2363003/eex6.pdf)
- [Sascha Dreyer's talk about jets](https://indico.cern.ch/event/666225/attachments/1528721/2391477/summerstudentPres.pdf)
