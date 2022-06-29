#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  fParticleGun(0), fMessenger(0), 
  fPositron(0),
  fGamma(0), fElectron(0)

{
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    
    fGamma = particleTable->FindParticle("gamma");
    fElectron = particleTable->FindParticle("e-");
    fPositron = particleTable->FindParticle(particleName="e+");    
    
    // default particle kinematics
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-7.*m));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticleDefinition(fElectron);
    fParticleGun->SetParticleEnergy(10*MeV);

    // define commands for this class
    DefineCommands();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
    delete fMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    G4ParticleDefinition* particle;
    
    particle = fParticleGun->GetParticleDefinition();
    
    fParticleGun->GeneratePrimaryVertex(event);
}

void PrimaryGeneratorAction::DefineCommands()
{
    // Define //generator command directory using generic messenger class
    fMessenger 
      = new G4GenericMessenger(this, 
                               "/leaf/generator/", 
                               "Primary generator control");
}

