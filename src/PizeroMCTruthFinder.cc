#include "RecoEgamma/EgammaMCTools/interface/PizeroMCTruthFinder.h" 

#include <algorithm> 

PizeroMCTruthFinder::PizeroMCTruthFinder() {

}

std::vector<PizeroMCTruth> PizeroMCTruthFinder::find(std::vector<SimTrack> theSimTracks, std::vector<SimVertex> theSimVertices) {

  std::vector<PizeroMCTruth> result;

  const int SINGLE=1;
  const int DOUBLE=2;
  const int PYTHIA=3;
  const int ELECTRON_FLAV=1;
  const int PIZERO_FLAV=2;
  const int PHOTON_FLAV=3;
  
  int ievtype=0;
  int ievflav=0;
  
  std::vector<SimTrack*> photonTracks;
  std::vector<SimTrack*> electronTracks;
  std::vector<SimTrack*> pizeroTracks;
  std::vector<const SimTrack *> trkFromConversion;
  SimVertex primVtx;   
  std::vector<int> convInd;
  
  fill(theSimTracks,  theSimVertices);
  
  int iPV=-1;   
  int partType1=0;
  int partType2=0;
  std::vector<SimTrack>::iterator iFirstSimTk = theSimTracks.begin();
  if (  !(*iFirstSimTk).noVertex() ) {
    iPV =  (*iFirstSimTk).vertIndex();
    
    int vtxId =   (*iFirstSimTk).vertIndex();
    primVtx = theSimVertices[vtxId];
    
    partType1 = (*iFirstSimTk).type();
    std::cout <<  " Primary vertex id " << iPV << " first track type " << (*iFirstSimTk).type() << std::endl;  
  } else {
    std::cout << " First track has no vertex " << std::endl;
  }
  
  // Look at a second track
  iFirstSimTk++;
  if ( iFirstSimTk!=  theSimTracks.end() ) {
    
    if (  (*iFirstSimTk).vertIndex() == iPV) {
      partType2 = (*iFirstSimTk).type();  
      std::cout <<  " second track type " << (*iFirstSimTk).type() << " vertex " <<  (*iFirstSimTk).vertIndex() << std::endl;  
      
    } else {
      std::cout << " Only one kine track from Primary Vertex " << std::endl;
    }
  }
  
  std::cout << " Loop over all particles " << std::endl;
  
  int npv=0;
  int iPho=0;
  int iElec=0;
  int iPizero=0;
  //   theSimTracks.reset();
  for (std::vector<SimTrack>::iterator iSimTk = theSimTracks.begin(); iSimTk != theSimTracks.end(); ++iSimTk){
    if (  (*iSimTk).noVertex() ) continue;

    int vertexId = (*iSimTk).vertIndex();
    SimVertex vertex = theSimVertices[vertexId];
 
    std::cout << " Particle type " <<  (*iSimTk).type() << " Sim Track ID " << (*iSimTk).trackId() << " momentum " << (*iSimTk).momentum() <<  " vertex position " << vertex.position() << std::endl;  
    if ( (*iSimTk).vertIndex() == iPV ) {
      npv++;
      if ( (*iSimTk).type() == 22) {
	std::cout << " Found a primary photon with ID  " << (*iSimTk).trackId() << " momentum " << (*iSimTk).momentum() <<  std::endl; 
	convInd.push_back(0);
        
	photonTracks.push_back( &(*iSimTk) );
	
	CLHEP::HepLorentzVector momentum = (*iSimTk).momentum();
	
	iPho++;

      } else if ( (*iSimTk).type() == 11 || (*iSimTk).type()==-11 ) {
	std::cout << " Found a primary electron with ID  " << (*iSimTk).trackId() << " momentum " << (*iSimTk).momentum() <<  std::endl;
	
	electronTracks.push_back( &(*iSimTk) );

	CLHEP::HepLorentzVector momentum = (*iSimTk).momentum();

	iElec++;
        
        // std::cout << "        iElec++;" << std::endl;
	
      } else if ( (*iSimTk).type() == 111 ) {
	std::cout << " Found a primary pi0 with ID  " << (*iSimTk).trackId() << " momentum " << (*iSimTk).momentum() <<  std::endl;	 
	
	pizeroTracks.push_back( &(*iSimTk) );
	
	CLHEP::HepLorentzVector momentum = (*iSimTk).momentum();
	
	iPizero++;
	
      }
      
    }
    
   } 
  
  std::cout << " There are " << npv << " particles originating in the PV " << std::endl;
  
   if(npv > 4) {
     ievtype = PYTHIA;
   } else if(npv == 1) {
     if( abs(partType1) == 11 ) {
       ievtype = SINGLE; ievflav = ELECTRON_FLAV;
     } else if(partType1 == 111) {
       ievtype = SINGLE; ievflav = PIZERO_FLAV;
     } else if(partType1 == 22) {
       ievtype = SINGLE; ievflav = PHOTON_FLAV;
     }
   } else if(npv == 2) {
     if (  abs(partType1) == 11 && abs(partType2) == 11 ) {
       ievtype = DOUBLE; ievflav = ELECTRON_FLAV;
     } else if(partType1 == 111 && partType2 == 111)   {
       ievtype = DOUBLE; ievflav = PIZERO_FLAV;
     } else if(partType1 == 22 && partType2 == 22)   {
       ievtype = DOUBLE; ievflav = PHOTON_FLAV;
     }
   }

   if (ievflav == PHOTON_FLAV) {
     std::cout << " It's a primary PHOTON event with " << photonTracks.size() << " photons " << std::endl;
   }

   if (ievflav == ELECTRON_FLAV) {
     std::cout << " It's a primary ELECTRON event with " << electronTracks.size() << " electrons " << std::endl;
   }

   if (ievflav == PIZERO_FLAV) {
     std::cout << " It's a primary PIZERO event with " << pizeroTracks.size() << " pizeros " << std::endl;
   }

   // loop over sim tracks for pizeros

   int jPartType, pizeroId, phoId, elecId, posId;
   CLHEP::HepLorentzVector momentum1, momentum2;
   PizeroMCTruth thePizero;
   std::vector<SimTrack> decayProducts;
   bool photon, electron, positron;

   for (std::vector<SimTrack>::iterator jSimTk = theSimTracks.begin();
        jSimTk != theSimTracks.end(); ++jSimTk) {
     
     jPartType = (*jSimTk).type();
     
     if (jPartType == 111) {
	pizeroId = (*jSimTk).trackId();
	// std::cout << "Found a pizero with track ID " << pizeroId << std::endl;

	for (std::vector<SimVertex>::iterator kSimVtx = theSimVertices.begin();
	     kSimVtx != theSimVertices.end(); ++kSimVtx) {
          if ((*kSimVtx).parentIndex() == pizeroId) {
	    std::cout << "Matched vtx " << (*kSimVtx) << " with pizero " << pizeroId << std::endl;
            
	    decayProducts.clear();
	    
	    // Fill decayProducts vector with tracks whose vertex match pizero
	    for (std::vector<SimTrack>::iterator nSimTk = jSimTk;
		 nSimTk != theSimTracks.end(); ++nSimTk) {

	      if ((theSimVertices.at((*nSimTk).vertIndex())).position() == (*kSimVtx).position()) {
		
		std::cout << "Found a decay product " << (*nSimTk).trackId() << std::endl;
		decayProducts.push_back(*nSimTk);
	      }

	    }

	    // Check number of decay products for type

	    if (decayProducts.size() == 2 && decayProducts.at(0).type() == 22 && decayProducts.at(1).type() == 22) {

	      thePizero.SetDecay((*kSimVtx).position().perp(), (*kSimVtx).position().z(), decayProducts.at(0).momentum(), decayProducts.at(1).momentum());
	      result.push_back(thePizero);

	    }

	    if (decayProducts.size() == 3) {

	      photon = false;
	      electron = false;
	      positron = false;

	      for (int m = 0; m < 3; m++) {
		if (decayProducts.at(m).type() == 22) {
		  photon = true;
		  phoId = m;
		}
		if (decayProducts.at(m).type() == 11) {
		  electron = true;
		  elecId = m;
		}
		if (decayProducts.at(m).type() == -11) {
		  positron = true;
		  posId = m;
		}
	      }

	      if (photon && electron && positron) {
		thePizero.SetDalitzDecay((*kSimVtx).position().perp(), (*kSimVtx).position().z(), decayProducts.at(phoId).momentum(), decayProducts.at(elecId).momentum(), decayProducts.at(posId).momentum());
		result.push_back(thePizero);
	      }

	    }
	    
	  }
	}
     }

   }

 return result;  
}

void PizeroMCTruthFinder::fill(std::vector<SimTrack>& simTracks, 
                                 std::vector<SimVertex>& simVertices ) {
  std::cout << "  ElectronMCTruthFinder::fill " << std::endl;

 // Watch out there ! A SimVertex is in mm (stupid), 
 
  unsigned nVtx = simVertices.size();
  unsigned nTks = simTracks.size();

  // Empty event, do nothin'
  if ( nVtx == 0 ) return;

  // create a map associating geant particle id and position in the 
  // event SimTrack vector
  for( unsigned it=0; it<nTks; ++it ) {
    // geantToIndex_[ simTracks[it].trackId() ] = it;

    // std::cout << "geantToIndex_[ simTracks[it].trackId() ] = it;" << std::endl;
    
    // std::cout << " ElectronMCTruthFinder::fill it " << it << " simTracks[it].trackId() " <<  simTracks[it].trackId() << std::endl;
 
  }  

  // std::cout << "  ::fill done." << std::endl;

}
