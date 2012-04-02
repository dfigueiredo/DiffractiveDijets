// State University of Rio de Janeiro
// Forward Dijets Analyzer
/////////////////////////////////////
//
// Author: Diego Figueiredo
//
/////////////////////////////////////
// Analyzer: EnergyFilter.cc

//--> System include files
#include <memory>

//--> Includes Default Files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

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

// RecoTowers
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include"DataFormats/CastorReco/interface/CastorTower.h"


#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"


using namespace edm;
using namespace std;
using namespace reco;


class EnergyFilter : public edm::EDFilter{
   public:
      explicit EnergyFilter(const edm::ParameterSet&);
      ~EnergyFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

double cuten_;

};


EnergyFilter::EnergyFilter(const edm::ParameterSet& iConfig):

	cuten_(iConfig.getUntrackedParameter<double>("cutEn",0)) {
}


EnergyFilter::~EnergyFilter(){ 
}

void EnergyFilter::beginJob(){
}


double totalF=0;
double cplusF=0;
double cnegF=0;

bool EnergyFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
++totalF;


bool FilterCheckF = false;

double sumPosF = 0;
double sumNegF = 0;

double ratioPosF = 0;
double ratioNegF = 0;

// caloTowers
Handle<CaloTowerCollection> caloTowers;
iEvent.getByLabel( "towerMaker", caloTowers );



CaloTowerCollection::const_iterator calo;


for(calo = caloTowers->begin(); calo != caloTowers->end(); ++calo){

   if (calo->status() == 1){

     if (calo->eta() > 0 && calo->eta() < 6) sumPosF += calo->energy();
     if (calo->eta() < 0 && calo->eta() > -6) sumNegF += calo->energy();

   }

}

ratioPosF = sumPosF/sumNegF;
ratioNegF = sumNegF/sumPosF;

if (ratioPosF > cuten_){

++cplusF;
cout << " " << endl;
cout << "Diffraction at Positive Side" << endl;
cout << " " << endl;
FilterCheckF = true;

}

if (ratioNegF > cuten_){

++cnegF;
cout << " " << endl;
cout << "Diffraction at Negative Side" << endl;
cout << " " << endl;
FilterCheckF = true;

}


return FilterCheckF;

}


void EnergyFilter::endJob(){

double effF = (cplusF+cnegF)/totalF;
double effpF = cplusF/totalF;
double effnF = cnegF/totalF;

cout << " " << endl;
cout << "-- Filter Efficiency Results --" << endl;
cout << " " << endl;
cout << "\t" << "\t" << "# Selections" << "\t" << "Efficiency" << endl;
cout << "Negative" << "\t" << cnegF << "\t\t" << effnF << endl;
cout << "Positive" << "\t" << cplusF << "\t\t" << effpF << endl;
cout << "Total   " << "\t" << cnegF+cplusF << "\t\t" << effF << endl;
cout << " " << endl;


}


DEFINE_FWK_MODULE(EnergyFilter);
