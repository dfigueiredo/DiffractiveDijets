// State University of Rio de Janeiro
// Forward Dijets AnalyzerEfficiency
//-----------------------------------
//
// Author: Diego Figueiredo
//
//-----------------------------------
//
// Goal: Show the CaloJet Distributions
//

//--> System include files
#include <memory>

//--> Includes Default Files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//--> Includes for Root
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "math.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

//--> Includes for CMSSW Class Standard Objects
#include "DataFormats/JetReco/interface/GenJetCollection.h"


// RecoTowers
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/CastorReco/interface/CastorTower.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"



//--> Functions and Methods
#include "DiffractiveFWJets/Dijets/interface/CaloJetPlotter.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


using namespace edm;
using namespace std;
using namespace reco;

CaloJetPlotter::CaloJetPlotter(const edm::ParameterSet& iConfig):

calAlgo(iConfig.getUntrackedParameter<string>("calAlgo")),
JetNoise(iConfig.getUntrackedParameter<double>("JetNoise",0))

{

edm::Service<TFileService> fs;
ttreecaljminus = fs->make<TTree>("Minus","Minus");
ttreecaljplus = fs->make<TTree>("Plus","Plus");

ttreecaljminus->Branch("variables",&calojm,"minusEnergy/F:minusPt/F:minusPhi/F:minusEta/F:minusEmEB/F:minusEmEE/F:minusEmHF/F:minusHadHB/F:minusHadHE/F:minusHadHO/F:minusHadHF/F");
ttreecaljplus->Branch("variables",&calojp,"plusEnergy/F:plusPt/F:plusPhi/F:plusEta/F:plusEmEB/F:plusEmEE/F:plusEmHF/F:plusHadHB/F:plusHadHE/F:plusHadHO/F:plusHadHF/F");

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CaloJetPlotter::~CaloJetPlotter(){}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CaloJetPlotter::beginJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CaloJetPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){


Handle<reco::CaloJetCollection> caljets;
iEvent.getByLabel(calAlgo, caljets );

reco::CaloJetCollection::const_iterator calo;

minuse = 0;
minuspt = 0;
minusphi = 0; 
minuseta = 0;
minusemEB = 0;
minusemEE = 0;
minusemHF = 0;
minushadHB = 0;
minushadHE = 0; 
minushadHO = 0; 
minushadHF = 0; 

pluse = 0;
pluspt = 0;
plusphi = 0; 
pluseta = 0;
plusemEB = 0;
plusemEE = 0;
plusemHF = 0;
plushadHB = 0;
plushadHE = 0; 
plushadHO = 0; 
plushadHF = 0; 


for(calo = caljets->begin(); calo != caljets->end(); ++calo){
   if (calo->energy() > JetNoise)
   {
         if (calo->eta() < 0)
         {
         minuse = calo->energy();
         minuspt = calo->pt();
         minusphi = calo->phi(); 
         minuseta = calo->eta();
         minusemEB = calo->emEnergyInEB();
         minusemEE = calo->emEnergyInEE();
         minusemHF = calo->emEnergyInHF();
         minushadHB = calo->hadEnergyInHB();
         minushadHE = calo->hadEnergyInHE(); 
         minushadHO = calo->hadEnergyInHO(); 
         minushadHF = calo->hadEnergyInHF(); 

         calojm.minusEnergy = minuse;
         calojm.minusPt = minuspt;
         calojm.minusPhi = minusphi;
         calojm.minusEta = minuseta;
         calojm.minusEmEB = minusemEB;
         calojm.minusEmEE = minusemEE;
         calojm.minusEmHF = minusemHF;
         calojm.minusHadHB = minushadHB;
         calojm.minusHadHE = minushadHE;
         calojm.minusHadHO = minushadHO;
         calojm.minusHadHF = minushadHF;
         ttreecaljminus->Fill();

         }
      
         if (calo->eta() > 0)
         {
         pluse = calo->energy();
         pluspt = calo->pt();
         plusphi = calo->phi(); 
         pluseta = calo->eta();
         plusemEB = calo->emEnergyInEB();
         plusemEE = calo->emEnergyInEE();
         plusemHF = calo->emEnergyInHF();
         plushadHB = calo->hadEnergyInHB();
         plushadHE = calo->hadEnergyInHE(); 
         plushadHO = calo->hadEnergyInHO(); 
         plushadHF = calo->hadEnergyInHF(); 

         calojp.plusEnergy = pluse;
         calojp.plusPt = pluspt;
         calojp.plusPhi = plusphi;
         calojp.plusEta = pluseta;
         calojp.plusEmEB = plusemEB;
         calojp.plusEmEE = plusemEE;
         calojp.plusEmHF = plusemHF;
         calojp.plusHadHB = plushadHB;
         calojp.plusHadHE = plushadHE;
         calojp.plusHadHO = plushadHO;
         calojp.plusHadHF = plushadHF;
         ttreecaljplus->Fill();

         }
   }
}

//cout<<"WARNING: NO calo jets in event "<<iEvent.id().event()<<", Run "<<iEvent.id().run()<<" !!!!"<<endl;

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CaloJetPlotter::endJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CaloJetPlotter);
