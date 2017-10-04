#!/bin/env python
from __future__ import print_function

import os

import grid_submission

# =====================
# ===== STEERING ======
# =====================
ACTIVE = False
RESUBMIT_MISSING = True
SUBMIT_ALL = False
ONLY_THOSE = []
# NOT_THOSE = ['3249.txt', '2166.txt', '5572.txt', '3246.txt']
NOT_THOSE = []
# =====================


DATA_DIR = 'file_lists'
BASE_CMD = 'python submit_marlin.py'

if not ACTIVE:
	print("Suppress submission. Enable the 'ACTIVE' setting to submit the jobs!")

for dataFile in os.listdir(DATA_DIR):
	dataID = dataFile.rstrip('.txt')
	if ONLY_THOSE and dataFile not in ONLY_THOSE:
		continue
	if NOT_THOSE and dataFile in NOT_THOSE:
		continue
	inFile = DATA_DIR + '/' + dataFile
	verbose = True
	jobBundle = grid_submission.check_job_completion(inFile, verbose)
	if SUBMIT_ALL and ACTIVE:
		dontPromptMe = True
		grid_submission.submit_jobs(dontPromptMe, inFile)
	elif RESUBMIT_MISSING and ACTIVE:
		dontPromptMe = True
		grid_submission.resubmit_jobs(jobBundle, dontPromptMe)
