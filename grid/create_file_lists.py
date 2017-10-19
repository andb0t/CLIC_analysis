#!/usr/bin/env python
from __future__ import print_function

import argparse
import os.path
import subprocess


parser = argparse.ArgumentParser()
parser.add_argument("--ID", nargs='*', help="Only process single ID")
args = parser.parse_args()

signal = [3249]  # ee->qqlv
bkg0 = [2645, 3246, 4266, 4269, 4272, 4275]  # ee -> qqll
bkg1 = [5572]  # ee -> qqqqll
bkg2 = [2094, 3243]  # ee -> qqvv
bkg3 = [4034]  # ee -> qqqq
bkg4 = [5527, 6183]  # ee -> qqqqvv
bkg5 = [5594, 6191]  # ee -> qqqqlv


allIDs = signal + bkg0 + bkg1 + bkg2 + bkg3 + bkg4 + bkg5
if args.ID:
    allIDs = args.ID

print('Creating file lists for', allIDs)

for thisID in allIDs:
    file = 'file_lists/{0}.txt'.format(thisID)
    command = 'dirac-dms-find-lfns ProdID={0} | grep dst >> {1}'.format(thisID, file)
    print(command)
    with open(file, 'w') as myfile:
        print('# Automatically created from', os.path.basename(__file__), file=myfile)
        print('# Command for single creation:', file=myfile)
        print('# ' + command.replace('>>', '>'), file=myfile)
        subprocess.call([command], shell=True)
