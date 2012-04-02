#ifndef HFPlotter_HFPlotter_h
#define HFPlotter_HFPlotter_h

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


class HFPlotter : public edm::EDAnalyzer {
   public:
      explicit HFPlotter(const edm::ParameterSet&);
      ~HFPlotter();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

TTree *ttreehf;


};


struct cal_t {
float CaloPlus;
float CaloPlusEM;
float CaloPlusHad;
float CaloMinus;
float CaloMinusEM;
float CaloMinusHad;
};

cal_t cal;

float CaloPlus;
float CaloPlusEM;
float CaloPlusHad;
float CaloMinus;
float CaloMinusEM;
float CaloMinusHad;


#endif 
