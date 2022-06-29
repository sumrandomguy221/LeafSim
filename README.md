# LeafSim

This repo is part of a research project I worked on at Texas A&M.

The idea is to simulate electron beam therapy using Geant 4.

Specifically, we have a 10 MeV electron beam hitting two types of targets, a CsI (Cesium Iodide) and water target. Theses targets are seperated into voxels and act as detectors to record energy desposition. Part of the electron beam is blocked by tungsten plates called 'Leaves'.

The goal is to measure dosage in these voxels based off of the position of these tungsten plates. 

PrimaryGeneratorAction is where we implement the electron beam. 

DetectorConstrunction is where we build the world box that holds the physics, detectors, and tungsten plates.

Script_Leaf_Analysis is where we analyze the data, utilizing ROOT, after the simulation is done. This produces the histograms in 'Images' Folder.
