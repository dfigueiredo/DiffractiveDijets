#ifndef TowerPlotter_TowerPlotter_h
#define TowerPlotter_TowerPlotter_h

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
#include "FWCore/Utilities/interface/InputTag.h"

#include <string>


class TowerPlotter : public edm::EDAnalyzer {
   public:
      explicit TowerPlotter(const edm::ParameterSet&);
      ~TowerPlotter();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

TTree *ttreehf;
double calonoise;


};


struct caltower_t {
float TowerPlus;
float TowerPlusEM;
float TowerPlusHad;
float TowerMinus;
float TowerMinusEM;
float TowerMinusHad;
};

caltower_t caltower;

float TowerPlus;
float TowerPlusEM;
float TowerPlusHad;
float TowerMinus;
float TowerMinusEM;
float TowerMinusHad;


#endif 
