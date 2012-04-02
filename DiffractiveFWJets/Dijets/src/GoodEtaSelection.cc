// ---------------------------------------------
// State University of Rio de Janeiro
// Author: Diego
// ---------------------------------------------
// Purpose: Good Eta Selection
// ---------------------------------------------
// Created:  Monday September 22 09:26 BRA 2010

// System Includes
#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "TH1D.h"
#include "TFile.h"
#include "TLorentzVector.h"


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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// Reco::TrackCollection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// Reco::CASTOR Tower
#include "DataFormats/CastorReco/interface/CastorJet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

// Reco::JetCollection and Particle Flow
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

class GoodEtaSelection : public edm::EDAnalyzer
{
public:
    explicit GoodEtaSelection(const edm::ParameterSet&);
    ~GoodEtaSelection();


private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    typedef std::vector<CaloJet> JetCollection;


    TH1D * h_MaximumJets;
    TH1D * h_MinimumJets;
    TH1D * h_MaximumPF;
    TH1D * h_MinimumPF;
    TH1D * h_MaximumPFPt;
    TH1D * h_MinimumPFPt;
    TH1D * h_MaximumJetsPt;
    TH1D * h_MinimumJetsPt;
    TH1D * h_DeltaEtaJets;
    TH1D * h_DeltaEtaPF;
};



GoodEtaSelection::GoodEtaSelection(const edm::ParameterSet& iConfig)

{


   edm::Service<TFileService> fs;

h_DeltaEtaJets = fs->make<TH1D>( "gapsizeJets", "GapSizeJets ; Jets_{#eta_{max}-#eta_{min}}; Entries", 200, 0, 12); 
h_DeltaEtaPF = fs->make<TH1D>( "gapsizePF", "GapSize PF; Particle Flow_{#eta_{max}-#eta_{min}}; Entries", 400, 0, 12);
h_MaximumJets = fs->make<TH1D>( "maxJets_Eta", "maxJets_Eta ; Jets - #eta_{max}; Entries", 200, -10, 10);
h_MinimumJets = fs->make<TH1D>( "minJets_Eta", "minJets_Eta ; Jets - #eta_{min}; Entries", 200, -10, 10);
h_MaximumPF = fs->make<TH1D>( "maxPF_Eta", "maxPF_Eta ; Particle Flow - #eta_{max}; Entries", 300, -10, 10);
h_MinimumPF = fs->make<TH1D>( "minPF_Eta", "minPF_Eta ; Particle Flow - #eta_{min}; Entries", 300, -10, 10);
h_MaximumPFPt= fs->make<TH1D>( "maxPF_pT", "maxPF_pT ; Particle Flow - p_{T} [GeV c^{-1}]; Entries", 200, 0, 100);
h_MinimumPFPt= fs->make<TH1D>( "minPF_pT", "minPF_pT ; Particle Flow - p_{T} [GeV c^{-1}]; Entries", 200, 0, 100);
h_MaximumJetsPt= fs->make<TH1D>( "maxJets_pT", "maxJets_pT ; Jets - p_{T} [GeV c^{-1}]; Entries", 200, 0, 100);
h_MinimumJetsPt= fs->make<TH1D>( "minJets_pT", "minJets_pT ; Jets - p_{T} [GeV c^{-1}]; Entries", 200, 0, 100);


}


GoodEtaSelection::~GoodEtaSelection()
{

}

void GoodEtaSelection::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

// Jet
/////////////////////

Handle<reco::CaloJetCollection> jetsis5;
iEvent.getByLabel("ak5CaloJets",jetsis5);

// Declaring Variables
int jet5size = jetsis5->size();
int Njets5=0.;
int ijet5;
const reco::CaloJet* jet1_max=NULL;
const reco::CaloJet* jet2_max=NULL;
const reco::CaloJet* jet1_min=NULL;
const reco::CaloJet* jet2_min=NULL;

// Jets Above Et cut
double jet1Eta_max = 0;
double jet2Eta_max = 0;
double jet1Eta_min = 0;
double jet2Eta_min = 0;

// Defense
if (jet5size >= 2){

   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;		
   const reco::CaloJet* jetAll_max = &((*jetsis5)[ijet5]);

      
            if (jetAll_max==NULL) continue;
            //cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll_max->eta() << endl;
            if (jet1_max==NULL) {jet1_max=jetAll_max; continue;}
            if (jetAll_max->eta()>jet1_max->eta()) {
            jet2_max=jet1_max;
            jet1_max=jetAll_max;
            continue;
            }
   
         if (jet2_max==NULL) {jet2_max=jetAll_max; continue;}
         if (jetAll_max->eta()>jet2_max->eta()) jet2_max = jetAll_max;


    }

    jet1Eta_max = jet1_max->eta();
    jet2Eta_max = jet2_max->eta();

    h_MaximumJets->Fill(jet1_max->eta());
    h_MaximumJetsPt->Fill(jet1_max->pt());

    //cout << "Maximum Eta" << jet1Eta_max << endl;
    //cout << "Second Maximum Eta" << jet2Eta_max << endl;


    }

if (jet5size >= 2){

   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;		
   const reco::CaloJet* jetAll_min = &((*jetsis5)[ijet5]);

      
            if (jetAll_min==NULL) continue;
            //cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll_min->eta() << endl;
            if (jet1_min==NULL) {jet1_min=jetAll_min; continue;}
            if (jetAll_min->eta()<jet1_min->eta()) {
            jet2_min=jet1_min;
            jet1_min=jetAll_min;
            continue;
            }
   
         if (jet2_min==NULL) {jet2_min=jetAll_min; continue;}
         if (jetAll_min->eta()<jet2_min->eta()) jet2_min = jetAll_min;



    }

    jet1Eta_min = jet1_min->eta();
    jet2Eta_min = jet2_min->eta();

    h_MinimumJets->Fill(jet1_min->eta());
    h_MinimumJetsPt->Fill(jet1_min->pt());

    //cout << "Minimum Eta" << jet1Eta_min << endl;
    //cout << "Second Minimum Eta" << jet2Eta_min << endl;

    h_DeltaEtaJets->Fill(fabs(jet1_max->eta() - jet1_min->eta()));


    }

// -----------------------------------------------------------------------------

// Particle Flow
/////////////////////

Handle<reco::PFCandidateCollection> pfsis;
iEvent.getByLabel("particleFlow",pfsis);

// Declaring Variables
int pfsissize = pfsis->size();
int Npfsis=0.;
int ipfsis;
const reco::PFCandidate* pfsis1_max=NULL;
const reco::PFCandidate* pfsis2_max=NULL;
const reco::PFCandidate* pfsis1_min=NULL;
const reco::PFCandidate* pfsis2_min=NULL;

// Jets Above Et cut
double pfsis1Eta_max = 0;
double pfsis2Eta_max = 0;
double pfsis1Eta_min = 0;
double pfsis2Eta_min = 0;

// Defense
if (pfsissize >= 2){

   for(ipfsis=0; ipfsis < pfsissize; ++ipfsis){
   ++Npfsis;		
   const reco::PFCandidate* pfsis_max = &((*pfsis)[ipfsis]);

      
            if (pfsis_max==NULL) continue;
            //cout << "PF[" << ipfsis<< "]: Eta = "<< pfsis_max->eta() << endl;
            if (pfsis1_max==NULL) {pfsis1_max=pfsis_max; continue;}
            if (pfsis_max->eta()>pfsis1_max->eta()) {
            pfsis2_max=pfsis1_max;
            pfsis1_max=pfsis_max;
            continue;
            }
   
         if (pfsis2_max==NULL) {pfsis2_max=pfsis_max; continue;}
         if (pfsis_max->eta()>pfsis2_max->eta()) pfsis2_max = pfsis_max;


    }

    pfsis1Eta_max = pfsis1_max->eta();
    pfsis2Eta_max = pfsis2_max->eta();

    //cout << "Maximum Eta" << pfsis1Eta_max << endl;
    //cout << "Second Maximum Eta" << pfsis2Eta_max << endl;

    h_MaximumPF->Fill(pfsis1_max->eta());
    h_MaximumPFPt->Fill(pfsis1_max->pt());


    }

if (pfsissize >= 2){

   for(ipfsis=0; ipfsis < pfsissize; ++ipfsis){
   ++Npfsis;		
   const reco::PFCandidate* pfsis_min = &((*pfsis)[ipfsis]);

      
            if (pfsis_min==NULL) continue;
            //cout << "PF[" << ipfsis<< "]: Eta = "<< pfsis_min->eta() << endl;
            if (pfsis1_min==NULL) {pfsis1_min=pfsis_min; continue;}
            if (pfsis_min->eta()<pfsis1_min->eta()) {
            pfsis2_min=pfsis1_min;
            pfsis1_min=pfsis_min;
            continue;
            }
   
         if (pfsis2_min==NULL) {pfsis2_min=pfsis_min; continue;}
         if (pfsis_min->eta()<pfsis2_min->eta()) pfsis2_min = pfsis_min;



    }

    pfsis1Eta_min = pfsis1_min->eta();
    pfsis2Eta_min = pfsis2_min->eta();

    //cout << "Minimum Eta" << pfsis1Eta_min << endl;
    //cout << "Second Minimum Eta" << pfsis2Eta_min << endl;

    h_MinimumPF->Fill(pfsis1_min->eta());
    h_MinimumPFPt->Fill(pfsis1_min->pt());

    h_DeltaEtaPF->Fill(fabs(pfsis1_max->eta() - pfsis1_min->eta()));

    }

}


// ------------ method called once each job just before starting event loop  ------------
void
GoodEtaSelection::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
GoodEtaSelection::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(GoodEtaSelection);

