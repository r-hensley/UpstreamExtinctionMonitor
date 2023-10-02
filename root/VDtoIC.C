#include "lib.h"

// ------------------------------- Modification Zone - begin --------------------------------
// beam 
int Nprotons = 1e07;

// file names
TString infileName  = "../g4bl/g4beamline.root";
TString outfileName = "rootfiles/ScatterDistribution.root";

// binning info for VD division
int START  = 40;
int END    = 500;
// ------------------------------- Modification Zone - end ----------------------------------

// ------------ Variables - begin -------------
// particles
float M_proton   = 938.272;// MeV 
float M_kaon     = 493.677;// MeV
float M_pion     = 139.6;// MeV
float M_muon     = 105.7;// MeV 
float M_electron = 0.511;// MeV

// for histogram: binning info for VD division
int Nstart = START / 10;// for the empty bins
int NBIN   = Nstart + ( END - START ) / 10;// total bins to draw

// for graph: E vs R
// NBIN
int   NBIN_graph     = NBIN - Nstart;// to save the mean energy
// to store sum of energies
float Energy[50]         = {0};// total
float EnergyProton[50]   = {0};// proton
float EnergyKaon[50]     = {0};// kaon
float EnergyPion[50]     = {0};// pion
float EnergyMuon[50]     = {0};// muon
float EnergyElectron[50] = {0};// electron
// to store the number of particles in each bin
int   NEnergy[50]         = {0};
int   NEnergyProton[50]   = {0};
int   NEnergyKaon[50]     = {0};
int   NEnergyPion[50]     = {0};
int   NEnergyMuon[50]     = {0};
int   NEnergyElectron[50] = {0};
// to calculate the Mean = Energy / NEnergy
float MeanEnergy[50]         = {0};
float MeanEnergyProton[50]   = {0};
float MeanEnergyKaon[50]     = {0};
float MeanEnergyPion[50]     = {0};
float MeanEnergyMuon[50]     = {0};
float MeanEnergyElectron[50] = {0};
// ------------ Variables - end ---------------


// ------------------ Functions ------------------
//////////////////////////////////////////////////
////////// 1. function for charge check //////////
bool ChargeCheck(float id){
    if ( std::abs(id) == 11   ) return true;// electron
    if ( std::abs(id) == 13   ) return true;// muon
    if ( std::abs(id) == 211  ) return true;// pion
    if ( std::abs(id) == 321  ) return true;// kaon
    if ( std::abs(id) == 2212 ) return true;// proton

    return false;
}

/////////////////////////////////////////////
////////// 2. function for tracing //////////
bool TraceCheck(float x, float y, float px, float py, float pz){
    // locations on the plane of IC
    float x1 = x - px*1000/pz;
    float y1 = y - py*1000/pz;

    float r1 = std::sqrt(x1*x1 + y1*y1);

    // IC (foil) geometry: 70mm(H) * 70mm(W) * 0.004*0.0625*25.4(mm)(L) 
    if ( std::abs( x1 ) < 35 && std::abs( y1 ) < 35 && r1 < 38.1 ) return true;

    return false;
}

//////////////////////////////////////////////////////
////////// 3. function for summing energies //////////
void SumEnergy(float id, float r, float px, float py, float pz){
    // index to fill
    int n = floor( r / 10 ) - Nstart;

    // determine mass and fill seperate energy
    float M = 0;
    switch ( int(std::abs(id)) ){
        case 11:   M = M_electron;
                   EnergyElectron[n]  += std::sqrt( px*px + py*py + pz*pz + M*M );
                   NEnergyElectron[n] += 1; 
                   break;
        case 13:   M = M_muon;
                   EnergyMuon[n]  += std::sqrt( px*px + py*py + pz*pz + M*M );
                   NEnergyMuon[n] += 1;
                   break;
        case 211:  M = M_pion;
                   EnergyPion[n]  += std::sqrt( px*px + py*py + pz*pz + M*M );
                   NEnergyPion[n] += 1;
                   break;
        case 321:  M = M_kaon;
                   EnergyKaon[n]  += std::sqrt( px*px + py*py + pz*pz + M*M );
                   NEnergyKaon[n] += 1;
                   break;
        case 2212: M = M_proton;
                   EnergyProton[n]  += std::sqrt( px*px + py*py + pz*pz + M*M );
                   NEnergyProton[n] += 1;
                   break;
    }

    // filling total particle energy
    Energy[n]  += std::sqrt( px*px + py*py + pz*pz + M*M );
    NEnergy[n] += 1;
}

///////////////////////////////////
////////// main function //////////
void VDtoIC(){
    // -- input rootfile --
    // Get the Virtual Detector tree which contains all the info of the detected particles
    TFile *f = new TFile( infileName );
    TDirectory *folder = (TDirectory*) f->Get( "VirtualDetector" );
    TTree *VDtree  = (TTree*) folder->Get( "VD" );

    // -------------------------------- Variables reading - begin ----------------------------
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

    // Using PDGid to check charge & determine mass (for energy calculation)
    float ID = 0;
    VDtree->SetBranchAddress( "PDGid", &ID );
    // -------------------------------- Variables reading - end ------------------------------

    // -------------------------------- Plotting defining - begin -----------------------------
    // Histograms of scattering-distribution (T: traced back to IC)
    TH1D *h_scatter = new TH1D( "scatter", "scatter", NBIN, 0, END );
    TH1D *h_scatterT = new TH1D( "scatterT", "scatterT", NBIN, 0, END );

    // Graph - E vs R
    // defining R
    float Radius[50] = {0};
    for (int i = 0; i < NBIN_graph; i++){
        Radius[i] = float(START + 5) + 10 * i;
    }
    // -------------------------------- Plotting defining - end -----------------------------

    // -------------------------------- Event looping - begin -----------------------------
    int Nevents = VDtree->GetEntries();
    for (int i = 0; i < Nevents; i++){
        // start looping
        VDtree->GetEntry(i);

        // Check charged particles
        if ( !ChargeCheck( ID ) ) continue;

        // get radius of detected particles
        float radius = std::sqrt( X*X + Y*Y );

        // <Histogram> filling - n/mm^2/proton
        // 1. All particles detected
        h_scatter->Fill(radius);
        // 2. Only particles traced back to IC
        if ( TraceCheck( X, Y, P_x, P_y, P_z ) ){
            h_scatterT->Fill(radius);  
        
            // <Graph> summing up energies
            SumEnergy( ID, radius, P_x, P_y, P_z );
        } 
    }
    // -------------------------------- Event looping - end -------------------------------

    // -------------------- <Histogram> reweighting to n / (area * Nprotons) - begin ------------------
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
    // -------------------- <Histogram> reweighting to n / (area * Nprotons) - end --------------------

    // -------------------- <Graph> drawing - begin -------------------
    for (int i = 0; i < NBIN_graph; i++){
        MeanEnergy[i]         = NEnergy[i]         != 0 ? Energy[i]         / NEnergy[i]         : 0; 
        MeanEnergyProton[i]   = NEnergyProton[i]   != 0 ? EnergyProton[i]   / NEnergyProton[i]   : 0;
        MeanEnergyKaon[i]     = NEnergyKaon[i]     != 0 ? EnergyKaon[i]     / NEnergyKaon[i]     : 0;
        MeanEnergyPion[i]     = NEnergyPion[i]     != 0 ? EnergyPion[i]     / NEnergyPion[i]     : 0;
        MeanEnergyMuon[i]     = NEnergyMuon[i]     != 0 ? EnergyMuon[i]     / NEnergyMuon[i]     : 0;
        MeanEnergyElectron[i] = NEnergyElectron[i] != 0 ? EnergyElectron[i] / NEnergyElectron[i] : 0;
    }

    TGraph *g  = new TGraph( NBIN_graph, Radius, MeanEnergy );
    TGraph *g1 = new TGraph( NBIN_graph, Radius, MeanEnergyProton );
    TGraph *g2 = new TGraph( NBIN_graph, Radius, MeanEnergyKaon );
    TGraph *g3 = new TGraph( NBIN_graph, Radius, MeanEnergyPion );
    TGraph *g4 = new TGraph( NBIN_graph, Radius, MeanEnergyMuon );
    TGraph *g5 = new TGraph( NBIN_graph, Radius, MeanEnergyElectron );

    g->SetName("energy");
    g1->SetName("energyProton");
    g2->SetName("energyKaon");
    g3->SetName("energyPion");
    g4->SetName("energyMuon");
    g5->SetName("energyElectron");
    // -------------------- <Graph> drawing - end ---------------------

    // -- output rootfile --
    TFile *outputFile = new TFile( outfileName, "RECREATE" );
    h_scatter->Write();
    h_scatterT->Write();
    g->Write();
    g1->Write();
    g2->Write();
    g3->Write();
    g4->Write();
    g5->Write();
}
