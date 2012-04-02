// ---------------------------------------------
// State University of Rio de Janeiro
// Author: Diego
// ---------------------------------------------
// Purpose: update code of Felipe Silva for
//          more recently CMSSW versions and
//          without EDProducers
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

class DijetsAnalysis : public edm::EDAnalyzer
{
public:
    explicit DijetsAnalysis(const edm::ParameterSet&);
    ~DijetsAnalysis();


private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    typedef std::vector<CaloJet> JetCollection;

    double HFNoise;
    double TrackPtCut;
    double CASTORNoise;
    int ntrackCut;
    double weight;

    //read gap side info and return correspondent multiplicity vector
    template <class T>
    pair< T , T >
    Gap2Mult(int GapSide, T Minus, T Plus)
    {
        pair< T , T > gap2mult;
        if (GapSide == -1)
        {
            gap2mult.first = Minus;
            gap2mult.second= Plus;
        }
        else
        {
            gap2mult.first = Plus;
            gap2mult.second= Minus;
        }
        return (gap2mult);
    }

    //gap side
    TH2D * h_Mult_HFlow_HFhigh;
    TH2D * h_Mult_HF_CASTOR;

    //opposite to gap side
    TH2D * h_Mult_HFlow_HFhigh_Opp;
    TH2D * h_Mult_HF_CASTOR_Opp;

    //plus side
    TH2D * h_Mult_HFlow_HFhigh_Plus;
    TH2D * h_Mult_HF_CASTOR_Plus;

    //minus side
    TH2D * h_Mult_HFlow_HFhigh_Minus;
    TH2D * h_Mult_HF_CASTOR_Minus;

    //gap side
    TH1D * h_GapSide;
    TH1D * h_leadingJet1;
    TH1D * h_leadingJet2;
    TH1D * h_tracks;
    TTree * ttreeEv;
    TH1D * h_MaximumPF;
    TH1D * h_MinimumPF;
    TH1D * h_MaximumPFPt;
    TH1D * h_MinimumPFPt;
    TH1D * h_DeltaEtaPF;
    TH1D * h_DiMass;

};

    double HFEnergyMinus;
    double HFEnergyPlus;
    int GapSide;
    double jet1Eta;
    double jet1Phi;
    double jet2Eta;
    double jet2Phi;
    int goodTracksCount;
    int trackMult;
    int nHFPlus;
    int nHFMinus;
    int nLowHFPlus;
    int nLowHFMinus;
    int nHighHFPlus;
    int nHighHFMinus;
    string GapSideSource;
    int hfMinus;
    int hfPlus;
    int lowetaMinus;
    int lowetaPlus;
    int highetaMinus;
    int highetaPlus;
    double hfNoiseThreshold;
    int nCASTORPlus;
    int nCASTORMinus;
    int castorMinus;
    int castorPlus;
    double castorNoiseThreshold;    
    int eventNumber;
    int eventRun;
    int lumiblock;

struct info_t {
int event;
int run;
int lumi;
};

info_t info;


DijetsAnalysis::DijetsAnalysis(const edm::ParameterSet& iConfig) :
        HFNoise( iConfig.getParameter<double>( "HFNoise" ) ),
        TrackPtCut( iConfig.getParameter<double>( "TrackPtCut" ) ),
        CASTORNoise( iConfig.getParameter<double>( "CASTORNoise" ) ),
        ntrackCut( iConfig.getParameter<int>( "ntrackCut" ) ),
        weight( iConfig.getParameter<double>( "Weight" ) )       
{


   edm::Service<TFileService> fs;

    //gap side
    h_Mult_HFlow_HFhigh = fs->make<TH2D>( "h_Mult_HFlow_HFhigh"  , "h_Mult_HFlow_HFhigh ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR = fs->make<TH2D>( "h_Mult_HF_CASTOR"  , "h_Mult_HF_CASTOR ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );


    //opposite to gap side
    h_Mult_HFlow_HFhigh_Opp = fs->make<TH2D>( "h_Mult_HFlow_HFhigh_Opp"  , "h_Mult_HFlow_HFhigh ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR_Opp = fs->make<TH2D>( "h_Mult_HF_CASTOR_Opp"  , "h_Mult_HF_CASTOR ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );


    //plus side
    h_Mult_HFlow_HFhigh_Plus = fs->make<TH2D>( "h_Mult_HFlow_HFhigh_Plus"  , "h_Mult_HFlow_HFhigh_Plus ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR_Plus = fs->make<TH2D>( "h_Mult_HF_CASTOR_Plus"  , "h_Mult_HF_CASTOR_Plus ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );


    //minus side
    h_Mult_HFlow_HFhigh_Minus = fs->make<TH2D>( "h_Mult_HFlow_HFhigh_Minus"  , "h_Mult_HFlow_HFhigh_Minus ; n HF \"low #eta slice\"; n HF \"forward slice\"; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );
    h_Mult_HF_CASTOR_Minus = fs->make<TH2D>( "h_Mult_HF_CASTOR_Minus"  , "h_Mult_HF_CASTOR_Minus ; n HF towers; n CASTOR; N events ( 10 pb^{-1} )", 10,  0., 10., 10,  0., 10. );

    //gap side
    h_GapSide = fs->make<TH1D>( "h_GapSide"  , "h_GapSide ; Gap Side; N events ( 10 pb^{-1} )", 2,  -1, 1);

 h_leadingJet1 = fs->make<TH1D>( "h_leadingJet1"  , "Leading Jet1 - p_{T}; p_{T} [GeV c^{-1}]; Multiplicity (1.2 10^{5} #mub^{-1})", 100,  0, 200);
 h_leadingJet2 = fs->make<TH1D>( "h_leadingJet2"  , "Leading Jet2 - p_{T}; p_{T} [GeV c^{-1}]; Multiplicity (1.2 10^{5} #mub^{-1})", 100,  0, 200);
 h_tracks = fs->make<TH1D>( "h_tracks"  , "Tracks Multiplicity; Number of Tracks Outside ConeJet; Multiplicity (1.2 10^{5} #mub^{-1})", 50,  0, 50);

ttreeEv = fs->make<TTree>("EventInfo","EventInfo");
ttreeEv->Branch("Info",&info,"event/I:run/I:lumi/I");

h_DeltaEtaPF = fs->make<TH1D>( "gapsizePF", "GapSize PF; Particle Flow_{#eta_{max}-#eta_{min}}; Entries", 400, 0, 12);
h_MaximumPF = fs->make<TH1D>( "maxPF_Eta", "maxPF_Eta ; Particle Flow - #eta_{max}; Entries", 300, -10, 10);
h_MinimumPF = fs->make<TH1D>( "minPF_Eta", "minPF_Eta ; Particle Flow - #eta_{min}; Entries", 300, -10, 10);
h_MaximumPFPt= fs->make<TH1D>( "maxPF_pT", "maxPF_pT ; Particle Flow - p_{T} [GeV c^{-1}]; Entries", 200, 0, 100);
h_MinimumPFPt= fs->make<TH1D>( "minPF_pT", "minPF_pT ; Particle Flow - p_{T} [GeV c^{-1}]; Entries", 200, 0, 100);
h_DiMass = fs->make<TH1D>( " DiMass", " h_DiMass ; Dijets Invariant Mass - [GeV]; Entries", 200, 0, 150);


}


DijetsAnalysis::~DijetsAnalysis()
{

}

// ------------ method called once each job just before starting event loop  ------------
void
DijetsAnalysis::beginJob()
{

}

void DijetsAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

//ofstream eventfile("events.txt");

        Handle<CaloTowerCollection> caloTowers;
        iEvent.getByLabel( "towerMaker", caloTowers );

        Handle<CastorTowerCollection> castorTowers;
        iEvent.getByLabel( "CastorTowerReco", castorTowers );

        HFEnergyMinus = 0;
        HFEnergyPlus = 0;
        GapSide = 0;

        for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
        {
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
            {
                if (cal->energy() > HFNoise)
                {
                    if (cal->zside() < 0)
                    {
                        HFEnergyMinus += cal->energy();
                    }
                    if (cal->zside() > 0)
                    {
                        HFEnergyPlus += cal->energy();
                    }
                }
            }

        }

        if (HFEnergyPlus >= HFEnergyMinus)
        {
            GapSide = -1;
        }
        if (HFEnergyPlus < HFEnergyMinus)
        {
            GapSide = 1;
        }

// Jet Reconstruction
/////////////////////

Handle<reco::CaloJetCollection> jetsis5;
iEvent.getByLabel("ak5CaloJets",jetsis5);

// Declaring Variables
int jet5size = jetsis5->size();
int Njets5=0.;
int ijet5;
const reco::CaloJet* jet1=NULL;
const reco::CaloJet* jet2=NULL;

    // Jets Above Et cut
    jet1Eta = 0;
    jet1Phi = 0;
    jet2Eta = 0;
    jet2Phi = 0;

// Just two Jets
if (jet5size >= 2){

   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;		
   const reco::CaloJet* jetAll = &((*jetsis5)[ijet5]);
   if (jetAll==NULL) continue;

//		cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll->eta() << " | Phi = " << jetAll->phi() << " | Et = " << jetAll->et() << endl;
	  
      if (jet1==NULL) {jet1=jetAll; continue;}
         if (jetAll->pt()>jet1->pt()) {
           jet2=jet1;
           jet1=jetAll;
           continue;
         }
   
      if (jet2==NULL) {jet2=jetAll; continue;}
      if (jetAll->pt()>jet2->pt()) jet2 = jetAll;

    }

jet1Eta = jet1->eta();
jet1Phi = jet1->phi();

jet2Eta = jet2->eta();
jet2Phi = jet2->phi();


    h_leadingJet1->Fill(jet1->pt(),weight);
    h_leadingJet2->Fill(jet2->pt(),weight);

    Handle< TrackCollection > trackHandle;
    iEvent.getByLabel("generalTracks",trackHandle);

    goodTracksCount = 0;
    trackMult = 0;
    for (TrackCollection::const_iterator track = trackHandle->begin(); track != trackHandle->end(); ++ track)
    {
        if (fabs(track->eta()) < 2.0 && track->pt() > TrackPtCut)
        {
            if ((deltaR(track->eta(),track->phi(),jet1Eta,jet1Phi) > 0.5) && (deltaR(track->eta(),track->phi(),jet2Eta,jet2Phi) > 0.5))
            {
                goodTracksCount++;
            }
        }

    }

    trackMult = goodTracksCount;
    h_tracks->Fill(trackMult,weight);

    // HF Multiplicities
    nHFPlus = 0;
    nHFMinus = 0;
    nLowHFPlus = 0;
    nLowHFMinus = 0;
    nHighHFPlus = 0;
    nHighHFMinus = 0;
    nCASTORPlus=0;
    nCASTORMinus=0;

    for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
    {
        hfNoiseThreshold = HFNoise;
        //nHF
        if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
        {
            if (cal->energy() > hfNoiseThreshold)
            {

                if (cal->zside() < 0)
                {
                    nHFMinus++;
                }
                if (cal->zside() > 0)
                {
                    nHFPlus++;
                }
            }

        }

        //nHFSlices - Low
        if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) <= 4.0)
        {
            if (cal->energy() > hfNoiseThreshold)
            {
                if (cal->zside() < 0)
                {
                    nLowHFMinus++;
                }
                if (cal->zside() > 0)
                {
                    nLowHFPlus++;
                }

            }
        }

        //nHFSlices - High
        if (fabs(cal->eta()) > 4.0 && fabs(cal->eta()) < 5.2)
        {
            if (cal->energy() > hfNoiseThreshold)
            {
                if (cal->zside() < 0)
                {
                    nHighHFMinus++;
                }
                if (cal->zside() > 0)
                {
                    nHighHFPlus++;
                }

            }
        }

   }


    hfMinus = nHFMinus;
    hfPlus = nHFPlus;
    lowetaMinus = nLowHFMinus;
    lowetaPlus = nLowHFPlus;
    highetaMinus = nHighHFMinus;
    highetaPlus = nHighHFPlus;


    //castor multiplicities
    //nCASTORPlus=0;
    //nCASTORMinus=0;

    for ( CastorTowerCollection::const_iterator cas = castorTowers->begin(); cas != castorTowers->end(); ++ cas )
    {
        castorNoiseThreshold = CASTORNoise;
        if (cas->energy() > castorNoiseThreshold)
        {
            if (cas->eta() < 0)
            {
                nCASTORMinus++;
            }
            if (cas->eta() > 0)
            {
                nCASTORPlus++;
            }
        }

    }

    castorMinus = nCASTORMinus;
    castorPlus = nCASTORPlus;

    pair < int , int > castor = Gap2Mult < int > (GapSide, castorMinus, castorPlus);
    pair < int , int > hf = Gap2Mult < int > (GapSide, hfMinus, hfPlus);
    pair < int , int > loweta = Gap2Mult < int > (GapSide, lowetaMinus, lowetaPlus);
    pair < int , int > highteta = Gap2Mult < int > (GapSide, highetaMinus, highetaPlus);


    if (GapSide > 0) h_GapSide->Fill(0.9,weight);
    if (GapSide < 0) h_GapSide->Fill(-0.9,weight);



    if (trackMult <= ntrackCut)
    {
        //gap side
        h_Mult_HFlow_HFhigh->Fill(loweta.first,highteta.first,weight);

        if (GapSide < 0)
        {
            h_Mult_HF_CASTOR->Fill(hf.first,castor.first,weight);
         
        }

        //opposite to gap side
        h_Mult_HFlow_HFhigh_Opp->Fill(loweta.second,highteta.second,weight);


        if (GapSide < 0)
        {
            h_Mult_HF_CASTOR_Opp->Fill(hf.second,castor.second,weight);
           
        }

        //plus side
        h_Mult_HFlow_HFhigh_Plus->Fill(lowetaPlus,highetaPlus,weight);
        h_Mult_HF_CASTOR_Plus->Fill(hfPlus,castorPlus,weight);
     
        //minus side
        h_Mult_HFlow_HFhigh_Minus->Fill(lowetaMinus,highetaMinus,weight);
        h_Mult_HF_CASTOR_Minus->Fill(hfMinus,castorMinus,weight);


       //////////////////////////////////
       // Particle Flow Eta Max e Eta Min
       //////////////////////////////////

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


   // Diffractive Events
   /////////////////////

       if(h_Mult_HFlow_HFhigh_Minus->GetBinContent(1,1) > 0 || h_Mult_HFlow_HFhigh_Plus->GetBinContent(1,1) > 0 ){


      // Di-jet mass
      math::XYZTLorentzVector DijetSystem(0.,0.,0.,0.);
      DijetSystem += jet1->p4();
      DijetSystem += jet2->p4();
      h_DiMass->Fill(DijetSystem.M(),weight);

       eventNumber=0;
       eventRun=0;
       lumiblock=0;

       eventNumber=iEvent.id().event();
       eventRun=iEvent.id().run();
       lumiblock=iEvent.id().luminosityBlock();


       info.event = eventNumber;
       info.run = eventRun;
       info.lumi = lumiblock;
       ttreeEv->Fill();
       //eventfile << eventRun << ":" << lumiblock << ":" << eventNumber << endl;

// Particle Flow Objects
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

    h_MaximumPF->Fill(pfsis1_max->eta(),weight);
    h_MaximumPFPt->Fill(pfsis1_max->pt(),weight);


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

    h_MinimumPF->Fill(pfsis1_min->eta(),weight);
    h_MinimumPFPt->Fill(pfsis1_min->pt(),weight);

    h_DeltaEtaPF->Fill(fabs(pfsis1_max->eta() - pfsis1_min->eta()),weight);

    }

// end PF Objects

       }

    } 

  }

//eventfile.close();

}

// ------------ method called once each job just after ending the event loop  ------------
void
DijetsAnalysis::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(DijetsAnalysis);

