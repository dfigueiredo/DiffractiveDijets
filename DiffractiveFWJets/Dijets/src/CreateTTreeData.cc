// ---------------------------------------------
// State University of Rio de Janeiro
// Author: Diego
// ---------------------------------------------
// Purpose: create all TTree Analysis
// ---------------------------------------------


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
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "RecoJets/JetProducers/interface/JetIDHelper.h"
#include "RecoJets/JetProducers/plugins/JetIDProducer.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"
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
#include "DataFormats/JetReco/interface/JetCollection.h"
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

//New
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h" 
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;

class CreateTTreeData : public edm::EDAnalyzer
{
public:
    explicit CreateTTreeData(const edm::ParameterSet&);
    ~CreateTTreeData();

private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    typedef std::vector<CaloJet> JetCollection;

reco::helper::JetIDHelper jet_ID_helper_;
reco::JetID jet_ID_struct_;

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

    bool accessMCInfo_;
    bool accessPileUpInfo_;
    string vertexTag_;
    string calAlgo_;
    double HFNoise;
    double TrackPtCut;
    double weight;

    double HFEnergyMinus;
    double HFEnergyPlus;
    int GapSide;

    double jet1Eta;
    double jet1Phi;
    double jet2Eta;
    double jet2Phi;
    double jet1Pt;
    double jet2Pt;
    double pthat;

    int goodTracksCount;

    int nHFPlus;
    int nHFMinus;
    int nLowHFPlus;
    int nLowHFMinus;
    int nHighHFPlus;
    int nHighHFMinus;
    double DijetMass;

     double pfetamax;
     double pfetamaxpT;
     double pfetamaxphi;
     double pfetamin;
     double pfetaminpT;
     double pfetaminphi;
     double pfdeltaeta;
    
    TTree * treeEventData;
    

};

struct eventData_t {

     // Event Info
     int processId_;
     int runNumber_;
     int eventNumber_;
     int lumiSection_;
     int bunchCrossing_;
     double lumiWeight_;
     int pileUp_;
     double ptGen_;
     double xi_;
     
     // HCal Noise
     int BeamHaloLooseId_;
     int BeamHaloTightId_;
     int LooseNoiseFilter_;
     int TightNoiseFilter_;

     // Vertex
     int nVertex_;
     double posXPrimVtx_;
     double posYPrimVtx_;
     double posZPrimVtx_;

     // HF Energy
     double sumEnergyHFPlus_;
     double sumEnergyHFMinus_;
     double aEnergy_;
     int GapSide_;
     int multiHFPlus_;
     int multiHFMinus_;
     int multiHFPlusLow_;
     int multiHFMinusLow_;
     int multiHFPlusHigh_;
     int multiHFMinusHigh_;

     // Leading Jets
     double LeadingJet1Eta_;
     double LeadingJet1Pt_;
     double LeadingJet1Phi_;
     double LeadingJet2Eta_;
     double LeadingJet2Pt_;
     double LeadingJet2Phi_;
     double DijetMass_;

     // Good Tracks
     int goodTracks_;

     // Particle Flow
     double pfetamax_;
     double pfetamaxpT_;
     double pfetamaxphi_;
     double pfetamin_;
     double pfetaminpT_;
     double pfetaminphi_;
     double pfdeltaeta_;

  };

eventData_t eventData;

CreateTTreeData::CreateTTreeData(const edm::ParameterSet& iConfig):
                 accessMCInfo_(iConfig.getUntrackedParameter<bool>("AccessMCInfo")),
                 accessPileUpInfo_(iConfig.getUntrackedParameter<bool>("AccessPileUpInfo")),
                 vertexTag_(iConfig.getUntrackedParameter<string>("VertexTag")),
                 calAlgo_(iConfig.getUntrackedParameter<string>("calAlgoJet")),
                 HFNoise( iConfig.getParameter<double>( "HFNoise" ) ),
                 TrackPtCut( iConfig.getParameter<double>( "TrackPtCut" ) ),
                 weight( iConfig.getParameter<double>( "Weight" ) )
{


edm::Service<TFileService> fs;

edm::ParameterSet jet_ID_helper_config = iConfig.getParameter<edm::ParameterSet>( "jetIDHelperConfig" );
jet_ID_helper_ = reco::helper::JetIDHelper( jet_ID_helper_config );

treeEventData = fs->make<TTree>("EventData","EventData");

//Event Info
treeEventData->Branch("processId",&eventData.processId_,"processId/I" );
treeEventData->Branch("eventNumber",&eventData.eventNumber_,"eventNumber/I");
treeEventData->Branch("runNumber",&eventData.runNumber_,"runNumber/I");
treeEventData->Branch("lumiSection",&eventData.lumiSection_,"lumiSection/I");
treeEventData->Branch("bunchCrossing",&eventData.bunchCrossing_,"bunchCrossing/I");
treeEventData->Branch("lumiWeight",&eventData.lumiWeight_,"lumiWeight/D");
treeEventData->Branch("pileUp",&eventData.pileUp_,"n_pu/I");
treeEventData->Branch("pTGen",&eventData.ptGen_,"pthat/D");
treeEventData->Branch("Xi",&eventData.xi_,"xi/D");

//HFNoise
treeEventData->Branch("BeamHaloLooseId",&eventData.BeamHaloLooseId_,"BeamHaloLooseId/I");
treeEventData->Branch("BeamHaloTightId",&eventData.BeamHaloTightId_,"BeamHaloTightId/I");
treeEventData->Branch("LooseNoiseFilter",&eventData.LooseNoiseFilter_,"LooseNoiseFilter/I");
treeEventData->Branch("TightNoiseFilter",&eventData.TightNoiseFilter_,"TightNoiseFilter/I");

//Vertex
treeEventData->Branch("nVertex",&eventData.nVertex_,"nVertex/I");
treeEventData->Branch("posXPrimVtx",&eventData.posXPrimVtx_,"posXPrimVtx/D");
treeEventData->Branch("posYPrimVtx",&eventData.posYPrimVtx_,"posYPrimVtx/D");
treeEventData->Branch("posZPrimVtx",&eventData.posZPrimVtx_,"posZPrimVtx/D");


//HFEnergy
treeEventData->Branch("sumHFPlus",&eventData.sumEnergyHFPlus_,"HFEnergyPlus/D");
treeEventData->Branch("sumHFMinus",&eventData.sumEnergyHFMinus_,"HFEnergyMinus/D");
treeEventData->Branch("aEnergy",&eventData.aEnergy_,"aSumE/D");
treeEventData->Branch("GapSide",&eventData.GapSide_,"GapSide/I");
treeEventData->Branch("multiHFPlus",&eventData.multiHFPlus_,"nHFPlus/I");
treeEventData->Branch("multiHFMinus",&eventData.multiHFMinus_,"nHFMinus/I");
treeEventData->Branch("multiHFPlusLow",&eventData.multiHFPlusLow_,"nLowHFPlus/I");
treeEventData->Branch("multiHFMinusLow",&eventData.multiHFMinusLow_,"nLowHFMinus/I");
treeEventData->Branch("multiHFPlusHigh",&eventData.multiHFPlusHigh_,"nHighHFPlus/I");
treeEventData->Branch("multiHFMinusHigh",&eventData.multiHFMinusHigh_,"nHighHFMinus/I");

//Leading Jets
treeEventData->Branch("LeadingJet1Eta",&eventData.LeadingJet1Eta_,"LeadingJet1Eta/D");
treeEventData->Branch("LeadingJet1Phi",&eventData.LeadingJet1Phi_,"LeadingJet1Phi/D");
treeEventData->Branch("LeadingJet1Pt",&eventData.LeadingJet1Pt_,"LeadingJet1Pt/D");
treeEventData->Branch("LeadingJet2Eta",&eventData.LeadingJet2Eta_,"LeadingJet2Eta/D");
treeEventData->Branch("LeadingJet2Phi",&eventData.LeadingJet2Phi_,"LeadingJet2Phi/D");
treeEventData->Branch("LeadingJet2Pt",&eventData.LeadingJet2Pt_,"LeadingJet2Pt/D");
treeEventData->Branch("DijetMass",&eventData.DijetMass_,"DijetMass/D");

// Good Tracks
treeEventData->Branch("GoodTracks",&eventData.goodTracks_,"goodTracksCount/I");

// Particle Flow
treeEventData->Branch("PfEtamax",&eventData.pfetamax_,"pfetamax/D");
treeEventData->Branch("PfEtamin",&eventData.pfetamin_,"pfetamin/D");
treeEventData->Branch("PfEtamaxpT",&eventData.pfetamaxpT_,"pfetamaxpT/D");
treeEventData->Branch("PfEtaminpT",&eventData.pfetaminpT_,"pfetaminpT/D");
treeEventData->Branch("PfEtamaxPhi",&eventData.pfetamaxphi_,"pfetamaxphi/D");
treeEventData->Branch("PfEtaminPhi",&eventData.pfetaminphi_,"pfetaminphi/D");
treeEventData->Branch("PfDeltaEta",&eventData.pfdeltaeta_,"pfdeltaeta/D");

}


CreateTTreeData::~CreateTTreeData()
{

}

// ------------ method called once each job just before starting event loop  ------------
void
CreateTTreeData::beginJob()
{

}

void CreateTTreeData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

JetIDSelectionFunctor jetId( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::LOOSE );

//
// Event Info
/////////////

 if(accessMCInfo_){
     edm::Handle<GenEventInfoProduct> genEventInfo;
     iEvent.getByLabel("generator",genEventInfo);

     int processId = -1;
     if(genEventInfo.isValid()){
        processId = genEventInfo->signalProcessID();
     } else {
        edm::Handle<edm::HepMCProduct> hepMCProduct;
        iEvent.getByLabel("source",hepMCProduct);
        processId = hepMCProduct->getHepMCData().signal_process_id();
     }
     eventData.processId_ = processId;
  } else{
     eventData.processId_ = -1;
  } 

  unsigned int eventNumber = iEvent.id().event();
  unsigned int runNumber = iEvent.id().run();
  unsigned int lumiSection = iEvent.luminosityBlock();
  int bunchCrossing = iEvent.bunchCrossing();

  edm::Handle<double> lumiWeight;
  iEvent.getByLabel("lumiWeight",lumiWeight);
  if(lumiWeight.isValid()){
     double lumi = *lumiWeight;
     eventData.lumiWeight_ = lumi;
  } else{
     eventData.lumiWeight_ = -1.; 
  }

 if(accessMCInfo_){
    Handle<GenEventInfoProduct > genEvt;
    iEvent.getByLabel( "generator", genEvt );
    pthat=0;
    if(genEvt->binningValues().size()==1) {pthat=genEvt->binningValues()[0];
    eventData.ptGen_ = pthat;
    }

  } else{
     eventData.ptGen_ = -999;
  } 

if(accessPileUpInfo_){
  //pileup
  int n_pu=0;
  Handle<PileupSummaryInfo> pileup;
  iEvent.getByLabel("addPileupInfo", pileup );
  n_pu=pileup->getPU_NumInteractions();

  eventData.pileUp_ = n_pu;
  } else{
  eventData.pileUp_ = -999;
  } 

  eventData.eventNumber_ = eventNumber;
  eventData.runNumber_ = runNumber;
  eventData.lumiSection_ = lumiSection;
  eventData.bunchCrossing_ = bunchCrossing;

//
// HCal Noise
//////////////

  edm::Handle<HcalNoiseSummary> noiseSummaryH;
  iEvent.getByLabel("hcalnoise",noiseSummaryH);   

  bool passNoiseLoose = noiseSummaryH->passLooseNoiseFilter();
  bool passNoiseTight = noiseSummaryH->passTightNoiseFilter();

  edm::Handle<reco::BeamHaloSummary> beamHaloSummaryH;
  iEvent.getByLabel("BeamHaloSummary",beamHaloSummaryH);

  bool beamHaloLooseId = beamHaloSummaryH->LooseId(); 
  bool beamHaloTightId = beamHaloSummaryH->TightId();

  eventData.LooseNoiseFilter_ = passNoiseLoose ? 1 : 0;
  eventData.TightNoiseFilter_ = passNoiseTight ? 1 : 0;

  eventData.BeamHaloLooseId_ = beamHaloLooseId ? 1 : 0;
  eventData.BeamHaloTightId_ = beamHaloTightId ? 1 : 0;


//
// Vertex
////////////////

  // Access vertex collection
  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  iEvent.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex>& vtxColl = *(vertexCollectionH.product());

  // Find number of good vertices
  int nGoodVertices = 0;
  for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
     if(!vtx->isValid()) continue; // skip non-valid vertices
     if(vtx->isFake()) continue; // skip vertex from beam spot
     ++nGoodVertices;
  } 
  eventData.nVertex_ = nGoodVertices;

  const reco::Vertex& primVertex = (*vertexCollectionH)[0];
  if(primVertex.isValid() && !primVertex.isFake()){
     eventData.posXPrimVtx_ = primVertex.x();
     eventData.posYPrimVtx_ = primVertex.y();
     eventData.posZPrimVtx_ = primVertex.z();
  } else{
     eventData.posXPrimVtx_ = -999.;
     eventData.posYPrimVtx_ = -999.;
     eventData.posZPrimVtx_ = -999.;
  }


//
// HF Energy
////////////

        Handle<CaloTowerCollection> caloTowers;
        iEvent.getByLabel( "towerMaker", caloTowers );

        HFEnergyMinus = 0;
        HFEnergyPlus = 0;
        GapSide = 0;
        nHFMinus = 0;
        nHFPlus = 0;
        nLowHFMinus = 0;
        nLowHFPlus = 0;
        nHighHFMinus = 0;
        nHighHFPlus = 0;

        for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
        {

            // All HF
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
            {
                if (cal->energy() > HFNoise)
                {
                    if (cal->zside() < 0)
                    {
                        nHFMinus++;
                        HFEnergyMinus += cal->energy();
                    }
                    if (cal->zside() > 0)
                    {
                        HFEnergyPlus += cal->energy();
                        nHFPlus++;
                    }
                }
            }

            //nHFSlices - Low
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) <= 4.0)
            {
                if (cal->energy() > HFNoise)
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
                if (cal->energy() > HFNoise)
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

        if (HFEnergyPlus >= HFEnergyMinus)
        {
            GapSide = -1;
        }
        if (HFEnergyPlus < HFEnergyMinus)
        {
            GapSide = 1;
        }

double aSumE = 0;
aSumE = (HFEnergyPlus - HFEnergyMinus)/(HFEnergyPlus + HFEnergyMinus);

eventData.sumEnergyHFPlus_ = HFEnergyPlus;
eventData.sumEnergyHFMinus_ = HFEnergyMinus;
eventData.aEnergy_ = aSumE;
eventData.GapSide_ = GapSide;

eventData.multiHFPlus_ = nHFPlus;
eventData.multiHFMinus_ = nHFMinus;
eventData.multiHFPlusLow_ = nLowHFPlus;
eventData.multiHFMinusLow_ = nLowHFMinus;
eventData.multiHFPlusHigh_ = nHighHFPlus;
eventData.multiHFMinusHigh_ = nHighHFMinus;


//
// Leading Jets
///////////////

Handle<reco::CaloJetCollection> jetsis5;
iEvent.getByLabel(calAlgo_,jetsis5);


// Declaring Variables
int jet5size = jetsis5->size();
int Njets5=0.;
int ijet5;
const reco::CaloJet* jet1=NULL;
const reco::CaloJet* jet2=NULL;


jet1Eta = 0;
jet1Phi = 0;
jet1Pt  = 0;

jet2Eta = 0;
jet2Phi = 0;
jet2Pt  = 0;

DijetMass = 0;


// Just two Jets
if (jet5size >= 2){

JetIDSelectionFunctor jet_ID_pure09_tight( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE );

   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;

    jet_ID_helper_.calculate( iEvent, ijet5 );
    bool passed_tight = jet_ID_pure09_tight( ijet5, jet_ID_struct_ );
		
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
jet1Pt  = jet1->pt();

jet2Eta = jet2->eta();
jet2Phi = jet2->phi();
jet2Pt  = jet2->pt();

// Di-jet mass
math::XYZTLorentzVector DijetSystem(0.,0.,0.,0.);
DijetSystem += jet1->p4();
DijetSystem += jet2->p4();
DijetMass = DijetSystem.M(); 

eventData.LeadingJet1Eta_= jet1Eta;
eventData.LeadingJet1Phi_= jet1Phi;
eventData.LeadingJet1Pt_= jet1Pt;
eventData.LeadingJet2Eta_= jet2Eta;
eventData.LeadingJet2Phi_= jet2Phi;
eventData.LeadingJet2Pt_= jet2Pt;
eventData.DijetMass_= DijetMass;

}

//
// Selecting Good Tracks
////////////////////////

    Handle< TrackCollection > trackHandle;
    iEvent.getByLabel("generalTracks",trackHandle);

    goodTracksCount = 0;

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

eventData.goodTracks_= goodTracksCount;

//
// Particle Flow Eta Max e Eta Min
////////////////////////////////////

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
double xi_n = 0;
double xi=0;

// Particle Flow Objects
   if (pfsissize >= 2){

   for(ipfsis=0; ipfsis < pfsissize; ++ipfsis){
   ++Npfsis;		
   const reco::PFCandidate* pfsis_max = &((*pfsis)[ipfsis]);

   if (pfsis_max->eta()>0){
   xi_n += (pfsis_max->energy()-pfsis_max->pz());
   }

   if(pfsis_max->eta()<0){
   xi_n += (pfsis_max->energy()+pfsis_max->pz());
   }



      
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

    xi = xi_n/(7000.0);

    pfsis1Eta_max = pfsis1_max->eta();
    pfsis2Eta_max = pfsis2_max->eta();

    //cout << "Maximum Eta" << pfsis1Eta_max << endl;
    //cout << "Second Maximum Eta" << pfsis2Eta_max << endl;


    eventData.xi_=xi;
    eventData.pfetamax_= pfsis1_max->eta();
    eventData.pfetamaxpT_= pfsis1_max->pt();
    eventData.pfetamaxphi_= pfsis1_max->phi();
     

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

    eventData.pfetamin_= pfsis1_min->eta();
    eventData.pfetaminpT_= pfsis1_min->pt();
    eventData.pfetaminphi_= pfsis1_min->phi();

    eventData.pfdeltaeta_=fabs(pfsis1_max->eta() - pfsis1_min->eta());

    }

treeEventData->SetWeight(weight);
treeEventData->Fill();

}

void
CreateTTreeData::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(CreateTTreeData);
