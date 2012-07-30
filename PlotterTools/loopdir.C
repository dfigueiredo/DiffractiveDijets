void loopdir() {

   TFile *f1= new TFile("DQM_V0002_R000000001__RelValSingleMuPt1000__CMSSW_5_2_0_pre6-START52_V2-v3__DQM.root");
   TList* tdf_obs = f1->GetDirectory("/DQMData/Run 1/Muons/Run summary/RecoMuonV")->GetListOfKeys(); 
   char *hName1 = new char [80];
   char *filesave = new char[80];

 TList *objlist = f1->GetListOfKeys();
 TIterator *itr = objlist->MakeIterator();
 TKey *key;

TString *currentdir;
TIter iter(tdf_obs);

  while ((key = static_cast<TKey*>(iter.Next())) != 0) {

    TObject *obj = key->ReadObj();
    TH1F *h = (TH1F*)key->ReadObj();
    sprintf(filesave,"%s%s.png",h->GetName(),"Validation");

    cout << "This object has name " << obj->GetName() << endl;

    if (obj->IsA()->InheritsFrom("TDirectory")) {
      TDirectory *d = static_cast<TDirectory*>(obj);
      cout << "Dir: " << d->GetPath() << endl;
    } 

    if (h->GetEntries() != 0){ // Defense
     if(obj->IsA()->InheritsFrom("TH1")){
       TCanvas *singleCanvas = new TCanvas("canvas","canvas");
       singleCanvas->cd();   
       hName1=h->GetName();
       h->Draw();
       singleCanvas->Update();
       singleCanvas->SaveAs(filesave);
       delete singleCanvas;
     }
    }

  }
  

}
      
