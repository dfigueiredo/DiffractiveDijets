// State University of Rio de Janeiro
// Forward Dijets AnalyzerEfficiency
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


// RecoTowers
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include"DataFormats/CastorReco/interface/CastorTower.h"


//--> Functions and Methods
#include "DiffractiveFWJets/Dijets/interface/FractionEnergy.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


using namespace edm;
using namespace std;
using namespace reco;


FractionEnergy::FractionEnergy(const edm::ParameterSet& iConfig)
{

edm::Service<TFileService> fs;

 h_fracpt = fs->make<TH1D>( "h_fracpt"  , "p_{T} Asymmetry Distribution for CaloTowers ; #sum p_{T}^{-}/#sum p_{T}^{+}; events", 300,  0, 50);
 h_fracet = fs->make<TH1D>( "h_fracet"  , "E_{T} Asymmetry Distribution for CaloTowers ; #sum E_{T}^{-}/#sum E_{T}^{+}; events", 300,  0, 50);
 h_fracen = fs->make<TH1D>( "h_fracen"  , "Energy Asymmetry Distribution for CaloTowers ; #sum Energy^{-}/#sum Energy^{+}; events", 300,  0, 50);
 h_fracfb = fs->make<TH1D>( "h_fracfb"  , "Forward Backward Asymmetry; (#sum Energy^{+}-#sum Energy^{-})x(#sum Energy^{+}+#sum Energy^{-})^{-1}; events", 200,  -1, 1);
 h_zero = fs->make<TH1D>( "h_zero"  , "Without Activity at Least in One Side; p_{T} = 10 / e_{T} = 20 / Energy = 30 / RatioN = 40 / RatioP = 50; events", 6,  0, 60);
 h_ratioN = fs->make<TH1D>( "h_ratioN"  , "Eletromagnetic and Hadronic Fraction - #eta < 0; em/Had; events", 300,  0, 200);
 h_ratioP = fs->make<TH1D>( "h_ratioP"  , "Eletromagnetic and Hadronic Fraction - #eta > 0 ; em/Had; events", 300,  0, 200);


/*
ttreefraction = fs->make<TTree>("RatioEnergy","RatioEnergy");
ttreefraction->Branch("variables",&frac,"et/F:pt/F:energy/F:aEnergy/F:without/F:EmHadRatioN/F:EmHadRatioP/F");
*/

}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FractionEnergy::~FractionEnergy(){}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void FractionEnergy::beginJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void FractionEnergy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){


// caloTowers
Handle<CaloTowerCollection> caloTowers;
iEvent.getByLabel( "towerMaker", caloTowers );


CaloTowerCollection::const_iterator calo;


float sumPartptP=0;
float sumPartptN=0;
float sumPartetP=0;
float sumPartetN=0;
float sumPartenP=0;
float sumPartenN=0;
float sumEmP=0;
float sumEmN=0;
float sumHadP=0;
float sumHadN=0;

// Initiation
fracpt=0;
fracet=0;
fracen=0;
aEnergy=0;




for(calo = caloTowers->begin(); calo != caloTowers->end(); ++calo){

     if (calo->eta() < 6. && calo->eta() > 0 ){
     sumPartptP += calo->pt();
     sumPartetP += calo->et();
     sumPartenP += calo->energy();
     sumEmP += calo->emEnergy();
     sumHadP += calo->hadEnergy();
     }

     if (calo->eta() > -6 && calo->eta() < 0){
     sumPartptN += calo->pt();
     sumPartetN += calo->et();
     sumPartenN += calo->energy();
     sumEmN += calo->emEnergy();
     sumHadN += calo->hadEnergy();
     }

}

aEnergy = (sumPartenP - sumPartenN)/(sumPartenP + sumPartenN);

//frac.aEnergy=aEnergy;
h_fracfb->Fill(aEnergy);

fracpt = sumPartptN/sumPartptP;
if (sumPartptN == 0 || sumPartptP == 0){
h_zero->Fill(0);
//frac.without=0;

}

else {
//frac.pt=fracpt;
h_fracpt->Fill(fracpt);
}

fracet = sumPartetN/sumPartetP;
if (sumPartetN == 0 || sumPartetP == 0){
//frac.without=10;
h_zero->Fill(10);

}

else {
//frac.et=fracet;
h_fracet->Fill(fracet);

}

fracen = sumPartenN/sumPartenP;
if (sumPartenN == 0 || sumPartenP == 0){
//frac.without=20;
h_zero->Fill(20);

}

else {
//frac.energy=fracen;
h_fracen->Fill(fracen);

}

fracRatioN = sumEmN/sumHadN;
if (sumEmN == 0 || sumHadN == 0){
//frac.without=30;
h_zero->Fill(30);

}

else {
//frac.EmHadRatioN=fracRatioN;
h_ratioN->Fill(fracRatioN);

}

fracRatioP = sumEmP/sumHadP;
if (sumEmP == 0 || sumHadP == 0){
h_zero->Fill(40);
//frac.without=40;

}

else {
//frac.EmHadRatioP=fracRatioP;
h_ratioP->Fill(fracRatioP);

}

//ttreefraction->Fill();


}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void FractionEnergy::endJob(){
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(FractionEnergy);
