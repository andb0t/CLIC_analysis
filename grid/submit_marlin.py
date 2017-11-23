#!/bin/env python
from __future__ import print_function

import argparse
import os

from src import grid_submission

ONLY_THOSE = []
# ONLY_THOSE = ['0000']
# NOT_THOSE = ['3249', '2166', '5572', '3246']
NOT_THOSE = []
parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("task", default='show', choices=['show', 'resubmit', 'all', 'test'], help='Task to be executed')
args = parser.parse_args()

DATA_DIR = 'file_lists'
BASE_CMD = 'python submit_marlin.py'

if args.task == 'show':
    print("Show available data files")
elif args.task == 'resubmit':
    print("Resubmit missing data files")
elif args.task == 'all':
    print("Resubmit all data files")
elif args.task == 'test':
    print("Submit test file")
    args.task = 'all'
    ONLY_THOSE = ['0000']

for dataFile in os.listdir(DATA_DIR):
    dataID = dataFile.rstrip('.txt')
    if ONLY_THOSE and dataFile.rstrip('.txt') not in ONLY_THOSE:
        continue
    if NOT_THOSE and dataFile.rstrip('.txt') in NOT_THOSE:
        continue
    inFile = DATA_DIR + '/' + dataFile
    verbose = True
    jobBundle = grid_submission.check_job_completion(inFile, verbose)
    if args.task == 'show':
        continue
    if args.task == 'all':
        dontPromptMe = True
        grid_submission.submit_jobs(dontPromptMe, inFile)
    elif args.task == 'resubmit':
        dontPromptMe = True
        grid_submission.resubmit_jobs(jobBundle, dontPromptMe)
