#!/usr/bin/env python
from __future__ import print_function

# from os.path import basename
import os
import functools

import numpy as np

import ROOT

MIN_NEVENT = 0
MAX_NEVENT = 50000
MAX_NDIM = 100
VERBOSE = 0

rootFiles = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files/output/output_batch_*.root'
rootFile = 'all_output.root'
NPZFile = 'all_output_all.npy'


def f7(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]


def remove_zero_columns(data):
	for index, event in enumerate(data):
		try:
			data[index] = event[:, (event != 0).sum(axis=0) > 0]
		except AttributeError:
			pass


def print_data(data):
	for index, event in enumerate(data):
		print('Event {index}:'.format(index=index))
		print(event)


def merge_root_files(rootFile, rootFiles):
	print('Merging the following files into', rootFile)
	cmd = 'ls ' + rootFiles
	os.system(cmd)
	cmd = 'hadd -f ' + rootFile + ' ' + rootFiles
	os.system(cmd)


def read_root_file(rootFile):
	print('Reading root file',rootFile)

	inFile = ROOT.TFile(rootFile, "READ")
	inTree = inFile.Get("rawTree")

	branchList = map(lambda x: x.GetName(), inTree.GetListOfBranches())
	branchList = f7(branchList)
	if VERBOSE: print('Branches in this tree:', branchList)

	data = []
	# data.append(branchList)
	for iEntry, entry in enumerate(inTree):
		if iEntry >= MAX_NEVENT  and MAX_NEVENT != -1:
			break
		if iEntry < MIN_NEVENT:
			continue

		if iEntry % 1000 == 0:
			print(iEntry)

		event = np.zeros( (len(branchList), MAX_NDIM) )

		for iBranch, branchName in enumerate(branchList):
			branch = getattr(entry, branchName)
			try:
				for index, element in enumerate(branch):
					event[iBranch][index] = element
			except TypeError:
				event[iBranch][0] = branch
		data.append(event)

	remove_zero_columns(data)


	shape = data[0].shape

	# verify same dimensions
	nValidEvents = MAX_NEVENT - MIN_NEVENT
	for iEvent, event in enumerate(data):
		if event.shape != data[0].shape:
			del data[iEvent]
			print('Warning: event', iEvent, 'has shape', event.shape,'. Drop it!')
			nValidEvents = nValidEvents - 1

	shape = [nValidEvents] + list(shape)

	dataNP = np.concatenate(data).reshape(shape)

	if VERBOSE > 1:
		print_data(dataNP)

	return dataNP

def convert_root_file(rootFile, NPZFile):
	dataNP = read_root_file(rootFile)
	print('Writing to',NPZFile)
	np.save(NPZFile, dataNP)


def read_npz_file(NPZFile):
	print('Reading file', NPZFile)
	# dataNP = np.load(NPZFile)['arr_0']
	dataNP = np.load(NPZFile)

	if VERBOSE > 1:
		print_data(dataNP)

	averageLepN = functools.reduce(lambda x, y: x + y[0][0], dataNP, 0) / MAX_NEVENT
	print(averageLepN)



merge_root_files(rootFile, rootFiles)

convert_root_file(rootFile, NPZFile)

read_npz_file(NPZFile)