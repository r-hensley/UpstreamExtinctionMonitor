# UpstreamExtinctionMonitor

### Plots
- (Histogram) **Particle Rate (n/mm^2/proton) vs Radius (r \[mm\])**:
  - All Charged Tracks
  - Tracks that Point to Foil
- (Graph) **Particle Mean Energy \[MeV\] vs Radius (r \[mm\])** - Tracks that Point to Foil
  - Dominant particles: Proton, Pion, Electron
  - "Rare" particles: Kaon, Muon

## Simulation of scattering in Ion Chamber using G4beamline

### Part 1 (g4bl - "g4bl/"): Simulation in g4bl
#### Source code
    vi g4bl/ICsimulation.g4bl
- **Beam:** gaussian, nEvents=1e07, particle = proton (M = 938.272 MeV, KE = 8000.0 MeV)
  > 6.2 beam --- This command places itself into the geometry
- **Beam pipe:** innerRadius = 38.1mm, radius=39.6mm
  > 6.92 tube / 6.93 tubs --- via place

- **Ion Chamber (IC):**
  #### Prototype:  Ti  =1=  Al  ==2==  Al  ====4====  Al  ====4====  Al  ==2==  Al  =1= Ti
  #### - material of spacers: Ar / CO2 (80 / 20%)
       material ArgCO2 Ar,0.80 CARBON_DIOXIDE,0.20 density=1.66*0.001
  #### - boxes to place:
  Ti Window -> box: 70mm * 70mm * 0.0015"\
  Foil (Al) -> box: 70mm * 70mm * 0.004"\
  Gas (spacers) -> box: 70mm * 70mm * (Nspacers*0.0625")
  ####
      box VacPlateF height=70.0  width=70.0  length=$LEN_VacPlate                color=0,1,1      material=Ti
      box ArCO2a    height=70.0  width=70.0  length=1*0.0625*25.4-$LEN_VacPlate  color=invisible  material=ArgCO2
      box GndPlateF height=70.0  width=70.0  length=$LEN_GndPlate                color=0,0,1      material=Al

- **Virtual Detector:**
  - innerRadius = 40mm (outer radius of beam pipe = 39.6mm), radius = 500mm
  - distance from IC: 1 m
  > 6.96 virtualdetector Construct a VirtualDetector that generates an NTuple --- via place
  
#### Run with g4blgui 
1. Root-output mode: "Run" without "Visualization" 
  (event rate: 13450 evt/s)\
  Output -- "g4bl/g4beamline.root"
<p align="center">
  <img width="500" alt="g4beamline" src="https://github.com/JingluWang/UpstreamExtinctionMonitor/assets/107279970/b7ce30c2-a917-4578-8044-1280f909e102">
</p>

2. Visualization mode
<p align="center">
  <img width="500" alt="ICsimulation" src="https://github.com/JingluWang/G4beamline/assets/107279970/2e058818-4c25-451b-bb87-bcdc267e2241">
</p>


### Part 2 (ROOT macro - "root/"): Loop to find two distributions of detected/traced scattering particles 
#### Get plots
    root -l root/VDtoIC.C
- Charged particles (PDGid): electron (11), muon (13), pion (211), kaon (321), proton (2212)
- **(Histogram)** Distributions of scattering particle rates
  1. All detected charged particles: "scatter"
     - Using $x$, $y$ for radius range division (binning) | $z$ fixed at VD\
       $$r = \sqrt{x^{2} + y^{2}}$$
  2. Only charged particles traced back to IC: "scatterT"
     - Using $p_x$, $p_y$, $p_z$ to trace back a detected particle
     - Location at the plane of IC:\
       $$x_1 = x - p_x\times\frac{1000}{p_z} $$
       $$y_1 = y - p_y\times\frac{1000}{p_z} $$
     - Check if inside IC: (IC/foil geometry: 70mm(H) * 70mm(W) * (0.004 * 0.0625 * 25.4mm)(L))
       $$|x_1| < 35 \quad\text{and}\quad |y_1| < 35$$
     - Check if inside beam pipe: (innerRadius = 38.1mm)
       $$r_1 = \sqrt{x_{1}^{2} + y_{1}^{2}} < 38.1$$
- **(Graph)** Distribution of energies
  - Using $p_x$, $p_y$, $p_z$ and $M$ to calculate the particle energy (M_proton = 938.272 MeV, M_kaon = 493.677 MeV, M_pion = 139.6 MeV, M_muon = 105.7 MeV, M_electron = 0.511 MeV)
    $$E = \sqrt{{p_x}^{2} + {p_y}^{2} + {p_z}^{2} + M^{2}}$$
  - The mean particle energy $\bar{E_i}$ at a given radius $r_i$
    $$\bar{E_i} = \frac{1}{\text{num of}j}\sum_{\[r_i-5, r_i+5\]} E_{j}$$

#### Output file
    root --web=off -l root/rootfiles/ScatterDistribution.root
<p align="center">
  <img width="500" alt="ScatterDistribution" src="https://github.com/JingluWang/UpstreamExtinctionMonitor/assets/107279970/d00ca585-f4cf-4455-8037-5afe937d55fd">
</p>

### Part 3 (ROOT macro - "root/"): Draw plots
#### Histograms - Rate vs Radius (Comparison between all charged and tracks that point to foil)
    root -l root/drawHist.C
- Output figure -- "root/figures/Nscatter_compare_1e07.png"\
  (VD: innerRadius=40 radius=500 | nEvents=1e07)
![Nscatter_compare_1e07](https://github.com/Mu2e/UpstreamExtinctionMonitor/assets/107279970/931dbc9a-bb3c-42d3-961c-ec1ab8e7afa0)

#### Graph - Mean Energy vs Radius (for different particles)
    root -l root/drawGraph.C
- Output figure -- "root/figures/MeanEnergy_vs_R_1e07_all.png"
  - Dominant particles (Smooth): Proton, Pion, Electron
  - "Rare" particles (Jagged): Kaon, Muon
![MeanEnergy_vs_R_1e07_all_inPipe](https://github.com/JingluWang/UpstreamExtinctionMonitor/assets/107279970/48409ea2-3f33-4e6e-b552-ec0f771c41ec)

