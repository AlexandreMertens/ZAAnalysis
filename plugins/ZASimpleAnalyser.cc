#include <cp3_llbb/ZAAnalysis/interface/ZATypes.h>
#include <cp3_llbb/ZAAnalysis/interface/Tools.h>
//#include <cp3_llbb/ZAAnalysis/interface/GenStatusFlags.h>
#include <cp3_llbb/ZAAnalysis/interface/ZASimpleAnalyzer.h>
#include <cp3_llbb/ZAAnalysis/interface/ZADileptonCategories.h>

#include <cp3_llbb/Framework/interface/MuonsProducer.h>
#include <cp3_llbb/Framework/interface/ElectronsProducer.h>
#include <cp3_llbb/Framework/interface/JetsProducer.h>
#include <cp3_llbb/Framework/interface/METProducer.h>
#include <cp3_llbb/Framework/interface/HLTProducer.h>
#include <cp3_llbb/Framework/interface/GenParticlesProducer.h>
#include <cp3_llbb/Framework/interface/BinnedValuesJSONParser.h>

#include <Math/PtEtaPhiE4D.h>
#include <Math/LorentzVector.h>
#include <Math/VectorUtil.h>

using namespace ROOT::Math;

using namespace ZAAnalysis;

float ZAAnalysis::DeltaEta(const myLorentzVector& v1, const myLorentzVector& v2){
  return std::abs(v1.Eta() - v2.Eta());
}

bool ZAAnalysis::sortByBtag(const ZAAnalysis::Jet& _jet1, const ZAAnalysis::Jet& _jet2){
      return _jet1.CSVv2 > _jet2.CSVv2;
}

bool ZAAnalysis::sortByPt(const ZAAnalysis::Jet& _jet1, const ZAAnalysis::Jet& _jet2){
      return _jet1.p4.Pt() > _jet2.p4.Pt();
}

void ZAAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup, const ProducersManager& producers, const AnalyzersManager& analyzers, const CategoryManager&) {

  #ifdef _ZA_DEBUG_
    std::cout << "Begin event." << std::endl;
  #endif
  
  ///////////////////////////
  //       ELECTRONS       //
  ///////////////////////////

  #ifdef _ZA_DEBUG_
    std::cout << "Electrons" << std::endl;
  #endif

  const ElectronsProducer& electrons = producers.get<ElectronsProducer>(m_electrons_producer);

  for(uint16_t ielectron = 0; ielectron < electrons.p4.size(); ielectron++){
    if( electrons.p4[ielectron].Pt() > m_electronPtCut && abs(electrons.p4[ielectron].Eta()) < m_electronEtaCut && fabs(electrons.dca[ielectron])<m_electronDcaCut ){
      
      Lepton m_lepton(
          electrons.p4[ielectron], 
          ielectron, 
          electrons.charge[ielectron],electrons.dca[ielectron], 
          true, false,
          electrons.ids[ielectron][m_electronVetoIDName],
          electrons.ids[ielectron][m_electronLooseIDName],
          electrons.ids[ielectron][m_electronMediumIDName],
          electrons.ids[ielectron][m_electronTightIDName],
          electrons.relativeIsoR03_withEA[ielectron]
      );
      
      if( electrons.ids[ielectron][m_electronLooseIDName] ) // No isolation cut now 
      {
          isolatedElectrons.push_back(m_lepton);
          leptons.push_back(m_lepton);  
      }      

      // if electron good enough for veto, adding to the collection vetoLepton
      else if (electrons.ids[ielectron][m_electronVetoIDName]) 
      {
          vetoLeptons.push_back(m_lepton);
      }

    }
  }

  ///////////////////////////
  //       MUONS           //
  ///////////////////////////
  
  #ifdef _ZA_DEBUG_
    std::cout << "Muons" << std::endl;
  #endif

  const MuonsProducer& muons = producers.get<MuonsProducer>(m_muons_producer);

  for(uint16_t imuon = 0; imuon < muons.p4.size(); imuon++){
    if(muons.p4[imuon].Pt() > m_muonPtCut && abs(muons.p4[imuon].Eta()) < m_muonEtaCut && fabs(muons.dca[imuon])<m_muonDcaCut){
      
      Lepton m_lepton(
          muons.p4[imuon], 
          imuon,
          muons.charge[imuon],muons.dca[imuon],
          false, true,
          muons.isLoose[imuon], // isVeto => for muons, re-use isLoose
          muons.isLoose[imuon],
          muons.isMedium[imuon],
          muons.isTight[imuon],
          muons.relativeIsoR04_deltaBeta[imuon],
          muons.relativeIsoR04_deltaBeta[imuon] < m_muonLooseIsoCut,
          muons.relativeIsoR04_deltaBeta[imuon] < m_muonTightIsoCut
      );

      if( muons.isLoose[imuon] && muons.relativeIsoR04_deltaBeta[imuon] < m_muonLooseIsoCut)
      {
          isolatedMuons.push_back(m_lepton);
          leptons.push_back(m_lepton);
      }
      // if muon good enough for veto, adding to the collection vetoLepton
      else if (muons.isLoose[imuon])
      {
          vetoLeptons.push_back(m_lepton);
      }
    }
  }


  ///////////////////////////
  //       JETS            //
  ///////////////////////////

  #ifdef _ZA_DEBUG_
    std::cout << "Jets" << std::endl;
  #endif

  const JetsProducer& jets = producers.get<JetsProducer>(m_jets_producer);

  // First find the jets passing kinematic cuts and save them as Jet objects

  uint16_t jetCounter(0);
  for(uint16_t ijet = 0; ijet < jets.p4.size(); ijet++){
    // Save the jets that pass the kinematic cuts
    if( abs(jets.p4[ijet].Eta()) < m_jetEtaCut && jets.p4[ijet].Pt() > m_jetPtCut){
      Jet m_jet;
      
      m_jet.p4 = jets.p4[ijet];
      m_jet.idx = ijet;
      m_jet.isIDLoose = jets.passLooseID[ijet]; 
      m_jet.isIDTight = jets.passTightID[ijet]; 
      m_jet.isTLV = jets.passTightLeptonVetoID[ijet];
      m_jet.CSVv2 = jets.getBTagDiscriminant(ijet, m_jetCSVv2Name);
      m_jet.isBWPL = m_jet.CSVv2 > m_jetCSVv2L;
      m_jet.isBWPM = m_jet.CSVv2 > m_jetCSVv2M;
      m_jet.isBWPT = m_jet.CSVv2 > m_jetCSVv2T;
      
      // Save minimal DR(l,j)
      // Looping over all leptons that pass at least the veto criteria and check the distance with the jet.      

      m_jet.minDRjl = std::numeric_limits<float>::max();   
 
      for(uint16_t il = 0; il < leptons.size(); il++)
      {
          const Lepton& m_lepton = leptons[il];
          float DR = (float) VectorUtil::DeltaR(jets.p4[ijet], m_lepton.p4);
          if( DR < m_jet.minDRjl)
              m_jet.minDRjl = DR;
      }

      for(uint16_t il = 0; il < vetoLeptons.size(); il++)
      {
          const Lepton& m_lepton = vetoLeptons[il];
          float DR = (float) VectorUtil::DeltaR(jets.p4[ijet], m_lepton.p4);
          if( DR < m_jet.minDRjl)
              m_jet.minDRjl = DR;
      }

      if (m_jet.minDRjl > m_jetDRleptonCut) 
      {
          selJets.push_back(m_jet); 
          jetCounter++;
          if (m_jet.isBWPM) selBjetsM.push_back(m_jet);
      }
    }
  }

  //////////////////////////////
  //       FATJETS            //
  //////////////////////////////

  #ifdef _ZA_DEBUG_
    std::cout << "fatJets" << std::endl;
  #endif

  const FatJetsProducer& fatjets = producers.get<FatJetsProducer>("fat_jets");

  // First find the fat jets passing kinematic cuts and save them as Jet objects

  uint16_t fatjetCounter(0);
  for(uint16_t ifatjet = 0; ifatjet < fatjets.p4.size(); ifatjet++){
    // Save the fat jets that pass the kinematic cuts 
    // FIXME : currently same cuts as for the jets
    if( abs(fatjets.p4[ifatjet].Eta()) < m_jetEtaCut && fatjets.p4[ifatjet].Pt() > m_jetPtCut){
      FatJet m_fatjet;

      m_fatjet.p4 = fatjets.p4[ifatjet];
      m_fatjet.isIDLoose = fatjets.passLooseID[ifatjet];
      m_fatjet.isIDTight = fatjets.passTightID[ifatjet];
      m_fatjet.isTLV = fatjets.passTightLeptonVetoID[ifatjet];
      m_fatjet.CSVv2 = fatjets.getBTagDiscriminant(ifatjet, m_jetCSVv2Name);
      m_fatjet.isBWPL = m_fatjet.CSVv2 > m_jetCSVv2L;
      m_fatjet.isBWPM = m_fatjet.CSVv2 > m_jetCSVv2M;
      m_fatjet.isBWPT = m_fatjet.CSVv2 > m_jetCSVv2T;

      m_fatjet.softdrop_mass = fatjets.softdrop_mass[ifatjet];
      m_fatjet.trimmed_mass = fatjets.trimmed_mass[ifatjet];
      m_fatjet.pruned_mass = fatjets.pruned_mass[ifatjet];
      m_fatjet.filtered_mass = fatjets.filtered_mass[ifatjet];

      m_fatjet.tau1 = fatjets.tau1[ifatjet];
      m_fatjet.tau2 = fatjets.tau2[ifatjet];
      m_fatjet.tau3 = fatjets.tau3[ifatjet];

      m_fatjet.nSDSubjets = fatjets.softdrop_subjets_p4[ifatjet].size();

      //std::cout << "looping over "; 
      //std::cout <<  fatjets.softdrop_subjets_p4[ifatjet].size()  << std::endl;

      std::vector<SubJet> m_subjets; 

      for(uint16_t isubjet = 0; isubjet < fatjets.softdrop_subjets_p4[ifatjet].size(); isubjet++){
          SubJet m_subjet;
          m_subjet.p4 = fatjets.softdrop_subjets_p4[ifatjet].at(isubjet);
          m_subjet.CSVv2 = fatjets.getSoftDropBTagDiscriminant(ifatjet, isubjet, m_jetCSVv2Name);
          m_subjet.isBWPL = m_subjet.CSVv2 > m_jetCSVv2L;
          m_subjet.isBWPM = m_subjet.CSVv2 > m_jetCSVv2M;
          m_subjet.isBWPT = m_subjet.CSVv2 > m_jetCSVv2T;
          m_subjet.EFrac = m_subjet.p4.E() / m_fatjet.p4.E();
          m_subjets.push_back(m_subjet);
      }
     
      m_fatjet.subjets = m_subjets;    
 
      if (m_subjets.size() >= 2){
          m_fatjet.subjetDR = ROOT::Math::VectorUtil::DeltaR(m_subjets[0].p4, m_subjets[1].p4);
      }
      else {m_fatjet.subjetDR = 10;}     

            
      // Save minimal DR(l,j)
      // Looping over all leptons that pass at least the veto criteria and check the distance with the jet.      

      m_fatjet.minDRjl = std::numeric_limits<float>::max();

      for(uint16_t il = 0; il < leptons.size(); il++)
      {
          const Lepton& m_lepton = leptons[il];
          float DR = (float) VectorUtil::DeltaR(fatjets.p4[ifatjet], m_lepton.p4);
          if( DR < m_fatjet.minDRjl)
              m_fatjet.minDRjl = DR;
      }

      for(uint16_t il = 0; il < vetoLeptons.size(); il++)
      {
          const Lepton& m_lepton = vetoLeptons[il];
          float DR = (float) VectorUtil::DeltaR(jets.p4[ifatjet], m_lepton.p4);
          if( DR < m_fatjet.minDRjl)
              m_fatjet.minDRjl = DR;
      }

      if (m_fatjet.minDRjl > m_fatjetDRleptonCut)
      {
          selFatJets.push_back(m_fatjet);
          fatjetCounter++;
      }
    }
  }

        
  /////////////////////////////////
  //    Trigger : Two Leptons    //
  /////////////////////////////////

  #ifdef _ZA_DEBUG_
  std::cout << "Trigger" << std::endl;
  #endif

  if (producers.exists("hlt")) {

#define ZA_HLT_DEBUG (false)

      const HLTProducer& hlt = producers.get<HLTProducer>("hlt");

      if (hlt.paths.empty()) {
#if ZA_HLT_DEBUG
          std::cout << "No HLT path triggered for this event. Skipping HLT matching." << std::endl;
#endif
          goto after_hlt_matching;
      }

#if ZA_HLT_DEBUG
      std::cout << "HLT path triggered for this event:" << std::endl;
      for (const std::string& path: hlt.paths) {
          std::cout << "\t" << path << std::endl;
      }
#endif

      /*
       * Try to match `lepton` with an online object, using a deltaR and a deltaPt cut
       * Returns the index inside the HLTProducer collection, or -1 if no match is found.
       */
      auto matchOfflineLepton = [&](Lepton& lepton) {

          if (lepton.hlt_already_matched)
              return lepton.hlt_idx;

#if ZA_HLT_DEBUG
          std::cout << "Trying to match offline lepton: " << std::endl;
          std::cout << "\tMuon? " << lepton.isMu << " ; Pt: " << lepton.p4.Pt() << " ; Eta: " << lepton.p4.Eta() << " ; Phi: " << lepton.p4.Phi() << " ; E: " << lepton.p4.E() << std::endl;
#endif

          float min_dr = std::numeric_limits<float>::max();

          int16_t index = -1;
          for (size_t hlt_object = 0; hlt_object < hlt.object_p4.size(); hlt_object++) {

              float dr = VectorUtil::DeltaR(lepton.p4, hlt.object_p4[hlt_object]);
              float dpt_over_pt = std::abs(lepton.p4.Pt() - hlt.object_p4[hlt_object].Pt()) / lepton.p4.Pt();

              if (dr > m_hltDRCut)
                  continue;

              if (dpt_over_pt > m_hltDPtCut)
                  continue;

              if (dr < min_dr) {
                  min_dr = dr;
                  index = hlt_object;
              }
          }

#if ZA_HLT_DEBUG
          if (index != -1) {
              std::cout << "\033[32mMatched with online object:\033[00m" << std::endl;
              std::cout << "\tPDG Id: " << hlt.object_pdg_id[index] << " ; Pt: " << hlt.object_p4[index].Pt() << " ; Eta: " << hlt.object_p4[index].Eta() << " ; Phi: " << hlt.object_p4[index].Phi() << " ; E: " << hlt.object_p4[index].E() << std::endl;
              std::cout << "\tΔR: " << min_dr << " ; ΔPt / Pt: " << std::abs(lepton.p4.Pt() - hlt.object_p4[index].Pt()) / lepton.p4.Pt() << std::endl;
          } else {
              std::cout << "\033[31mNo match found\033[00m" << std::endl;
          }
#endif

          lepton.hlt_idx = index;
          lepton.hlt_already_matched = true;

          return index;
      };

      for(uint16_t i = 0; i < leptons.size(); i++){
          matchOfflineLepton(leptons[i]);
      }
      
  }

  after_hlt_matching:

  //////////////////////////////////////
  //    Two Leptons, Two Jets Case    //
  //////////////////////////////////////

  #ifdef _ZA_DEBUG_
    std::cout << "Dileptons-dijets" << std::endl;
  #endif

  // Reconstruction of the Z candidate
  // Basic selection: Two selected leptons that matches the trigger and two jets
  
  if (leptons.size() == 2){

    const Lepton& l1 = leptons[0];
    const Lepton& l2 = leptons[1];

    if (l1.p4.Pt() > l2.p4.Pt())
      {
      dilep_ptOrdered.push_back(l1);
      dilep_ptOrdered.push_back(l2);
      }
    else
      {
      dilep_ptOrdered.push_back(l2);
      dilep_ptOrdered.push_back(l1);
      }

    DiLepton m_diLepton(l1, l2);

    //m_diLepton.p4 = l1.p4 + l2.p4;
    m_diLepton.isElEl = dilep_ptOrdered[0].isEl && dilep_ptOrdered[1].isEl;
    m_diLepton.isElMu = dilep_ptOrdered[0].isEl && dilep_ptOrdered[1].isMu;
    m_diLepton.isMuEl = dilep_ptOrdered[0].isMu && dilep_ptOrdered[1].isEl;
    m_diLepton.isMuMu = dilep_ptOrdered[0].isMu && dilep_ptOrdered[1].isMu;
    m_diLepton.isOS = l1.charge != l2.charge;
    m_diLepton.isSF = m_diLepton.isElEl || m_diLepton.isMuMu;
    m_diLepton.triggerMatched = (leptons[0].hlt_idx > -1 && leptons[1].hlt_idx > -1);

    if (m_diLepton.isMuMu) {
        //std::cout << "eta :"  << TMath::Abs(dilep_ptOrdered[0].p4.Eta()) << " " << TMath::Abs(dilep_ptOrdered[1].p4.Eta()) << std::endl;
        //std::cout << m_hlt_scale_factors["HLTDoubleMuonSFs"].get({TMath::Abs(dilep_ptOrdered[0].p4.Eta()), TMath::Abs(dilep_ptOrdered[1].p4.Eta())})[0] << std::endl;
        m_diLepton.triggerSF = m_hlt_scale_factors["HLTDoubleMuonSFs"].get({TMath::Abs(dilep_ptOrdered[0].p4.Eta()), TMath::Abs(dilep_ptOrdered[1].p4.Eta())})[0];
        }
    else  {m_diLepton.triggerSF = 1;}


    diLeptons.push_back(m_diLepton);

    // Selection: Two selected leptons that matches the trigger and two jets

    if (diLeptons.size() >= 1 && selJets.size() >=2) {
         
      sort(selJets.begin(), selJets.end(), sortByBtag);

      const Jet& jet1 = selJets[0];
      const Jet& jet2 = selJets[1];

      if (jet1.p4.Pt() > jet2.p4.Pt())
        {
        dijet_ptOrdered.push_back(jet1);
        dijet_ptOrdered.push_back(jet2);
        }
      else
        {
        dijet_ptOrdered.push_back(jet2);
        dijet_ptOrdered.push_back(jet1);
        }

      DiJet m_diJet(jet1, jet2);
      diJets.push_back(m_diJet);


      // chapter 3: event variable

      DiLepDiJet m_diLepDiJet(m_diLepton, m_diJet);

      m_diLepDiJet.minDRjl = std::min( {
        (float) VectorUtil::DeltaR(l1.p4, jet1.p4),
        (float) VectorUtil::DeltaR(l1.p4, jet2.p4),
        (float) VectorUtil::DeltaR(l2.p4, jet1.p4),
        (float) VectorUtil::DeltaR(l2.p4, jet2.p4)
        } );
      m_diLepDiJet.maxDRjl = std::max( {
        (float) VectorUtil::DeltaR(l1.p4, jet1.p4),
        (float) VectorUtil::DeltaR(l1.p4, jet2.p4),
        (float) VectorUtil::DeltaR(l2.p4, jet1.p4),
        (float) VectorUtil::DeltaR(l2.p4, jet2.p4)
        } );
      m_diLepDiJet.minDEtajl = std::min( {
        DeltaEta(l1.p4, jet1.p4),
        DeltaEta(l1.p4, jet2.p4),
        DeltaEta(l2.p4, jet1.p4),
        DeltaEta(l2.p4, jet2.p4)
        } );
      m_diLepDiJet.maxDEtajl = std::max( {
        DeltaEta(l1.p4, jet1.p4),
        DeltaEta(l1.p4, jet2.p4),
        DeltaEta(l2.p4, jet1.p4),
        DeltaEta(l2.p4, jet2.p4)
        } );
      m_diLepDiJet.minDPhijl = std::min( {
        (float) VectorUtil::DeltaPhi(l1.p4, jet1.p4),
        (float) VectorUtil::DeltaPhi(l1.p4, jet2.p4),
        (float) VectorUtil::DeltaPhi(l2.p4, jet1.p4),
        (float) VectorUtil::DeltaPhi(l2.p4, jet2.p4)
        } );
      m_diLepDiJet.maxDPhijl = std::max( {
        (float) VectorUtil::DeltaPhi(l1.p4, jet1.p4),
        (float) VectorUtil::DeltaPhi(l1.p4, jet2.p4),
        (float) VectorUtil::DeltaPhi(l2.p4, jet1.p4),
        (float) VectorUtil::DeltaPhi(l2.p4, jet2.p4)
        } );

      diLepDiJets.push_back(m_diLepDiJet);
    }
    // Selection: Two selected leptons that matches the trigger and a fatjet
    if (diLeptons.size() >= 1 && selFatJets.size() >=1) {

      const FatJet& m_fatjet = selFatJets[0];
      DiLepFatJet m_diLepFatJet(m_diLepton, m_fatjet);
      diLepFatJets.push_back(m_diLepFatJet);
    }
  }

}


void ZAAnalyzer::registerCategories(CategoryManager& manager, const edm::ParameterSet& config) {
  manager.new_category<ZAAnalysis::ElElCategory>("elel", "Category with leading leptons as two electrons", config);
  manager.new_category<ZAAnalysis::ElMuCategory>("elmu", "Category with leading leptons as electron, muon", config);
  manager.new_category<ZAAnalysis::MuElCategory>("muel", "Category with leading leptons as muon, electron", config);
  manager.new_category<ZAAnalysis::MuMuCategory>("mumu", "Category with leading leptons as two muons", config);
}

#include <FWCore/PluginManager/interface/PluginFactory.h>
DEFINE_EDM_PLUGIN(ExTreeMakerAnalyzerFactory, ZAAnalyzer, "za_analyzer");
