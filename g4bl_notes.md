g4bl notes

```
Run complete  10000000 Events  567 seconds
NTuple VD                108007 entries
NTuple wrote Root File 'g4beamline.root'

root -l g4beamline.root 

TFile *f = new TFile("g4beamline.root");

root [2] f->ls();
TFile**		g4beamline.root	
 TFile*		g4beamline.root	
  KEY: TDirectoryFile	VirtualDetector;1	VirtualDetector

root [7] TDirectory *vdDir = (TDirectory*)f->Get("VirtualDetector");

root [14] vdDir
(TDirectory *) 0x570bc5cea210

root [15] TTree *t = (TTree*)vdDir->Get("VD")
(TTree *) 0x570bc5c71f80
// OR
TTree *t = (TTree*)f->Get("VirtualDetector/VD")


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
```

Getting variable values
```
root [101] Float_t x
(float) 0.00000f
root [102] t->SetBranchAddress("x", &x);
root [103] x
(float) 0.00000f
root [104] t->GetEntry(0)
(int) 112
root [105] x
(float) -290.944f
root [106] t->GetEntry(1)  // valid until 108006
(int) 112
root [107] x
(float) 240.731f
```




# PyROOT

To load a file and get show the directory structure.
```py
f = r.TFile("g4beamline.root")

f.cd("VirtualDetector")
f.cd("NTuple")
f.ls()
TFile**		g4beamline.root	
 TFile*		g4beamline.root	
  TDirectoryFile*		VirtualDetector	VirtualDetector
   KEY: TNtuple	VD;1	VirtualDetector/VD
  TDirectoryFile*		NTuple	NTuple
   KEY: TNtuple	NEW;1	NTuple/NEW
   KEY: TNtuple	LOSS;1	NTuple/LOSS
  KEY: TDirectoryFile	NTuple;1	NTuple
  KEY: TDirectoryFile	VirtualDetector;1	VirtualDetector
```

To show the number of entries in each detector
```py
>>> vd = f.Get("VirtualDetector/VD")
>>> new = f.Get("NTuple/NEW")
>>> loss = f.Get("NTuple/LOSS")
>>> vd.GetEntries()
5
>>> new.GetEntries()
1340
>>> loss.GetEntries()
1340
```

This shows that five particles reached the virtual detector at Z=1000, and in general 1,340 particles were created (simulation started with 1,000 protons).

Getting entry data
```py
new.GetEntry(0)
entry_data = {branch.GetName(): getattr(new, branch.GetName()) for branch in new.GetListOfBranches()}
print(entry_data)
{'x': -0.2529256343841553, 'y': -0.0026775484438985586, 'z': -1.0, 'Px': 0.0, 'Py': 0.0, 'Pz': 8888.8896484375, 't': 0.0, 'PDGid': 2212.0, 'EventID': 1.0, 'TrackID': 1.0, 'ParentID': 0.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.2529256343841553, 'InitY': -0.0026775484438985586, 'InitZ': -1.0, 'InitT': 0.0, 'InitKE': 8000.0}

>>> for i in range(new.GetEntries()):
...     new.GetEntry(i)
...     entry_data = {branch.GetName(): getattr(new, branch.GetName()) for branch in new.GetListOfBranches()}
...     print(entry_data)

{'x': -0.2529256343841553, 'y': -0.0026775484438985586, 'z': -1.0, 'Px': 0.0, 'Py': 0.0, 'Pz': 8888.8896484375, 't': 0.0, 'PDGid': 2212.0, 'EventID': 1.0, 'TrackID': 1.0, 'ParentID': 0.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.2529256343841553, 'InitY': -0.0026775484438985586, 'InitZ': -1.0, 'InitT': 0.0, 'InitKE': 8000.0}
{'x': -0.01945296674966812, 'y': 0.2503873407840729, 'z': -1.0, 'Px': 0.0, 'Py': 0.0, 'Pz': 8888.8896484375, 't': 0.0, 'PDGid': 2212.0, 'EventID': 2.0, 'TrackID': 1.0, 'ParentID': 0.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.01945296674966812, 'InitY': 0.2503873407840729, 'InitZ': -1.0, 'InitT': 0.0, 'InitKE': 8000.0}
{'x': 0.1830507069826126, 'y': -0.06554963439702988, 'z': -1.0, 'Px': 0.0, 'Py': 0.0, 'Pz': 8888.8896484375, 't': 0.0, 'PDGid': 2212.0, 'EventID': 3.0, 'TrackID': 1.0, 'ParentID': 0.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': 0.1830507069826126, 'InitY': -0.06554963439702988, 'InitZ': -1.0, 'InitT': 0.0, 'InitKE': 8000.0}
{'x': 0.3990275263786316, 'y': 0.09649430960416794, 'z': -1.0, 'Px': 0.0, 'Py': 0.0, 'Pz': 8888.8896484375, 't': 0.0, 'PDGid': 2212.0, 'EventID': 4.0, 'TrackID': 1.0, 'ParentID': 0.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': 0.3990275263786316, 'InitY': 0.09649430960416794, 'InitZ': -1.0, 'InitT': 0.0, 'InitKE': 8000.0}
{'x': -0.20500990748405457, 'y': -0.08534087985754013, 'z': -1.0, 'Px': 0.0, 'Py': 0.0, 'Pz': 8888.8896484375, 't': 0.0, 'PDGid': 2212.0, 'EventID': 5.0, 'TrackID': 1.0, 'ParentID': 0.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.20500990748405457, 'InitY': -0.08534087985754013, 'InitZ': -1.0, 'InitT': 0.0, 'InitKE': 8000.0}
{'x': -0.20374812185764313, 'y': -0.07837454229593277, 'z': 19.15735626220703, 'Px': 0.027519045397639275, 'Py': -0.02017834223806858, 'Pz': 0.0011514662764966488, 't': 0.06761126220226288, 'PDGid': 11.0, 'EventID': 5.0, 'TrackID': 1001.0, 'ParentID': 1.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.20374812185764313, 'InitY': -0.07837454229593277, 'InitZ': 19.15735626220703, 'InitT': 0.06761126220226288, 'InitKE': 0.0011394261382520199}
{'x': -0.20406924188137054, 'y': -0.08007010072469711, 'z': 14.684804916381836, 'Px': -0.013368900865316391, 'Py': 0.033406537026166916, 'Pz': 0.0012622579233720899, 't': 0.05260954797267914, 'PDGid': 11.0, 'EventID': 5.0, 'TrackID': 1002.0, 'ParentID': 1.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.20406924188137054, 'InitY': -0.08007010072469711, 'InitZ': 14.684804916381836, 'InitT': 0.05260954797267914, 'InitKE': 0.0012668449198827147}
{'x': -0.20432332158088684, 'y': -0.08159082382917404, 'z': 10.642716407775879, 'Px': 0.23494568467140198, 'Py': -0.041706494987010956, 'Pz': 0.05605607107281685, 't': 0.039051685482263565, 'PDGid': 11.0, 'EventID': 5.0, 'TrackID': 1003.0, 'ParentID': 1.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': -0.20432332158088684, 'InitY': -0.08159082382917404, 'InitZ': 10.642716407775879, 'InitT': 0.039051685482263565, 'InitKE': 0.055747129023075104}
{'x': 2.126086950302124, 'y': -0.662456214427948, 'z': 11.022119522094727, 'Px': 0.00780130922794342, 'Py': -0.009068513289093971, 'Pz': 0.032682131975889206, 't': 0.05834537371993065, 'PDGid': 11.0, 'EventID': 5.0, 'TrackID': 1004.0, 'ParentID': 1003.0, 'Weight': 1.0, 'Bx': 0.0, 'By': 0.0, 'Bz': 0.0, 'Ex': 0.0, 'Ey': 0.0, 'Ez': 0.0, 'ProperTime': 0.0, 'PathLength': 0.0, 'PolX': 0.0, 'PolY': 0.0, 'PolZ': 0.0, 'InitX': 2.126086950302124, 'InitY': -0.662456214427948, 'InitZ': 11.022119522094727, 'InitT': 0.05834537371993065, 'InitKE': 0.0011837781639769673}
[...]
```

### An example hadron shower event

```
    TrackID  ParentID       PDGid     KE_birth    z_birth   t_birth       z_loss          t_loss
0         1         0        2212  8000.000000  -1.000000  0.000000    22.325642        0.078238
1      1001         1  1000180416     6.048697  22.325642  0.078238    22.326263  1000000.000000
2      1002         1          22     5.960425  22.325642  0.078238  -270.620178        3.327777
3      1003         1          22     3.025318  22.325642  0.078238     7.806882        2.757112
4      1004         1        2212    11.753773  22.325642  0.078238    57.384563  1000000.000000
5      1005         1        2112     1.037227  22.325642  0.078238  -726.541870       78.318832
6      1006         1        2112     7.739597  22.325642  0.078238   558.678833       28.102497
7      1007         1        2112     6.424326  22.325642  0.078238   840.154968       33.982067
8      1008         1        2212    19.850798  22.325642  0.078238    48.143345       13.476734
9      1009         1        2212    40.323685  22.325642  0.078238   258.968628       10.102807
10     1010         1        2112   162.705521  22.325642  0.078238  1301.678467        9.650825
11     1011         1        2112   873.436523  22.325642  0.078238  1301.678467        5.709786
12     1012         1        -211   210.890610  22.325642  0.078238   372.040070        3.632657
13     1013         1         211  3183.074463  22.325642  0.078238  1301.678467        4.584225
14     1014         1         111   107.712585  22.325642  0.078238    22.325724        0.078239
15     1017         1        2112    64.962257  22.325642  0.078238 -1301.678467       12.918581
16     1018         1        2112    49.259792  22.325642  0.078238  -728.524292       12.174812
17     1019         1        2212    64.164330  22.325642  0.078238   237.041824        8.444948
18     1020         1         211   436.418396  22.325642  0.078238  1301.678467        4.563375
19     1021         1        -211    91.876152  22.325642  0.078238  1269.972534        6.410444
20     1022         1        2212  1856.086304  22.325642  0.078238  1301.678467        4.630719
21     1015      1014          22   171.026321  22.325724  0.078239  1301.678467        4.813799
22     1016      1014          22    71.662865  22.325724  0.078239   273.176880        2.982502
```

