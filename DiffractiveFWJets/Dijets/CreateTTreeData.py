#!/usr/bin/env python
import os
import random
import time

def crabCall(uiworkingdir):
    print ''
    print '##########################################'
    print 'Creating job...'
    print '##########################################'
    print ''
    os.system('crab -create')
    print ''
    print '##########################################'
    print 'Submiting job...'
    print '##########################################'
    os.system('crab -submit -c '+ uiworkingdir)

def cmsRunCall():
    print ''
    print '##########################################'
    print 'Starting CMSSW...'
    print '##########################################'
    print ''
    os.system('cmsRun pset.py')
    print ''

def ConfigHandler(output, parametersList, dataset, lumis, lumisjob, jsonfile, uiworkingdir, userremotedir, sewhitelist, weight):
    pset_py1 = """
import FWCore.ParameterSet.Config as cms
class config: pass
config.runOnMC = False


process = cms.Process("DijetsAnalysis")

##########################################
# Import of Standard CMSSW Configurations
##########################################

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# --------------------------------------------------------------

##############
# Global Tags
##############

# MC
#####
if (config.runOnMC):
    process.GlobalTag.globaltag ='START38_V12::All'

# DATA
#######
else:
# GR_R_38X_V13A (3_8_3)
# GR10_P_V9 (3_6_1_p4)

    process.GlobalTag.globaltag ='FT_R_38X_V14A::All'

# Other statements
process.MessageLogger.cerr.FwkReport.reportEvery = 1

## -------------------------------------------------------------

###########
# Triggers 
###########

process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

## Jet Trigger Filter
######################

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltJets = copy.deepcopy(hltHighLevel)
process.hltJets.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
#process.hltJets.HLTPaths = ['HLT_Jet15U','HLT_Jet30U','HLT_Jet50U']
process.hltJets.HLTPaths = ['HLT_Jet15U']


# Pre-Scale
############

#process.prescale = cms.EDFilter("Prescaler",
#    prescaleFactor = cms.int32(50)
#)

# L1 Trigger
#############

process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.L1T1=process.hltLevel1GTSeed.clone()
process.L1T1.L1TechTriggerSeeding = cms.bool(True)

if (config.runOnMC):process.L1T1.L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)')
else:process.L1T1.L1SeedsLogicalExpression = cms.string('0 AND NOT (36 OR 37 OR 38 OR 39)')


## -------------------------------------------------------------

#################
# JetCorrections 
#################

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')


## -------------------------------------------------------------

###############################
# Removing Beam Scraping Events
###############################

process.noscraping = cms.EDFilter("FilterOutScraping",
                                applyfilter = cms.untracked.bool(True),
                                debugOn = cms.untracked.bool(True),
                                numtrack = cms.untracked.uint32(10),
                                thresh = cms.untracked.double(0.25)
                                )


## -------------------------------------------------------------

#################
# Vertex Filter
#################

process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15"), 
   filter = cms.bool(True),   
)

## -------------------------------------------------------------

##########
# Source
##########

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
	'file:/export/home/dfigueiredo/DataRecoTest/PYTHIA6_QCD_Pthat_30_7TeV_cff_py_RAW2DIGI_L1Reco_RECO.root'

    ),
#inputCommands = cms.untracked.vstring(
#          'keep *',
#          'drop *_particleFlowBlock_*_*',
#          'drop *_particleFlow_*_*',
#          'drop *_particleFlowClusterECAL_*_*',
#          'drop *_particleFlowClusterHCAL_*_*',
#          'drop *_particleFlowClusterPS_*_*'
#           ),
skipBadFiles = cms.untracked.bool(True)
)

process.TFileService = cms.Service("TFileService",
fileName = cms.string('@@OUTPUT@@')
)



"""

    pset_py2 = """
###################
# Loading Plug-ins
###################

#
# JetID
#######
process.load("RecoJets.JetProducers.ak5JetID_cfi")
process.CaloJetsLooseId = cms.EDProducer("CaloJetIdSelector",
                src = cms.InputTag("ak5CaloJets"),
                idLevel = cms.string("LOOSE"),
                jetIDMap = cms.untracked.InputTag("ak5JetID"))

#process.CaloJetsTightId = cms.EDProducer("CaloJetIdSelector",
#                src = cms.InputTag("ak5CaloJets"),
#                idLevel = cms.string("TIGHT"),
#                jetIDMap = cms.untracked.InputTag("ak5JetID"))

#from RecoJets.JetProducers.ak5JetID_cfi import *
#module CaloJetsLooseId = cms.EDProducer("CaloJetIdSelector",
#                         src = cms.InputTag("ak5CaloJets"),
#                         idLevel = cms.string("LOOSE"),
#                         jetIDMap = cms.untracked.InputTag("ak5JetID")
#                         )

# Analyzers and Filters
#-----------------------

process.LeadingJetsfilter = cms.EDFilter("LeadingJetsFilter", 
                                          calAlgoFilter = cms.untracked.string("@@ALGOJET@@"), 
                                          PtCut1 = cms.double(@@PTCUT1@@), 
                                          PtCut2 = cms.double(@@PTCUT2@@),
                                          JetsForward = cms.untracked.bool(@@FORWARD@@)
                                         )

process.efficiencyStep1 = cms.EDAnalyzer('EfficiencyStudies', Weight = cms.double(@@WEIGHT@@))
process.efficiencyStep2 = cms.EDAnalyzer('EfficiencyStudies', Weight = cms.double(@@WEIGHT@@))
process.efficiencyStep3 = cms.EDAnalyzer('EfficiencyStudies', Weight = cms.double(@@WEIGHT@@))

process.createTTreeStep1 = cms.EDAnalyzer('CreateTTreeData',
                                      AccessMCInfo = cms.untracked.bool(False),
                                      AccessPileUpInfo = cms.untracked.bool(False),
                                      VertexTag = cms.untracked.string("offlinePrimaryVertices"),
                                      calAlgoJet = cms.untracked.string("@@ALGOJET@@"),
                                      HFNoise = cms.double(@@HFNOISE@@),
                                      TrackPtCut = cms.double (@@TRACKPT@@),
                                      Weight = cms.double(@@WEIGHT@@)              
                                    )

process.createTTreeStep2 = cms.EDAnalyzer('CreateTTreeData',
                                      AccessMCInfo = cms.untracked.bool(False),
                                      AccessPileUpInfo = cms.untracked.bool(False),
                                      VertexTag = cms.untracked.string("offlinePrimaryVertices"),
                                      calAlgoJet = cms.untracked.string("@@ALGOJET@@"),
                                      HFNoise = cms.double(@@HFNOISE@@),
                                      TrackPtCut = cms.double (@@TRACKPT@@),
                                      Weight = cms.double(@@WEIGHT@@)               
                                    )

process.createTTreeStep3 = cms.EDAnalyzer('CreateTTreeData',
                                      AccessMCInfo = cms.untracked.bool(False),
                                      AccessPileUpInfo = cms.untracked.bool(False),
                                      VertexTag = cms.untracked.string("offlinePrimaryVertices"),
                                      calAlgoJet = cms.untracked.string("@@ALGOJET@@"),
                                      HFNoise = cms.double(@@HFNOISE@@),
                                      TrackPtCut = cms.double (@@TRACKPT@@),
                                      Weight = cms.double(@@WEIGHT@@)              
                                    )

## -------------------------------------------------------------

#####################
# Services and Path
#####################

# Summary
#--------
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound'))

# Check Plug-ins Status (Accepted or failed)
#-------------------------------------------
#process.Tracer = cms.Service("Tracer")

# Path to Execute
#-----------------
process.p = cms.Path(process.hltPhysicsDeclared*process.noscraping*process.primaryVertexFilter*(process.ak5JetID+process.CaloJetsLooseId)*(process.efficiencyStep1+process.createTTreeStep1)*process.hltJets*(process.efficiencyStep2+process.createTTreeStep2)*process.LeadingJetsfilter*(process.efficiencyStep3+process.createTTreeStep3))

"""

    crab_cfg = """
[CRAB]

jobtype = cmssw
scheduler = glite
#server_name = cnaf

[CMSSW]
datasetpath=@@DATASET@@
pset=pset.py
output_file=@@OUTPUT@@
lumi_mask=@@JSONFILE@@
total_number_of_lumis =@@LUMIS@@
lumis_per_job = @@LUMISJOB@@

[USER]
ui_working_dir=@@UIWORKINGDIR@@
return_data = 1
copy_data = 0
publish_data=0
email = dmf@cern.ch

#ui_working_dir=@@UIWORKINGDIR@@
#return_data = 0
#copy_data = 1
#publish_data=0
#email = dmf@cern.ch
#check_user_remote_dir = 1
#storage_element = se-dcache.hepgrid.uerj.br 
#storage_path = /srm/managerv2?SFN=/pnfs/hepgrid.uerj.br/data/cms/store/user/dmf
#storage_port = 8443
#user_remote_dir = @@USEREMOTE@@


[GRID]
proxy_server = myproxy.cern.ch
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@
#se_black_list = T2_US_Nebraska, T2_DE_RWTH, T2_US_MIT
#se_black_list = T2_KR_KNU
virtual_organization = cms


"""
    print ''
    print '##########################################'
    print 'Removing old stuff...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')
    print ''
    print '##########################################'
    print 'Creating PSet file...'
    print '##########################################'


    pset_py1 = pset_py1.replace('@@LUMIS@@',lumis,-1)
    pset_py1 = pset_py1.replace('@@LUMISJOB@@',lumisjob,-1)
    pset_py1 = pset_py1.replace('@@JSONFILE@@',jsonfile,-1)
    pset_py1 = pset_py1.replace('@@OUTPUT@@',output,-1)
    pset_py = pset_py1

    for param in parametersList:
        pset_py2Bias = pset_py2
        pset_py2Bias = pset_py2Bias.replace('@@PTCUT1@@',param["ptcut1"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@PTCUT2@@',param["ptcut2"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@ALGOJET@@',param["algojet"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@FORWARD@@',param["forward"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@NAME@@',param["name"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@HFNOISE@@',param["hfnoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@TRACKPT@@',param["trackpt"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@WEIGHT@@',weight,-1)
        pset_py = pset_py + pset_py2Bias


    pset_pyFile = open('pset.py','a+')
    pset_pyFile.write(pset_py)
    pset_pyFile.close()
    print ''
    print '##########################################'
    print 'Creating CRAB Config file...'
    print '##########################################'
    crab_cfg = crab_cfg.replace('@@DATASET@@',dataset,-1)
    crab_cfg = crab_cfg.replace('@@OUTPUT@@',output,-1)
    crab_cfg = crab_cfg.replace('@@LUMIS@@',lumis,-1)
    crab_cfg = crab_cfg.replace('@@LUMISJOB@@',lumisjob,-1)
    crab_cfg = crab_cfg.replace('@@JSONFILE@@',jsonfile,-1)
    crab_cfg = crab_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    crab_cfg = crab_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
    crab_cfg = crab_cfg.replace('@@USEREMOTE@@',userremotedir,-1)

    #use or not se_white_list
    if (sewhitelist == ''):
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','#',-1)
    else:
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','',-1)
    crab_cfgFile = open('crab.cfg','a+')
    crab_cfgFile.write(crab_cfg)
    crab_cfgFile.close()
    ##crab or cmsRun
    crabCall(uiworkingdir)
    #cmsRunCall()
    #clear
    print ''
    print '##########################################'
    print 'Backuping crab.cfg and pset.py...'
    print '##########################################'
    os.system('mkdir -p '+uiworkingdir+'/crabCallerBackup')
    os.system('cp crab.cfg '+uiworkingdir+'/crabCallerBackup/.')
    os.system('cp pset.py '+uiworkingdir+'/crabCallerBackup/.')
    print ''
    print 'Backups saved: '+uiworkingdir+'/crabCallerBackup/'
    print ''
    print ''
    print '##########################################'
    print 'Cleaning...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')


print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Starting submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'


parametersList = []
CreateTree = {"name":"CreateTree","hfnoise":"4","algojet":"CaloJetsLooseId","trackpt":"0.3","forward":"False","ptcut1":"35","ptcut2":"35"}
parametersList.append(CreateTree)

##############
# Real Data
##############

dataset = '/MinimumBias/Commissioning10-Sep17ReReco_v2/RECO'  
uiworkingdir = '/tmp/dmf/MinimumBias3'
userremotedir = '/Data/MinimumBias3'
jsonfile = 'Cert_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON.json'
#lumis ='-1'
#lumisjob = '100'
lumis ='1'
lumisjob = '1'
sewhitelist = ''
output = 'MB.root'
weight = '1.'
ConfigHandler(output, parametersList, dataset, lumis, lumisjob, jsonfile, uiworkingdir, userremotedir, sewhitelist, weight)

#dataset = '/JetMETTau/Run2010A-Sep17ReReco_v2/RECO'  
#uiworkingdir = '/tmp/dmf/JetMETTau'
#userremotedir = '/Data/JetMETTau'
#jsonfile = 'Cert_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON.json'
#lumis ='-1'
#lumisjob = '100'
#lumis ='1'
#lumisjob = '5'
#sewhitelist = ''
#output = 'JetMETTau.root'
#weight = '1.'
#ConfigHandler(output, parametersList, dataset, lumis, lumisjob, jsonfile, uiworkingdir, userremotedir, sewhitelist, weight)

#dataset = '/JetMET/Run2010A-Sep17ReReco_v2/RECO'  
#uiworkingdir = '/tmp/dmf/JetMET'
#userremotedir = '/Data/JetMET'
#jsonfile = 'Cert_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON.json'
#lumis ='-1'
#lumisjob = '100'
#sewhitelist = ''
#output = 'JetMET.root'
#weight = '1.'
#ConfigHandler(output, parametersList, dataset, lumis, lumisjob, jsonfile, uiworkingdir, userremotedir, sewhitelist, weight)

#dataset = '/MinimumBias/Commissioning10--GoodColSlim-Sep17Skim-v1/RECO'  
#uiworkingdir = '/tmp/dmf/Bias'
#userremotedir = 'Data/Bias'
#jsonfile = 'Cert_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON.json'
#lumis ='-1'
#lumisjob = '100'
#sewhitelist = ''
#output = 'minBias.root'
#weight = '1.'
#ConfigHandler(output, parametersList, dataset, lumis, lumisjob, jsonfile, uiworkingdir, userremotedir, sewhitelist, weight)


##############################################################################################################################
##############################################################################################################################
##############################################################################################################################

print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'



