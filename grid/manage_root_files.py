#!/usr/bin/env python
from __future__ import print_function

import argparse
import os
import os.path


STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files'
STORAGE_FILE_LIST_PATH = '/afs/cern.ch/user/a/amaier/CLIC_analysis/grid/file_lists'
STORAGE_USER_PATH = '/ilc/user/a/amaier/files'
DIR_PREFIX = 'output_'


parser = argparse.ArgumentParser()
parser.add_argument("--remove", nargs='*', help='Remove specified datasets')
parser.add_argument("--cleanup", nargs='*', help='Remove all datasets not in file_lists')
args = parser.parse_args()


def scan_file_list():
    fileList = []
    for dataFile in os.listdir(STORAGE_FILE_LIST_PATH):
        fileList.append(dataFile[:len('.csv')])
    print(fileList)
    return fileList


def remove_folder(inputFile):
    diracPaths = []
    for dataFile in os.listdir(STORAGE_BASE_PATH + '/' + DIR_PREFIX + inputFile):
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
    print('Processing files in', STORAGE_BASE_PATH + '/', ':')
    for dataFile in os.listdir(STORAGE_BASE_PATH):
        if not dataFile.startswith(DIR_PREFIX):
            continue
        thisFile = dataFile[len(DIR_PREFIX):]
        fileList = scan_file_list()
        if args.cleanup and thisFile in fileList:
            continue
        elif args.remove and thisFile not in args.remove:
            continue

        nFiles = len(os.listdir(STORAGE_BASE_PATH + '/' + dataFile))
        isEmpty = True
        if nFiles:
            isEmpty = False
            emptyString = '({0} files)'.format(nFiles)
        else:
            isEmpty = True
            emptyString = '(empty)'
        print(dataFile.ljust(30), emptyString.rjust(15), '-> file ID:', thisFile)
        if not isEmpty:
            inputFiles.append(thisFile)

    if not inputFiles:
        print('No matching file found. Abort.')
        return

    if args.remove:
        for inputFile in inputFiles:
            remove_folder(inputFile)


if __name__ == '__main__':
    main()
