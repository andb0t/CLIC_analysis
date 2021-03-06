# CLIC analysis

[![Build Status](https://gitlab.cern.ch/amaier/CLIC_analysis/badges/master/build.svg)](https://gitlab.cern.ch/amaier/CLIC_analysis/commits/master)

## Purpose
This code is written to investigate the ee -> WW -> qqln process at 1.4 and 3 TeV at CLIC ILD


### Physics motivation
* important for EFT analyses
* most sensitive WW final state to extract the triple gauge couplings by measuring the WW charge from the leptons


### Analysis goals
* channel specific:
  * &mu;: measure WW charge from leptons
  * e: electron bremsstrahlung recovery


### Analysis strategy

#### W reconstruction
* at 1.4 TeV: reconstruct two jets
  * Valencia algorithm with R = 0.8 (jet size), β = 1.0 (clustering order) and γ = 0.7 (jet shrinking for forward jets), see [page 12](https://indico.cern.ch/event/666225/attachments/1528721/2391477/summerstudentPres.pdf)
* at 3 TeV: reconstruct one jet
* reconstruct total cross-section close to S` maximum
* triple gauge couplings in the WW process can be fully reconstructed from those five observables:
    - production angle &theta<sub>W</sub>
    - polar angle of the leptonically decaying W boson &theta<sub>L</sub>
    - polar angle of the hadronically decaying W boson &theta<sub>H</sub>
    - azimuthal of the leptonically decaying W boson angle &phi<sub>L</sub>
    - azimuthal of the hadronically decaying W boson angle &phi<sub>H</sub>
* analyze MC samples with different coupling settings and fit the observed angle distributions to them



## Usage


### Modifying processors
Modify code in processors `NtupleMaker`, then
```shell
clic_init
cd processors/[PROCESSOR_NAME]
mkdir build && cd "$_"
cmakenewclic
make install  # creates *.so object in ../lib
make format  # calls clang-format -i -style=file on relevant files
```
Make sure the `*.so` is contained in the environmental variable MARLIN_DLL to be picked up.
This variable does not tolerate duplicate entries.

To modify existing processors contained in compiled shared objects a complete custom recompilation is necessary.
For this, remove the corresponding path from MARLIN_DLL and add your own one.

Some valuable documentation:
* [LCIO: Class List](http://lcio.desy.de/v02-09/doc/doxygen_api/html/annotated.html)
* [EVENT::ReconstructedParticle](http://lcio.desy.de/v02-09/doc/doxygen_api/html/classEVENT_1_1ReconstructedParticle.html)


### Processing the ntuples locally
```shell
clic_init  # add custom processors to MARLIN_DLL
cd grid
Marlin marlin/NtupleMaker.xml  # just NtupleMaker
Marlin marlin/FullChain.xml  # entire chain
```
Then inspect the output with
```
root example_data/output.root
anajob example_data/output.slcio
dumpevent example_data/output.slcio 1 | less
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
Chose the file ID for samples, download the file list and inspect one file:
```shell
./create_file_lists.py --ID 3249
dirac-dms-get-file [FILE_PATH]  # to download a single file from a file list
```
Then (re-)submit the jobs either all at once or singular files by temporarily modifying the script:
```shell
./submit_marlin.py show  # show info about processed data files
./submit_marlin.py resubmit  # resubmit missing data files
./submit_marlin.py test  # submit some test files
./submit_marlin.py all  # submit all in file_lists dir
```
Monitor jobs with the [Job Monitor](https://voilcdiracwebapp.cern.ch/DIRAC/?view=tabs&theme=Grey&url_state=1|*DIRAC.JobMonitor.classes.JobMonitor:,)

To download a job for error inspection:
```shell
dirac-wms-job-get-output [JOB_ID]  # JOB_ID e.g. 29417635
```

To inspect the content of the EOS file storage and remove some files:
```
dirac-dms-remove-files /ilc/user/a/amaier/files/output_3246/output_3246_batch_35.root  # remove single files
./manage_root_files.py  # show available files
./manage_root_files.py --remove FILE_ID  # remove all files of a given FILE_ID
```


### CSV analysis
Adapt `convert_root_files.py` to get the branches you'd like to save. Then
```shell
# set_python2  # resets the python path to python version with PyRoot
cd analysis
./convert_root_files.py  # show available files
./convert_root_files.py --input 3249  # a single dataset
./convert_root_files.py --all  # all available datasets
./convert_root_files.py --onlynew  # only data files which have no corresponding csv file already
```
If wanted, update the list of example data after in-script specification of samples with
```shell
./example_data/update_example_data.sh
```
Execution of `plot_data.py` with hydrogen or via
```shell
python3 plot_data.py
```
The required python3 packages are listed in `Pipfile`. In case you use pipenv, simply execute `pipenv install`. For conversion of `.dot` files to `.png` the `graphviz` package's `dot` command has to be available.



## CI
The gitlab CI automatically performs the analysis and compiles the documentation. For this, it needs access to the AFS file system, where the data are stored. `KRB_USERNAME` and `KRB_PASSWORD` for a service account are saved as secret gitlab variables. The service account needs AFS access to the folder with the data and all parent folders. This can be granted by
```shell
fs setacl -dir [DIRNAME] -acl [SERVICE_ACCOUNT_NAME] read  # to set
fs listacl [DIRNAME]  # to verify
```


## Files
- Locate files on this [website](https://twiki.cern.ch/twiki/bin/view/CLIC/MonteCarloSamplesForTheHiggsPaper)
  - with beam recoil
- Chosen files are listed in `grid/create_file_lists.py`
- for files in `example_data` an increase of the git HTTP buffer is necessary via `git config http.postBuffer 52428800`
- get basic info on file content via `dirac-ilc-production-summary --prods=3249`

## Documentation

### LateX
The directory `doc` contains files to present the status of the analysis.

The directory `doc/qqln` contains the note concering the analysis. The template has been taken from the [repository for CLICdp LaTeX templates and plot styles](https://gitlab.cern.ch/CLICdp/Publications/Templates/Style), using the `createNewCLICdpNote.sh -p [PATH] [PROJECT_NAME]` script.

### References
- [Philipp's talk](https://indico.cern.ch/event/633975/contributions/2680919/attachments/1516016/2366826/clicdp_collaboration_meeting_august_2017_roloff.pdf)
- [Jiayin Gu's talk, referenced by Philipp](https://indico.cern.ch/event/633975/contributions/2689107/attachments/1514499/2363003/eex6.pdf)
- [Sascha Dreyer's talk about jets](https://indico.cern.ch/event/666225/attachments/1528721/2391477/summerstudentPres.pdf)
- [Rickard Strom's talk about s prime reconstruction](https://indico.cern.ch/event/667391/contributions/2747948/attachments/1538122/2411311/update_boosted_top_clicdp_analysis_meeting_20171010.pdf)
- [iLCsoft clang-format file](https://github.com/iLCSoft/ConformalTracking/blob/master/.clang-format)
- [iLCsoft coding conventions](http://ilcsoft.desy.de/portal/e279/e281/infoboxContent282/sw_guidelines_ilcsoft.pdf) and [a project supposedly following them](https://github.com/iLCSoft/LCIO)
- [ILC paper on triple gauge coupling extraction](https://www.sciencedirect.com/science/article/pii/S2405601415008512?via%3Dihub)
- [ILC dissertation on triple gauge couplings](http://www-library.desy.de/preparch/desy/thesis/desy-thesis-11-044.pdf)
- [Nice read on W decays, cross-sections and angles](http://www.hep.ucl.ac.uk/~jpc/all/ulthesis/node45.html)
