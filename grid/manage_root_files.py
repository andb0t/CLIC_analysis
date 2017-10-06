#!/usr/bin/env python
from __future__ import print_function

import argparse
import os
import os.path
import math
import sys
# import functools

import numpy as np


STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files'
STORAGE_USER_PATH = '/ilc/user/a/amaier/files'
DIR_PREFIX = 'output_'


parser = argparse.ArgumentParser()
parser.add_argument("--show", action="store_true", default=False, help='Show all available datasets')
parser.add_argument("--remove", nargs='*', help='Remove specified datasets')
args = parser.parse_args()

def remove_folder(inputFile):
  diracPaths = []
  for dataFile in os.listdir(STORAGE_BASE_PATH + '/' + DIR_PREFIX + inputFile ):
    diracPath = STORAGE_USER_PATH + '/' + DIR_PREFIX + inputFile + '/' + dataFile
    diracPaths.append(diracPath)

  print('\nFiles to marked for removal:')
  for file in diracPaths:
    print('  ', file)
  print('\nRemove those files? y/[n]')
  deleteThem = raw_input()

  if deleteThem == 'y':
    commands = map(lambda x: 'dirac-dms-remove-files ' + x, diracPaths)
    for cmd in commands:
      # print(cmd)
      # dirac-dms-remove-files /ilc/user/a/amaier/files/output_qq_ln_dst_3249_114/output_qq_ln_dst_3249_114_batch_0.root
      os.system(cmd)
  else:
    print('Abort deletion.')


def main():
  inputFiles = []
  if args.show or args.remove:
    print('Processing files in', STORAGE_BASE_PATH + '/', ':')
    for dataFile in os.listdir(STORAGE_BASE_PATH):
      if not dataFile.startswith(DIR_PREFIX):
        continue
      thisFile = dataFile[len(DIR_PREFIX):]
      if args.remove and thisFile not in args.remove:
        continue
      isEmpty = '(empty)'
      if os.listdir(STORAGE_BASE_PATH + '/' + dataFile): 
        isEmpty = ' ' * len(isEmpty)
      print(dataFile.ljust(30), isEmpty, '-> file ID:', thisFile)
      inputFiles.append(thisFile)
  else:
    print('No input file specified. Abort.')
    return

  if not inputFiles:
    print('No matching file found. Abort.')
    return

  if args.show:
    return

  for inputFile in inputFiles:

    if args.remove:
      remove_folder(inputFile)
      continue


if __name__ == '__main__':
  main()