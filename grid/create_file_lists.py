#!/usr/bin/env python
from __future__ import print_function

import os.path
import subprocess



signal = [3249]  # ee->qqlv
bkg0 = [2166, 5572]  # ee -> qqqqll
bkg1 = [2645, 3246, 4266, 4269, 4272, 4275]  # ee -> qqll
bkg2 = [2717]  # ee -> hvv, h->ZZ
bkg3 = []  # h->ZZ

allIDs = signal + bkg0 + bkg1 + bkg2 + bkg3

for thisID in allIDs:
	file = 'file_lists/{0}.txt'.format(thisID)
	command = 'dirac-dms-find-lfns ProdID={0} | grep dst >> {1}'.format(thisID, file)
	print(command)
	with open(file, 'w') as myfile:
		print('# Automatically created from', os.path.basename(__file__), file=myfile)
		print('# Command for single creation:', file=myfile)
		print('# ' + command.replace('>>', '>'), file=myfile)
	subprocess.call([command], shell=True)	

