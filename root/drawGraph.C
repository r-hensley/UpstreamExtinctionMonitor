#include "lib.h"

// ------------------------------- Modification Zone - begin --------------------------------
// 1. input rootfile
TString Nevents = "1e07";
TString input   = "rootfiles/ScatterDistribution.root";

// 2. graph to draw
float MAX = 7000;
float MIN = 0;

TString graph  = "energy";
TString graph1 = "energyProton";
TString graph2 = "energyKaon";
TString graph3 = "energyPion";
TString graph4 = "energyMuon";
TString graph5 = "energyElectron";

// 3. Axis
TString XAXIS = "r [mm]";
TString YAXIS = "Mean Energy [MeV]";

// 4. legend
TString LEGEND1 = "Proton";
TString LEGEND2 = "Kaon";
TString LEGEND3 = "Pion";
TString LEGEND4 = "Muon";
TString LEGEND5 = "Electron";

// 5. text for explanation
TString TEXT    = "";
TString TEXTALL = "Tracks that Point to Foil";

// 6. output label
TString LABEL    = "figures/MeanEnergy_vs_R_" + Nevents + ".png";
TString LABELALL = "figures/MeanEnergy_vs_R_" + Nevents + "_all.png";
// ------------------------------- Modification Zone - end ----------------------------------


void GraphOne(){
    TFile *f = new TFile( input );// 1. input rootfile
    TGraph *g = (TGraph*) f->Get( graph );// 2. graph imported
    g->PaintStats(0);
    g->SetTitle(0);
    g->SetLineStyle(1);
    g->SetLineColor( kBlue );
    g->SetLineWidth(3);
    g->GetXaxis()->SetTitle( XAXIS );// 3. Axis
    g->GetXaxis()->SetLabelSize(0.03);
    g->GetXaxis()->SetLabelFont(62);
    g->GetXaxis()->SetTitleSize(30);
    g->GetXaxis()->SetTitleFont(63);
    g->GetYaxis()->SetTitle( YAXIS );// 3. Axis
    g->GetYaxis()->SetLabelFont(62);
    g->GetYaxis()->SetLabelSize(0.03);
    g->GetYaxis()->SetTitleSize(30);
    g->GetYaxis()->SetTitleFont(63);

    g->SetMaximum(MAX);

    TCanvas *c = new TCanvas("c", "", 900, 800);
    c->SetMargin(0.17, 0.12, 0.23, 0.12);
    c->cd();
    g->Draw("");

    TLegend *legend = new TLegend(0.5, 0.7, 0.8, 0.85);
    legend->AddEntry(g, LEGEND1);// 4. legend
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->SetLineColor(0);
    legend->SetLineWidth(0);
    legend->SetTextSize(0.03);
    legend->SetTextFont(62);
    legend->Draw("same");

    TLatex *text1 = new TLatex(0.2, 0.8, TEXT);// 5. text for explanation
    text1->SetNDC();
    text1->SetTextSize(0.04);
    text1->Draw("same");

    c->Update();
    TGaxis *axis1 = new TGaxis(c->GetUxmax(), c->GetUymin(), c->GetUxmax(), c->GetUymax(),0,MAX,510,"+L");
    axis1->SetLineColor(kBlack);
    axis1->SetLabelSize(0);
    axis1->Draw();

    c->Update();
    TGaxis *axis2 = new TGaxis(c->GetUxmin(), c->GetUymax(), c->GetUxmax(), c->GetUymax(),0,MAX,510,"-");
    axis2->SetLineColor(kBlack);
    axis2->SetLabelSize(0);
    axis2->Draw();

    c->SaveAs( LABEL );// 6. output label
}

void GraphAll(){
    TFile *f = new TFile( input );// 1. input rootfile

    TGraph *g1 = (TGraph*) f->Get( graph1 );// 2. graph imported
    g1->PaintStats(0);
    g1->SetTitle(0);
    g1->SetLineStyle(1);
    g1->SetLineColor( kRed );
    g1->SetLineWidth(3);
    g1->GetXaxis()->SetTitle( XAXIS );// 3. Axis
    g1->GetXaxis()->SetLabelSize(0.03);
    g1->GetXaxis()->SetLabelFont(62);
    g1->GetXaxis()->SetTitleSize(30);
    g1->GetXaxis()->SetTitleFont(63);
    g1->GetYaxis()->SetTitle( YAXIS );// 3. Axis
    g1->GetYaxis()->SetLabelFont(62);
    g1->GetYaxis()->SetLabelSize(0.03);
    g1->GetYaxis()->SetTitleSize(30);
    g1->GetYaxis()->SetTitleFont(63);
 
    g1->SetMaximum(MAX);
    g1->SetMinimum(MIN);

    TGraph *g2 = (TGraph*) f->Get( graph2 );// 2. graph imported
    g2->PaintStats(0);
    g2->SetTitle(0);
    g2->SetLineStyle(1);
    g2->SetLineColor( kBlue );  
    g2->SetLineWidth(3);

    TGraph *g3 = (TGraph*) f->Get( graph3 );// 2. graph imported
    g3->PaintStats(0);
    g3->SetTitle(0);
    g3->SetLineStyle(1);
    g3->SetLineColor( kGreen );
    g3->SetLineWidth(3);

    TGraph *g4 = (TGraph*) f->Get( graph4 );// 2. graph imported
    g4->PaintStats(0);
    g4->SetTitle(0);
    g4->SetLineStyle(1);
    g4->SetLineColor( kViolet );
    g4->SetLineWidth(3);

    TGraph *g5 = (TGraph*) f->Get( graph5 );// 2. graph imported
    g5->PaintStats(0);
    g5->SetTitle(0);
    g5->SetLineStyle(1);
    g5->SetLineColor( kOrange );
    g5->SetLineWidth(3);

    TCanvas *c = new TCanvas("c", "", 900, 800);
    c->SetMargin(0.17, 0.12, 0.23, 0.12);
    c->cd();
    g1->Draw("");
    g2->Draw("same");
    g3->Draw("same");
    g4->Draw("same");
    g5->Draw("same");

    TLegend *legend = new TLegend(0.7, 0.7, 0.8, 0.85);
    legend->AddEntry(g1, LEGEND1);// 4. legend
    legend->AddEntry(g2, LEGEND2);
    legend->AddEntry(g3, LEGEND3);
    legend->AddEntry(g4, LEGEND4);
    legend->AddEntry(g5, LEGEND5);
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->SetLineColor(0);
    legend->SetLineWidth(0);
    legend->SetTextSize(0.03);
    legend->SetTextFont(62);
    legend->Draw("same");

    TLatex *text1 = new TLatex(0.25, 0.8, TEXTALL);// 5. text for explanation
    text1->SetNDC();
    text1->SetTextSize(0.04);
    text1->Draw("same");

    c->Update();
    TGaxis *axis1 = new TGaxis(c->GetUxmax(), c->GetUymin(), c->GetUxmax(), c->GetUymax(),0,MAX,510,"+L");
    axis1->SetLineColor(kBlack);
    axis1->SetLabelSize(0);
    axis1->Draw();

    c->Update();
    TGaxis *axis2 = new TGaxis(c->GetUxmin(), c->GetUymax(), c->GetUxmax(), c->GetUymax(),0,MAX,510,"-");
    axis2->SetLineColor(kBlack);
    axis2->SetLabelSize(0);
    axis2->Draw();

    c->SaveAs( LABELALL );// 6. output label
}

void drawGraph(){
    //GraphOne();
    GraphAll();
}
