#!/bin/python

#####################################################################

#parameters
nJobs = 250
nEvts = 25000
nameTag = '_geantino'
nameJobGroup = 'FCCee_o5_v03_verification_1'
#  nameJobGroup = 'FCCee_OT_test'
ddsimVersion = 'ILCSoft-2017-06-21_gcc62'
marlinVersion = 'ILCSoft-2017-06-21_gcc62'
#  detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/FCCee_o5_v02_jul12.tgz'
#  detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/FCCee_o5_v02_OT_fix.tgz'
#  detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/FCCee_o5_v02_scaledCLICVTX_fixedOT.tgz'
detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/FCCee_o5_v03_VTX_IT_OT_jul24.tgz'
#  detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/FCCee_o5_v02_noMagField_OT_test.tgz'
#  detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/CLIC_o3_v11_noMagField_onlyTracker.tgz'
#  detectorModel =  'LFN:/ilc/user/o/oviazlo/FCCee_o5/compact/CLIC_o3_v11_noMagField_onlyTracker_fixedLength.tgz'
templateSteeringMarlin = 'jobs/fccReconstruction_template.xml'
templateOutRoot = '__outputRootFile__'
nameDir = nameJobGroup+'_files'+nameTag
nameRepositoryFile = nameJobGroup+nameTag+'_repository.rep'


#####################################################################
 
#####################################################################     
#set enviroment          
import os
import sys

from DIRAC.Core.Base import Script #dirac enviroment                                              
Script.parseCommandLine() #dirac enviroment     

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC #job receiver class   
dirac = DiracILC(True,nameRepositoryFile) #important to retrive jobs        
#####################################################################         

#####################################################################        
#job definition                             
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
#####################################################################   

#####################################################################        
#filecatalog                             
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient 
#####################################################################   

#####################################################################    
#ddsim
from ILCDIRAC.Interfaces.API.NewInterface.Applications import DDSim
#####################################################################    

# #####################################################################    
#marlin
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin
# #####################################################################    


#####################################################################      
 
for i in range(0, nJobs): # nJobs

    #####################################################################      
    # build names

    print "file number ", i 

    jobSuffix = 'j%d' % (i)
    nameJob = nameJobGroup+'_'+jobSuffix
    outputFile = nameJobGroup+'_'+jobSuffix+'.slcio'
    rootFile = nameJobGroup+'_'+jobSuffix +'.root'
    templateSuffix = '_'+jobSuffix+'.xml'
    nameSteeringMarlin = templateSteeringMarlin.replace('_template.xml',templateSuffix)

    #####################################################################      

    # #####################################################################      
    #prepare file for running marlin                                                                                     
    with open(templateSteeringMarlin) as f:
         open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))

    # #####################################################################      


    #####################################################################      
    #job definition   
              
    job = UserJob() #use UserJob unless recommended differently      
    job.setName(nameJob)
    job.setJobGroup(nameJobGroup)
    job.setCPUTime(86400)
    job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
    job.setInputSandbox([nameSteeringMarlin,'LFN:/ilc/user/o/oviazlo/FCCee_o5/ilcsoft_2017-06-21/lcgeo_28_06_2017_v3.tgz',detectorModel,'LFN:/ilc/user/o/oviazlo/PandoraSettings.tar.gz','LFN:/ilc/user/o/oviazlo/FCCee_o5/marlin_lib_simHits_v3.tgz'])
    job.setOutputSandbox(["*.log"])  #files that should be brought back when retrieving the job outputs 
    job.setOutputData([rootFile],nameDir,"CERN-DST-EOS")   
    
    #####################################################################    


    #####################################################################  
    #ddsim

    ddsim = DDSim()
    ddsim.setVersion(ddsimVersion)
    ddsim.setDetectorModel(detectorModel)
    ddsim.setOutputFile(outputFile)
    ddsim.setSteeringFile("/afs/cern.ch/work/v/viazlo/gridSubmission/FCCee_o5/files/clic_steer.py")
    ddsim.setNumberOfEvents(nEvts)
    res = job.append(ddsim)

    if not res['OK']:
        print res['Message']
        sys.exit(2)

    #####################################################################  


    # #####################################################################  
    #marlin
    ma = Marlin()
    ma.setVersion(marlinVersion)
    ma.setNbEvts(nEvts)
    ma.setDetectorModel(detectorModel)
    ma.getInputFromApp(ddsim)
    ma.setProcessorsToUse([])
    ma.setSteeringFile(nameSteeringMarlin)
    res = job.append(ma) 

    if not res['OK']:
        print res['Message']
        sys.exit(2)

    
    #####################################################################  
    #submit          

    job.dontPromptMe()
    print job.submit(dirac)
    #  print job.submit(dirac,mode='local')

    #####################################################################  
