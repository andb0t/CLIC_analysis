#!/usr/bin/env python
from __future__ import print_function

# from os.path import basename
import os
# import functools

import numpy as np

import ROOT

MERGE_ROOTFILES = False
WRITE_TO_TXT = True

MIN_N_EVENT = 0
MAX_N_EVENT = 10000
VERBOSE = 0

txtFile = '/afs/cern.ch/work/a/amaier/CLIC/csv/all_output_small.txt'
rootFiles = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files/output/output_batch_*.root'
rootFile = '/afs/cern.ch/work/a/amaier/CLIC/rootfiles/all_output.root'

MAX_N_LEP = 5
MAX_N_JET = 4
branchSelection = {'lep_n': 1,
                   'lep_etot': 1,
                   'lep_pt': MAX_N_LEP,
                   'lep_theta': MAX_N_LEP,
                   'lep_phi': MAX_N_LEP,
                   'lep_e': MAX_N_LEP,

                   'jet_DH_n': 1,
                   'jet_DH_pt': MAX_N_JET,
                   'jet_DH_theta': MAX_N_JET,
                   'jet_DH_phi': MAX_N_JET,
                   'jet_DH_e': MAX_N_JET,

                   'jet_KT_R05_n': 1,
                   'jet_KT_R05_pt': MAX_N_JET,
                   'jet_KT_R05_theta': MAX_N_JET,
                   'jet_KT_R05_phi': MAX_N_JET,
                   'jet_KT_R05_e': MAX_N_JET,

                   'jet_KT_R07_n': 1,
                   'jet_KT_R07_pt': MAX_N_JET,
                   'jet_KT_R07_theta': MAX_N_JET,
                   'jet_KT_R07_phi': MAX_N_JET,
                   'jet_KT_R07_e': MAX_N_JET,
                   }


def f7(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]


def merge_root_files(rootFile, rootFiles):
    print('Merging the following files into', rootFile)
    cmd = 'ls ' + rootFiles
    os.system(cmd)
    cmd = 'hadd -f ' + rootFile + ' ' + rootFiles
    os.system(cmd)


def write_root_file_to_txt(rootFile, txtFile):
    print('Reading root file', rootFile)

    inFile = ROOT.TFile(rootFile, "READ")
    inTree = inFile.Get("rawTree")

    branchList = map(lambda x: x.GetName(), inTree.GetListOfBranches())
    branchList = f7(branchList)
    print('Branches in this tree:', branchList)

    with open(txtFile, "w") as outFile:

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
                print('Processing entry', iEntry, '/', inTree.GetEntries())

            print(iEntry, end='\t', file=outFile)

            for branchName in branchList:
                if branchName not in branchSelection.keys():
                    continue
                event = np.zeros(branchSelection[branchName])
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
                    if leaf:
                        print(round(leaf, 3), end='\t', file=outFile)
                    else:
                        print('0', end='\t', file=outFile)

            print('', file=outFile)


def convert_root_file(rootFile, outputFile):
    print('Writing to', outputFile)
    if outputFile.find('.txt') is not -1:
        write_root_file_to_txt(rootFile, outputFile)
    else:
        print("Unknown file extension, abort!")


if MERGE_ROOTFILES:
    merge_root_files(rootFile, rootFiles)

if WRITE_TO_TXT:
    convert_root_file(rootFile, txtFile)
