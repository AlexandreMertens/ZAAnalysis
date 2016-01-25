import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from cp3_llbb.Framework import Framework

process = Framework.create(False, eras.Run2_25ns, '76X_mcRun2_asymptotic_v12', cms.PSet(
    za = cms.PSet(
        type = cms.string('za_analyzer'),
        prefix = cms.string('za_'),
        enable = cms.bool(True),
        parameters = cms.PSet(
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
    ), redoJEC=False
    )

Framework.schedule(process, ['za'])

#process.source.firstEvent = cms.untracked.uint32(13083444)
#process.source.firstLuminosityBlock = cms.untracked.uint32(52386)

# Tricky gen event from /store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/00000/0014DC94-DC5C-E511-82FB-7845C4FC39F5.root
# First one is g g -> t tbar with one W -> bbar c
# Second is b bar -> t tbar semi-leptonic
#process.source.eventsToProcess = cms.untracked.VEventRange(
        #'1:52386:13083444',
        #'1:34020:8496854'
        #)

# Other tricky gen events, with lots of ISR
# From file:/nfs/scratch/fynu/swertz/CMSSW_7_4_15/src/cp3_llbb/TTAnalysis/test/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_miniAODv2_oneFile.root
#process.source.eventsToProcess = cms.untracked.VEventRange(
        #'1:321521:80300260',
        #'1:357590:89308562',
        #'1:387992:96901374'
        #)
#process.MessageLogger.cerr.FwkReport.reportEvery = 1



# mH=200 mA=100
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-200_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/22F46581-0EAD-E511-8EBB-0025907B4EC0.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-200_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/3C2F4BCE-0EAD-E511-90FC-90B11C2ACF20.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-200_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/3E0FAFCE-0EAD-E511-9872-002590E2DD10.root' )
process.framework.output = cms.string('output_signal_200_100.root')

'''
# mH=250 mA=100
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-250_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/0625DC2A-83C2-E511-8384-0CC47A57D086.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-250_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/78A7E41E-83C2-E511-AC63-003048322C5D.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-250_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/EC1D6545-83C2-E511-B437-002590D9D88C.root' )
process.framework.output = cms.string('output_signal_250_100.root')

# mH=300 mA=50
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/14A8DF66-E0C1-E511-91FD-E41D2D08DD70.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/1E963366-E0C1-E511-88AD-FACADE00008D.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/22F119B3-D0C1-E511-AF01-002590596490.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/2E8D9F28-E2C1-E511-AD39-E41D2D08E0F0.root' )
process.framework.output = cms.string('output_signal_300_50.root')

# mH=300 mA=100
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/80823A43-18AD-E511-81D9-0CC47A13CFC0.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/8ED6483C-18AD-E511-91B4-0CC47A4C8E3C.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-100_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/F670A43D-18AD-E511-BB60-0CC47A78A446.root' )
process.framework.output = cms.string('output_signal_300_100.root')

# mH=300 mA=200
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/1EF31389-69BF-E511-ADB8-00266CFFC51C.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/6A4855B5-69BF-E511-A375-002590747D9C.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-300_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/B66BDA66-69BF-E511-9131-001F29652580.root' )
process.framework.output = cms.string('output_signal_300_200.root')

# mH=500 mA=50
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/740B1137-EFB1-E511-855B-0002C92A1020.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/8CC14A67-F3B1-E511-9904-A0000420FE80.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-50_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/F2A1013C-EFB1-E511-9C1F-0CC47A4D764A.root')
process.framework.output = cms.string('output_signal_500_50.root')

# mH=500 mA=200
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/068357D7-30C1-E511-96E9-002590A80DA4.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/2ABD3AD2-30C1-E511-9279-002590A8881E.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/44A27ED0-30C1-E511-B772-B499BAAC078E.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/6839F7E7-30C1-E511-91CC-001E67398791.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/B847F3C4-30C1-E511-AADF-549F35AD8BA2.root' )
process.framework.output = cms.string('output_signal_500_200.root')

# mH=500 mA=300
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-300_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/04FD48FB-21B1-E511-8D5F-0CC47A4D761A.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-300_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/0A4CC6FB-21B1-E511-B766-002618FDA28E.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-500_MA-300_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/8C8B9DFE-21B1-E511-A4DE-0CC47A4D76B2.root' )
process.framework.output = cms.string('output_signal_500_300.root')

# mH=800 mA=200
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/2C700538-4CC0-E511-94CB-6C3BE5B59210.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/62A6A8F6-4BC0-E511-8F21-001F2907EF28.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/904C43EC-4BC0-E511-94E5-6C3BE5B51238.root' )
process.framework.output = cms.string('output_signal_800_200.root')

# mH=800 mA=400
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-400_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/06129A17-E2C1-E511-BA61-001517FBE024.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-400_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/40D949FC-E2C1-E511-A63B-00266CFFCD6C.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-400_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/58D02512-E2C1-E511-A0F4-F01FAFE157DF.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-400_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/7EC3154A-E2C1-E511-9CBB-00261894390E.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-400_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/AECABBC1-E2C1-E511-9176-00266CFFC0C0.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-800_MA-400_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/6E1DFE57-A7C1-E511-A02C-003048FFD796.root')
process.framework.output = cms.string('output_signal_800_400.root')

# mH=1000 mA=200
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-1000_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/2A257410-DAB0-E511-80F0-0CC47A4C8E2A.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-1000_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/4ED335BE-D9B0-E511-9B77-003048F5B6B0.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-1000_MA-200_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/606D66C8-D9B0-E511-A8D8-0CC47A6C115A.root' )
process.framework.output = cms.string('output_signal_1000_200.root')

# mH=1000 mA=200
process.source.fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/108FDA74-FEC1-E511-A3C3-001E67DDC254.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/12281D38-C3C0-E511-8341-F01FAFE15CBD.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/9EEC4893-FEC1-E511-AFA4-00266CF94EAC.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/ACCD725C-8BC1-E511-BDAD-00266CF331D0.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/D020A6A2-C7C1-E511-B895-003048F2E65E.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/D2B7656D-B8C1-E511-A7C1-0CC47A78A418.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/E0913D6A-05C2-E511-9284-001E67456EBE.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/EC04A45E-FEC1-E511-A124-001EC94BFFEB.root',
       '/store/mc/RunIIFall15MiniAODv1/HToZATo2L2B_MH-3000_MA-2000_13TeV-madgraph-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/F874ED2C-07C1-E511-BFC6-001E67DFF4F6.root' )
process.framework.output = cms.string('output_signal_3000_2000.root')
'''
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
