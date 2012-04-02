// ---------------------------------------------
// State University of Rio de Janeiro
// Author: Diego
// ---------------------------------------------
// Created:  Monday September 13 09:26 BRA 2010

// System Includes
#include <memory>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <math.h>
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <cmath>
#include <string>

// Standard CMSSW Includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// ROOT Services
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// Reco::TrackCollection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// Reco::CASTOR Tower
#include "DataFormats/CastorReco/interface/CastorJet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

// Reco::JetCollection
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

//GenParticle
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// DeltaR
#include "DataFormats/Math/interface/deltaR.h"

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;

class EfficiencyStudies : public edm::EDAnalyzer
{
public:
    explicit EfficiencyStudies(const edm::ParameterSet&);
    ~EfficiencyStudies();


private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    typedef std::vector<CaloJet> JetCollection;

    double weight;

TProfile * hf_HFMinusNoise;
TProfile * hf_HFPlusNoise;
TProfile * hf_Trackpt;
TProfile * hf_TrackOutSideCone;
TProfile * hf_TrackMult;
TProfile * hf_TrackOutSideConeStep;
TProfile * hf_JetCut;

};


EfficiencyStudies::EfficiencyStudies(const edm::ParameterSet& iConfig):
                   weight( iConfig.getParameter<double>( "Weight" ) )
{

edm::Service<TFileService> fs;

hf_HFMinusNoise = fs->make<TProfile> ("hf_HFMinusNoise", "HF Minus Noise", 25, 0, 25, 0, 50);
hf_HFPlusNoise = fs->make<TProfile> ("hf_HFPlusNoise", "HF Plus Noise", 25, 0, 25, 0, 50);
hf_TrackMult = fs->make<TProfile>("hf_TrackMult", "hf_TrackMult", 50, 0, 50, 0, 50);
hf_TrackOutSideCone = fs->make<TProfile>("hf_TrackOutSideCone", "hf_TrackOutSideCone", 30, 0, 3, 0, 50);
hf_Trackpt = fs->make<TProfile>("hf_Trackpt", "hf_Trackpt", 30, 0, 3, 0, 50);
hf_JetCut = fs->make<TProfile>("hf_JetCut", "hf_JetCut", 100, 0, 100, 0, 100);
hf_TrackOutSideConeStep = fs->make<TProfile>("hf_TrackOutSideConeStep", "hf_TrackOutSideConeStep", 15, 0, 1, 0, 50);

}


EfficiencyStudies::~EfficiencyStudies()
{

}

// ------------ method called once each job just before starting event loop  ------------
void
EfficiencyStudies::beginJob()
{

}

void EfficiencyStudies::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    int goodTracksCount;
    int trackMult;


// HF Noise Efficiency
//------------------------------------------------------------------------------

Handle<CaloTowerCollection> caloTowers;
iEvent.getByLabel( "towerMaker", caloTowers );

for (int istep = 0; istep < 11; ++istep) {

     int HFPlus = 0;
     int HFMinus = 0;
     float HFStep = (istep * 2.);
  

        for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
        {
            
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
            {
                if (cal->energy() > HFStep)
                {

                    if (cal->zside() < 0)
                    {
                        HFMinus++;
                    }
                    if (cal->zside() > 0)
                    {
                        HFPlus++;
                    }

                }
            }

        }

   hf_HFMinusNoise->Fill(HFStep, HFMinus, weight);
   hf_HFPlusNoise->Fill(HFStep, HFPlus, weight);

}

// JetPt Cut
//------------------------------------------------------------------------------

Handle<reco::CaloJetCollection> jetsis5;
iEvent.getByLabel("ak5CaloJets",jetsis5);

// Declaring Variables
int jet5size = jetsis5->size();
int Njets5=0.;
int ijet5;
const reco::CaloJet* jet1=NULL;
const reco::CaloJet* jet2=NULL;

double jet1Pt = 0;
double jet2Pt = 0;
double jet1Eta = 0;
double jet2Eta = 0;
double jet1Phi = 0;
double jet2Phi = 0;

// Just two Jets
if (jet5size >= 2){


for (int istep = 0; istep < 80; ++istep) {
     
     int jetCounter = 0;
     float JetStep = (istep * 5.);


   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;		
   const reco::CaloJet* jetAll = &((*jetsis5)[ijet5]);
   if (jetAll==NULL) continue;

// cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll->eta() << " | Phi = " << jetAll->phi() << " | Et = " << jetAll->et() << endl;
	  
      if (jet1==NULL) {jet1=jetAll; continue;}
         if (jetAll->pt()>jet1->pt()) {
           jet2=jet1;
           jet1=jetAll;
           continue;
         }
   
      if (jet2==NULL) {jet2=jetAll; continue;}
      if (jetAll->pt()>jet2->pt()) jet2 = jetAll;

    }

      jet1Pt = jet1->pt();
      jet2Pt = jet2->pt();
      jet1Eta = jet1->eta();
      jet2Eta = jet2->eta();
      jet1Phi = jet1->phi();
      jet2Phi = jet2->phi();

      if (jet1Pt >= JetStep && jet2Pt >= JetStep){
          ++jetCounter;
      }

   hf_JetCut->Fill(JetStep, jetCounter, weight);

}


// Tracks Efficiency
//------------------------------------------------------------------------------

    Handle< TrackCollection > trackHandle;
    iEvent.getByLabel("generalTracks",trackHandle);

    goodTracksCount = 0;
    trackMult = 0;

// pT Efficiency
//------------------------------------------------------------------------------
 
for (int istep = 0; istep < 31; ++istep) {
     
     int countTrack = 0;
     float TrackStep = (istep * (1./10.));

    for (TrackCollection::const_iterator track = trackHandle->begin(); track != trackHandle->end(); ++ track)
    {
        if (fabs(track->eta()) < 2.0 && track->pt() > TrackStep)
        {

            countTrack++;

            if ((deltaR(track->eta(),track->phi(),jet1Eta,jet1Phi) > 0.5) && (deltaR(track->eta(),track->phi(),jet2Eta,jet2Phi) > 0.5))
            {
                goodTracksCount++;
            }

        }

    }

   hf_Trackpt->Fill(TrackStep, countTrack, weight);
   hf_TrackOutSideCone->Fill(TrackStep, goodTracksCount, weight);

 }

 trackMult = goodTracksCount;

// Cone Efficiency
 
for (int istep = 0; istep < 11; ++istep) {
     
     int countCone = 0;
     float coneStep = (istep * (1./10.));

    for (TrackCollection::const_iterator track = trackHandle->begin(); track != trackHandle->end(); ++ track)
    {
        if (fabs(track->eta()) < 2.0 && track->pt() > 0.3)
        {

            if ((deltaR(track->eta(),track->phi(),jet1Eta,jet1Phi) > coneStep) && (deltaR(track->eta(),track->phi(),jet2Eta,jet2Phi) > coneStep))
            {
                countCone++;
            }

        }

    }

 hf_TrackOutSideConeStep->Fill(coneStep, countCone, weight);

 }

 // Count Jets above Pt cut
 for (int istep = 0; istep < 50; ++istep) {
     
     int countTrack = 0;
     int TrackStep = (istep);


    if (trackMult <= TrackStep)
    {
      
    countTrack++;

    }

   hf_TrackMult->Fill(TrackStep, countTrack, weight);

 }

 } 

}

// ------------ method called once each job just after ending the event loop  ------------
void
EfficiencyStudies::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(EfficiencyStudies);

