#!/bin/bash
echo "Setting python3 environment..."
export PYTHONDIR=/cvmfs/clicdp.cern.ch/software/Python/3.5.2/x86_64-slc6-gcc61-opt
export PATH=$PYTHONDIR/bin:$PATH
export LD_LIBRARY_PATH=$PYTHONDIR/lib:$LD_LIBRARY_PATH
# export PYTHONPATH=/afs/cern.ch/user/a/amaier/.local/lib/python3.5/site-packages/
# export PATH=/afs/cern.ch/user/a/amaier/.local/bin:$PATH
