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

class LeadingJetsFilter : public edm::EDFilter
{
public:
    explicit LeadingJetsFilter(const edm::ParameterSet&);
    ~LeadingJetsFilter();

private:
    virtual void beginJob() ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    string calAlgoFilter;
    double PtCut1;
    double PtCut2;
    bool jets_forward;
    
};

LeadingJetsFilter::LeadingJetsFilter(const edm::ParameterSet& iConfig) :
        calAlgoFilter( iConfig.getUntrackedParameter<string>("calAlgoFilter")),
        PtCut1( iConfig.getParameter<double>( "PtCut1" )), 
        PtCut2( iConfig.getParameter<double>( "PtCut2" )),
        jets_forward( iConfig.getUntrackedParameter<bool>("JetsForward"))
{
}


LeadingJetsFilter::~LeadingJetsFilter()
{
}

bool
LeadingJetsFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

bool FilterResult = false;

Handle<reco::CaloJetCollection> jetsis5;
iEvent.getByLabel(calAlgoFilter,jetsis5);

int jet5size = jetsis5->size();
int ijet5;

double jet1Pt = 0;
double jet2Pt = 0;
double jet1Eta = 0;
double jet2Eta = 0;

const reco::CaloJet* jet1=NULL;
const reco::CaloJet* jet2=NULL;


   if (jet5size >= 2){

     for(ijet5=0; ijet5 < jet5size; ++ijet5){
     const reco::CaloJet* jetAll = &((*jetsis5)[ijet5]);
 
            if (jetAll==NULL) continue;
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

//--------------

if(jets_forward){

      if ( (jet1Eta > 2.9 && jet2Eta < 5.2) || (jet1Eta < -2.9 && jet2Eta > -5.2)){
        if (jet1Pt >= PtCut1 && jet2Pt >= PtCut2){
            FilterResult = true;
        }

        else {
            FilterResult = false;
        }

      }

}

else {

      if (jet1Pt >= PtCut1 && jet2Pt >= PtCut2){
          FilterResult = true;
      }

      else {
          FilterResult = false;
      }

}

//--------------

}

      return FilterResult;

}

void
LeadingJetsFilter::beginJob()
{
}

void
LeadingJetsFilter::endJob()
{
}

DEFINE_FWK_MODULE(LeadingJetsFilter);
