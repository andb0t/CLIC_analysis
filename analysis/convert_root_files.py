#!/usr/bin/env python
from __future__ import print_function

# from os.path import basename
import os
import functools

import numpy as np

import ROOT

MIN_N_EVENT = 0
MAX_N_EVENT = 10547
VERBOSE = 0

rootFiles = '/eos/experiment/clicdp/grid/ilc/user/a/amaier/files/output/output_batch_*.root'
rootFile = '/afs/cern.ch/work/a/amaier/CLIC/rootfiles/all_output.root'
NPZFile = '/afs/cern.ch/work/a/amaier/CLIC/csv/all_output.npy'
TXTFile = '/afs/cern.ch/work/a/amaier/CLIC/csv/all_output.txt'

MAX_N_LEP = 4
MAX_N_JET = 3
branchSelection = {'lep_n': 1, 
				   'lep_etot': 1, 
				   'lep_pt': MAX_N_LEP, 
				   'lep_theta': MAX_N_LEP, 
				   'lep_phi': MAX_N_LEP, 
				   'lep_e': MAX_N_LEP,
				   }


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


def write_root_file_to_txt(rootFile, txtFile):
	print('Reading root file',rootFile)

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
			if iEntry >= MAX_N_EVENT  and MAX_N_EVENT != -1:
				break
			if iEntry < MIN_N_EVENT:
				continue
			if iEntry % 1000 == 0 or VERBOSE:
				print('Processing entry', iEntry, '/', inTree.GetEntries())

			print(iEntry, end='\t', file=outFile)

			event = np.zeros( (len(branchList), max(branchSelection.values())) )
			for iBranch, branchName in enumerate(branchList):
				if branchName not in branchSelection.keys():
					continue
				branch = getattr(entry, branchName)
				try:
					for index, element in enumerate(branch):
						try:
							event[iBranch][index] = element
						except IndexError:
							print('Warning: event', iEntry, 'requests', len(branch), 'max entries instead of', branchSelection[branchName], 'for branch', branchName)
							break
				except TypeError:
					event[iBranch][0] = branch

				for idx, leaf in enumerate(event[iBranch]):
					if idx >= branchSelection[branchName]:
						break
					if leaf:
						print(round(leaf,3), end='\t', file=outFile)
					else:
						print('0', end='\t', file=outFile)

			print('', file=outFile)


def read_root_file_to_np(rootFile):
	print('Reading root file',rootFile)

	inFile = ROOT.TFile(rootFile, "READ")
	inTree = inFile.Get("rawTree")

	branchList = map(lambda x: x.GetName(), inTree.GetListOfBranches())
	branchList = f7(branchList)
	if VERBOSE: print('Branches in this tree:', branchList)

	data = []
	# data.append(branchList)
	for iEntry, entry in enumerate(inTree):
		if iEntry >= MAX_N_EVENT  and MAX_N_EVENT != -1:
			break
		if iEntry < MIN_N_EVENT:
			continue

		if iEntry % 1000 == 0:
			print(iEntry)

		MAX_N_VECTOR_ELEMENTS = 10
		event = np.zeros( (len(branchList), MAX_N_VECTOR_ELEMENTS) )

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
	nValidEvents = MAX_N_EVENT - MIN_N_EVENT
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

def convert_root_file(rootFile, outputFile):
	print('Writing to',outputFile)
	if outputFile.find('.npy') is not -1:
		dataNP = read_root_file_to_np(rootFile)
		np.save(outputFile, dataNP)
	elif outputFile.find('.txt') is not -1:
		write_root_file_to_txt(rootFile, outputFile)
	else:
		print("Unknown file extension, abort!")


def read_npz_file(NPZFile):
	print('Reading file', NPZFile)
	# dataNP = np.load(NPZFile)['arr_0']
	dataNP = np.load(NPZFile)

	if VERBOSE > 1:
		print_data(dataNP)

	averageLepN = functools.reduce(lambda x, y: x + y[0][0], dataNP, 0) / MAX_N_EVENT
	print(averageLepN)



# merge_root_files(rootFile, rootFiles)

convert_root_file(rootFile, TXTFile)

# convert_root_file(rootFile, NPZFile)
# read_npz_file(NPZFile)