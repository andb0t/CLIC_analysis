#!/bin/env python
import os
import os.path

from DIRAC.Core.Base import Script
Script.parseCommandLine()

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin

MAX_N_FILES = 6
BATCH_SIZE = 2
FILE_COLLECTION = 'qq_ln_samples.txt'
ONLY_THIS_FILE = ''
JOB_NAME = 'output'

# for EOS
STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/'
STORAGE_USER_PATH = '/ilc/user/a/amaier/'
STORAGE_SE = 'CERN-DST-EOS'

def get_input_files():

	allFilesList = []
	with open(FILE_COLLECTION) as inputDatFile:
		for index, line in enumerate(inputDatFile):
			line = line.strip()
			if line.startswith('#'): 
				continue
			if index > MAX_N_FILES and MAX_N_FILES != -1:
				break
			allFilesList.append('LFN:' + line)


	inputDataList = []
	for i in xrange(0, len(allFilesList), BATCH_SIZE):
	    batch = allFilesList[i:i+BATCH_SIZE]
	    inputDataList.append(batch)

	if ONLY_THIS_FILE:
		inputDataList.push_back([ONLY_THIS_FILE])

	print('Total of ' + str(len(allFilesList)) + ' files! Splitting it in ' + str(len(inputDataList)) + ' batches with length ' + str(BATCH_SIZE) )

	return inputDataList


def remove_file(path, file):
	global replaceAll
	fullpath = STORAGE_USER_PATH + path + '/' + file
	if replaceAll:
		os.system('dirac-dms-remove-files ' +fullpath )
		return False
		
	print('Warning! ', fullpath, 'exists! Delete this? y/[n]/all')
	deleteIt = raw_input()

	if deleteIt == 'y' or deleteIt == 'all':
		os.system('dirac-dms-remove-files ' +fullpath )
		if deleteIt == 'all':
			replaceAll = True
		return False
	else:
		print('Aborting!')
		return True


def check_file_existence(path, file):
	fullpath = STORAGE_BASE_PATH + STORAGE_USER_PATH + path + '/' + file
	if os.path.isfile(fullpath):
		return remove_file(path, file)
	else:
		return False


def create_job(inputData, savename, dontPromptMe):

	outputPath = 'files/'+JOB_NAME
	slcioFile = savename + '.slcio'
	rootFile = savename + '.root'

	if check_file_existence(outputPath, slcioFile): return True
	if check_file_existence(outputPath, rootFile): return True

	dIlc = DiracILC()

	job = UserJob()
	job.setOutputSandbox(['*.out', '*.log', '*.sh', '*.py', '*.xml'])
	job.setOutputData([slcioFile, rootFile], OutputPath=outputPath, OutputSE=STORAGE_SE)
	job.setJobGroup( 'myMarlinRun1' )
	job.setName( 'MyMarlinJob1' )
	# job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk'])
	job.setInputSandbox( ['LFN:/ilc/user/a/amaier/mylibs.tar.gz'] )

	marl = Marlin ()
	marl.setVersion('ILCSoft-2017-05-30_gcc62')

	marl.setInputFile(inputData)
	# marl.setInputFile(['LFN:/ilc/prod/clic/1.4tev/qq_ln/ILD/DST/00003249/010/qq_ln_dst_3249_10000.slcio'])
	marl.setSteeringFile('full_ntuple_maker.xml')
	marl.setOutputFile(slcioFile)
	marl.setGearFile('/afs/cern.ch/user/a/amaier/CLIC/grid/clic_ild_cdr.gear')
	marl.setExtraCLIArguments("--my_ntuple_maker.OutputFileName={rootOutfile}".format(rootOutfile = rootFile))
	marl.setNumberOfEvents(1000)

	job.append(marl)
	if dontPromptMe: 
		job.dontPromptMe()
	job.submit(dIlc)

	return False


replaceAll = False
dontPromptMe = False
for index, inputData in enumerate(get_input_files()):
	if index == 1:
		print("Don't prompt and accept all following submissions? y/[n]")
		inp =  raw_input()
		if inp == 'y':
			dontPromptMe = True 
	savename = JOB_NAME + '_batch_' + str(index)
	if create_job(inputData, savename, dontPromptMe):
		break


