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


def ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist):
    pset_py1 = """
import FWCore.ParameterSet.Config as cms
class config: pass
config.runOnMC = True

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

# START38_V9::All (Old)
if (config.runOnMC):
    process.GlobalTag.globaltag ='START38_V12::All'

# DATA
#######
else:
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


# Analyzers and Filters
#-----------------------


process.DijetsAnalysisNoFilter = cms.EDAnalyzer('DijetsAnalysis',
                                         HFNoise = cms.double(@@HFNOISE@@),
                                         CASTORNoise = cms.double(@@CASTORNOISE@@),
                                         ntrackCut = cms.int32(@@NTRACK@@),
                                         TrackPtCut = cms.double(@@TRACKPT@@)
                                       )


process.LeadingJetsfilter@@NAME@@ = cms.EDFilter("LeadingJetsFilter", 
                                          calAlgoFilter = cms.untracked.string("@@ALGOJET@@"), 
                                          PtCut1 = cms.double(@@PTCUT1@@), 
                                          PtCut2 = cms.double(@@PTCUT2@@),
                                          JetsForward = cms.untracked.bool(@@FORWARD@@)
                                         )

process.DijetsAnalysis@@NAME@@ = cms.EDAnalyzer('DijetsAnalysis',
                                         HFNoise = cms.double(@@HFNOISE@@),
                                         CASTORNoise = cms.double(@@CASTORNOISE@@),
                                         ntrackCut = cms.int32(@@NTRACK@@),
                                         TrackPtCut = cms.double(@@TRACKPT@@)
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

#MyOwn

process.default_step = cms.Path(process.hltPhysicsDeclared*process.noscraping*process.primaryVertexFilter*process.hltJets)
process.noFilter_step = cms.Path(process.DijetsAnalysisNoFilter)
process.Filter_step@@NAME@@ = cms.Path(process.LeadingJetsfilter@@NAME@@*process.DijetsAnalysis@@NAME@@)

process.schedule@@NAME@@ = cms.Schedule(process.default_step,process.noFilter_step,process.Filter_step@@NAME@@)

"""

    crab_cfg = """
[CRAB]

jobtype = cmssw
scheduler = glite
#server_name = cnaf

[CMSSW]
total_number_of_events = @@NEVENTS@@
events_per_job = @@EJOB@@
datasetpath=@@DATASET@@
pset=pset.py
output_file=@@OUTPUT@@

[USER]
ui_working_dir=@@UIWORKINGDIR@@
return_data = 1
copy_data = 0
publish_data=0
email = dmf@cern.ch

[GRID]
proxy_server = myproxy.cern.ch
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@
se_black_list = T2_US_UCSD
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


    pset_py1 = pset_py1.replace('@@EJOB@@',ejob,-1)
    pset_py1 = pset_py1.replace('@@OUTPUT@@',output,-1)
    pset_py1 = pset_py1.replace('@@NEVENTS@@',nevent,-1)
    pset_py = pset_py1
    for param in parametersList:
        pset_py2Bias = pset_py2
        pset_py2Bias = pset_py2Bias.replace('@@PTCUT1@@',param["ptcut1"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@PTCUT2@@',param["ptcut2"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@JETNOISE@@',param["jetnoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@ALGOJET@@',param["algojet"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@CALONOISE@@',param["calonoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@FORWARD@@',param["forward"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@NAME@@',param["name"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@HFNOISE@@',param["hfnoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@CASTORNOISE@@',param["castornoise"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@NTRACK@@',param["ntrack"],-1)
        pset_py2Bias = pset_py2Bias.replace('@@TRACKPT@@',param["trackpt"],-1)
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
    crab_cfg = crab_cfg.replace('@@EJOB@@',ejob,-1)
    crab_cfg = crab_cfg.replace('@@NEVENTS@@',nevent,-1)
    crab_cfg = crab_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    crab_cfg = crab_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
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

LeadingJets5 = {"name":"NotForwardNTrack5","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"5","forward":"False","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJets5)

LeadingJetsForward5 = {"name":"ForwardNTrack5","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"5","forward":"True","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJetsForward5)

LeadingJets20 = {"name":"NotForwardNTrack20","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"20","forward":"False","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJets20)

LeadingJetsForward20 = {"name":"ForwardNTrack20","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"20","forward":"True","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJetsForward20)

LeadingJets35 = {"name":"NotForwardNTrack35","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"35","forward":"False","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJets35)

LeadingJetsForward35 = {"name":"ForwardNTrack35","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"35","forward":"True","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJetsForward35)

LeadingJetsInf = {"name":"NotForwardNTrackInf","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"999999","forward":"False","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJetsInf)

LeadingJetsForwardInf = {"name":"ForwardNTrackInf","calonoise":"4","hfnoise":"4","jetnoise":"4","castornoise":"10","algojet":"ak5CaloJets",
"trackpt":"0.3","ntrack":"999999","forward":"True","ptcut1":"35","ptcut2":"35"}
parametersList.append(LeadingJetsForwardInf)


##############
# Diffractive
##############
#dataset = '/SDDiJetsPlus_Pt30-pomwig/Spring10-START3X_V26_S09_E7TeV_L69E30_BX156-v1/GEN-SIM-RECO'
#uiworkingdir = '/tmp/dmf/Signal'
#nevent ='100000'
#ejob = '500'
#sewhitelist = ''
#output = 'signal.root'
#ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

#################
# non-diffractive
#################


dataset = '/QCD_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_15to30'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_15to30.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

dataset = '/QCD_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_30to50'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_30to50.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

dataset = '/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_50to80'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_50to80.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

dataset = '/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_80to120'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_80to120.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

dataset = '/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_120to170'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_120to170.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

dataset = '/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_170to300'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_170to300.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

dataset = '/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO'
uiworkingdir = '/tmp/dmf/local/qcd_pt_300to470'
nevent ='1000000'
ejob = '5000'
sewhitelist = ''
output = 'qcd_pt_300to470.root'
ConfigHandler(output, parametersList, dataset, ejob, nevent, uiworkingdir, sewhitelist)

print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'



