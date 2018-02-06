#!/bin/bash
echo "Changing datasets inline"

# FILE_NAMES="*"
FILE_NAMES="test"

if [[ "$FILE_NAMES" == "test" ]]; then
    head "2091.csv" > "bk.csv"
    cp "bk.csv" "$FILE_NAMES.csv"
fi

echo $FILE_NAMES.csv

sed -i 's/\bbeam_e\b/mc_beam_e/g' $FILE_NAMES.csv
sed -i 's/\bbeam_m\b/mc_beam_m/g' $FILE_NAMES.csv
sed -i 's/\(charge_1\)\t*$/\1\tbeam_e\tbeam_m\tqq_m\tln_m/g' $FILE_NAMES.csv
sed -i 's/\b\([0-9]\)\t*$/\1\t0\t0\t0\t0/g' $FILE_NAMES.csv
