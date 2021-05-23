#include <TTree.h>
#include <TEventList.h>
#include <TMultiLayerPerceptron.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TMLPAnalyzer.h>
#include <TH1.h>
#include <TH2D.h>
#include <TLegend.h>
#include <Riostream.h>

void NN_exa1(){
//
// Macro to implement a neural network for particle identification (pion, kaon, proton)
// with a multi-layer (6+1) silicon telescope
//
// Input data are read from a tree containing GEANT3 simulated data
// 
//      iev   = event number
//      kpart = particle code (GEANT notation)
//      pmod  = Particle momentum
//      E1,E2,..E6 = Energy loss in each of the 6 layers telescope
//      ERES =  Residual Energy
//	
	gROOT->SetStyle("Plain");

	TChain ch("tree");
	ch.Add("rangetelescope_exp.root");
        Int_t iev;
	Double_t e1,e2,e3,e4,e5,e6,eres,pmod;
        Int_t kpart,npion_c,nkaon_c,nproton_c;
//
// Tree structure
//
    tree->SetBranchAddress("iev"  , &iev);
   	tree->SetBranchAddress("kpart", &kpart);
	tree->SetBranchAddress("pmod", &pmod);
    tree->SetBranchAddress("e1"  , &e1);
	tree->SetBranchAddress("e2"  , &e2);
	tree->SetBranchAddress("e3"  , &e3);
	tree->SetBranchAddress("e4"  , &e4);
	tree->SetBranchAddress("e5"  , &e5);
	tree->SetBranchAddress("e6"  , &e6);
	tree->SetBranchAddress("eres"  , &eres);

    //    ofstream f("effpurity_550600.txt");

	cout << "TChain entries " << tree->GetEntries() << endl;
	
	
	TEventList *learn = new TEventList("learn", "learn");
	TEventList *test = new TEventList("test", "test");
//
// nlearn = number of events for the learning phase
// ntest  = number of events for testing
//
        Int_t nlearn=4000; 
        Int_t ntest=2000; 
// Learning and test phase: select events which gave a signal in all layers
//
// Learning phase from event=0 to event=nlearn
// Test phase     from event nlearn to event (nlearn+ntest)
//
	tree->Draw(">>learn","e1*e2*e3*e4*e5*e6>0&&pmod<0.50","",nlearn,0);
	tree->Draw(">>test","e1*e2*e3*e4*e5*e6>0&&pmod<0.50","",ntest,nlearn);
//
// Implement the network topology
// 7 input neurons: E1,...E6,pmod
// 8 hidden neurons
// 1 output neuron: kpart
//
	TMultiLayerPerceptron network("e1,e2,e3,e4,e5,e6,pmod:8:kpart", tree, learn, test, TNeuron::kTanh);
//
// Choose the learning method
//
	network.SetLearningMethod(network.kBFGS);

// Possible changes in precision
//
//	network.SetTau(0.001);
//  network.SetEta(0.001);
//	network.SetReset(250);
//
// Learning phase
//
// No.of epochs
// Update every 5 epochs
//
	network.Train(200,"text,graph,update=5");
//	network.Train(50,"text,graph,current,update=5, +");
//
// Save the weights for future use
//
	network.DumpWeights("weights");

//
// Apply the trained network 
//
	network.LoadWeights("weights");
   	TMLPAnalyzer ana(network);

// Initialisation
   	ana.GatherInformations();
//   
// Define output histograms
//
	TH1D *hdiffcode = new TH1D("hdiffcode", "", 100, 0.,  20.);
	TH1D *hpion = new TH1D("hpion", "", 100, 0.,  20.);
	TH1D *hkaon = new TH1D("hkaon", "", 100, 0.,  20.);
	TH1D *hproton = new TH1D("hproton", "", 100, 0.,  20.);


   	Double_t params[6], diffcode, NNOut;
	Int_t i;
        Double_t npion_pion,npion_kaon,npion_proton;
        Double_t nkaon_pion,nkaon_kaon,nkaon_proton;
        Double_t nproton_pion,nproton_kaon,nproton_proton;
        Double_t ntrue_pion,ntrue_kaon,ntrue_proton;
        Double_t eff_pion,eff_kaon,eff_proton;
        Double_t pur_pion,pur_kaon,pur_proton;

        ntrue_pion=0;
        ntrue_kaon=0;
        ntrue_proton=0;
//
// Loop on remaining events, from event=nlearn+ntest to the end
//
   	for (i = nlearn+ntest; i < tree->GetEntries(); i++) {
//
// Read the event from tree
//
      	tree->GetEntry(i);
      
//
// Apply selection on events with a signal on all layers
// and in a momentum window
//	
        if(e1*e2*e3*e4*e5*e6==0.0) continue;
        if(pmod>0.5) continue;
// 
// Count the true particles
//
        if(kpart==8) ntrue_pion++;
        if(kpart==11) ntrue_kaon++;
        if(kpart==14) ntrue_proton++;
//
// Apply the network
//
	params[0]  = e1;   
    params[1]  = e2; 
	params[2]  = e3;
	params[3]  = e4;
	params[4]  = e5;
	params[5]  = e6;
	params[6]  = pmod;
//
// Evaluate the difference between output from the network and true value
//		
        NNOut = network.Evaluate(0,params);
//        diffcode = NNOut - kpart; 

	hdiffcode->Fill(NNOut);
        if(kpart==8) hpion->Fill(NNOut);
        if(kpart==11) hkaon->Fill(NNOut);
        if(kpart==14) hproton->Fill(NNOut);
//
// Choose cuts from the previous histograms 
// to classify particles according to the output from the network
//
// npion_pion = number of true pions recognized as pions
// npion_kaon = number of true kaons recognized as pions
// ........................
//    
        if(NNOut<9.3&&kpart==8) npion_pion++;
        if(NNOut<9.3&&kpart==11) npion_kaon++;
        if(NNOut<9.3&&kpart==14) npion_proton++;
        
        if(NNOut>9.3&&NNOut<12&&kpart==8) nkaon_pion++;
        if(NNOut>9.3&&NNOut<12&&kpart==11) nkaon_kaon++;    
        if(NNOut>9.3&&NNOut<12&&kpart==14) nkaon_proton++;
        
        if(NNOut>12&&kpart==8) nproton_pion++;
        if(NNOut>12&&kpart==11) nproton_kaon++;
        if(NNOut>12&&kpart==14) nproton_proton++;
//
// Evaluates efficiency and purity on each species
// 
        eff_pion=npion_pion/ntrue_pion;
        pur_pion=npion_pion/(npion_pion+npion_kaon+npion_proton);

        eff_kaon=nkaon_kaon/ntrue_kaon;
        pur_kaon=nkaon_kaon/(nkaon_kaon+nkaon_pion+nkaon_proton);
   
        eff_proton=nproton_proton/ntrue_proton;
        pur_proton=nproton_proton/(nproton_proton+nproton_kaon+nproton_pion);
//
// Debug along the data
//   
//   f<<i-6000<<" "<<100*eff_pion<<" "<<100*pur_pion<<" "<<100*eff_kaon<<" "<<100*pur_kaon<<" "<<100*eff_proton<<" "<<100*pur_proton<<endl;
   

//        cout<<"N.eventi = "<<i<<"ntrue proton ="<<ntrue_proton<<endl;
//cout<<"Pion efficiency ="<<100*eff_pion<<" Purity = "<<100*pur_pion<<endl;
//cout<<"Kaon efficiency ="<<100*eff_kaon<<" Purity = "<<100*pur_kaon<<endl;
//cout<<"Proton efficiency ="<<100*eff_proton<<" Purity = "<<100*pur_proton<<endl;
//cout<<"*************************"<<endl;
    
   	}
//   cout<<"ntrue pion ="<<ntrue_pion<<" ntrue kaon = "<<ntrue_kaon<<" ntrue proton ="<<ntrue_proton<<endl;

//
// Plot results
//
	TCanvas *c1 = new TCanvas("c1", "c1");
	hdiffcode->Draw();
    TCanvas *PlotAll = new TCanvas("PlotAll", "PlotAll");
    hpion->SetLineColor(kRed); 
	hpion->Draw();
    hkaon->SetLineColor(kGreen+1); 
	hkaon->Draw("same");
    hproton->SetLineColor(kBlue+1); 
	hproton->Draw("same");

	TCanvas *c2 = new TCanvas("c2", "c2");
	network->Draw();

	//TCanvas *c3 = new TCanvas("c3", "c3");
	//ana.DrawDInputs();



}





