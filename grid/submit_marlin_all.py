#!/bin/env python
from __future__ import print_function

import os
import subprocess


DATA_DIR = 'file_lists'
BASE_CMD = 'python submit_marlin.py'

for datafile in os.listdir(DATA_DIR):
    print(BASE_CMD + ' ' + datafile)


# subprocess.call([command], shell=True)
