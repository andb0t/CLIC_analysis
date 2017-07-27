#!/bin/bash
echo "Running several Marlin executions behind each other!"
ENERGIES=( 91 100 200 380 500 750 1000 1500 2000 3000 )
for i in "${ENERGIES[@]}"
do
	echo Running on energy $i
	# ~/Utils/bash/copyreplaceandexec.sh Marlin xmlparams_$i.txt myclicReconstruction.xml
	~/Utils/bash/copyreplaceandexec.sh Marlin xmlparams_$i.txt myAnalysis.xml
done
