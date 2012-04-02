#ifndef TrackPlotter_TrackPlotter_h
#define TrackPlotter_TrackPlotter_h

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

//TrackCollection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//deltaR
#include "DataFormats/Math/interface/deltaR.h"

#include <string>


class TrackPlotter : public edm::EDAnalyzer {
   public:
      explicit TrackPlotter(const edm::ParameterSet&);
      ~TrackPlotter();

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;


// ----------member data -----------
double TrackPtCut;
double TrackEtaCut;
TTree *ttreetr;


};


struct ttrack_t {
float tracketa;
float trackphi;
float trackpt;
};

ttrack_t ttrack;

#endif 
