#ifndef FractionEnergy_FractionEnergy_h
#define FractionEnergy_FractionEnergy_h

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


class FractionEnergy : public edm::EDAnalyzer {
   public:
      explicit FractionEnergy(const edm::ParameterSet&);
      ~FractionEnergy();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

//TTree *ttreefraction;

TH1D * h_fracpt;
TH1D * h_fracen;
TH1D * h_fracet;
TH1D * h_fracfb;
TH1D * h_zero;
TH1D * h_ratioN;
TH1D * h_ratioP;
};

/*
struct frac_t {
float pt;
float et;
float energy;
float aEnergy;
float without;
float EmHadRatioP;
float EmHadRatioN;
};

frac_t frac;

*/

// Declaring Counters 
float fracpt;
float fracet;
float fracen;
float aEnergy;
float fracRatioP;
float fracRatioN;


#endif 
