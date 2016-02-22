#pragma once

#include <string>
#include <utility>
#include <vector>
#include <limits>


#include <cp3_llbb/Framework/interface/Analyzer.h>
#include <cp3_llbb/Framework/interface/MuonsProducer.h>
#include <cp3_llbb/Framework/interface/ElectronsProducer.h>
#include <cp3_llbb/Framework/interface/JetsProducer.h>
#include <cp3_llbb/Framework/interface/FatJetsProducer.h>
#include <cp3_llbb/Framework/interface/BinnedValuesJSONParser.h>

#include <cp3_llbb/ZAAnalysis/interface/ZATypes.h>
#include <cp3_llbb/ZAAnalysis/interface/Tools.h>

class ZAAnalyzer: public Framework::Analyzer {
    public:
        ZAAnalyzer(const std::string& name, const ROOT::TreeGroup& tree_, const edm::ParameterSet& config):
            Analyzer(name, tree_, config),
            // Not untracked as these parameters are mandatory
            m_electrons_producer( config.getParameter<std::string>("electronsProducer") ),
            m_muons_producer( config.getParameter<std::string>("muonsProducer") ),
            m_jets_producer( config.getParameter<std::string>("jetsProducer") ),
            m_met_producer( config.getParameter<std::string>("metProducer") ),
            m_nohf_met_producer( config.getParameter<std::string>("nohfMETProducer") ),
            // other parameters
            m_electronPtCut( config.getUntrackedParameter<double>("electronPtCut", 20) ),
            m_electronEtaCut( config.getUntrackedParameter<double>("electronEtaCut", 2.5) ),
            m_electronDcaCut( config.getUntrackedParameter<double>("electronDcaCut", 5) ),
            m_electronVetoIDName( config.getUntrackedParameter<std::string>("electronVetoIDName") ),
            m_electronLooseIDName( config.getUntrackedParameter<std::string>("electronLooseIDName") ),
            m_electronMediumIDName( config.getUntrackedParameter<std::string>("electronMediumIDName") ),
            m_electronTightIDName( config.getUntrackedParameter<std::string>("electronTightIDName") ),
//            m_electronSelectionID( config.getUntrackedParameter<std::string>("electronSelectionID") ),
            
            m_muonPtCut( config.getUntrackedParameter<double>("muonPtCut", 20) ),
            m_muonEtaCut( config.getUntrackedParameter<double>("muonEtaCut", 2.4) ),
            m_muonDcaCut( config.getUntrackedParameter<double>("muonDcaCut", 5.0) ),
//            m_muonSelectionID( config.getUntrackedParameter<std::string>("muonSelectionID","tight") ),
            m_muonLooseIsoCut( config.getUntrackedParameter<double>("muonLooseIsoCut", 0.25) ),
            m_muonTightIsoCut( config.getUntrackedParameter<double>("muonTightIsoCut", 0.15) ),
//            m_muonSelectionIsoCut( config.getUntrackedParameter<double>("muonSelectionIsoCut", 0.12) ),
            
            m_jetPtCut( config.getUntrackedParameter<double>("jetPtCut", 30) ),
            m_jetEtaCut( config.getUntrackedParameter<double>("jetEtaCut", 2.5) ),
            m_jetPUID( config.getUntrackedParameter<double>("jetPUID", std::numeric_limits<float>::min()) ),
            m_jetDRleptonCut( config.getUntrackedParameter<double>("jetDRleptonCut", 0.3) ),
            m_jetID( config.getUntrackedParameter<std::string>("jetID", "tight") ),
            m_jetCSVv2Name( config.getUntrackedParameter<std::string>("jetCSVv2Name", "pfCombinedInclusiveSecondaryVertexV2BJetTags") ),
            m_jetCSVv2L( config.getUntrackedParameter<double>("jetCSVv2L", 0.605) ),
            m_jetCSVv2M( config.getUntrackedParameter<double>("jetCSVv2M", 0.89) ),
            m_jetCSVv2T( config.getUntrackedParameter<double>("jetCSVv2T", 0.97) ),
            m_fatjetDRleptonCut( config.getUntrackedParameter<double>("fatjetDRleptonCut", 0.8) ),
            
            m_hltDRCut( config.getUntrackedParameter<double>("hltDRCut", std::numeric_limits<float>::max()) ),
            m_hltDPtCut( config.getUntrackedParameter<double>("hltDPtCut", std::numeric_limits<float>::max()) )

        {
            //if (config.existsAs<edm::ParameterSet>("hlt_scale_factors")) {
            if (config.exists("hlt_scale_factors")){
                std::cout << "getting SF .... " << std::endl;
                const edm::ParameterSet& hlt_scale_factors = config.getUntrackedParameter<edm::ParameterSet>("hlt_scale_factors");
                std::vector<std::string> hlt_scale_factors_name = hlt_scale_factors.getParameterNames();
                for (const std::string& hlt_scale_factor: hlt_scale_factors_name) {
                    std::cout << "adding hlt SF : " << hlt_scale_factor << std::endl; 
                    BinnedValuesJSONParser parser(hlt_scale_factors.getUntrackedParameter<edm::FileInPath>(hlt_scale_factor).fullPath());
                    m_hlt_scale_factors.emplace(hlt_scale_factor, std::move(parser.get_values()));
                }
            }

        }
        

        virtual void analyze(const edm::Event&, const edm::EventSetup&, const ProducersManager&, const AnalyzersManager&, const CategoryManager&) override;
        virtual void registerCategories(CategoryManager& manager, const edm::ParameterSet&) override;


        BRANCH(leptons, std::vector<ZAAnalysis::Lepton>);
        BRANCH(isolatedElectrons, std::vector<ZAAnalysis::Lepton>);
        BRANCH(isolatedMuons, std::vector<ZAAnalysis::Lepton>);
        BRANCH(vetoLeptons, std::vector<ZAAnalysis::Lepton>);
        BRANCH(selJets, std::vector<ZAAnalysis::Jet>);
        BRANCH(selFatJets, std::vector<ZAAnalysis::FatJet>);
        BRANCH(selBjetsM, std::vector<ZAAnalysis::Jet>);

        BRANCH(dilep_ptOrdered, std::vector<ZAAnalysis::Lepton>);  //FIXME remove the vectors and define "clearing mechanism", or use pair?
        BRANCH(dijet_ptOrdered, std::vector<ZAAnalysis::Jet>);
        BRANCH(dijet_CSVv2Ordered, std::vector<ZAAnalysis::Jet>);

        BRANCH(diLeptons, std::vector<ZAAnalysis::DiLepton>);
        BRANCH(diJets, std::vector<ZAAnalysis::DiJet>);
        BRANCH(diLepDiJets, std::vector<ZAAnalysis::DiLepDiJet>);
        BRANCH(diLepFatJets, std::vector<ZAAnalysis::DiLepFatJet>);

    private:
        // Producers name
        std::string m_electrons_producer;
        std::string m_muons_producer;
        std::string m_jets_producer;
        std::string m_met_producer;
        std::string m_nohf_met_producer;

        const float m_electronPtCut, m_electronEtaCut,m_electronDcaCut;
        const std::string m_electronVetoIDName;
        const std::string m_electronLooseIDName;
        const std::string m_electronMediumIDName;
        const std::string m_electronTightIDName;
        const std::string m_electronSelectionID;

//        const std::string m_muonSelectionID;
        const float m_muonPtCut, m_muonEtaCut, m_muonDcaCut, m_muonLooseIsoCut, m_muonTightIsoCut;

        const float m_jetPtCut, m_jetEtaCut, m_jetPUID, m_jetDRleptonCut;
        const std::string m_jetID, m_jetCSVv2Name;
        const float m_jetCSVv2L, m_jetCSVv2M, m_jetCSVv2T;
        const float m_fatjetDRleptonCut;

        const float m_hltDRCut, m_hltDPtCut;

        static inline bool muonIDAccessor(const MuonsProducer& muons, const uint16_t index, const std::string& muonID){
            if(index >= muons.p4.size())
              throw edm::Exception(edm::errors::StdException, "Invalid muon index passed to ID accessor");

            if(muonID == "loose")
                return muons.isLoose[index];
            
            if(muonID == "medium")
                return muons.isMedium[index];
            
            if(muonID == "tight")
                return muons.isTight[index];
            
            throw edm::Exception(edm::errors::NotFound, "Unknown muonID passed to analyzer");
        }
        
        static inline bool jetIDAccessor(const JetsProducer& jets, const uint16_t index, const std::string& jetID){
            if(index >= jets.p4.size())
              throw edm::Exception(edm::errors::StdException, "Invalid jet index passed to ID accessor");
            
            if(jetID == "loose")
                return jets.passLooseID[index];
            
            if(jetID == "tight")
                return jets.passTightID[index];
            
            if(jetID == "tightLeptonVeto")
                return jets.passTightLeptonVetoID[index];
            
            throw edm::Exception(edm::errors::NotFound, "Unknown jetID passed to analyzer");
        }

        std::map<std::string, BinnedValues> m_hlt_scale_factors;

       
};
