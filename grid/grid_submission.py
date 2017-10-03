#!/bin/env python
from __future__ import print_function

import argparse
import os
import os.path

from DIRAC.Core.Base import Script
Script.parseCommandLine()

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin


# =====================
# ===== STEERING ======
# =====================
INFILE = 'file_lists/qqll_samples.txt'
# =====================

MAX_N_FILES = -1
BATCH_SIZE = 200
SAVE_SLCIO = False
# for EOS
STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/'
STORAGE_USER_PATH = '/ilc/user/a/amaier/'
STORAGE_SE = 'CERN-DST-EOS'


def get_job_name(inFile):
    return 'output_' + os.path.basename(inFile.rstrip('.txt').rstrip('.slcio'))


def get_input_files(inFile):
    inputDataList = []

    if inFile.endswith('.slcio'):
        inputDataList = ['LFN:' + inFile]
    else:
        allFilesList = []
        with open(inFile) as inputDatFile:
            for index, line in enumerate(inputDatFile):
                line = line.strip()
                if line.startswith('#'):
                    continue
                if index > MAX_N_FILES and MAX_N_FILES != -1:
                    break
                allFilesList.append('LFN:' + line)

        for i in xrange(0, len(allFilesList), BATCH_SIZE):
            batch = allFilesList[i:i+BATCH_SIZE]
            inputDataList.append(batch)

        print('Total of ' + str(len(allFilesList)) +
              ' files! Splitting it in ' + str(len(inputDataList)) +
              ' batches with length ' + str(BATCH_SIZE))

    return inputDataList


replaceAll = False


def remove_file(path, file, dontPromptMe):
    global replaceAll
    fullpath = STORAGE_USER_PATH + path + '/' + file
    if replaceAll:
        os.system('dirac-dms-remove-files ' + fullpath)
        return False

    if not dontPromptMe:
	    print('Warning! ' + fullpath + ' exists! Delete this? y/[n]/all')
	    deleteIt = raw_input()
    else:
	    deleteIt = 'all'

    if deleteIt == 'y' or deleteIt == 'all':
        os.system('dirac-dms-remove-files ' + fullpath)
        if deleteIt == 'all':
            replaceAll = True
        return False
    else:
        print('Aborting!')
        return True


def check_file_existence(path, file, dontPromptMe):
    fullpath = STORAGE_BASE_PATH + STORAGE_USER_PATH + path + '/' + file
    if os.path.isfile(fullpath):
        return remove_file(path, file, dontPromptMe)
    else:
        return False


def create_job(inputData, saveName, dontPromptMe, inFile):

    outputPath = 'files/' + get_job_name(inFile)
    slcioFile = saveName + '.slcio'
    rootFile = saveName + '.root'

    if check_file_existence(outputPath, slcioFile, dontPromptMe):
        return True
    if check_file_existence(outputPath, rootFile, dontPromptMe):
        return True

    dIlc = DiracILC()

    job = UserJob()
    job.setOutputSandbox(['*.out', '*.log', '*.sh', '*.py', '*.xml'])
    if SAVE_SLCIO:
        job.setOutputData([slcioFile, rootFile], OutputPath=outputPath, OutputSE=STORAGE_SE)
    else:
        job.setOutputData(rootFile, OutputPath=outputPath, OutputSE=STORAGE_SE)
    job.setJobGroup('myMarlinRun1')
    job.setName('MyMarlinJob1')
    # job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk'])
    job.setInputSandbox(['LFN:/ilc/user/a/amaier/mylibs.tar.gz'])

    marl = Marlin()
    marl.setVersion('ILCSoft-2017-05-30_gcc62')

    marl.setInputFile(inputData)
    # marl.setInputFile(['LFN:/ilc/prod/clic/1.4tev/qq_ln/ILD/DST/00003249/010/qq_ln_dst_3249_10000.slcio'])
    marl.setSteeringFile('job_files/full_ntuple_maker.xml')
    marl.setOutputFile(slcioFile)
    marl.setGearFile('/afs/cern.ch/user/a/amaier/CLIC/grid/job_files/clic_ild_cdr.gear')
    marl.setExtraCLIArguments("--my_ntuple_maker.OutputFileName={rootOutfile}".format(rootOutfile=rootFile))
    marl.setNumberOfEvents(1000)

    job.append(marl)
    if dontPromptMe:
        job.dontPromptMe()
    job.submit(dIlc)

    return False


def submit_jobs(dontPromptMe, inFile):
    if not inFile:
        print('No file provided. Abort.')
        return
    print('Submitting jobs based on:', inFile)
    for index, inputData in enumerate(get_input_files(inFile)):
        if index == 1:
            if not dontPromptMe:
                print("Don't prompt and accept all following submissions? y/[n]")
                inp = raw_input()
            else:
                inp = 'y'
            if inp == 'y':
                dontPromptMe = True
        saveName = get_job_name(inFile) + '_batch_' + str(index)
        if create_job(inputData, saveName, dontPromptMe, inFile):
            break


def main():
    dontPromptMe = False
    submit_jobs(dontPromptMe, INFILE)


if __name__ == '__main__':
	main()