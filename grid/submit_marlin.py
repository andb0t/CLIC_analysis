#!/bin/env python
from __future__ import print_function

import os
import subprocess

import grid_submission

# =====================
# ===== STEERING ======
# =====================
DISARM = True
ONLY_THOSE = []
NOT_THOSE = []
# =====================


DATA_DIR = 'file_lists'
BASE_CMD = 'python submit_marlin.py'
if DISARM:
	print("These files would be submitted! Disable the 'DISARM' setting to submit the jobs!")

for dataFile in os.listdir(DATA_DIR):
	if ONLY_THOSE and dataFile not in ONLY_THOSE:
		continue
	if NOT_THOSE and dataFile in NOT_THOSE:
		continue
	print(dataFile)
	dontPromptMe = True
	inFile = DATA_DIR + '/' + dataFile
	if not DISARM:
		grid_submission.submit_jobs(dontPromptMe, inFile)