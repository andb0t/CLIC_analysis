#!/usr/bin/env python
from __future__ import print_function

import argparse
import os
import os.path
import math

import numpy as np

try:
    import ROOT
except ImportError:
    print('Did you forget to set the correct python path?')
    raise


VERBOSE = 0
ACTIVATE = True


MAX_N_LEP = 5
MAX_N_JET = 2
branchSelection = {'lep_n': 1,
                   'lep_etot': 1,
                   'lep_type': MAX_N_LEP,
                   'lep_pt': MAX_N_LEP,
                   'lep_theta': MAX_N_LEP,
                   'lep_phi': MAX_N_LEP,
                   'lep_e': MAX_N_LEP,
                   'lep_charge': MAX_N_LEP,

                   'jet_vlc_R08_n': 1,
                   'jet_vlc_R08_etot': 1,
                   'jet_vlc_R08_pt': MAX_N_JET,
                   'jet_vlc_R08_theta': MAX_N_JET,
                   'jet_vlc_R08_phi': MAX_N_JET,
                   'jet_vlc_R08_e': MAX_N_JET,
                   'jet_vlc_R08_charge': MAX_N_JET,

                   'beam_e': 1,
                   'beam_m': 1,

                   'miss_pt': 1,
                   'miss_theta': 1,
                   'miss_phi': 1,
                   'miss_e': 1,

                   'mc_qq_m': 1,
                   'mc_ln_m': 1,
                   }

STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files'
STORAGE_USER_PATH = '/ilc/user/a/amaier/files'
USR_STORAGE_BASE_PATH = '/afs/cern.ch/work/a/amaier/CLIC'
DIR_PREFIX = 'output_'

parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("--nomerge", action="store_true", default=False, help='Do not merge root files')
parser.add_argument("--nocsv", action="store_true", default=False, help='Do not create csv file')
parser.add_argument("--input", nargs='*', help="Only process those datasets with this identifier")
parser.add_argument("--not", nargs='*', dest='notthis', help="Do not process those datasets with this identifier")
parser.add_argument("--all", action="store_true", default=False, help='Process all available datasets')
parser.add_argument("--onlynew", action="store_true", default=False, help='Process only datasets which have no corresponding csv file already')
parser.add_argument("--minevt", nargs='?', type=int, default=0, help="Start processing at this event number")
parser.add_argument("--maxevt", nargs='?', type=int, default=None, help="End processing at this event number")
args = parser.parse_args()


warnignList = []


def f7(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]


def merge_root_files(outFile, mergeFiles):
    print('Merging the following files into', outFile)
    cmd = 'ls ' + mergeFiles
    os.system(cmd)
    cmd = 'hadd -f ' + outFile + ' ' + mergeFiles
    os.system(cmd)
    print('Saved to', outFile)


def write_root_file_to_csv(rootFile, csvFile):
    global warnignList
    print('Reading root file', rootFile)

    inFile = ROOT.TFile(rootFile, "READ")
    inTree = inFile.Get("rawTree")

    branchList = map(lambda x: x.GetName(), inTree.GetListOfBranches())
    branchList = f7(branchList)
    print('Branches in this tree:', branchList)

    mode = 'w'
    if not ACTIVATE:
        mode = 'r'

    with open(csvFile, mode) as outFile:

        if not ACTIVATE:
            outFile = None

        # print header
        print('i', end='\t', file=outFile)
        for branch in branchList:
            if branch not in branchSelection.keys():
                continue
            for times in range(branchSelection[branch]):
                if branchSelection[branch] > 1:
                    print(branch + '_' + str(times), end='\t', file=outFile)
                else:
                    print(branch, end='\t', file=outFile)
        print('', file=outFile)

        maxString = '/ '
        if args.maxevt:
            maxEvt = min(inTree.GetEntries(), args.maxevt)
        else:
            maxEvt = inTree.GetEntries()
        if args.maxevt:
            maxString += str(args.maxevt) + ' (' + str(inTree.GetEntries()) + ' total)'
        else:
            maxString += str(inTree.GetEntries())

        # print events
        for iEntry, entry in enumerate(inTree):
            if args.maxevt and iEntry >= args.maxevt:
                break
            if iEntry < args.minevt:
                continue
            if iEntry % 1000 == 0 or VERBOSE:
                progress = float(iEntry) / maxEvt
                print('Processing entry', str(iEntry), maxString, '-> {0:.1%}'.format(progress))

            if ACTIVATE:
                print(iEntry, end='\t', file=outFile)

                for branchName in branchList:
                    if branchName not in branchSelection.keys():
                        continue
                    event = np.zeros(branchSelection[branchName])
                    event[:] = np.NAN
                    branch = getattr(entry, branchName)
                    try:
                        for index, element in enumerate(branch):
                            try:
                                event[index] = element
                            except IndexError:
                                warning = ' '.join(['Warning in', rootFile, ':',
                                                    'event', str(iEntry),
                                                    'requests', str(len(branch)),
                                                    'max entries instead of', str(branchSelection[branchName]),
                                                    'for branch', branchName])
                                print(warning)
                                warnignList.append(warning)
                                break
                    except TypeError:
                        event[0] = branch

                    for idx, leaf in enumerate(event):
                        if idx >= branchSelection[branchName]:
                            break
                        if math.isnan(leaf):
                            print('-', end='\t', file=outFile)
                        elif leaf:
                            print(round(leaf, 3), end='\t', file=outFile)
                        else:
                            print('0', end='\t', file=outFile)
                print('', file=outFile)
        print('Done!')


def convert_root_file(rootFile, outputCSVFile):
    print('Writing to', outputCSVFile)
    if outputCSVFile.find('.csv') is not -1:
        write_root_file_to_csv(rootFile, outputCSVFile)
    else:
        print('Unknown file extension for {0}. Abort!'.format(outputCSVFile))
    print('Saved to', outputCSVFile)


def main():
    inputFiles = []
    print('Processing files in', STORAGE_BASE_PATH + '/', ':')
    presentFiles = os.listdir(USR_STORAGE_BASE_PATH + '/csv/')
    presentFiles = map(lambda x: x.rstrip('.csv'), presentFiles)
    for dataFile in os.listdir(STORAGE_BASE_PATH):
        if not dataFile.startswith(DIR_PREFIX):
            continue
        thisFile = dataFile[len(DIR_PREFIX):]
        if args.onlynew:
            vetoFiles = presentFiles
            if args.notthis:
                vetoFiles.extend(args.notthis)
            if thisFile in vetoFiles:
                if not args.input:
                    continue
                elif thisFile not in args.input:
                    continue
        else:
            if args.input and thisFile not in args.input:
                continue
            if args.notthis and thisFile in args.notthis:
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

    if args.input or args.notthis or args.all or args.onlynew:
        for inputFile in inputFiles:

            rootFiles = STORAGE_BASE_PATH + '/output_' + inputFile + '/output_' + inputFile + '_batch_*.root'
            rootFile = USR_STORAGE_BASE_PATH + '/rootfiles/' + inputFile + '.root'
            csvFile = USR_STORAGE_BASE_PATH + '/csv/' + inputFile + '.csv'

            if not args.nomerge:
                print('Merging', rootFiles, 'into', rootFile)
                merge_root_files(rootFile, rootFiles)

            if not args.nocsv:
                print('Converting content of', rootFile, 'to csv in', csvFile)
                convert_root_file(rootFile, csvFile)

        if warnignList:
            print('\nThe following warnings were encountered:')
            for warning in warnignList:
                print(warning)
        else:
            print('Success!')


if __name__ == '__main__':
    main()
