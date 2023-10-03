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
float NEnergy[50]         = {0};
float NEnergyProton[50]   = {0};
float NEnergyKaon[50]     = {0};
float NEnergyPion[50]     = {0};
float NEnergyMuon[50]     = {0};
float NEnergyElectron[50] = {0};
// to calculate the Mean = Energy / NEnergy
float MeanEnergy[50]         = {0};
float MeanEnergyProton[50]   = {0};
float MeanEnergyKaon[50]     = {0};
float MeanEnergyPion[50]     = {0};
float MeanEnergyMuon[50]     = {0};
float MeanEnergyElectron[50] = {0};

// Cherenkov detector
float Quartz_n    = 1.47;
float AreaCrystal = 25. * 25.;
float AreaEff     = AreaCrystal / 4.;

// incident protons
float IncidentProton = 1e10;
float Extinction     = 1e-5;
float OOT            = IncidentProton * Extinction;
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
    switch ( int(std::abs( id )) ){
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

//////////////////////////////////////////////////////////
////////// 4. Passing Cherenkov Threshold Check //////////
bool CherenkovCheck(float id, float px, float py, float pz){
    // calculate momentum
    float P = std::sqrt( px*px + py*py + pz*pz );

    // determine mass
    float M = 0;
    switch ( int(std::abs( id )) ){
        case 11:   M = M_electron;
        case 13:   M = M_muon;
        case 211:  M = M_pion;
        case 321:  M = M_kaon;
        case 2212: M = M_proton;
    }

    // Cherenkov threshold
    float thre = M / std::sqrt( Quartz_n*Quartz_n - 1 );
    if ( P > thre ) return true;

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

    // Histograms of PMT coincidence
    TH1D *h_1PMT = new TH1D( "1PMT", "1PMT", NBIN, 0, END );
    TH1D *h_3PMT = new TH1D( "3PMT", "3PMT", NBIN, 0, END );

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

        // <Histogram - 1> Normalized Rate - n/mm^2/proton
        // 1. All particles detected
        h_scatter->Fill(radius);

        // Trace back to IC + pipe
        if ( TraceCheck( X, Y, P_x, P_y, P_z ) ){
            // <Histogram - 1> Normalized Rate - n/mm^2/proton
            // 2. Only particles traced back to IC
            h_scatterT->Fill(radius);  
        
            // <Graph> summing up energies
            SumEnergy( ID, radius, P_x, P_y, P_z );

            // <Histogram - 2> Rate in the first PMT / 1 Coincidence
            if ( CherenkovCheck( ID, P_x, P_y, P_z ) ) h_1PMT->Fill(radius);

            // <Histogram - 2> Rate passing all three PMTs / 3-fold Coincidence
            if ( CherenkovCheck( ID, P_x, P_y, P_z ) ) h_3PMT->Fill(radius);
        } 
    }
    // -------------------------------- Event looping - end -------------------------------

    // -------------------- <Histogram - Rate vs Radius> reweighting to n / (area * Nprotons) - begin ------------------
    for (int i = 1; i <= NBIN; i++){
        // Get i_th number of scattering particles 
        float n = h_scatter->GetBinContent(i);
        float nT = h_scatterT->GetBinContent(i);
        float n_1PMT = h_1PMT->GetBinContent(i);
        float n_3PMT = h_3PMT->GetBinContent(i);

        // Divided by i_th Annular area 
        float area = TMath::Pi() * ( std::pow( START + 10*i, 2 ) - std::pow( START - 10 + 10*i, 2 ) );
        n /= ( area * Nprotons );
        nT /= ( area * Nprotons );
        n_1PMT /= ( area * Nprotons / (AreaCrystal * OOT) );
        n_3PMT /= ( area * Nprotons / (AreaEff     * OOT) );

        // Reweighting
        h_scatter->SetBinContent(i, n);
        h_scatterT->SetBinContent(i, nT);
        h_1PMT->SetBinContent(i, n_1PMT);
        h_3PMT->SetBinContent(i, n_3PMT);
    }
    // -------------------- <Histogram - Rate vs Radius> reweighting to n / (area * Nprotons) - end --------------------

    // -------------------- <Graph> drawing - begin -------------------
    // <Graph - 1> Mean Energy vs Radius
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

    // <Graph - 2> Nparticles vs Radius
    TGraph *g1N = new TGraph( NBIN_graph, Radius, NEnergyProton );
    TGraph *g2N = new TGraph( NBIN_graph, Radius, NEnergyKaon );
    TGraph *g3N = new TGraph( NBIN_graph, Radius, NEnergyPion );
    TGraph *g4N = new TGraph( NBIN_graph, Radius, NEnergyMuon );
    TGraph *g5N = new TGraph( NBIN_graph, Radius, NEnergyElectron );

    g1N->SetName("nProton");
    g2N->SetName("nKaon");
    g3N->SetName("nPion");
    g4N->SetName("nMuon");
    g5N->SetName("nElectron");
    // -------------------- <Graph> drawing - end ---------------------

    // -- output rootfile --
    TFile *outputFile = new TFile( outfileName, "RECREATE" );
    h_scatter->Write();
    h_scatterT->Write();

    h_1PMT->Write();
    h_3PMT->Write();

    g->Write();
    g1->Write();
    g2->Write();
    g3->Write();
    g4->Write();
    g5->Write();

    g1N->Write();
    g2N->Write();
    g3N->Write();
    g4N->Write();
    g5N->Write();
}
