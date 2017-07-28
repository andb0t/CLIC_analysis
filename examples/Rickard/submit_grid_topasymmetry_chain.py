#!/usr/bin/env python

'''
Script to submit jobs to the cluster

Created on March 1, 2017
'''

__RCSID__ = '$Id$'

import string
import os
import subprocess

from DIRAC.Core.Base import Script
from DIRAC import S_OK, S_ERROR, gLogger, exit as dexit

class Params:
  '''
    Class holding the parameters for the top tagger and callbacks for their
    respective switches.
  '''

  def __init__( self ):
    
    ##Input file
    self.prodID = "6601"
    self.suffix = "test"

    ##Jet clustering
    self.jetAlgo = "ValenciaPlugin 1.5 1 1"
    self.jetCluster = "ExclusiveNJets 2"
    self.jetRecomb = "E_scheme"
    self.jetRadius = 1.5
    self.jetBeta = 1.0
    self.jetGamma = 1.0

    ##Top tagging
    self.deltaR = 0.01
    self.deltaP = 0.01
    self.cos_theta_W_max = 1.0    

    ##Other
    self.sixFermionSample = True
    self.nrFilesPerJob = 100

  def setProdID(self, value):
    try:
      self.prodID = value
    except ValueError:
      return S_ERROR( "prodID has to be a string" )
    return S_OK()

  def setSuffix(self, value):
    try:
      self.suffix = value
    except ValueError:
      return S_ERROR( "Suffix has to be a string" )
    return S_OK()

  def setAlgorithm(self, value):
    try:
      self.jetAlgo = value
    except ValueError:
      return S_ERROR( "Jet settings must be string" )
    return S_OK()

  def setClusteringMode(self, value):
    try:
      self.jetCluster = value
    except ValueError:
      return S_ERROR( "Jet settings must be string" )
    return S_OK()

  def setRecombinationScheme(self, value):
    try:
      self.jetRecomb = value
    except ValueError:
      return S_ERROR( "Jet settings must be string" )
    return S_OK()

  def setJetRadius(self, value):
    try:
      self.jetRadius = value
    except ValueError:
      return S_ERROR( "Jet radius must be string" )
    return S_OK()

  def setJetBeta(self, value):
    try:
      self.jetBeta = value
    except ValueError:
      return S_ERROR( "Jet beta must be string" )
    return S_OK()

  def setJetGamma(self, value):
    try:
      self.jetGamma = value
    except ValueError:
      return S_ERROR( "Jet gamma must be string" )
    return S_OK()

  def setDeltaR(self, value):
    try:
      self.deltaR = value
    except ValueError:
      return S_ERROR( "deltaR has to be a number" )
    return S_OK()

  def setDeltaP(self, value):
    try:
      self.deltaP = value
    except ValueError:
      return S_ERROR( "deltaP has to be a number" )
    return S_OK()

  def setCos_theta_W_max(self, value):
    try:
      self.cos_theta_W_max = value
    except ValueError:
      return S_ERROR( "cos_theta_W_max has to be a number" )
    return S_OK()

  def setSixFermionSample(self, value):
    try:
      self.sixFermionSample = value
    except ValueError:
      return S_ERROR( "sixFermionSample was not given" )
    return S_OK()
  
  def setNrFilesPerJob(self, value):
    try:
      self.nrFilesPerJob = value
    except ValueError:
      return S_ERROR( "nrFilesPerJob was not given" )
    return S_OK()

  def checkConsistency(self):
    """Check if all necessary parameter were defined"""
    
    if not self.deltaR:
      return S_ERROR("deltaR must be given")
    
    if not self.deltaP:
      return S_ERROR("deltaP must be given")

    return S_OK()

  def registerSwitches(self):

    switches = [
      ( "i:", "prodID=", "Set prodID for input files", self.setProdID ),
      ( "x:", "suffix=", "Set suffix for output", self.setSuffix ),
      ( "a:", "algorithm=", "Set algorithm for the jet clustering", self.setAlgorithm ),
      ( "m:", "clusteringMode=", "Set clusteringMode for the jet clustering", self.setClusteringMode ),
      ( "e:", "recombinationScheme=", "Set recombinationScheme for the jet clustering", self.setRecombinationScheme ),
      ( "j:", "jetRadius=", "Set jet radius for the jet clustering", self.setJetRadius ),
      ( "b:", "jetBeta=", "Set beta parameter for the jet clustering", self.setJetBeta ),
      ( "g:", "jetGamma=", "Set gamma parameter the jet clustering", self.setJetGamma ),
      ( "r:", "deltaR=", "Set deltaR for top tagger", self.setDeltaR ),
      ( "p:", "deltaP=", "Set deltaP for top tagger", self.setDeltaP ),
      ( "y:", "cos_theta_W_max=", "Set cos_theta_W_max for top tagger", self.setCos_theta_W_max ),
      ( "k:", "sixFermionSample=", "Set bool sixFermionSample", self.setSixFermionSample ),
      ( "l:", "nrFilesPerJob=", "Set nrFilesPerJob", self.setNrFilesPerJob )
      ]
    
    #Register switches
    for switch in switches:
      Script.registerSwitch( *switch )
      
    #Define a help message
    Script.setUsageMessage( '\n'.join( [ __doc__.split( '\n' )[1],
                                         '\nUsage:',
                                         '  %s [option|cfgfile] ...\n' % Script.scriptName ] ) )
    
def defGridJob(jobName, jobGroup, inputFile):
  '''Defines Grid job'''

  from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
  job = UserJob()
  job.setName(jobName)
  job.setJobGroup(jobGroup)
  job.setCPUTime(86400)
  job.dontPromptMe()
  job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk'])
  
  job.setInputData(inputFile)
  job.setInputSandbox(['LFN:/ilc/user/l/lstroem/topasymmetry/lib_ilcsoft_2017-05-30_gcc62.tar.gz']) #only need LFN: for InputSandbox
  job.setOutputSandbox(["*.out", "*.log"])
  
  outputFile = jobName+".slcio"
  rootFile = jobName+".root"
  
  #job.setOutputData([outputFile, rootFile], jobGroup, "CERN-DST-EOS") #keep some files on the GRID for further processing
  job.setOutputData([rootFile], jobGroup, "CERN-DST-EOS")
  
  return job, outputFile, rootFile

def defMarlin(outputFile, rootFile, **kwargs):
  '''Defines Marlin job'''
  
  ##Jet Settings
  jetAlgo = kwargs.pop('jetAlgo', 'ValenciaPlugin 1.5 1 1')
  jetCluster = kwargs.pop('jetCluster', 'ExclusiveNJets 2')
  jetRecomb = kwargs.pop('jetRecomb', 'E_scheme')

  ##Top Tagger Settings
  deltaR = kwargs.pop('deltaR', 0.01)
  deltaP = kwargs.pop('deltaP', 0.01)
  cos_theta_W_max = kwargs.pop('cos_theta_W_max', 1.0)
  
  ##Sixfermion sample
  sixFermionSample = kwargs.pop('sixFermionSample', True)

  from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin    
  ma = Marlin()
  ma.setVersion('ILCSoft-2017-05-30_gcc62')
  ma.setSteeringFile("/home/lstroem/clicdp/analysis/steering/chain/topasymmetry_wflavourtag/topasymmetry_template_new.xml")
  ma.setGearFile("/home/lstroem/clicdp/gear/clic_ild_cdr.gear")
  ma.setOutputFile(outputFile, rootFile)
  ma.setExtraCLIArguments("--MyTopTaggerProcessorJ2.algorithm=\"{algorithm}\"\
                           --MyTopTaggerProcessorJ2.clusteringMode=\"{clusteringMode}\"\
                           --MyTopTaggerProcessorJ2.recombinationScheme={recombinationScheme}\
                           --MyTopTaggerProcessorJ2.deltaR={deltaR:.2f}\
                           --MyTopTaggerProcessorJ2.deltaP={deltaP:.2f}\
                           --MyTopTaggerProcessorJ2.cos_theta_W_max={cos_theta_W_max:.2f}\
                           --MyTopTaggerProcessorJ2wTrimming.algorithm=\"{algorithm}\"\
                           --MyTopTaggerProcessorJ2wTrimming.clusteringMode=\"{clusteringMode}\"\
                           --MyTopTaggerProcessorJ2wTrimming.recombinationScheme={recombinationScheme}\
                           --MyTopTaggerProcessorJ2wTrimming.deltaR={deltaR:.2f}\
                           --MyTopTaggerProcessorJ2wTrimming.deltaP={deltaP:.2f}\
                           --MyTopTaggerProcessorJ2wTrimming.cos_theta_W_max={cos_theta_W_max:.2f}\
                           --MyTopAsymmetryStoreProcessor.useSixFermionSample={sixFermionSample}\
                           --MyTopAsymmetryStoreProcessor.outFileName={outfile}"
                           .format(algorithm = jetAlgo,
                                   clusteringMode = jetCluster, 
                                   recombinationScheme = jetRecomb,
                                   deltaR = deltaR, 
                                   deltaP = deltaP, 
                                   cos_theta_W_max = cos_theta_W_max,
                                   sixFermionSample = sixFermionSample,
                                   outfile = rootFile)
                          )
  
  return ma

def getInputFiles(prodID):
  '''Returns list of files given prodID'''

  ##Mapping - ProdID vs. Fermions
  prodID2fermions = {6589: 'yyveyx_o',
                     6592: 'yyxyev_o',
                     6595: 'yyuyyc_o',
                     6598: 'yycyyu_o',
                     6601: 'dduyyu_o',
                     6604: 'ssussu_o',
                     6607: 'bbubbu_o',
                     6610: 'ssubbu_o',
                     6613: 'ddcyyc_o',
                     6616: 'sscssc_o',
                     6619: 'sscbbc_o',
                     6622: 'bbcbbc_o',
                     6634: 'yyvlyx_o',
                     6637: 'yyxylv_o',
                     2091: 'qq',
                     3249: 'qq_ln',
                     3243: 'qq_nunu',
                     3246: 'qq_ll',
                     4034: 'qqqq',
                     5527: 'qqqq_nunu',
                     5572: 'qqqq_ll',
                     5594: 'qqqq_lnu',
                     6183: 'qqqq_nunu',
                     6191: 'qqqq_lnu',
                     6646: 'yyveyx_o',
                     6697: 'yyxyev_o',
                     6652: 'yyuyyc_o',
                     6655: 'yycyyu_o',
                     6658: 'dduyyu_o',
                     6661: 'ssussu_o',
                     6664: 'ssubbu_o',
                     6667: 'bbubbu_o',
                     6670: 'ddcyyc_o',
                     6673: 'sscssc_o',
                     6676: 'sscbbc_o',
                     6679: 'bbcbbc_o',
                     6691: 'yyvlyx_o',
                     6694: 'yyxylv_o',
                     7477: 'qq_ln',
                     7480: 'qq_ln'}
  
  ##Setting up LFNS path
  baseName = '/afs/cern.ch/user/l/lstroem/clicdp/analysis/steering/chain/topasymmetry_wflavourtag/submit/{prodID:04.0f}/'.format(prodID = prodID)
  lfnsFile = 'ilc-user-r-rstrom-topasymmetry_chain_flavourtagging_{prodID:04.0f}.lfns'.format(prodID = prodID)
  lfnsPath = baseName+lfnsFile
  
  ##Create LFNS if missing
  inputFilePath = '/ilc/user/r/rstrom/topasymmetry_chain_flavourtagging_{prodID:04.0f}/'.format(prodID = prodID)

  if not os.path.isfile(lfnsPath):
    gLogger.notice('Creating LNFS dir file...')
    print("inputFilePath: ", inputFilePath)
    subprocess.call('dirac-dms-user-lfns -b '+inputFilePath, shell = True)
    subprocess.call('mkdir -p '+baseName, shell = True)
    subprocess.call('mv '+lfnsFile+' '+lfnsPath, shell = True)

  ##Count number of files
  pOut = subprocess.Popen('less '+lfnsPath, shell = True, stdout = subprocess.PIPE)
  out, err = pOut.communicate()

  return  out.split('\n')[:-1]    

def getJetSettings(jetAlgo, jetCluster, jetRecomb):
  '''String conversion for jet settings'''

  jetstr = jetAlgo+"_"+jetCluster+"_"+jetRecomb
  jetstr = string.replace(jetstr, "ValenciaPlugin", "vlc")
  jetstr = string.replace(jetstr, "ExclusiveNJets ", "excl")
  jetstr = string.replace(jetstr, "E_scheme", "Es")
  jetstr = string.replace(jetstr, " ", "_")

  return jetstr

##gLogger
def doLogger(jobName, inputFiles, outputFile, rootFile):
  '''Prints input using dirac command'''

  gLogger.notice("jobNameId: ", jobName)
  gLogger.notice("inputFile: ", inputFiles)
  gLogger.notice("outputFile: ", outputFile)
  gLogger.notice("RootFile: ", rootFile)

def main():
  '''
    This is the script main method, which will hold all the logic.
  '''

  cliParams = Params()
  cliParams.registerSwitches()
  Script.parseCommandLine( ignoreErrors = True )

  consistent = cliParams.checkConsistency()
  if not consistent['OK']:
    gLogger.error("Error checking consistency:", consistent['Message'])
    Script.showHelp()
    dexit(2)
  
  ##Get prodID
  prodID = float(cliParams.prodID)

  ##Get all possible input files
  inputFiles = getInputFiles(prodID)

  ##Get suffix
  suffix = cliParams.suffix
  if suffix: suffix = "_"+suffix 
  
  ##Jet clustering
  algorithm = "ValenciaPlugin {radius:.1f} {beta:.1f} {gamma:.1f}"
  jetRadius = float(cliParams.jetRadius)
  jetBeta = float(cliParams.jetBeta)
  jetGamma = float(cliParams.jetGamma)

  jetCluster = "ExclusiveNJets 2"
  jetRecomb = "E_scheme"

  ##Top tagger
  deltaR = float(cliParams.deltaR)
  deltaP = float(cliParams.deltaP)
  cos_theta_W_max = float(cliParams.cos_theta_W_max)

  ##Sixfermion sample
  sixFermionSample = cliParams.sixFermionSample

  ##Number of files per job
  nrFilesPerJob = int(cliParams.nrFilesPerJob)

  from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
  repDir = "/afs/cern.ch/user/l/lstroem/clicdp/analysis/steering/chain/topasymmetry_wflavourtag/submit/{prodID:04.0f}/rep/".format(prodID = prodID)
  subprocess.call("mkdir -p "+repDir, shell = True)
  dirac = DiracILC(False)#, repDir+"topasymmetry_vlc{suffix}.rep".format(suffix = suffix))

  jetAlgo = algorithm.format(radius = jetRadius, beta = jetBeta, gamma = jetGamma)
  inputFileList = []
  i = 0; j = 1
  for inputFile in inputFiles:
    inputFileList.append(inputFile); i+=1
    if (i >= nrFilesPerJob*j) or (i == len(inputFiles)):  
      
      jobName = "topasymmetry_chain_{jetSettings}_dR{deltaR:.2f}_dP{deltaP:.2f}_cthetaWmax{cos_theta_W_max:.2f}_1Jun2017_part{index}_{prodID:04.0f}".format(jetSettings = getJetSettings(jetAlgo, jetCluster, jetRecomb),
                                                                                                                                                   deltaR = deltaR,
                                                                                                                                                   deltaP = deltaP, 
                                                                                                                                                   cos_theta_W_max = cos_theta_W_max,
                                                                                                                                                   index = j,
                                                                                                                                                   prodID = prodID)
      jobGroup = "topasymmetry_chain_{prodID:04.0f}".format(prodID = prodID)
      job, outputFile, rootFile = defGridJob(jobName, jobGroup, inputFileList)
      
      ##Check if outputfile already exists
      pOutCheck = subprocess.Popen("dirac-dms-lfn-replicas /ilc/user/r/rstrom/"+jobGroup+"/"+rootFile, shell = True, stdout = subprocess.PIPE, stderr=subprocess.PIPE)
      outCheck, errCheck = pOutCheck.communicate()
      #gLogger.notice(jobName)
      if "no such file" not in outCheck.lower():
        gLogger.notice('File exists! Skipping!')
        inputFileList = []
        j+=1  
        continue#continue #use break if only part1, use continue of run over all parts
      #gLogger.notice(jetAlgo)
      #doLogger(jobName, inputFileList, outputFile, rootFile)
      res = job.append(defMarlin(outputFile, rootFile, jetAlgo = jetAlgo, jetCluster = jetCluster, jetRecomb = jetRecomb, deltaR = deltaR, deltaP = deltaP, cos_theta_W_max = cos_theta_W_max, sixFermionSample = sixFermionSample))
  
      if not res['OK']: #Catch if there is an error
        print res['Message'] #Print the error message
        dexit
          
      ##Job submission
      print job.submit(dirac)  
      inputFileList = []
      j+=1
      #break #add break of only part1
  
  gLogger.notice("All done!")
  dexit(0)

if __name__ == "__main__":
  main()
