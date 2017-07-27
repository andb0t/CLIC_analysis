#!/bin/env python
from __future__ import print_function
import os


basePath = '/afs/cern.ch/user/a/amaier/CLIC/'

libs = ['TrackCutChecker/lib/libTrackCutChecker.so']

print('Packing custom libraries in tar:', libs)

dirName = 'lib/marlin_dll'
rootDirName = dirName.split('/')[0]
eosName = '/ilc/user/a/amaier/mylibs.tar.gz'

libs = map(lambda a: basePath + a, libs)

os.system('cp -r ' + rootDirName + ' $TRASH/')
os.system('rm -rf ' + rootDirName)
os.system('mkdir -p ' + dirName)

for lib in libs:
	os.system('cp ' + lib + ' ' + dirName)

os.system('tar -zcvf ' + rootDirName + '.tar.gz ' + rootDirName + '/')


os.system('dirac-dms-add-file ' + eosName + ' ' + rootDirName + '.tar.gz CERN-DST-EOS')
os.system('mv ' + rootDirName + '.tar.gz lib_mv.tar.gz')
os.system('dirac-dms-replicate-lfn ' + eosName + ' RAL-SRM')
os.system('dirac-dms-replicate-lfn ' + eosName + ' DESY-SRM')
os.system('dirac-dms-replicate-lfn ' + eosName + ' CERN-DIP-4')

print('Refer to LFN:' + eosName + ' in your python submit script')