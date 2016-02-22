#include <cp3_llbb/Framework/interface/MuonsProducer.h>
#include <cp3_llbb/Framework/interface/ElectronsProducer.h>
#include <cp3_llbb/Framework/interface/HLTProducer.h>

#include <cp3_llbb/ZAAnalysis/interface/ZADileptonCategories.h>
#include <cp3_llbb/ZAAnalysis/interface/ZASimpleAnalyzer.h>

#include <cp3_llbb/ZAAnalysis/interface/ZATypes.h>

using namespace ZAAnalysis;

// ***** ***** *****
// Dilepton El-El category
// ***** ***** *****
bool ElElCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
  const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");
  return electrons.p4.size() >= 2;
}

bool ElElCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {
  
  //const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");

  // If at least one DiLepton of highest Pt and of type ElEl among all ID pairs is found, keep event in this category

  return true;
}

void ElElCategory::register_cuts(CutManager& manager) {
    
    manager.new_cut("fire_trigger_Ele17_Ele12", "HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ*");          
    manager.new_cut(baseStrCategory, baseStrCategory);
    manager.new_cut(baseStrMllCut, baseStrMllCut);
    manager.new_cut(baseStrDiLeptonIsOS, baseStrDiLeptonIsOS);
    manager.new_cut(baseStrDileptonIsIDMM, baseStrDileptonIsIDMM);
    manager.new_cut(baseStrDileptonIsIDTT, baseStrDileptonIsIDTT);
    manager.new_cut(baseStrDileptonIsoLL, baseStrDileptonIsoLL);
    manager.new_cut(baseStrLooseZCandidate, baseStrLooseZCandidate);
    manager.new_cut(baseStrTightZCandidate, baseStrTightZCandidate);
    manager.new_cut(baseStrDiJetBWP_ML, baseStrDiJetBWP_ML);
    manager.new_cut(baseStrDiJetBWP_MM, baseStrDiJetBWP_MM);
    manager.new_cut(baseStrDiJetBWP_TM, baseStrDiJetBWP_TM);
    manager.new_cut(baseStrOneJet,baseStrOneJet);
    manager.new_cut(baseStrTwoJets,baseStrTwoJets);
    manager.new_cut(baseStrTwoJetsExcl,baseStrTwoJetsExcl);
    manager.new_cut(baseStrOneBjet,baseStrOneBjet);
    manager.new_cut(baseStrTwoBjets,baseStrTwoBjets);
    manager.new_cut(baseStrTwoBjetsExcl,baseStrTwoBjetsExcl);
    manager.new_cut(baseStrThreeJets,baseStrThreeJets);
    manager.new_cut(baseStrThreeBjets,baseStrThreeBjets);
    manager.new_cut(baseStrOneFatJet,baseStrOneFatJet);
    manager.new_cut(baseStrTwoSubJets,baseStrTwoSubJets);
    manager.new_cut(baseStrOneBFatJetM,baseStrOneBFatJetM);
    manager.new_cut(baseStrOneBFatJetT,baseStrOneBFatJetT);
    manager.new_cut(baseStrTwoBSubJetsLL,baseStrTwoBSubJetsLL);
    manager.new_cut(baseStrTwoBSubJetsMM,baseStrTwoBSubJetsMM);

}

void ElElCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {
  
  const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");
  //const HLTProducer& hlt = producers.get<HLTProducer>("hlt");

  for (const std::string& path: hlt.paths) {
    if (path.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") != std::string::npos) manager.pass_cut("fire_trigger_Ele17_Ele12");
  }
    
  if(za.diLeptons.size() >= 1) {
    const DiLepton& m_diLepton = za.diLeptons[0];

    if(m_diLepton.isElEl) {
      manager.pass_cut(baseStrCategory);
              
      if(m_diLepton.p4.M() > m_MllCutSF)
        manager.pass_cut(baseStrMllCut);

      if(m_diLepton.isMM) manager.pass_cut(baseStrDileptonIsIDMM);
      if(m_diLepton.isTT) manager.pass_cut(baseStrDileptonIsIDTT);
      if(m_diLepton.isIsoLL) manager.pass_cut(baseStrDileptonIsoLL);

      if(m_diLepton.isOS)
      {
        manager.pass_cut(baseStrDiLeptonIsOS);
        if (m_diLepton.p4.M() > m_lowLooseZcut  && m_diLepton.p4.M() < m_highLooseZcut )
        {
          manager.pass_cut(baseStrLooseZCandidate);
          if (m_diLepton.p4.M() > m_lowTightZcut  && m_diLepton.p4.M() < m_highTightZcut )
            manager.pass_cut(baseStrTightZCandidate);
        }
      }

      if (za.selJets.size() >= 1) manager.pass_cut(baseStrOneJet);
      if (za.selJets.size() >= 2) manager.pass_cut(baseStrTwoJets);
      if (za.selJets.size() == 2) manager.pass_cut(baseStrTwoJetsExcl);
      if (za.selJets.size() >= 3) manager.pass_cut(baseStrThreeJets);

      if (za.selBjetsM.size() >= 1) manager.pass_cut(baseStrOneBjet);
      if (za.selBjetsM.size() >= 2) manager.pass_cut(baseStrTwoBjets);
      if (za.selBjetsM.size() == 2) manager.pass_cut(baseStrTwoBjetsExcl);
      if (za.selBjetsM.size() >= 3) manager.pass_cut(baseStrThreeBjets);

      if (za.diJets.size() >= 1){
        const DiJet& m_diJet = za.diJets[0];
        if (m_diJet.isML) manager.pass_cut(baseStrDiJetBWP_ML);
        if (m_diJet.isMM) manager.pass_cut(baseStrDiJetBWP_MM);
        if (m_diJet.isTM) manager.pass_cut(baseStrDiJetBWP_TM);
      }
      
      if (za.selFatJets.size() >= 1){
        manager.pass_cut(baseStrOneFatJet);
        const FatJet& m_fatJet = za.selFatJets[0];
        if (m_fatJet.nSDSubjets >= 2){
          manager.pass_cut(baseStrTwoSubJets);
          if (m_fatJet.isBWPM) manager.pass_cut(baseStrOneBFatJetM);
          if (m_fatJet.isBWPT) manager.pass_cut(baseStrOneBFatJetT);
          if (m_fatJet.nBtaggedSDSubjetsLL) manager.pass_cut(baseStrTwoBSubJetsLL);
          if (m_fatJet.nBtaggedSDSubjetsMM) manager.pass_cut(baseStrTwoBSubJetsMM);
        }
      }
    }
  }
}


// ***** ***** *****
// Dilepton Mu-Mu category
// ***** ***** *****
bool MuMuCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
  const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
  return muons.p4.size() >= 2;
}

bool MuMuCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {
  
  //const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");

  // It at least one DiLepton of highest Pt and of type MuMu among all ID pairs is found, keep event in this category

  return true;
}

void MuMuCategory::register_cuts(CutManager& manager) {
 
    manager.new_cut("fire_trigger_Mu17_Mu8", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*");
    manager.new_cut("fire_trigger_Mu17_TkMu8", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*");
    manager.new_cut("fire_trigger_IsoMu27", "HLT_IsoMu27_v*"); 
          
    manager.new_cut(baseStrCategory, baseStrCategory);
    manager.new_cut(baseStrMllCut, baseStrMllCut);
    manager.new_cut(baseStrDiLeptonIsOS, baseStrDiLeptonIsOS);
    manager.new_cut(baseStrDileptonIsIDMM, baseStrDileptonIsIDMM);
    manager.new_cut(baseStrDileptonIsIDTT, baseStrDileptonIsIDTT);
    manager.new_cut(baseStrDileptonIsoLL, baseStrDileptonIsoLL);
    manager.new_cut(baseStrLooseZCandidate, baseStrLooseZCandidate);
    manager.new_cut(baseStrTightZCandidate, baseStrTightZCandidate);
    manager.new_cut(baseStrDiJetBWP_ML, baseStrDiJetBWP_ML);
    manager.new_cut(baseStrDiJetBWP_MM, baseStrDiJetBWP_MM);
    manager.new_cut(baseStrDiJetBWP_TM, baseStrDiJetBWP_TM);
    manager.new_cut(baseStrOneJet,baseStrOneJet);
    manager.new_cut(baseStrTwoJets,baseStrTwoJets);
    manager.new_cut(baseStrTwoJetsExcl,baseStrTwoJetsExcl);
    manager.new_cut(baseStrOneBjet,baseStrOneBjet);
    manager.new_cut(baseStrTwoBjets,baseStrTwoBjets);
    manager.new_cut(baseStrTwoBjetsExcl,baseStrTwoBjetsExcl);
    manager.new_cut(baseStrThreeJets,baseStrThreeJets);
    manager.new_cut(baseStrThreeBjets,baseStrThreeBjets);
    manager.new_cut(baseStrOneFatJet,baseStrOneFatJet);
    manager.new_cut(baseStrTwoSubJets,baseStrTwoSubJets);
    manager.new_cut(baseStrOneBFatJetM,baseStrOneBFatJetM);
    manager.new_cut(baseStrOneBFatJetT,baseStrOneBFatJetT);
    manager.new_cut(baseStrTwoBSubJetsLL,baseStrTwoBSubJetsLL);
    manager.new_cut(baseStrTwoBSubJetsMM,baseStrTwoBSubJetsMM);


}

void MuMuCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {
  
  const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");
  //const HLTProducer& hlt = producers.get<HLTProducer>("hlt");

  for (const std::string& path: hlt.paths) 
  {
      if (path.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") != std::string::npos) manager.pass_cut("fire_trigger_Mu17_Mu8");
      if (path.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v") != std::string::npos) manager.pass_cut("fire_trigger_Mu17_TkMu8");
      if (path.find("HLT_IsoMu27_v") != std::string::npos) manager.pass_cut("fire_trigger_IsoMu27");
  }


  if(za.diLeptons.size() >= 1) {
    const DiLepton& m_diLepton = za.diLeptons[0];
          
    if(m_diLepton.isMuMu) {
      manager.pass_cut(baseStrCategory );

      if(m_diLepton.p4.M() > m_MllCutSF)
        manager.pass_cut(baseStrMllCut);

      if(m_diLepton.isMM) manager.pass_cut(baseStrDileptonIsIDMM);
      if(m_diLepton.isTT) manager.pass_cut(baseStrDileptonIsIDTT);
      if(m_diLepton.isIsoLL) manager.pass_cut(baseStrDileptonIsoLL);
              
      if(m_diLepton.isOS)
      {
        manager.pass_cut(baseStrDiLeptonIsOS);
        if (m_diLepton.p4.M() > m_lowLooseZcut  && m_diLepton.p4.M() < m_highLooseZcut )
        {
          manager.pass_cut(baseStrLooseZCandidate);
          if (m_diLepton.p4.M() > m_lowTightZcut  && m_diLepton.p4.M() < m_highTightZcut )
            manager.pass_cut(baseStrTightZCandidate);
        }
      } 
      if (za.selJets.size() >= 1) manager.pass_cut(baseStrOneJet);
      if (za.selJets.size() >= 2) manager.pass_cut(baseStrTwoJets);
      if (za.selJets.size() == 2) manager.pass_cut(baseStrTwoJetsExcl);
      if (za.selJets.size() >= 3) manager.pass_cut(baseStrThreeJets);

      if (za.selBjetsM.size() >= 1) manager.pass_cut(baseStrOneBjet);
      if (za.selBjetsM.size() >= 2) manager.pass_cut(baseStrTwoBjets);
      if (za.selBjetsM.size() == 2) manager.pass_cut(baseStrTwoBjetsExcl);
      if (za.selBjetsM.size() >= 3) manager.pass_cut(baseStrThreeBjets);

      if (za.diJets.size() >= 1) {
        const DiJet& m_diJet = za.diJets[0];
        if (m_diJet.isML) manager.pass_cut(baseStrDiJetBWP_ML);
        if (m_diJet.isMM) manager.pass_cut(baseStrDiJetBWP_MM);
        if (m_diJet.isTM) manager.pass_cut(baseStrDiJetBWP_TM);
      }
      if (za.selFatJets.size() >= 1){
        manager.pass_cut(baseStrOneFatJet);
        const FatJet& m_fatJet = za.selFatJets[0];
        if (m_fatJet.nSDSubjets >= 2){
          manager.pass_cut(baseStrTwoSubJets);
          if (m_fatJet.isBWPM) manager.pass_cut(baseStrOneBFatJetM);
          if (m_fatJet.isBWPT) manager.pass_cut(baseStrOneBFatJetT);
          if (m_fatJet.nBtaggedSDSubjetsLL) manager.pass_cut(baseStrTwoBSubJetsLL);
          if (m_fatJet.nBtaggedSDSubjetsMM) manager.pass_cut(baseStrTwoBSubJetsMM);
        }
      }
    }
  }
}

// ***** ***** *****
// Dilepton Mu-El category
// ***** ***** *****

bool MuElCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
  const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");
  const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
  return muons.p4.size()>=1 &&   electrons.p4.size() >= 1;
}

bool MuElCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {

  //const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");
return true;
}

void MuElCategory::register_cuts(CutManager& manager) {


    manager.new_cut(baseStrCategory, baseStrCategory);
    manager.new_cut(baseStrMllCut, baseStrMllCut);
    manager.new_cut(baseStrDiLeptonIsOS, baseStrDiLeptonIsOS);
    manager.new_cut(baseStrDileptonIsIDMM, baseStrDileptonIsIDMM);
    manager.new_cut(baseStrDileptonIsIDTT, baseStrDileptonIsIDTT);
    manager.new_cut(baseStrDileptonIsoLL, baseStrDileptonIsoLL);
    manager.new_cut(baseStrLooseZCandidate, baseStrLooseZCandidate);
    manager.new_cut(baseStrTightZCandidate, baseStrTightZCandidate);
    manager.new_cut(baseStrDiJetBWP_ML, baseStrDiJetBWP_ML);
    manager.new_cut(baseStrDiJetBWP_MM, baseStrDiJetBWP_MM);
    manager.new_cut(baseStrDiJetBWP_TM, baseStrDiJetBWP_TM);
    manager.new_cut(baseStrOneJet,baseStrOneJet);
    manager.new_cut(baseStrTwoJets,baseStrTwoJets);
    manager.new_cut(baseStrTwoJetsExcl,baseStrTwoJetsExcl);
    manager.new_cut(baseStrOneBjet,baseStrOneBjet);
    manager.new_cut(baseStrTwoBjets,baseStrTwoBjets);
    manager.new_cut(baseStrTwoBjetsExcl,baseStrTwoBjetsExcl);
    manager.new_cut(baseStrThreeJets,baseStrThreeJets);
    manager.new_cut(baseStrThreeBjets,baseStrThreeBjets);
    manager.new_cut(baseStrOneFatJet,baseStrOneFatJet);
    manager.new_cut(baseStrTwoSubJets,baseStrTwoSubJets);
    manager.new_cut(baseStrOneBFatJetM,baseStrOneBFatJetM);
    manager.new_cut(baseStrOneBFatJetT,baseStrOneBFatJetT);
    manager.new_cut(baseStrTwoBSubJetsLL,baseStrTwoBSubJetsLL);
    manager.new_cut(baseStrTwoBSubJetsMM,baseStrTwoBSubJetsMM);


}

void MuElCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {

  const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");
  //const HLTProducer& hlt = producers.get<HLTProducer>("hlt");


  if(za.diLeptons.size() >= 1) {
    const DiLepton& m_diLepton = za.diLeptons[0];
    
    if(m_diLepton.isMuEl) {
      manager.pass_cut(baseStrCategory );

      if(m_diLepton.p4.M() > m_MllCutSF)
        manager.pass_cut(baseStrMllCut);

      if(m_diLepton.isMM) manager.pass_cut(baseStrDileptonIsIDMM);
      if(m_diLepton.isTT) manager.pass_cut(baseStrDileptonIsIDTT);
      if(m_diLepton.isIsoLL) manager.pass_cut(baseStrDileptonIsoLL);

      if(m_diLepton.isOS)
      {
        manager.pass_cut(baseStrDiLeptonIsOS);
        if (m_diLepton.p4.M() > m_lowLooseZcut  && m_diLepton.p4.M() < m_highLooseZcut )
        {
          manager.pass_cut(baseStrLooseZCandidate);
          if (m_diLepton.p4.M() > m_lowTightZcut  && m_diLepton.p4.M() < m_highTightZcut )
            manager.pass_cut(baseStrTightZCandidate);
        }
      }
      if (za.selJets.size() >= 1) manager.pass_cut(baseStrOneJet);
      if (za.selJets.size() >= 2) manager.pass_cut(baseStrTwoJets);
      if (za.selJets.size() == 2) manager.pass_cut(baseStrTwoJetsExcl);
      if (za.selJets.size() >= 3) manager.pass_cut(baseStrThreeJets);

      if (za.selBjetsM.size() >= 1) manager.pass_cut(baseStrOneBjet);
      if (za.selBjetsM.size() >= 2) manager.pass_cut(baseStrTwoBjets);
      if (za.selBjetsM.size() == 2) manager.pass_cut(baseStrTwoBjetsExcl);
      if (za.selBjetsM.size() >= 3) manager.pass_cut(baseStrThreeBjets);

      if (za.diJets.size() >= 1) {
        const DiJet& m_diJet = za.diJets[0];
        if (m_diJet.isML) manager.pass_cut(baseStrDiJetBWP_ML);
        if (m_diJet.isMM) manager.pass_cut(baseStrDiJetBWP_MM);
        if (m_diJet.isTM) manager.pass_cut(baseStrDiJetBWP_TM);
      }
      if (za.selFatJets.size() >= 1){
        manager.pass_cut(baseStrOneFatJet);
        const FatJet& m_fatJet = za.selFatJets[0];
        if (m_fatJet.nSDSubjets >= 2){
          manager.pass_cut(baseStrTwoSubJets);
          if (m_fatJet.isBWPM) manager.pass_cut(baseStrOneBFatJetM);
          if (m_fatJet.isBWPT) manager.pass_cut(baseStrOneBFatJetT);
          if (m_fatJet.nBtaggedSDSubjetsLL) manager.pass_cut(baseStrTwoBSubJetsLL);
          if (m_fatJet.nBtaggedSDSubjetsMM) manager.pass_cut(baseStrTwoBSubJetsMM);
        }
      }
    }
  }
}






bool ElMuCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
  const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");
  const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
  return muons.p4.size()>=1 &&   electrons.p4.size() >= 1;
}

bool ElMuCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {

    //const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");
    return true;
}

void ElMuCategory::register_cuts(CutManager& manager) {


    manager.new_cut(baseStrCategory, baseStrCategory);
    manager.new_cut(baseStrMllCut, baseStrMllCut);
    manager.new_cut(baseStrDiLeptonIsOS, baseStrDiLeptonIsOS);
    manager.new_cut(baseStrDileptonIsIDMM, baseStrDileptonIsIDMM);
    manager.new_cut(baseStrDileptonIsIDTT, baseStrDileptonIsIDTT);
    manager.new_cut(baseStrDileptonIsoLL, baseStrDileptonIsoLL);
    manager.new_cut(baseStrLooseZCandidate, baseStrLooseZCandidate);
    manager.new_cut(baseStrTightZCandidate, baseStrTightZCandidate);
    manager.new_cut(baseStrDiJetBWP_ML, baseStrDiJetBWP_ML);
    manager.new_cut(baseStrDiJetBWP_MM, baseStrDiJetBWP_MM);
    manager.new_cut(baseStrDiJetBWP_TM, baseStrDiJetBWP_TM);
    manager.new_cut(baseStrOneJet,baseStrOneJet);
    manager.new_cut(baseStrTwoJets,baseStrTwoJets);
    manager.new_cut(baseStrTwoJetsExcl,baseStrTwoJetsExcl);
    manager.new_cut(baseStrOneBjet,baseStrOneBjet);
    manager.new_cut(baseStrTwoBjets,baseStrTwoBjets);
    manager.new_cut(baseStrTwoBjetsExcl,baseStrTwoBjetsExcl);
    manager.new_cut(baseStrThreeJets,baseStrThreeJets);
    manager.new_cut(baseStrThreeBjets,baseStrThreeBjets);
    manager.new_cut(baseStrOneFatJet,baseStrOneFatJet);
    manager.new_cut(baseStrTwoSubJets,baseStrTwoSubJets);
    manager.new_cut(baseStrOneBFatJetM,baseStrOneBFatJetM);
    manager.new_cut(baseStrOneBFatJetT,baseStrOneBFatJetT);
    manager.new_cut(baseStrTwoBSubJetsLL,baseStrTwoBSubJetsLL);
    manager.new_cut(baseStrTwoBSubJetsMM,baseStrTwoBSubJetsMM);


}

void ElMuCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {

  const ZAAnalyzer& za = analyzers.get<ZAAnalyzer>("za");
  //const HLTProducer& hlt = producers.get<HLTProducer>("hlt");


  if(za.diLeptons.size() >= 1) {
    const DiLepton& m_diLepton = za.diLeptons[0];

    if(m_diLepton.isElMu) {
      manager.pass_cut(baseStrCategory );

      if(m_diLepton.p4.M() > m_MllCutSF)
        manager.pass_cut(baseStrMllCut);

      if(m_diLepton.isMM) manager.pass_cut(baseStrDileptonIsIDMM);
      if(m_diLepton.isTT) manager.pass_cut(baseStrDileptonIsIDTT);
      if(m_diLepton.isIsoLL) manager.pass_cut(baseStrDileptonIsoLL);

      if(m_diLepton.isOS)
      {
        manager.pass_cut(baseStrDiLeptonIsOS);
        if (m_diLepton.p4.M() > m_lowLooseZcut  && m_diLepton.p4.M() < m_highLooseZcut )
        {
          manager.pass_cut(baseStrLooseZCandidate);
          if (m_diLepton.p4.M() > m_lowTightZcut  && m_diLepton.p4.M() < m_highTightZcut )
            manager.pass_cut(baseStrTightZCandidate);
        }
      }
      if (za.selJets.size() >= 1) manager.pass_cut(baseStrOneJet);
      if (za.selJets.size() >= 2) manager.pass_cut(baseStrTwoJets);
      if (za.selJets.size() == 2) manager.pass_cut(baseStrTwoJetsExcl);
      if (za.selJets.size() >= 3) manager.pass_cut(baseStrThreeJets);

      if (za.selBjetsM.size() >= 1) manager.pass_cut(baseStrOneBjet);
      if (za.selBjetsM.size() >= 2) manager.pass_cut(baseStrTwoBjets);
      if (za.selBjetsM.size() == 2) manager.pass_cut(baseStrTwoBjetsExcl);
      if (za.selBjetsM.size() >= 3) manager.pass_cut(baseStrThreeBjets);

      if (za.diJets.size() >= 1) {
        const DiJet& m_diJet = za.diJets[0];
        if (m_diJet.isML) manager.pass_cut(baseStrDiJetBWP_ML);
        if (m_diJet.isMM) manager.pass_cut(baseStrDiJetBWP_MM);
        if (m_diJet.isTM) manager.pass_cut(baseStrDiJetBWP_TM);
      }
      if (za.selFatJets.size() >= 1){
        manager.pass_cut(baseStrOneFatJet);
        const FatJet& m_fatJet = za.selFatJets[0];
        if (m_fatJet.nSDSubjets >= 2){
          manager.pass_cut(baseStrTwoSubJets);
          if (m_fatJet.isBWPM) manager.pass_cut(baseStrOneBFatJetM);
          if (m_fatJet.isBWPT) manager.pass_cut(baseStrOneBFatJetT);
          if (m_fatJet.nBtaggedSDSubjetsLL) manager.pass_cut(baseStrTwoBSubJetsLL);
          if (m_fatJet.nBtaggedSDSubjetsMM) manager.pass_cut(baseStrTwoBSubJetsMM);
        }
      }
    }
  }
}



