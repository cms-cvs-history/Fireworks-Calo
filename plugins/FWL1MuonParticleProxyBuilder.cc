// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWL1MuonParticleProxyBuilder
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: FWL1MuonParticleProxyBuilder.cc,v 1.7 2010/08/30 15:42:32 amraktad Exp $
//

// system include files
#include "TEveScalableStraightLineSet.h"

// user include files
#include "Fireworks/Core/interface/FWSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"

class FWL1MuonParticleProxyBuilder : public FWSimpleProxyBuilderTemplate<l1extra::L1MuonParticle>
{
public:
   FWL1MuonParticleProxyBuilder() {}
   virtual ~FWL1MuonParticleProxyBuilder() {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWL1MuonParticleProxyBuilder(const FWL1MuonParticleProxyBuilder&);    // stop default
   const FWL1MuonParticleProxyBuilder& operator=(const FWL1MuonParticleProxyBuilder&);    // stop default
  
   virtual void build( const l1extra::L1MuonParticle& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext*);
};

void
FWL1MuonParticleProxyBuilder::build( const l1extra::L1MuonParticle& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext*) 
{
   double scale = 10;
   double phi = iData.phi();
   double theta = iData.theta();
   double size = iData.pt() * scale;

   // distance from the origin of the jet centroid
   // energy is measured from this point
   // if jet is made of a single tower, the length of the jet will
   // be identical to legth of the displayed tower
   double r(0);
   if( theta < context().caloTransAngle() || M_PI-theta < context().caloTransAngle())
      r = context().caloZ2()/fabs(cos(theta));
   else
      r = context().caloR1()/sin(theta);
   
   TEveScalableStraightLineSet* marker = new TEveScalableStraightLineSet("l1MuonParticle");
   marker->SetLineWidth( 2 );
   marker->SetLineStyle( 2 );
   marker->AddLine( r*cos(phi)*sin(theta), r*sin(phi)*sin(theta), r*cos(theta),
		    (r+size)*cos(phi)*sin(theta), (r+size)*sin(phi)*sin(theta), (r+size)*cos(theta) );
   setupAddElement(marker, &oItemHolder);
}

REGISTER_FWPROXYBUILDER(FWL1MuonParticleProxyBuilder, l1extra::L1MuonParticle, "L1MuonParticle", FWViewType::kAllRPZBits);

//==============================================================================
/*
class FWL1MuonParticleLegoProxyBuilder : public FWSimpleProxyBuilderTemplate<l1extra::L1MuonParticle>
{
public:
   FWL1MuonParticleLegoProxyBuilder() {}
   virtual ~FWL1MuonParticleLegoProxyBuilder() {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWL1MuonParticleLegoProxyBuilder(const FWL1MuonParticleLegoProxyBuilder&);    // stop default
   const FWL1MuonParticleLegoProxyBuilder& operator=(const FWL1MuonParticleLegoProxyBuilder&);    // stop default
  
   virtual void build( const l1extra::L1MuonParticle& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext*);
};

void
FWL1MuonParticleLegoProxyBuilder::build( const l1extra::L1MuonParticle& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext*) 
{
//FIXME: Crashes on exit.
//    TGeoTube *shape = new TGeoTube(0.48, 0.5, 0.0001);

//    TEveTrans t;
//    t.RotateLF(1, 2, M_PI/2);
   
//    // Find eta and phi based on object type
//    double eta = iData.eta();
//    double phi = iData.phi();

//    // Fill eta and phi
//    t(1,4) = eta;
//    t(2,4) = phi;
//    t(3,4) = 0.1;
//    TEveGeoShape *egs = new TEveGeoShape("outline");
//    egs->SetTransMatrix( t.Array() );
//    egs->SetShape( shape );
//    egs->SetPickable( kTRUE );

//    setupAddElement(egs, &oItemHolder);
}


REGISTER_FWPROXYBUILDER(FWL1MuonParticleLegoProxyBuilder, l1extra::L1MuonParticle, "L1MuonParticle", FWViewType::kLegoBit);
*/
