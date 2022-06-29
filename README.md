# LeafSim

This repo is part of a research project I worked on at Texas A&M.

The idea is to simulate electron beam therapy using Geant4.

Specifically, we have a 10 MeV electron beam hitting two types of targets, a CsI (Cesium Iodide) and water target. Theses targets are seperated into voxels and act as detectors to record energy desposition. Part of the electron beam is blocked by tungsten plates called 'Leaves'.

The goal is to measure radiation dosage in these voxels based off of the position of these tungsten plates. 

PrimaryGeneratorAction is where we implement the electron beam and any secondary particles.

DetectorConstrunction is where we build the world box that holds the physics, detectors, tungsten plates, etc.

Script_Leaf_Analysis is where we analyze the data, utilizing ROOT, after the simulation is done. This produces the histograms TotalWaterEnergy and WaterHitDet listed below or in the 'Images' Folder.

Some of the pictures below are visual representations of the simulations created by Geant4.
  - Red lines with yellow dots -> electron beam
  - Blue/cyan rectangular boxes -> tungsten plates (leaves)
  - Yellow voxeled sqaure -> CsI detectors
  - White voxeled square in Sim_picture -> Water detector
  - green lines shown in screenshots -> Neutral particles (in this case gamma particles)

Yellow dots in the electron beam show step points used by Geant4.


![Screenshot from 2017-11-13 11-08-27](https://user-images.githubusercontent.com/108433853/176556099-86129efd-e07c-4eef-b956-5e8f45bf43a1.png)

![Sim_picture](https://user-images.githubusercontent.com/108433853/176556151-257672c3-2489-49ab-a3e0-092d52f32d5f.png)

![TotalWaterEnergy](https://user-images.githubusercontent.com/108433853/176556170-96f025b3-c9e3-442f-9bf8-5fcfc74a36e5.png)

TotalWaterEnergy: Shows the amount of energy each individual particle (from the electron beam) deposited into the water detectors (white voxeled square).

![WaterHitDet](https://user-images.githubusercontent.com/108433853/176556181-dfba8d29-f307-46a8-8147-7f289364fc4b.png)

WaterHitDet: Shows which voxel a particular particle hit. The voxels are numbered from 0 to 99. They start from the top column, then left to right, then second column, left to right, and so on. 
