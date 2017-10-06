#!/bin/bash
DATA_DIR="/afs/cern.ch/work/a/amaier/CLIC/csv"
THIS_DIR="$(dirname $0)"
N_EVENTS=10000

echo "Updating example data from $DATA_DIR"
ls $DATA_DIR


declare -a arr=("3246" "3249" "5572")

## now loop through the above array
for i in "${arr[@]}"
do
	FILE_NAME="$DATA_DIR/$i.csv"
	echo "Copying $N_EVENTS events to $FILE_NAME"
	head -n $N_EVENTS $FILE_NAME > $THIS_DIR/$i.csv
done