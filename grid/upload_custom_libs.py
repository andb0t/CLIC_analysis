#!/bin/env python
from __future__ import print_function
import os


basePath = '/afs/cern.ch/user/a/amaier/projects/'

libs = ['CLIC_analysis/processors/NtupleMaker/lib/libNtupleMaker.so', 'MarlinReco/lib/libMarlinReco.so']

libs = map(lambda a: basePath + a, libs)
print('Packing custom libraries in tar:', libs)

dirName = 'lib/marlin_dll'
mainDirName = dirName.split('/')[0]
eosName = '/ilc/user/a/amaier/mylibs.tar.gz'


os.system('set -x')
os.system('mkdir -p ' + dirName)

for lib in libs:
    os.system('cp ' + lib + ' ' + dirName)

os.system('tar -zcvf ' + mainDirName + '.tar.gz ' + mainDirName + '/')


os.system('dirac-dms-remove-files ' + eosName)
os.system('dirac-dms-add-file ' + eosName + ' ' + mainDirName + '.tar.gz CERN-DST-EOS')
os.system('mv ' + mainDirName + '.tar.gz $TRASH/lib.tar.gz')
os.system('cp -r ' + mainDirName + ' $TRASH/')
os.system('rm -rf ' + mainDirName)
print('Creating replicates...')
os.system('dirac-dms-replicate-lfn ' + eosName + ' RAL-SRM')
os.system('dirac-dms-replicate-lfn ' + eosName + ' DESY-SRM')
os.system('dirac-dms-replicate-lfn ' + eosName + ' CERN-DIP-4')

print('Refer to LFN:' + eosName + ' in your python submit script input sandbox')
