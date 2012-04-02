//
// Author: Diego Figueiredo
//


// system include files
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//JetCollection
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"

//deltaR
#include "DataFormats/Math/interface/deltaR.h"

//ROOT
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;

//
// class declaration
//

class JetsSelection : public edm::EDFilter
{
public:
    explicit JetsSelection(const edm::ParameterSet&);
    ~JetsSelection();

private:
    virtual void beginJob() ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------

    string calAlgoFilter;
    bool forward;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
JetsSelection::JetsSelection(const edm::ParameterSet& iConfig) :
        calAlgoFilter( iConfig.getUntrackedParameter<string>("calAlgoFilter")),
        forward ( iConfig.getUntrackedParameter<bool>("forward"))
{
    //now do what ever initialization is needed

}


JetsSelection::~JetsSelection()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
JetsSelection::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

bool FilterResult = false;

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
double jet1Phi_max = 0;
double jet2Phi_max = 0;
double jet1Phi_min = 0;
double jet2Phi_min = 0;
double jet1Pt_max = 0;
double jet2Pt_max = 0;
double jet1Pt_min = 0;
double jet2Pt_min = 0;


// Choose the GapSide

Handle<CaloTowerCollection> caloTowers;
iEvent.getByLabel( "towerMaker", caloTowers );

double HFEnergyMinus = 0;
double HFEnergyPlus = 0;
int GapSide = 0;

        for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
        {
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
            {
                if (cal->energy() > 4)
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
    jet1Phi_max = jet1_max->phi();
    jet2Phi_max = jet2_max->phi();
    jet1Pt_max = jet1_max->pt();
    jet2Pt_max = jet2_max->pt();

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
    jet1Phi_min = jet1_min->phi();
    jet2Phi_min = jet2_min->phi();
    jet1Pt_min = jet1_min->pt();
    jet2Pt_min = jet2_min->pt();

    }

    if (fabs(jet1Eta_max - jet1Eta_min) < 4)
    {
        if (GapSide > 0)
        {

            if(forward){
            
               if ( (jet1Eta_max > 2.9 && jet1Eta_max < 5.2) && (jet1Eta_min > 2.9 && jet1Eta_min < 5.2) )
               {
                   FilterResult = true;
               }
            
            }

            else {FilterResult = true;}

        }
        if (GapSide < 0)
        {

            if(forward){
            
               if ( (jet1Eta_max < -2.9 && jet1Eta_max > -5.2) && (jet1Eta_min < -2.9 && jet1Eta_min > -5.2) )
               {
                   FilterResult = true;
               }
            
            }

            else {FilterResult = true;}
        }
    }


return FilterResult;


}

// ------------ method called once each job just before starting event loop  ------------
void
JetsSelection::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
JetsSelection::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetsSelection);
