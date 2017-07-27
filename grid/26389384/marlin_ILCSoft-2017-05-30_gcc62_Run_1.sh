#!/bin/bash 
#####################################################################
# Dynamically generated script to run a production or analysis job. #
#####################################################################
source /cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/init_ilcsoft.sh
declare -x MARLIN_DLL=/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/MarlinDD4hep/HEAD/lib/libMarlinDD4hep.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/DDMarlinPandora/HEAD/lib/libDDMarlinPandora.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/MarlinReco/HEAD/lib/libMarlinReco.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/PandoraAnalysis/HEAD/lib/libPandoraAnalysis.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/MarlinPandora/HEAD/lib/libMarlinPandora.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/LCFIVertex/HEAD/lib/libLCFIVertex.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/LCFIPlus/HEAD/lib/libLCFIPlus.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/CEDViewer/HEAD/lib/libCEDViewer.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/Overlay/HEAD/lib/libOverlay.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/MarlinFastJet/HEAD/lib/libMarlinFastJet.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/LCTuple/HEAD/lib/libLCTuple.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/MarlinKinfit/HEAD/lib/libMarlinKinfit.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/MarlinTrkProcessors/HEAD/lib/libMarlinTrkProcessors.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/ILDPerformance/HEAD/lib/libILDPerformance.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/ClicPerformance/HEAD/lib/libClicPerformance.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/ConformalTracking/HEAD/lib/libConformalTracking.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/Physsim/HEAD/lib/libPhyssim.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/FCalClusterer/HEAD/lib/libFCalClusterer.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/ForwardTracking/HEAD/lib/libForwardTracking.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/Clupatra/HEAD/lib/libClupatra.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/Garlic/HEAD/lib/libGarlic.so:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2017-05-30/x86_64-slc6-gcc62-opt/LICH/HEAD/lib/libLICH.so:./lib/marlin_dll/libTrackCutChecker.so
declare -x LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
declare -x PATH=$ROOTSYS/bin:$PATH
declare -x MARLIN_DEBUG=1

if [ -e "${PANDORASETTINGS}" ]
then
   cp $PANDORASETTINGS .
fi    
echo =============================
echo LD_LIBRARY_PATH is
echo $LD_LIBRARY_PATH | tr ":" "
"
echo =============================
echo PATH is
echo $PATH | tr ":" "
"
echo =============================
echo MARLIN_DLL is
echo $MARLIN_DLL | tr ":" "
"
echo =============================
env | sort >> localEnv.log
Marlin -c marlinxml_1.xml 
Marlin marlinxml_1.xml 
declare -x appstatus=$?
exit $appstatus
