#ifndef DijetsPlotter_DijetsPlotter_h
#define DijetsPlotter_DijetsPlotter_h

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


class DijetsPlotter : public edm::EDAnalyzer {
   public:
      explicit DijetsPlotter(const edm::ParameterSet&);
      ~DijetsPlotter();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      edm::InputTag calAlgo_p;

TTree *ttreejets;
TTree *ttreetwojets;

};


struct alljets_t {
float jEta;
float jPhi;
float jPt;
};

alljets_t alljets;

struct twojets_t {
float j1Eta;
float j1Phi;
float j1Pt;
float j2Eta;
float j2Phi;
float j2Pt;
float deta;
float dphi;
float ptsum;
float massDijet;
};

twojets_t twojets;

#endif 
