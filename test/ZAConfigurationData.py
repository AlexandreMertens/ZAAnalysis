import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

framework = Framework.Framework(True, eras.Run2_25ns, globalTag='74X_dataRun2_v2', processName= 'RECO')
framework.addAnalyzer('za',cms.PSet(
        type = cms.string('za_analyzer'),
        prefix = cms.string('za_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
            # Producers
            electronsProducer = cms.string('electrons'),
            muonsProducer = cms.string('muons'),
            jetsProducer = cms.string('jets'),
            metProducer = cms.string('met'),
            nohfMETProducer = cms.string('nohf_met'),
            # Here are the default value (just to show what is configurable) 
            electronPtCut = cms.untracked.double(20),
            electronEtaCut = cms.untracked.double(2.5),
            electronVetoIDName = cms.untracked.string('cutBasedElectronID-Spring15-25ns-V1-standalone-veto'),
            electronLooseIDName = cms.untracked.string('cutBasedElectronID-Spring15-25ns-V1-standalone-loose'),
            electronMediumIDName = cms.untracked.string('cutBasedElectronID-Spring15-25ns-V1-standalone-medium'),
            electronTightIDName = cms.untracked.string('cutBasedElectronID-Spring15-25ns-V1-standalone-tight'),
            
            muonPtCut = cms.untracked.double(20),
            muonEtaCut = cms.untracked.double(2.4),
            muonLooseIsoCut = cms.untracked.double(.25), # Loose cut recommended for dilepton analysis
            muonTightIsoCut = cms.untracked.double(.15),

            jetPtCut = cms.untracked.double(30),
            jetEtaCut = cms.untracked.double(2.5),
            #jetPUID = cms.untracked.double(-9999999),
            jetDRleptonCut = cms.untracked.double(0.3),
            jetID = cms.untracked.string('tight'), # not tightLeptonVeto since DeltaR(l,j) cut should be enough
            jetCSVv2Name = cms.untracked.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'),
            jetCSVv2L = cms.untracked.double(0.605),
            jetCSVv2M = cms.untracked.double(0.89),
            jetCSVv2T = cms.untracked.double(0.97),
            fatjetDRleptonCut = cms.untracked.double(0.8),

            hltDRCut = cms.untracked.double(0.3), # DeltaR cut for trigger matching
            hltDPtCut = cms.untracked.double(0.5), #Delta(Pt)/Pt cut for trigger matching
            ),
        categories_parameters = cms.PSet(
            MllCutSF = cms.untracked.double(20),
            MllCutDF = cms.untracked.double(20),
            HLTDoubleMuon = cms.untracked.vstring('HLT_Mu17_TrkIsoVVL_(Tk)?Mu8_TrkIsoVVL_DZ_v.*'),
            HLTDoubleEG = cms.untracked.vstring('HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v.*'),
            HLTMuonEG = cms.untracked.vstring('HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v.*', 'HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v.*'),
            ),
        )
    )

#framework.redoJEC()
framework.doSystematics(['jec', 'jer'])
process = framework.create()

process.framework.producers.muons.parameters.applyRochester = cms.untracked.bool(True)

#Framework.schedule(process, ['za']) 

process.source.fileNames = cms.untracked.vstring(
        '/store/data/Run2015B/DoubleMuon/MINIAOD/05Oct2015-v1/40000/F6B9EFF5-BE6E-E511-A5DD-002618943964.root'
        )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5000))
