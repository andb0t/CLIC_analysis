#!/usr/bin/env python3
from __future__ import print_function

import argparse
import os
import os.path
import math
import sys
# import functools

import numpy as np

import ROOT


MIN_N_EVENT = 0
MAX_N_EVENT = 10000
VERBOSE = 0


MAX_N_LEP = 4
MAX_N_JET = 2
branchSelection = {'lep_n': 1,
                   'lep_etot': 1,
                   'lep_type': MAX_N_LEP,
                   'lep_pt': MAX_N_LEP,
                   'lep_theta': MAX_N_LEP,
                   'lep_phi': MAX_N_LEP,
                   'lep_e': MAX_N_LEP,

                   'jet_vlc_R06_n': 1,
                   'jet_vlc_R06_etot': 1,
                   'jet_vlc_R06_pt': MAX_N_JET,
                   'jet_vlc_R06_theta': MAX_N_JET,
                   'jet_vlc_R06_phi': MAX_N_JET,
                   'jet_vlc_R06_e': MAX_N_JET,

                   'jet_vlc_R08_n': 1,
                   'jet_vlc_R08_etot': 1,
                   'jet_vlc_R08_pt': MAX_N_JET,
                   'jet_vlc_R08_theta': MAX_N_JET,
                   'jet_vlc_R08_phi': MAX_N_JET,
                   'jet_vlc_R08_e': MAX_N_JET,

                   'jet_vlc_R10_n': 1,
                   'jet_vlc_R10_etot': 1,
                   'jet_vlc_R10_pt': MAX_N_JET,
                   'jet_vlc_R10_theta': MAX_N_JET,
                   'jet_vlc_R10_phi': MAX_N_JET,
                   'jet_vlc_R10_e': MAX_N_JET,
                   }

STORAGE_BASE_PATH = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files'
USR_STORAGE_BASE_PATH = '/afs/cern.ch/work/a/amaier/CLIC'

parser = argparse.ArgumentParser()
parser.add_argument("--nomerge", action="store_true", default=False, help='Do not merge root files')
parser.add_argument("--nocsv", action="store_true", default=False, help='Do not create csv file')
parser.add_argument("--input", nargs='*', help="Only process those datasets")
parser.add_argument("--not", nargs='*', dest='notthis', help="Do not process those datasets")
parser.add_argument("--all", action="store_true", default=False, help='Process all available datasets')
parser.add_argument("--show", action="store_true", default=False, help='Show all available datasets')
args = parser.parse_args()

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
    print('Reading root file', rootFile)

    inFile = ROOT.TFile(rootFile, "READ")
    inTree = inFile.Get("rawTree")

    branchList = map(lambda x: x.GetName(), inTree.GetListOfBranches())
    branchList = f7(branchList)
    print('Branches in this tree:', branchList)

    with open(csvFile, "w") as outFile:

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

        # print events
        for iEntry, entry in enumerate(inTree):
            if iEntry >= MAX_N_EVENT and MAX_N_EVENT != -1:
                break
            if iEntry < MIN_N_EVENT:
                continue
            if iEntry % 1000 == 0 or VERBOSE:
                print('Processing entry', iEntry, '/', MAX_N_EVENT, '(' + str(inTree.GetEntries()) + ' total)')

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
                            print('Warning: event', iEntry, 'requests', len(branch), 'max entries instead of',
                                  branchSelection[branchName], 'for branch', branchName)
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


def convert_root_file(rootFile, outputCSVFile):
    print('Writing to', outputCSVFile)
    if outputCSVFile.find('.csv') is not -1:
        write_root_file_to_csv(rootFile, outputCSVFile)
    else:
        print('Unknown file extension for {0}. Abort!'.format(outputCSVFile))
    print('Saved to', outputCSVFile)


def main():
  inputFiles = []
  if args.input or args.notthis or args.all:
    for dataFile in os.listdir(STORAGE_BASE_PATH):
      prefix = 'output_'
      if not dataFile.startswith(prefix):
        continue
      thisFile = dataFile[len(prefix):]
      if args.input and thisFile not in args.input:
        continue
      if args.notthis and thisFile in args.notthis:
        continue
      print(thisFile)
      inputFiles.append(thisFile)
  else:
    print('No input file specified. Abort.')

  if args.show:
    for file in inputFiles:
      print(file)
    return


  print('Converting files with identifyer', thisFile)

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


if __name__ == '__main__':
  main()