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
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient

# =====================
# ===== STEERING ======
# =====================
INFILE = 'file_lists/qqll_samples.txt'
# =====================

MAX_N_FILES = -1
BATCH_SIZE = 50
SAVE_SLCIO = False
# for EOS
STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/'
STORAGE_USER_PATH = '/ilc/user/a/amaier/'
STORAGE_SE = 'CERN-DST-EOS'


def delete_file(fullPath):
    print('Deleting', fullPath, '...')
    os.system('dirac-dms-remove-files ' + fullPath)


def get_job_name(inFile):
    return 'output_' + os.path.basename(inFile.rstrip('.txt').rstrip('.slcio'))


def get_input_files(inFile, verbose=True):
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

        if verbose:
            print('Total of ' + str(len(allFilesList)) +
                  ' files! Splitting it in ' + str(len(inputDataList)) +
                  ' batches with length ' + str(BATCH_SIZE))

    return inputDataList


replaceAll = False


def remove_file(directory, file, dontPromptMe):
    global replaceAll
    fullpath = STORAGE_USER_PATH + directory + '/' + file
    if replaceAll:
        delete_file(fullpath)
        return
    if not dontPromptMe:
        print('Warning! ' + fullpath + ' exists! Delete this? y/[n]/all')
        deleteIt = raw_input()
    else:
        deleteIt = 'all'
    if deleteIt == 'y' or deleteIt == 'all':
        delete_file(fullpath)
        if deleteIt == 'all':
            replaceAll = True
    else:
        print('Aborting deletion!')


def check_file_existence(directory, file, dontPromptMe=False):
    myLFN = 'LFN:' + STORAGE_USER_PATH + directory + '/' + file
    fcc = FileCatalogClient()
    result = fcc.exists( myLFN )
    if myLFN in result.get('Value', {} ).get( 'Successful', {} ) and result.get('Value', {} ).get( 'Successful', {} )[myLFN]:
        return True
    return False


def create_job(inputData, saveName, outputDir, dontPromptMe):

    slcioFile = saveName + '.slcio'
    rootFile = saveName + '.root'

    if check_file_existence(outputDir, slcioFile, dontPromptMe):
        remove_file(outputDir, slcioFile, dontPromptMe)
    if check_file_existence(outputDir, rootFile, dontPromptMe):
        remove_file(outputDir, rootFile, dontPromptMe)

    dIlc = DiracILC()

    job = UserJob()
    job.setOutputSandbox(['*.out', '*.log', '*.sh', '*.py', '*.xml'])
    if SAVE_SLCIO:
        job.setOutputData([slcioFile, rootFile], OutputPath=outputDir, OutputSE=STORAGE_SE)
    else:
        job.setOutputData(rootFile, OutputPath=outputDir, OutputSE=STORAGE_SE)
    job.setJobGroup('myMarlinRun1')
    job.setName('MyMarlinJob1')
    # job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk'])
    job.setInputSandbox(['LFN:/ilc/user/a/amaier/mylibs.tar.gz'])

    marl = Marlin()
    marl.setVersion('ILCSoft-2017-05-30_gcc62')

    marl.setInputFile(inputData)
    job.setInputData(list(map(lambda x: x.lstrip('LFN:'), inputData)))
    # marl.setInputFile(['LFN:/ilc/prod/clic/1.4tev/qq_ln/ILD/DST/00003249/010/qq_ln_dst_3249_10000.slcio'])
    marl.setSteeringFile('marlin/FullChain.xml')
    marl.setOutputFile(slcioFile)
    gearFile = '/afs/cern.ch/user/a/amaier/projects/CLIC_analysis/grid/marlin/clic_ild_cdr.gear'
    if not os.path.isfile(gearFile):
        print('Error: gear file', gearFile, 'does not exist! Abort submission.')
        return
    marl.setGearFile(gearFile)
    marl.setExtraCLIArguments("--MyNtupleMaker.OutputFileName={rootOutfile}".format(rootOutfile=rootFile))
    # marl.setNumberOfEvents(1000)

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
        outputDir = 'files/' + get_job_name(inFile)
        print(len(inputData), 'data files for', saveName, 'to be saved in', outputDir)
        create_job(inputData, saveName, outputDir, dontPromptMe)


def check_job_completion(inFile, verbose=False):
    if not inFile:
        print('No file provided. Abort.')
        return
    # print('Checking job completion based on:', inFile)
    outputDir = 'files/' + get_job_name(inFile)
    # print('Checking dir:', outputDir)
    missingFiles = []
    fileCounter = 0
    inputFileList = get_input_files(inFile, verbose=False)
    for index, inputData in enumerate(inputFileList):
        filePath = STORAGE_BASE_PATH + STORAGE_USER_PATH + outputDir + '/' + get_job_name(inFile) + '_batch_' + str(index) + '.root'
        saveName =  get_job_name(inFile) + '_batch_' + str(index)
        if not os.path.isfile(filePath):
            missingFiles.append((inputData, saveName, outputDir))
            # print(saveName)
    dataID = inFile.rstrip('.txt').lstrip('file_lists/')
    filesPresent = float(len(inputFileList) - len(missingFiles))
    percentage = filesPresent / len(inputFileList) * 100
    if verbose:
        print('{2:>3.0f}% for {3}: {0:.0f}/{1} files present'.format(filesPresent, len(inputFileList), percentage, dataID))
    # for data, name, path in missingFiles:
    #     print('Missing this file', name)
    if missingFiles:
        return missingFiles
    else:
        return None


def resubmit_jobs(jobBundle, dontPromptMe):
    if not jobBundle:
        return
    print('Resubmitting those jobs:')
    for inputData, saveName, outputDir in jobBundle:
        print(len(inputData), 'data files for', saveName, 'to be saved in', outputDir)
        create_job(inputData, saveName, outputDir, dontPromptMe)


def main():
    dontPromptMe = False
    submit_jobs(dontPromptMe, INFILE)


if __name__ == '__main__':
    main()
