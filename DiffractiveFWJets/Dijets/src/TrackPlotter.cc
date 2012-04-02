// State University of Rio de Janeiro
// Forward Dijets AnalyzerEfficiency
//-----------------------------------
//
// Author: Diego Figueiredo
//
//-----------------------------------
//
// Goal: Show the Tracks Multiplicities
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//ROOT
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>

//TrackCollection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//deltaR
#include "DataFormats/Math/interface/deltaR.h"

//--> Functions and Methods
#include "DiffractiveFWJets/Dijets/interface/TrackPlotter.h"

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;


TrackPlotter::TrackPlotter(const edm::ParameterSet& iConfig) :
        TrackPtCut( iConfig.getParameter<double>( "TrackPtCut" )), 
        TrackEtaCut( iConfig.getParameter<double>( "TrackEtaCut" ))

{
 
edm::Service<TFileService> fs;
ttreetr = fs->make<TTree>("Tracks","Tracks");
ttreetr->Branch("variables",&ttrack,"tracketa/F:trackphi/F:trackpt/F");


}


TrackPlotter::~TrackPlotter()
{

}

void TrackPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    
    //track Handle
    Handle< TrackCollection > trackHandle;
    iEvent.getByLabel ("generalTracks", trackHandle);

    for (TrackCollection::const_iterator track = trackHandle->begin(); track != trackHandle->end(); ++ track)
    {

        if (fabs(track->eta()) < TrackEtaCut && track->pt() > TrackPtCut)
        {
          ttrack.tracketa = track->eta();
          ttrack.trackphi = track->phi();
          ttrack.trackpt = track->pt();
          ttreetr->Fill();
        }

    }

    
}

// ------------ method called once each job just before starting event loop  ------------
void
TrackPlotter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TrackPlotter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackPlotter);
