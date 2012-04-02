// State University of Rio de Janeiro
// Forward Dijets Analyzer
/////////////////////////////////////
//
// Author: Diego Figueiredo
//
/////////////////////////////////////
// Analyzer: JetsFilter.cc

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

//JetCollection (CaloTower)
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"


using namespace edm;
using namespace std;
using namespace reco;


class JetsFilter : public edm::EDFilter{
   public:
      explicit JetsFilter(const edm::ParameterSet&);
      ~JetsFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

int nJets_;
edm::InputTag calAlgo_;

};


JetsFilter::JetsFilter(const edm::ParameterSet& iConfig):
	nJets_(iConfig.getUntrackedParameter<int>("nJets",0)),
        calAlgo_(iConfig.getUntrackedParameter<string>("calAlgo"))

{

}


JetsFilter::~JetsFilter(){ 
}

void JetsFilter::beginJob(){
}

int counter_jets=0;
int counter_filter=0;


bool JetsFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
++counter_jets;

bool Filter_jets = false;

// caloTowers
Handle<reco::CaloJetCollection> jets;
iEvent.getByLabel(calAlgo_, jets );

int jet5size = jets->size();

   if (jet5size==nJets_) {
   ++counter_filter;
   Filter_jets = true;
   }

return Filter_jets;

}


void JetsFilter::endJob(){


cout << " " << endl;
cout << "-- Filter Efficiency Results --" << endl;
cout << "Selected: " << "\t" << counter_filter << endl;
cout << "Total:    " << "\t" << counter_jets << endl;
cout << " " << endl;

}


DEFINE_FWK_MODULE(JetsFilter);
