g4bl notes

Run complete  10000000 Events  567 seconds
NTuple VD                108007 entries
NTuple wrote Root File 'g4beamline.root'

root -l g4beamline.root 

TFile *f = new TFile("g4beamline.root");

root [2] f->ls();
TFile**		g4beamline.root	
 TFile*		g4beamline.root	
  KEY: TDirectoryFile	VirtualDetector;1	VirtualDetector

root [6] // Step 1: Get the directory
root [7] TDirectory *vdDir = (TDirectory*)f->Get("VirtualDetector");

root [14] vdDir
(TDirectory *) 0x570bc5cea210

root [15] TTree *t = (TTree*)vdDir->Get("VD")
(TTree *) 0x570bc5c71f80

root [16] t->Print();
******************************************************************************
*Tree    :VD        : VirtualDetector/VD                                     *
*Entries :   108007 : Total =        12140560 bytes  File  Size =    5080835 *
*        :          : Tree compression factor =   2.39                       *
******************************************************************************
*Br    0 :x         : Float_t                                                *
*Entries :   108007 : Total  Size=     433519 bytes  File Size  =     400414 *
*Baskets :       14 : Basket Size=      32000 bytes  Compression=   1.08     *
[ ... ]



