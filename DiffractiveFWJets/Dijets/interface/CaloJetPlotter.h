#ifndef CaloJetPlotter_CaloJetPlotter_h
#define CaloJetPlotter_CaloJetPlotter_h

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
#include <map>

class CaloJetPlotter : public edm::EDAnalyzer {
   public:
      explicit CaloJetPlotter(const edm::ParameterSet&);
      ~CaloJetPlotter();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      edm::InputTag calAlgo;
      double JetNoise;

TTree *ttreecaljminus;
TTree *ttreecaljplus;

};


struct calojm_t {
float minusEnergy;
float minusPt;
float minusPhi;
float minusEta;
float minusEmEB;
float minusEmEE;
float minusEmHF;
float minusHadHB;
float minusHadHE;
float minusHadHO;
float minusHadHF;
};

calojm_t calojm;


struct calojp_t {
float plusEnergy;
float plusPt;
float plusPhi;
float plusEta;
float plusEmEB;
float plusEmEE;
float plusEmHF;
float plusHadHB;
float plusHadHE;
float plusHadHO;
float plusHadHF;
};

calojp_t calojp;

float minuse;
float minuspt;
float minusphi; 
float minuseta;
float minusemEB;
float minusemEE;
float minusemHF;
float minushadHB;
float minushadHE; 
float minushadHO; 
float minushadHF; 

float pluse;
float pluspt;
float plusphi; 
float pluseta;
float plusemEB;
float plusemEE;
float plusemHF;
float plushadHB;
float plushadHE; 
float plushadHO; 
float plushadHF; 

#endif 
