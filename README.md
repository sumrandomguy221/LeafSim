# LeafSim

This repo is part of a research project I worked on at Texas A&M.

The idea is to simulate electron beam therapy using Geant4.

Specifically, we have a 10 MeV electron beam hitting two types of targets, a CsI (Cesium Iodide) and water target. Theses targets are seperated into voxels and act as detectors to record energy desposition. Part of the electron beam is blocked by tungsten plates called 'Leaves'.

The goal is to measure dosage in these voxels based off of the position of these tungsten plates. 

PrimaryGeneratorAction is where we implement the electron beam. 

DetectorConstrunction is where we build the world box that holds the physics, detectors, and tungsten plates.

Script_Leaf_Analysis is where we analyze the data, utilizing ROOT, after the simulation is done. This produces the histograms TotalWaterEnergy and WaterHitDet in 'Images' Folder.

You can view screenshots of the simulation setup in the 'Images' folder as well.
  - Red lines with yellow dots -> electron beam
  - Blue rectangular boxes -> tungsten plates (leaves)
  - Yellow voxeled sqaure -> CsI detectors
  - White voxeled square in Sim_picture -> Water detector
  - green lines shown in screenshots -> Neutral particles (in this case gamma particles)

Yellow dots in the electron beam show step points used by Geant4.


![Screenshot from 2017-11-13 11-08-27](https://user-images.githubusercontent.com/108433853/176556099-86129efd-e07c-4eef-b956-5e8f45bf43a1.png)
