#!/bin/env python
from DIRAC.Core.Base import Script
Script.parseCommandLine()

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin




jobName = 'output'
outputFile = jobName + ".slcio"
rootFile = jobName + ".root"






dIlc = DiracILC()

job = UserJob()
job.setOutputSandbox(["*.out", "*.log", "*.sh", "*.py", "*.xml"])
job.setOutputData([outputFile, rootFile], OutputSE='CERN-DST-EOS')
job.setJobGroup( "myMarlinRun1" )
job.setName( "MyMarlinJob1" )
# job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk'])
job.setInputSandbox( ['LFN:/ilc/user/a/amaier/mylibs.tar.gz'] )


marl = Marlin ()
# marl.setVersion("ILCSoft-01-17-11")
marl.setVersion("ILCSoft-2017-05-30_gcc62")
# marl.setVersion("ILCSoft-2016-11-24_gcc62")

marl.setInputFile(["LFN:/ilc/prod/clic/1.4tev/qq_ln/ILD/DST/00003249/010/qq_ln_dst_3249_10000.slcio"])
# marl.setSteeringFile("philipp_example.xml")
marl.setSteeringFile("full_ntuple_maker.xml")
marl.setOutputFile(outputFile)
marl.setGearFile("/afs/cern.ch/user/a/amaier/CLIC/grid/clic_ild_cdr.gear")
marl.setNumberOfEvents(5)


job.append(marl)
job.submit(dIlc)



