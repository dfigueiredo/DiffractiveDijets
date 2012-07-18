# Auto generated configuration file
# using: 
# Revision: 1.303.2.7 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: GeneratorInterface/PomwigInterface/python/POMWIG_DPEDijets_7TeV_Pt_30_cff.py --step GEN,SIM --beamspot Realistic7TeVCollision2010B --conditions START42_V17D::All --pileup NoPileUp --datamix NODATAMIXER --customise GeneratorInterface/PomwigInterface/MyShowerLib.customise --eventcontent RAWSIM --datatier GEN-SIM -n 30 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic7TeVCollision2010B_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(30)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.303.2.7 $'),
    annotation = cms.untracked.string('GeneratorInterface/PomwigInterface/python/POMWIG_DPEDijets_7TeV_Pt_30_cff.py nevts:30'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('POMWIG_DPEDijets_7TeV_Pt_30_cff_py_GEN_SIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START42_V17D::All'

process.generator = cms.EDFilter("PomwigGeneratorFilter",
    doMPInteraction = cms.bool(False),
    useJimmy = cms.bool(False),
    herwigHepMCVerbosity = cms.untracked.bool(False),
    filterEfficiency = cms.untracked.double(1.0),
    herwigVerbosity = cms.untracked.int32(1),
    comEnergy = cms.double(7000.0),
    printCards = cms.untracked.bool(True),
    crossSection = cms.untracked.double(-1.0),
    maxEventsToPrint = cms.untracked.int32(2),
    survivalProbability = cms.double(0.05),
    HerwigParameters = cms.PSet(
        DPEInclusiveJets = cms.vstring('NSTRU      = 14         ! H1 Pomeron Fit B', 
            'Q2WWMN     = 1E-6       ! Minimum |t|', 
            'Q2WWMX     = 4.0        ! Maximum |t|', 
            'YWWMIN     = 1E-6       ! Minimum xi', 
            'YWWMAX     = 0.2        ! Maximum xi', 
            'IPROC      = 11500      ! Process PomPom -> jets', 
            'PTMIN      = 30         ! 2->2 PT min', 
            'MODPDF(1)  = -1         ! Set MODPDF', 
            'MODPDF(2)  = -1         ! Set MODPDF'),
        parameterSets = cms.vstring('DPEInclusiveJets')
    ),
    h1fit = cms.int32(2),
    doPDGConvert = cms.bool(False),
    diffTopology = cms.int32(0)
)


process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from GeneratorInterface.PomwigInterface.MyShowerLib
from GeneratorInterface.PomwigInterface.MyShowerLib import customise 

#call to customisation function customise imported from GeneratorInterface.PomwigInterface.MyShowerLib
process = customise(process)

# End of customisation functions
