// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     TkMuon

#include "DataFormats/Phase2L1Correlator/interface/TkMuon.h"

using namespace l1t;


TkMuon::TkMuon( const LorentzVector& p4,
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



TkMuon::TkMuon( const LorentzVector& p4,
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




