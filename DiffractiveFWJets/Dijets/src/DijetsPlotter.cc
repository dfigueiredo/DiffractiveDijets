// State University of Rio de Janeiro
// Forward DijetsPlotter AnalyzerEfficiency
//-----------------------------------
//
// Author: Diego Figueiredo
//
//-----------------------------------

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
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//JetCollection (CaloTower)
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//--> Functions and Methods
#include "DiffractiveFWJets/Dijets/interface/DijetsPlotter.h"

using namespace edm;
using namespace std;
using namespace reco;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DijetsPlotter::DijetsPlotter(const edm::ParameterSet& iConfig):

calAlgo_p(iConfig.getUntrackedParameter<string>("calAlgo"))

{

edm::Service<TFileService> fs;
ttreejets = fs->make<TTree>("AllJets","AllJets");
ttreetwojets = fs->make<TTree>("SelectedJets","TwoJets");

ttreejets->Branch("variables",&alljets,"jEta/F:jPhi/F:jPt/F");
ttreetwojets->Branch("variables",&twojets,"j1Eta/F:j1Phi/F:j1Pt/F:j2Eta/F:j2Phi/F:j2Pt/F:deta/F:dphi/F:ptsum/F:massDijet/F");

}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DijetsPlotter::~DijetsPlotter(){}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DijetsPlotter::beginJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DijetsPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){


// Get the jet collection
Handle<CaloJetCollection> jets;
iEvent.getByLabel(calAlgo_p, jets );

// Declaring Variables
int jet5size = jets->size();
int Njets5=0.;
int ijet5;
double differenceEta=0;
double differencePhi=0;
const reco::CaloJet* jet1=NULL;
const reco::CaloJet* jet2=NULL;

// Program Defense
if (jet5size >= 2) { 

   // Loop Over All Jets
   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;

     const reco::CaloJet* jetAll = &((*jets)[ijet5]);

     if (jetAll==NULL) continue;

      // Printing All Jets in an Event
      //cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll->eta() << " | Phi = " << jetAll->phi() << " | Et = " << jetAll->et() << endl;
     
      //Fill Jets Tree
      alljets.jEta=jetAll->eta();
      alljets.jPhi=jetAll->phi();
      alljets.jPt=jetAll->pt();
      ttreejets->Fill();
     
      // Selecting Two Most Energetic Jets in an Event	  
      if (jet1==NULL) {jet1=jetAll; continue;}
         if (jetAll->pt()>jet1->pt()) {
           jet2=jet1;
           jet1=jetAll;
           continue;
         }
   
      if (jet2==NULL) {jet2=jetAll; continue;}
      if (jetAll->pt()>jet2->pt()) jet2 = jetAll;

    }

    // Calculating Two Most Energetic Jets Difference (Eta and Phi)
    differenceEta = fabs(jet2->eta()-jet1->eta());
    differencePhi = fabs(jet2->phi()-jet1->phi());
    //cout << "DeltaEta: " << fabs(jet2->eta()-jet1->eta()) << endl;
    //cout << "DeltaPhi: " << fabs(jet2->phi()-jet1->phi()) << endl;

    // Fill Two Jets
    twojets.j1Eta = jet1->eta();
    twojets.j1Phi = jet1->phi();
    twojets.j1Pt = jet1->pt();
    twojets.j2Eta = jet2->eta();
    twojets.j2Phi = jet2->phi();
    twojets.j2Pt = jet2->pt();
    twojets.deta = fabs(jet2->eta()-jet1->eta());
    twojets.dphi = fabs(jet2->phi()-jet1->phi());
    twojets.ptsum = jet1->pt() + jet2->pt();

    // Di-jet mass
    math::XYZTLorentzVector DijetSystem(0.,0.,0.,0.);
    DijetSystem += jet1->p4();
    DijetSystem += jet2->p4();
    twojets.massDijet = DijetSystem.M();
    ttreetwojets->Fill();

 }//End If Defense

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void DijetsPlotter::endJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(DijetsPlotter);
