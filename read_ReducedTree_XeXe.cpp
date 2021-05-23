#include "TTree.h"
#include "TFile.h"
#include "TList.h"
#include "TH1F.h"

//____________________________________________________________________________________________________________________________________________________________________________________________________
void read_ReducedTree_XeXe ()  {
  
    Float_t m1qpi=0.1396*0.1396;
	Float_t m2qpi=0.1396*0.1396;  
	Float_t m1qka=0.493677*0.493677;
	Float_t m2qka=0.493677*0.493677;
//    Float_t m1qpr=0.938272*0.938272;
    Float_t pt;
    
    //Open Input File
    const Int_t nFiles=1;
 
//DEFINIZIONI ISTOGRAMMI
     TH1I* hntracks= new TH1I("hntracks","No.of tracks",5000,0,5000);  
     TH1F* hxv= new TH1F("hxv","X-vertex",200,-0.5,0.5);  
     TH1F* hyv= new TH1F("hyv","Y-vertex",200,-0.5,0.5);  
	 TH1F* hzv= new TH1F("hzv","Z-vertex",400,-20.,20.);  
	 TH1F* hcentrality= new TH1F("hcentrality","Centrality",100,0.,100.);   
	 TH1F* hnV0= new TH1F("hnV0","No. of V0",10000,0.,10000.);  
	 
	 TH1F* hpt= new TH1F("hpt","pt",1000,0.,10.); 
	 
	 TH2F* hArmenteros= new TH2F("hArmenteros","Armenteros Plot",100,-1.0,1.,100,0.0,0.5); 	      
 	 TH1F* hmass= new TH1F("hmass"," Invariant Mass",1000,0.,2.0);
 	 
	       
    for ( Int_t iFile=0 ; iFile<nFiles ; iFile++ )  {

        TFile *inputFile = TFile::Open (Form("C:\\Users\\Riggi\\Desktop\\XeXe_data\\reducedTree_XeXeCollisions_%d.root",iFile));
        TList *inputList = (TList*) inputFile -> Get ("Input");
   
        
//Variables (Global tracks) LE PRIME SONO VARIABILI GLOBALI
         Float_t centrality;//centrality in percentage VG
         Int_t   nTracks;//total number of charged tracks in each event VB
         Int_t   nV0particles;//total number of V0 candidates in each eventVB
         Float_t xVertex;
         Float_t yVertex;
         Float_t zVertex;

//VARIABILI DEFINITNITE PER LA TRACCIA
         Float_t px;
         Float_t py;
         Float_t pz;
         Float_t integratedLength;//considered only for L > 350, INTEGRATA SU TUTTA LA LUNGHEZZA DELLA TRACCIA
         Float_t timeOfFlight;  // Non chiaro il significato dello spettro di qs variabile *******
         Int_t trackID;//sign gives the charge

         Float_t dcaxy;//transverse DCA to primary vertex LE USO PER CAPIRE LA DISTANZA DAL VERTICE PRIMARIO
         Float_t dcaz;//longitudinal DCA to primary vertex

//VARIBILI DELLA TPC E ITS 
         Int_t nTPC_Clusters;
         Int_t nITS_Clusters;
         Int_t nTPC_FindableClusters;
         Int_t nTPC_CrossedRows;
         Int_t nTPC_Clusters_dEdx; //CLUSTER CHE HANNO DATO INFORMAZIONE SU DE/DX

//I SEI STRATI DI SILICIO DELL'ITS HANNO DATO SEGNALE??
         Int_t HasPointOnITSLayer0;//0=no hit, 1=hit
         Int_t HasPointOnITSLayer1;//0=no hit, 1=hit
         Int_t HasPointOnITSLayer2;//0=no hit, 1=hit
         Int_t HasPointOnITSLayer3;//0=no hit, 1=hit
         Int_t HasPointOnITSLayer4;//0=no hit, 1=hit
         Int_t HasPointOnITSLayer5;//0=no hit, 1=hit


         Float_t chi2_NDF;

//DI QUANTE DEVIAZIONI STANDARD IL SEGNALE MISURATO SI DISCOSTA DA QUELLO ATTESSO SE FOSSE QUEL TIPO DI PARTICELLA
         Float_t nSigmaITS_Pion;
         Float_t nSigmaTPC_Pion;
         Float_t nSigmaTOF_Pion;
         Float_t nSigmaITS_Kaon;
         Float_t nSigmaTPC_Kaon;
         Float_t nSigmaTOF_Kaon;
         Float_t nSigmaITS_Prot;
         Float_t nSigmaTPC_Prot;
         Float_t nSigmaTOF_Prot;
         Float_t nSigmaITS_Elec;
         Float_t nSigmaTPC_Elec;
         Float_t nSigmaTOF_Elec;

//posso imporre Nsigmpion<3 & nsigmapionTPC<3, ma così forse è troppo stringente la condizione 
//Usare apprroccio Bayesiano e dire la probabilità per cui è un pione nella TPC/ITS/TOF, valutare una prob complessiva e accettare 
//solo valori > di una certa quantità

//Variables (V0 - First Daughter: positive charge)
         Float_t px_Daughter1;
         Float_t py_Daughter1;
         Float_t pz_Daughter1;
         Float_t dcaxy_Daughter1;
         Float_t dcaz_Daughter1;
         Int_t nTPC_Clusters_Daughter1;
         Int_t nTPC_FindableClusters_Daughter1;
         Int_t nTPC_CrossedRows_Daughter1;
         Int_t nTPC_Clusters_dEdx_Daughter1;
         Int_t nITS_Clusters_Daughter1;
         Int_t HasPointOnITSLayer0_Daughter1;
         Int_t HasPointOnITSLayer1_Daughter1;
         Int_t HasPointOnITSLayer2_Daughter1;
         Int_t HasPointOnITSLayer3_Daughter1;
         Int_t HasPointOnITSLayer4_Daughter1;
         Int_t HasPointOnITSLayer5_Daughter1;
         Float_t chi2_NDF_Daughter1;
         Float_t nSigmaITS_Pion_Daughter1;
         Float_t nSigmaTPC_Pion_Daughter1;
         Float_t nSigmaTOF_Pion_Daughter1;
         Float_t nSigmaITS_Kaon_Daughter1;
         Float_t nSigmaTPC_Kaon_Daughter1;
         Float_t nSigmaTOF_Kaon_Daughter1;
         Float_t nSigmaITS_Prot_Daughter1;
         Float_t nSigmaTPC_Prot_Daughter1;
         Float_t nSigmaTOF_Prot_Daughter1;
         Float_t nSigmaITS_Elec_Daughter1;
         Float_t nSigmaTPC_Elec_Daughter1;
         Float_t nSigmaTOF_Elec_Daughter1;
         
//Variables (V0 - Second Daughter: negative charge)
         Float_t px_Daughter2;
         Float_t py_Daughter2;
         Float_t pz_Daughter2;
         Float_t dcaxy_Daughter2;
         Float_t dcaz_Daughter2;
         Int_t nTPC_Clusters_Daughter2;
         Int_t nTPC_FindableClusters_Daughter2;
         Int_t nTPC_CrossedRows_Daughter2;
         Int_t nTPC_Clusters_dEdx_Daughter2;
         Int_t nITS_Clusters_Daughter2;
         Int_t HasPointOnITSLayer0_Daughter2;
         Int_t HasPointOnITSLayer1_Daughter2;
         Int_t HasPointOnITSLayer2_Daughter2;
         Int_t HasPointOnITSLayer3_Daughter2;
         Int_t HasPointOnITSLayer4_Daughter2;
         Int_t HasPointOnITSLayer5_Daughter2;
         Float_t chi2_NDF_Daughter2;
         Float_t nSigmaITS_Pion_Daughter2;
         Float_t nSigmaTPC_Pion_Daughter2;
         Float_t nSigmaTOF_Pion_Daughter2;
         Float_t nSigmaITS_Kaon_Daughter2;
         Float_t nSigmaTPC_Kaon_Daughter2;
         Float_t nSigmaTOF_Kaon_Daughter2;
         Float_t nSigmaITS_Prot_Daughter2;
         Float_t nSigmaTPC_Prot_Daughter2;
         Float_t nSigmaTOF_Prot_Daughter2;
         Float_t nSigmaITS_Elec_Daughter2;
         Float_t nSigmaTPC_Elec_Daughter2;
         Float_t nSigmaTOF_Elec_Daughter2;
         
//Pair Variables: LEGATE ALLA COPPIA
         Float_t cosPointingAngle;
         Float_t dcaV0Daughters; //distance closest approche
         Float_t radius;//radial distance between secondary vertex and geometrical center of ALICE
         Float_t decayLength;//distance between secondary vertex and primary vertex (the latter might be shifted compared to the geometrical center of ALICE)
         Float_t alphaV0; //hermenteros
         Float_t qtV0; //hermenteros
         
         
//Tree with global tracks
         TTree *treePrimaryTracks = (TTree*) inputFile -> Get ("reducedTree");         
         treePrimaryTracks -> SetBranchAddress("centrality",&centrality);
         treePrimaryTracks -> SetBranchAddress("nTracks",&nTracks);
         treePrimaryTracks -> SetBranchAddress("xVertex",&xVertex);
         treePrimaryTracks -> SetBranchAddress("yVertex",&yVertex);
         treePrimaryTracks -> SetBranchAddress("zVertex",&zVertex);
         treePrimaryTracks -> SetBranchAddress("px",&px);
         treePrimaryTracks -> SetBranchAddress("py",&py);
         treePrimaryTracks -> SetBranchAddress("pz",&pz);
         treePrimaryTracks -> SetBranchAddress("integratedLength",&integratedLength);
         treePrimaryTracks -> SetBranchAddress("timeOfFlight",&timeOfFlight);
         treePrimaryTracks -> SetBranchAddress("trackID",&trackID);
         treePrimaryTracks -> SetBranchAddress("dcaxy",&dcaxy);
         treePrimaryTracks -> SetBranchAddress("dcaz",&dcaz);
         treePrimaryTracks -> SetBranchAddress("nTPC_Clusters",&nTPC_Clusters);
         treePrimaryTracks -> SetBranchAddress("nITS_Clusters",&nITS_Clusters);
         treePrimaryTracks -> SetBranchAddress("nTPC_FindableClusters",&nTPC_FindableClusters);
         treePrimaryTracks -> SetBranchAddress("nTPC_CrossedRows",&nTPC_CrossedRows);
         treePrimaryTracks -> SetBranchAddress("nTPC_Clusters_dEdx",&nTPC_Clusters_dEdx);
         treePrimaryTracks -> SetBranchAddress("HasPointOnITSLayer0",&HasPointOnITSLayer0);
         treePrimaryTracks -> SetBranchAddress("HasPointOnITSLayer1",&HasPointOnITSLayer1);
         treePrimaryTracks -> SetBranchAddress("HasPointOnITSLayer2",&HasPointOnITSLayer2);
         treePrimaryTracks -> SetBranchAddress("HasPointOnITSLayer3",&HasPointOnITSLayer3);
         treePrimaryTracks -> SetBranchAddress("HasPointOnITSLayer4",&HasPointOnITSLayer4);
         treePrimaryTracks -> SetBranchAddress("HasPointOnITSLayer5",&HasPointOnITSLayer5);
         treePrimaryTracks -> SetBranchAddress("chi2_NDF",&chi2_NDF);
         treePrimaryTracks -> SetBranchAddress("nSigmaITS_Pion",&nSigmaITS_Pion);
         treePrimaryTracks -> SetBranchAddress("nSigmaTPC_Pion",&nSigmaTPC_Pion);
         treePrimaryTracks -> SetBranchAddress("nSigmaTOF_Pion",&nSigmaTOF_Pion);
         treePrimaryTracks -> SetBranchAddress("nSigmaITS_Kaon",&nSigmaITS_Kaon);
         treePrimaryTracks -> SetBranchAddress("nSigmaTPC_Kaon",&nSigmaTPC_Kaon);
         treePrimaryTracks -> SetBranchAddress("nSigmaTOF_Kaon",&nSigmaTOF_Kaon);
         treePrimaryTracks -> SetBranchAddress("nSigmaITS_Prot",&nSigmaITS_Prot);
         treePrimaryTracks -> SetBranchAddress("nSigmaTPC_Prot",&nSigmaTPC_Prot);
         treePrimaryTracks -> SetBranchAddress("nSigmaTOF_Prot",&nSigmaTOF_Prot);
         treePrimaryTracks -> SetBranchAddress("nSigmaITS_Elec",&nSigmaITS_Elec);
         treePrimaryTracks -> SetBranchAddress("nSigmaTPC_Elec",&nSigmaTPC_Elec);
         treePrimaryTracks -> SetBranchAddress("nSigmaTOF_Elec",&nSigmaTOF_Elec);
         
         
//Tree with V0 candidates
         TTree *treeV0 = (TTree*)  inputFile -> Get ("reducedTree_V0");         
         treeV0 -> SetBranchAddress("nV0particles",&nV0particles);
         treeV0 -> SetBranchAddress("px_Daughter1",&px_Daughter1);
         treeV0 -> SetBranchAddress("py_Daughter1",&py_Daughter1);
         treeV0 -> SetBranchAddress("pz_Daughter1",&pz_Daughter1);
         treeV0 -> SetBranchAddress("dcaxy_Daughter1",&dcaxy_Daughter1);
         treeV0 -> SetBranchAddress("dcaz_Daughter1",&dcaz_Daughter1);
         treeV0 -> SetBranchAddress("nTPC_Clusters_Daughter1",&nTPC_Clusters_Daughter1);
         treeV0 -> SetBranchAddress("nTPC_FindableClusters_Daughter1",&nTPC_FindableClusters_Daughter1);
         treeV0 -> SetBranchAddress("nTPC_CrossedRows_Daughter1",&nTPC_CrossedRows_Daughter1);
         treeV0 -> SetBranchAddress("nTPC_Clusters_dEdx_Daughter1",&nTPC_Clusters_dEdx_Daughter1);
         treeV0 -> SetBranchAddress("nITS_Clusters_Daughter1",&nITS_Clusters_Daughter1);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer0_Daughter1",&HasPointOnITSLayer0_Daughter1);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer1_Daughter1",&HasPointOnITSLayer1_Daughter1);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer2_Daughter1",&HasPointOnITSLayer2_Daughter1);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer3_Daughter1",&HasPointOnITSLayer3_Daughter1);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer4_Daughter1",&HasPointOnITSLayer4_Daughter1);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer5_Daughter1",&HasPointOnITSLayer5_Daughter1);
         treeV0 -> SetBranchAddress("chi2_NDF_Daughter1",&chi2_NDF_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaITS_Pion_Daughter1",&nSigmaITS_Pion_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTPC_Pion_Daughter1",&nSigmaTPC_Pion_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTOF_Pion_Daughter1",&nSigmaTOF_Pion_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaITS_Kaon_Daughter1",&nSigmaITS_Kaon_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTPC_Kaon_Daughter1",&nSigmaTPC_Kaon_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTOF_Kaon_Daughter1",&nSigmaTOF_Kaon_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaITS_Prot_Daughter1",&nSigmaITS_Prot_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTPC_Prot_Daughter1",&nSigmaTPC_Prot_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTOF_Prot_Daughter1",&nSigmaTOF_Prot_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaITS_Elec_Daughter1",&nSigmaITS_Elec_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTPC_Elec_Daughter1",&nSigmaTPC_Elec_Daughter1);
         treeV0 -> SetBranchAddress("nSigmaTOF_Elec_Daughter1",&nSigmaTOF_Elec_Daughter1);
         treeV0 -> SetBranchAddress("nV0particles",&nV0particles);
         treeV0 -> SetBranchAddress("px_Daughter2",&px_Daughter2);
         treeV0 -> SetBranchAddress("py_Daughter2",&py_Daughter2);
         treeV0 -> SetBranchAddress("pz_Daughter2",&pz_Daughter2);
         treeV0 -> SetBranchAddress("dcaxy_Daughter2",&dcaxy_Daughter2);
         treeV0 -> SetBranchAddress("dcaz_Daughter2",&dcaz_Daughter2);
         treeV0 -> SetBranchAddress("nTPC_Clusters_Daughter2",&nTPC_Clusters_Daughter2);
         treeV0 -> SetBranchAddress("nTPC_FindableClusters_Daughter2",&nTPC_FindableClusters_Daughter2);
         treeV0 -> SetBranchAddress("nTPC_CrossedRows_Daughter2",&nTPC_CrossedRows_Daughter2);
         treeV0 -> SetBranchAddress("nTPC_Clusters_dEdx_Daughter2",&nTPC_Clusters_dEdx_Daughter2);
         treeV0 -> SetBranchAddress("nITS_Clusters_Daughter2",&nITS_Clusters_Daughter2);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer0_Daughter2",&HasPointOnITSLayer0_Daughter2);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer1_Daughter2",&HasPointOnITSLayer1_Daughter2);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer2_Daughter2",&HasPointOnITSLayer2_Daughter2);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer3_Daughter2",&HasPointOnITSLayer3_Daughter2);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer4_Daughter2",&HasPointOnITSLayer4_Daughter2);
         treeV0 -> SetBranchAddress("HasPointOnITSLayer5_Daughter2",&HasPointOnITSLayer5_Daughter2);
         treeV0 -> SetBranchAddress("chi2_NDF_Daughter2",&chi2_NDF_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaITS_Pion_Daughter2",&nSigmaITS_Pion_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTPC_Pion_Daughter2",&nSigmaTPC_Pion_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTOF_Pion_Daughter2",&nSigmaTOF_Pion_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaITS_Kaon_Daughter2",&nSigmaITS_Kaon_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTPC_Kaon_Daughter2",&nSigmaTPC_Kaon_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTOF_Kaon_Daughter2",&nSigmaTOF_Kaon_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaITS_Prot_Daughter2",&nSigmaITS_Prot_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTPC_Prot_Daughter2",&nSigmaTPC_Prot_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTOF_Prot_Daughter2",&nSigmaTOF_Prot_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaITS_Elec_Daughter2",&nSigmaITS_Elec_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTPC_Elec_Daughter2",&nSigmaTPC_Elec_Daughter2);
         treeV0 -> SetBranchAddress("nSigmaTOF_Elec_Daughter2",&nSigmaTOF_Elec_Daughter2);
         treeV0 -> SetBranchAddress("cosPointingAngle",&cosPointingAngle);
         treeV0 -> SetBranchAddress("dcaV0Daughters",&dcaV0Daughters);
         treeV0 -> SetBranchAddress("radius",&radius);
         treeV0 -> SetBranchAddress("decayLength",&decayLength);
         treeV0 -> SetBranchAddress("alphaV0",&alphaV0);
         treeV0 -> SetBranchAddress("qtV0",&qtV0);
         
    
                     
         Int_t nevent=0;
         Int_t nTracks_old=-1000;
         Float_t px_old=-1000.;
        for ( Int_t iEntry=0 ; iEntry<treePrimaryTracks->GetEntries() ; iEntry++ )  {
              //    for ( Int_t iEntry=0 ; iEntry<10 ; iEntry++ )  {	
         treePrimaryTracks -> GetEntry (iEntry);
         if( nTracks!=nTracks_old && px!=px_old) {//New event NB
			hntracks->Fill(nTracks);
		    hxv->Fill(xVertex);
		    hyv->Fill(yVertex);
		    hzv->Fill(zVertex);	
			hcentrality->Fill(centrality);	    
		    nTracks_old=nTracks;
			px_old=px;	    
		    nevent++;
          }      
// Istruzioni all'interno dello stesso evento
          pt=TMath::Sqrt(px*px+py*py);
          hpt->Fill(pt);
         }        
           cout<<"N.events from Global Tracks tree = "<<nevent<<endl; 
         
//
// V0 analysis
//       
         Int_t nev_V0=0;
         Int_t nV0_old=-1000;
         Float_t px_Daughter1_old=-1000.0;
         Float_t px_Daughter2_old=-1000.0;
         for ( Int_t iEntry=0 ; iEntry<treeV0->GetEntries() ; iEntry++ )  {     
		//for ( Int_t iEntry=0 ; iEntry<1000 ; iEntry++ )  {         
         treeV0 -> GetEntry (iEntry); 
//
// Cut to define Armenteros plot per K0s
//
          if(abs(alphaV0)<1 && qtV0<0.3 && cosPointingAngle>0.98 && dcaV0Daughters<0.5 
      //if(abs(alphaV0)<1 && qtV0<0.3 && dcaV0Daughters<0.5  
		   && abs(nSigmaTPC_Pion_Daughter1)<3 && abs(nSigmaTPC_Pion_Daughter2)<3   
		   && dcaxy_Daughter1*dcaxy_Daughter1+dcaz_Daughter1*dcaz_Daughter1>0.05
		   && dcaxy_Daughter2*dcaxy_Daughter2+dcaz_Daughter2*dcaz_Daughter2>0.05
		   && nTPC_CrossedRows_Daughter1>=70 && nTPC_CrossedRows_Daughter2>=70
		   && nTPC_CrossedRows_Daughter1/nTPC_FindableClusters_Daughter1>0.8
//		   && radius>5.0
           ) hArmenteros->Fill(alphaV0,qtV0);
		   

//MAGARI UNO GUARDA HARMENTEROS E POI SI FA I TAGLI 
          
// Build invariant mass spectra

	TLorentzVector particle1;
	TLorentzVector particle2;
   	TLorentzVector pair12;

	  Float_t E1=TMath::Sqrt(px_Daughter1*px_Daughter1+py_Daughter1*py_Daughter1+
	                         pz_Daughter1*pz_Daughter1+m1qpi);
	  particle1.SetPxPyPzE(px_Daughter1, py_Daughter1, pz_Daughter1, E1);
	  Float_t E2=TMath::Sqrt(px_Daughter2*px_Daughter2+py_Daughter2*py_Daughter2+
	                         pz_Daughter2*pz_Daughter2+m2qpi);
	  particle2.SetPxPyPzE(px_Daughter2, py_Daughter2, pz_Daughter2, E2);	      
          pair12 = particle1 + particle2;
	  Float_t M_inv = pair12.M();

// Lambda selection
/*
	  if( qtV0<0.11  && qtV0> 0.03 && abs(alphaV0)>0.45
       	   && cosPointingAngle>0.99 && dcaV0Daughters<0.5
		   && abs(nSigmaTPC_Prot_Daughter1)<1 && abs(nSigmaTPC_Pion_Daughter2)<1 
		   && dcaxy_Daughter1*dcaxy_Daughter1+dcaz_Daughter1*dcaz_Daughter1>0.05  //Sono secondarie, quindi >di una certa quantità
		   && dcaxy_Daughter2*dcaxy_Daughter2+dcaz_Daughter2*dcaz_Daughter2>0.05
		   && nTPC_CrossedRows_Daughter1>=70 && nTPC_CrossedRows_Daughter2>=70
//		   && nTPC_CrossedRows_Daughter1/nTPC_FindableClusters_Daughter1>0.8
           && radius>5.0 ) hmass->Fill(M_inv); 
           

// Phi selection
	  if( qtV0<0.15  && qtV0> 0.03 && abs(alphaV0)<0.3 && cosPointingAngle>0.98 
       	   && dcaV0Daughters<0.5
		   && abs(nSigmaTPC_Kaon_Daughter1)<1 && abs(nSigmaTPC_Kaon_Daughter2)<1 
		   && dcaxy_Daughter1*dcaxy_Daughter1+dcaz_Daughter1*dcaz_Daughter1<0.5
		   && dcaxy_Daughter2*dcaxy_Daughter2+dcaz_Daughter2*dcaz_Daughter2<0.5
		   && nTPC_CrossedRows_Daughter1>=70 && nTPC_CrossedRows_Daughter2>=70
//		   && nTPC_CrossedRows_Daughter1/nTPC_FindableClusters_Daughter1>0.8
           ) hmass->Fill(M_inv); 
*/

// K0s selection TAGLI PIU' STRINGENTI
	  if( qtV0<0.25  && qtV0> 0.03 && abs(alphaV0)<1.0 && cosPointingAngle>0.98 
       	   && dcaV0Daughters<0.5
		   && abs(nSigmaTPC_Pion_Daughter1)<3 && abs(nSigmaTPC_Pion_Daughter2)<3 
		   && dcaxy_Daughter1*dcaxy_Daughter1+dcaz_Daughter1*dcaz_Daughter1>0.05
		   && dcaxy_Daughter2*dcaxy_Daughter2+dcaz_Daughter2*dcaz_Daughter2>0.05
		   && nTPC_CrossedRows_Daughter1>=70 && nTPC_CrossedRows_Daughter2>=70
		   && nTPC_CrossedRows_Daughter1/nTPC_FindableClusters_Daughter1>0.8
           ) hmass->Fill(M_inv); 
           
		    
       if(nV0particles!=nV0_old && px_Daughter1!=px_Daughter1_old && px_Daughter2!=px_Daughter2_old) {
          hnV0->Fill(nV0particles);	 	          
          nV0_old=nV0particles;
          px_Daughter1_old=px_Daughter1;
          px_Daughter2_old=px_Daughter2;        
          nev_V0++;
          cout<<nev_V0<<"  "<<nV0particles<<endl;
          } 
          
          
          
// Istruzioni all'interno dello stesso evento          
          } 
          cout<<"N.events V0 ="<<nev_V0<<endl;
       
 	inputFile  -> cd();
    inputFile  -> Close(); 
    }

	 
	    
  	TCanvas* c1=new TCanvas("c1","c1");
	hntracks->Draw(); 
	TCanvas* c2=new TCanvas("c2","c2");
	hxv->Draw();  
	TCanvas* c3=new TCanvas("c3","c3");
	hyv->Draw();
	TCanvas* c4=new TCanvas("c4","c4");
	hzv->Draw();
	TCanvas* c5=new TCanvas("c5","c5");
	hcentrality->Draw();
	TCanvas* c6=new TCanvas("c6","c6");
	hnV0->Draw();	
	TCanvas* c7=new TCanvas("c7","c7");
	hArmenteros->Draw("");	
	TCanvas* c8=new TCanvas("c8","c8");
	hmass->Draw();	
	TCanvas* c9=new TCanvas("c9","c9");
	hpt->Draw();	
}
//____________________________________________________________________________________________________________________________________________________________________________________________________

