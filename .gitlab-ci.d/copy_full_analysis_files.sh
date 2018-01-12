#!/bin/bash
KRB_USERNAME=$1
REMOTEPATH=/afs/cern.ch/work/a/amaier/CLIC/csv/

echo "Copying full data files from $REMOTEPATH"

ssh $KRB_USERNAME@lxplus "ls $REMOTEPATH"
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/3249.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/3246.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/5572.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/2094.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/4034.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/5527.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/5594.csv analysis/example_data/
scp ${KRB_USERNAME}@lxplus:$REMOTEPATH/9402.csv analysis/example_data/