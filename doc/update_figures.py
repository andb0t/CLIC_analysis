#!/usr/bin/env python
from __future__ import print_function

import argparse
import os


parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("file", help='LateX .dep file with figures to be extracted')
args = parser.parse_args()

KEYS = ['.pdf', '.png', '.eps', '.jpg']


def find_between( s, first, last ):
    try:
        start = s.index( first ) + len( first )
        end = s.rindex( last )
        return s[start:end]
    except ValueError:
        return s


with open(args.file, 'r') as file:
	for line in file:
		if any(key in line for key in KEYS):
			# print(line)
			line = line.rstrip().lstrip().lstrip('*{file}')
			resource = find_between(line, '{', '}{')
			print(resource)
			# os.system('ls {0}'.format(resource))