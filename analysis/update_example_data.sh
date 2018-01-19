#!/bin/bash
DATA_DIR="/afs/cern.ch/work/a/amaier/CLIC/csv"
THIS_DIR="$(dirname "$0")/example_data"
N_EVENTS=5000
N_EVENTS_OFFSET=1

echo "Updating example data from $DATA_DIR"
ls $DATA_DIR


declare -a arr=("2091")
# declare -a arr=("3249" "3246" "5572" "2094" "2091" "4034" "5527" "5594" "9402")

## now loop through the above array
N_LINES=$((N_EVENTS + N_EVENTS_OFFSET))
for i in "${arr[@]}"
do
	FILE_NAME="$DATA_DIR/$i.csv"
    TARGET_NAME="$THIS_DIR"/"$i".csv
	echo "Copying $N_LINES events from $FILE_NAME to $TARGET_NAME"
	head -n $N_LINES "$FILE_NAME" > "$TARGET_NAME"
done
