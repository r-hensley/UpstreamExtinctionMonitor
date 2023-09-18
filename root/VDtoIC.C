#include "lib.h"

// ------------------------------- Modification Zone - begin --------------------------------
// beam 
int Nprotons = 1e07;

// file names
TString infileName = "../g4bl/g4beamline.root";
TString outfileName = "rootfiles/ScatterDistribution.root";

// binning info for VD division
int START = 20;
int END = 500;
int NBIN = ( END - START ) / 10;
// ------------------------------- Modification Zone - end ----------------------------------

// -------------- Functions --------------
//////////////////////////////////////////
////////// function for tracing //////////
bool TraceCheck(float x, float y, float px, float py, float pz){
    float x1 = x - px*1000/pz;
    float y1 = y - py*1000/pz;

    // IC (foil) geometry: 70mm(H) * 70mm(W) * 0.004*0.0625*25.4(mm) 
    if ( std::abs( x1 ) < 35 && std::abs( y1 ) < 35) return true;

    return false;
}

///////////////////////////////////
////////// main function //////////
void VDtoIC(){
    // -- input rootfile --
    // Get the Virtual Detector tree which contains all the info of the detected particles
    TFile *f = new TFile( infileName );
    TDirectory *folder = (TDirectory*) f->Get( "VirtualDetector" );
    TTree *VDtree  = (TTree*) folder->Get( "VD" );

    // -------------------------------- Variables reading & Histograms defining - begin ----------------------------
    // Using x, y for radius range division (binning) | z fixed at VD
    float X = 0;
    float Y = 0;
    VDtree->SetBranchAddress( "x", &X );
    VDtree->SetBranchAddress( "y", &Y );

    // Using Px, Py, Pz to trace back a detected particle 
    // within certain radius/bin
    float P_x = 0;
    float P_y = 0;
    float P_z = 0;
    VDtree->SetBranchAddress( "Px", &P_x );
    VDtree->SetBranchAddress( "Py", &P_y );
    VDtree->SetBranchAddress( "Pz", &P_z );

    // Histograms of scattering-distribution (T: traced back to IC)
    TH1D *h_scatter = new TH1D("scatter", "scatter", NBIN, START, END);
    TH1D *h_scatterT = new TH1D("scatterT", "scatterT", NBIN, START, END);
    // -------------------------------- Variables reading & Histograms defining - end ------------------------------

    // -------------------------------- Event looping - begin -----------------------------
    int Nevents = VDtree->GetEntries();
    for (int i = 0; i < Nevents; i++){
        // start looping
        VDtree->GetEntry(i);

        // get radius of detected particles
        float radius = std::sqrt( X*X + Y*Y );

        // 1. All particles detected
        h_scatter->Fill(radius);

        // 2. Only particles traced back to IC
        if ( TraceCheck( X, Y, P_x, P_y, P_z ) ) h_scatterT->Fill(radius); 
    }
    // -------------------------------- Event looping - end -------------------------------

    // -------------------- reweighting to n / (area * Nprotons) - begin ------------------
    for (int i = 1; i <= NBIN; i++){
        float n = h_scatter->GetBinContent(i);
        float nT = h_scatterT->GetBinContent(i);

        // i_th Annular area 
        float area = TMath::Pi() * ( std::pow( START + 10*i, 2 ) - std::pow( START - 10 + 10*i, 2 ) );
        n /= ( area * Nprotons );
        nT /= ( area * Nprotons );

        // reweighting
        h_scatter->SetBinContent(i, n);
        h_scatterT->SetBinContent(i, nT);
    }
    // -------------------- reweighting to n / (area * Nprotons) - end --------------------

    // -- output rootfile --
    TFile *outputFile = new TFile( outfileName, "RECREATE" );
    h_scatter->Write();
    h_scatterT->Write();
}
