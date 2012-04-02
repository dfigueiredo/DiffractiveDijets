// State University of Rio de Janeiro
// Forward Dijets AnalyzerEfficiency
//-----------------------------------
//
// Author: Diego Figueiredo
//
//-----------------------------------
//
// Goal: Show the Towerrimeters Distributions
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
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CastorReco/interface/CastorTower.h"


//--> Functions and Methods
#include "DiffractiveFWJets/Dijets/interface/TowerPlotter.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


using namespace edm;
using namespace std;
using namespace reco;


TowerPlotter::TowerPlotter(const edm::ParameterSet& iConfig):
calonoise( iConfig.getParameter<double>( "calonoise" ) )
{

edm::Service<TFileService> fs;
ttreehf = fs->make<TTree>("Tower","Tower");
ttreehf->Branch("variables",&caltower,"TowerPlus/F:TowerPlusEM/F:TowerPlusHad/F:TowerMinus/F:TowerMinusEM/F:TowerMinusHad/F");

}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TowerPlotter::~TowerPlotter(){}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void TowerPlotter::beginJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void TowerPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){


// Tower
Handle<CaloTowerCollection> Tower;
iEvent.getByLabel( "towerMaker", Tower );


CaloTowerCollection::const_iterator calo;

TowerPlus=0;
TowerPlusEM=0;
TowerPlusHad=0;
TowerMinus=0;
TowerMinusEM=0;
TowerMinusHad=0;

for(calo = Tower->begin(); calo != Tower->end(); ++calo){

if (fabs(calo->eta()) > 2.9 && fabs(calo->eta()) < 5.2){
   
    if (calo->energy() > calonoise){

     if (calo->zside() > 0 ){
     TowerPlus += calo->energy();
     TowerPlusEM += calo->hadEnergy();
     TowerPlusHad += calo->emEnergy();
     }

     if (calo->zside() < 0){
     TowerMinus += calo->energy();
     TowerMinusEM += calo->hadEnergy();
     TowerMinusHad += calo->emEnergy();
     }

   }   

}

}

caltower.TowerPlus = TowerPlus;
caltower.TowerPlusEM = TowerPlusEM;
caltower.TowerPlusHad = TowerPlusHad;
caltower.TowerMinus = TowerMinus;
caltower.TowerMinusEM = TowerMinusEM;
caltower.TowerMinusHad = TowerMinusHad;

ttreehf->Fill();

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void TowerPlotter::endJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(TowerPlotter);
