#ifndef PizeroMCTruth_h
#define PizeroMCTruth_h

#include "SimDataFormats/Track/interface/SimTrack.h"
#include <CLHEP/Matrix/Vector.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <vector>

class PizeroMCTruth {

  public:

    PizeroMCTruth();
    void SetDecay(float r, float z, CLHEP::HepLorentzVector momentum1, CLHEP::HepLorentzVector momentum2);
    void SetDalitzDecay(float r, float z, CLHEP::HepLorentzVector momentum1, CLHEP::HepLorentzVector momentum2, CLHEP::HepLorentzVector momentum3);

    bool IsDalitz() { return dalitz_; };
    float GetR() { return r_; };
    float GetZ() { return z_; };
    CLHEP::HepLorentzVector GetMomentum1() { return momentum1_; };
    CLHEP::HepLorentzVector GetMomentum2() { return momentum2_; };
    CLHEP::HepLorentzVector GetMomentum3() { return momentum3_; };

  private:
    bool dalitz_;
    float r_;
    float z_;
    CLHEP::HepLorentzVector momentum1_;
    CLHEP::HepLorentzVector momentum2_;
    CLHEP::HepLorentzVector momentum3_;
};

#endif
  
