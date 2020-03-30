// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     L1TkMuonParticle

#include "DataFormats/Phase2L1Correlator/interface/L1TkMuonParticle.h"

using namespace l1t;


L1TkMuonParticle::L1TkMuonParticle( const LorentzVector& p4,
         const edm::Ref< RegionalMuonCandBxCollection > &muRef,
         const edm::Ptr< L1TTTrackType >& trackPtr,
         float tkisol )
   : L1Candidate( p4 ),
     muRef_ ( muRef ) ,
     trkPtr_ ( trackPtr ) ,
     theIsolation ( tkisol ),
     TrkzVtx_(999),
     quality_(999)
{

 if ( trkPtr_.isNonnull() ) {
	float z = trkPtr() -> POCA().z();
	setTrkzVtx( z );
 }
}



L1TkMuonParticle::L1TkMuonParticle( const LorentzVector& p4,
         const edm::Ptr< L1TTTrackType >& trackPtr,
         float tkisol )
   : L1Candidate( p4 ),
     trkPtr_ ( trackPtr ) ,
     theIsolation ( tkisol ),
     TrkzVtx_(999),
     quality_(999)
{

 if ( trkPtr_.isNonnull() ) {
	float z = trkPtr() -> POCA().z();
	setTrkzVtx( z );
 }
}




