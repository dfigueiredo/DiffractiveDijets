# Auto generated configuration file
# using: 
# Revision: 1.303.2.7 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: GeneratorInterface/ExhumeInterface/python/ExHuME_CEPDijetsGG_M100_7TeV_cff.py --step GEN,SIM --beamspot Realistic7TeVCollision2010B --conditions START42_V17D::All --pileup NoPileUp --datamix NODATAMIXER --customise GeneratorInterface/ExhumeInterface/MyShowerLib.customise --eventcontent RAWSIM --datatier GEN-SIM -n 30 --no_exec
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
    annotation = cms.untracked.string('GeneratorInterface/ExhumeInterface/python/ExHuME_CEPDijetsGG_M100_7TeV_cff.py nevts:30'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('ExHuME_CEPDijetsGG_M100_7TeV_cff_py_GEN_SIM.root'),
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

process.generator = cms.EDFilter("ExhumeGeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    ExhumeProcess = cms.PSet(
        ThetaMin = cms.double(0.3),
        MassRangeHigh = cms.double(9999.0),
        ProcessType = cms.string('GG'),
        MassRangeLow = cms.double(100.0)
    ),
    ExhumeParameters = cms.PSet(
        HiggsVev = cms.double(246.0),
        HiggsMass = cms.double(120.0),
        MuonMass = cms.double(0.1057),
        LambdaQCD = cms.double(80.0),
        StrangeMass = cms.double(0.19),
        Survive = cms.double(0.03),
        TauMass = cms.double(1.77),
        BottomMass = cms.double(4.6),
        Rg = cms.double(1.2),
        AlphaEw = cms.double(0.0072974),
        TopMass = cms.double(175.0),
        ZMass = cms.double(91.187),
        PDF = cms.double(20250),
        B = cms.double(4.0),
        CharmMass = cms.double(1.42),
        MinQt2 = cms.double(0.64),
        WMass = cms.double(80.33)
    ),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(7000.0),
    maxEventsToPrint = cms.untracked.int32(2),
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring()
    )
)


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
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from GeneratorInterface.ExhumeInterface.MyShowerLib
from GeneratorInterface.ExhumeInterface.MyShowerLib import customise 

#call to customisation function customise imported from GeneratorInterface.ExhumeInterface.MyShowerLib
process = customise(process)

# End of customisation functions
