# UpstreamExtinctionMonitor

## Simulation of scattering in Ion Chamber using G4beamline

### Part 1 (g4bl - "src/"): Simulation in g4bl
#### Source code
    vi g4bl/ICsimulation.g4bl
- Beam: gaussian, nEvents=1e07, particle = proton (M = 938.272 MeV, KE = 8000.0 MeV)
  > 6.2 beam --- This command places itself into the geometry
- Beam pipe: innerRadius = 38.1mm, radius=39.6mm
  > 6.92 tube / 6.93 tubs --- via place
- Virtual Detector: innerRadius = 40mm (outer radius of beam pipe = 39.6mm), radius = 500mm
  > 6.96 virtualdetector Construct a VirtualDetector that generates an NTuple --- via place
- Ion Chamber:
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
  
#### Run with g4blgui 
1. Root-output mode: "Run" without "Visualization" 
  (event rate: 13450 evt/s)\
  Output -- "src/g4beamline.root"
<p align="center">
  <img width="500" alt="g4beamline" src="https://github.com/JingluWang/UpstreamExtinctionMonitor/assets/107279970/b7ce30c2-a917-4578-8044-1280f909e102">
</p>

3. Visualization mode
<p align="center">
  <img width="500" alt="ICsimulation" src="https://github.com/JingluWang/G4beamline/assets/107279970/2e058818-4c25-451b-bb87-bcdc267e2241">
</p>


### Part 2 (ROOT macro - "root/"): Loop to find two distributions of detected/traced scattering particles 
#### Get histograms
    root -l root/VDtoIC.C
- Charged particles (PDGid): electron (11), muon (13), pion (211), kaon (321)
- Two distributions:
  1. All detected charged particles: "scatter"
  2. Only charged particles traced back to IC: "scatterT"
- Output -- "root/rootfiles/ScatterDistribution.root"


### Part 3 (ROOT macro - "root/"): Draw plots comparing two histograms
#### Draw plots
    root -l root/drawHist.C
- Output figure -- "root/figures/Nscatter_compare_1e07.png"\
  (VD: innerRadius=40 radius=500 | nEvents=1e07)
![Nscatter_compare_1e07](https://github.com/JingluWang/UpstreamExtinctionMonitor/assets/107279970/46afce28-cbd9-422c-b2f0-9d89b17d490f)
