#include "lib.h"

// ------------------------------- Modification Zone - begin --------------------------------
// 1. input rootfile
TString Nevents = "1e07";
TString input = "rootfiles/ScatterDistribution_charged.root";

// 2. hists to compare
TString hist1 = "scatter";
TString hist2 = "scatterT";

// 3. Axis
TString XAXIS = "r [mm]";
TString YAXIS = "n/mm^{2}/proton";

// 4. hists' legends
TString LEGEND1 = "All Charged Tracks";
TString LEGEND2 = "Tracks that Point to Foil";

// 5. text for explanation
TString TEXT = "";

// 6. output label
TString LABEL = "figures/Nscatter_compare_" + Nevents + "_charged.png";
// ------------------------------- Modification Zone - end ----------------------------------


void drawHist(){
    TFile *f = new TFile( input );// 1. input rootfile
    TH1D *HIST = (TH1D*) f->Get( hist1 );// 2. hist1 imported
    HIST->SetDirectory(0);
    HIST->SetLineColor(kBlue);
    HIST->SetStats(0);
    HIST->SetTitle(0);
    HIST->SetLineWidth(2);
    HIST->GetXaxis()->SetTitle( XAXIS );// 3. X-axis label
    HIST->GetXaxis()->SetLabelSize(0.03);
    HIST->GetXaxis()->SetLabelFont(62);
    HIST->GetXaxis()->SetTitleSize(30);
    HIST->GetXaxis()->SetTitleFont(63);
    HIST->GetYaxis()->SetTitle( YAXIS );// 3. Y-axis label
    HIST->GetYaxis()->SetLabelFont(62);
    HIST->GetYaxis()->SetLabelSize(0.03);
    HIST->GetYaxis()->SetTitleSize(30);
    HIST->GetYaxis()->SetTitleFont(63);

    TH1D *HIST2 = (TH1D*) f->Get( hist2 );// 2. hist2 imported
    HIST2->SetDirectory(0);
    HIST2->SetLineColor(kRed);
    HIST2->SetStats(0);
    HIST2->SetTitle(0);
    HIST2->SetLineWidth(2);

    double mmax, t, MAX;
    mmax = HIST->GetBinContent(1);
    for(int i = 2; i <= 100; i++)
    {
        t = HIST->GetBinContent(i);
        if(mmax < t) {mmax = t;}
    }
    MAX = 1.1*mmax;
    HIST->SetMaximum(MAX);

    TCanvas *c = new TCanvas("c", "", 900, 800);
    c->SetMargin(0.17, 0.12, 0.23, 0.12);
    c->cd();
    HIST->Draw("HIST");
    HIST2->Draw("HISTsame");

    TLegend *legend = new TLegend(0.5, 0.7, 0.8, 0.85);
    legend->AddEntry(HIST, LEGEND1);// 4. hist1 legend
    legend->AddEntry(HIST2, LEGEND2);// 4. hist2 legend
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
