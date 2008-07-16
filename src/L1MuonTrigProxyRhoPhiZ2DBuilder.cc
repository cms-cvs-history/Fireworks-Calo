// -*- C++ -*-
//
// Package:     Calo
// Class  :     L1MuonTrigProxyRhoPhiZ2DBuilder
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: L1MuonTrigProxyRhoPhiZ2DBuilder.cc,v 1.3 2008/07/08 06:53:20 dmytro Exp $
//

// system include files
#include "TEveGeoNode.h"
#include "TEveGeoShapeExtract.h"
#include "TGeoArb8.h"
#include "TEveManager.h"
#include "TGeoSphere.h"
#include "TGeoTube.h"
#include "TH1F.h"
#include "TColor.h"
#include "TROOT.h"
#include "TEvePointSet.h"
#include "TEveScalableStraightLineSet.h"
#include "TEveCompound.h"

// user include files
#include "Fireworks/Calo/interface/L1MuonTrigProxyRhoPhiZ2DBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FW3DLegoDataProxyBuilder.h"
#include "Fireworks/Calo/interface/ECalCaloTowerProxyRhoPhiZ2DBuilder.h"
#include "Fireworks/Core/interface/BuilderUtils.h"

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "Fireworks/Core/interface/FWDisplayEvent.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1MuonTrigProxyRhoPhiZ2DBuilder::L1MuonTrigProxyRhoPhiZ2DBuilder()
{
}

// L1MuonTrigProxyRhoPhiZ2DBuilder::L1MuonTrigProxyRhoPhiZ2DBuilder(const L1MuonTrigProxyRhoPhiZ2DBuilder& rhs)
// {
//    // do actual copying here;
// }

L1MuonTrigProxyRhoPhiZ2DBuilder::~L1MuonTrigProxyRhoPhiZ2DBuilder()
{
}

//
// mmuonber functions
//
void 
L1MuonTrigProxyRhoPhiZ2DBuilder::buildRhoPhi(const FWEventItem* iItem,
					    TEveElementList** product)
{


  TEveElementList* tList = *product;

  // Make the eve element list
  if(0 == tList) {
    tList =  new TEveElementList(iItem->name().c_str(),"L1 RhoPhi",true);
    *product = tList;
    tList->SetMainColor(iItem->defaultDisplayProperties().color());
  } else {
    tList->DestroyElements();
  }
   
  // Get the particle map collection for L1MuonParticles
  l1extra::L1MuonParticleCollection const * triggerColl=0;
  iItem->get(triggerColl);
  if(0==triggerColl) return;
   
   double r_ecal = 126;
   fw::NamedCounter counter("l1muontrigs");

   // Ready to loop over the triggered objects
   l1extra::L1MuonParticleCollection::const_iterator trigIt = triggerColl->begin(),
     trigEnd = triggerColl->end();
   // Loop over triggered objects and make some 4-vectors
   for ( ; trigIt != trigEnd; ++trigIt ) {
      const unsigned int nBuffer = 1024;
      char title[nBuffer]; 
      snprintf(title, nBuffer, "L1 Muon %d, Et: %0.1f GeV",counter.index(),trigIt->et());
     TEveCompound* container = new TEveCompound( counter.str().c_str(), title );
     container->OpenCompound();
     //guarantees that CloseCompound will be called no matter what happens
     boost::shared_ptr<TEveCompound> sentry(container,boost::mem_fn(&TEveCompound::CloseCompound));
      
     double phi = trigIt->phi();
     double size = trigIt->pt();
      
      TEveScalableStraightLineSet* marker = new TEveScalableStraightLineSet("energy");
      marker->SetLineWidth(1);
      marker->SetLineStyle(7);
      marker->SetLineColor(  iItem->defaultDisplayProperties().color() );
      marker->SetScaleCenter( r_ecal*cos(phi), r_ecal*sin(phi), 0 );
      marker->AddLine( r_ecal*cos(phi), r_ecal*sin(phi), 0, (r_ecal+size)*cos(phi), (r_ecal+size)*sin(phi), 0);
      container->AddElement(marker);
      container->SetRnrSelf(     iItem->defaultDisplayProperties().isVisible() );
      container->SetRnrChildren( iItem->defaultDisplayProperties().isVisible() );

     tList->AddElement(container);

   }// end loop over muon particle objects

}


void 
L1MuonTrigProxyRhoPhiZ2DBuilder::buildRhoZ(const FWEventItem* iItem,
					    TEveElementList** product)
{


  TEveElementList* tList = *product;

  // Make the eve element list
  if(0 == tList) {
    tList =  new TEveElementList(iItem->name().c_str(),"L1 RhoZ",true);
    *product = tList;
    tList->SetMainColor(iItem->defaultDisplayProperties().color());
  } else {
    tList->DestroyElements();
  }

  // Get the particle map collection for L1MuonParticles
  l1extra::L1MuonParticleCollection const * triggerColl=0;
  iItem->get(triggerColl);
  if(0==triggerColl) return;

   double z_ecal = 306; // ECAL endcap inner surface
   double r_ecal = 126;
   double transition_angle = atan(r_ecal/z_ecal);
   fw::NamedCounter counter("l1muontrigs");


   // Ready to loop over the triggered objects
   l1extra::L1MuonParticleCollection::const_iterator trigIt = triggerColl->begin(),
     trigEnd = triggerColl->end();
   // Loop over triggered objects and make some 4-vectors
   for ( ; trigIt != trigEnd; ++trigIt ) {
      const unsigned int nBuffer = 1024;
      char title[nBuffer]; 
      snprintf(title, nBuffer, "L1 Muon %d, Et: %0.1f GeV",counter.index(),trigIt->et());
     TEveCompound* container = new TEveCompound( counter.str().c_str(), title );
     container->OpenCompound();
     //guarantees that CloseCompound will be called no matter what happens
     boost::shared_ptr<TEveCompound> sentry(container,boost::mem_fn(&TEveCompound::CloseCompound));
      
     double theta = trigIt->theta();
      
     // distance from the origin of the jet centroid
     // energy is measured from this point
     // if jet is made of a single tower, the length of the jet will 
     // be identical to legth of the displayed tower
     double r(0); 
     if ( theta < transition_angle || M_PI-theta < transition_angle )
       r = z_ecal/fabs(cos(theta));
     else
       r = r_ecal/sin(theta);
      
     double size = trigIt->pt();
      
      TEveScalableStraightLineSet* marker = new TEveScalableStraightLineSet("energy");
      marker->SetLineWidth(1);
      marker->SetLineStyle(7);
      marker->SetLineColor(  iItem->defaultDisplayProperties().color() );
      marker->SetScaleCenter(0., (trigIt->phi()>0 ? r*fabs(sin(theta)) : -r*fabs(sin(theta))), r*cos(theta));
      marker->AddLine(0., (trigIt->phi()>0 ? r*fabs(sin(theta)) : -r*fabs(sin(theta))), r*cos(theta),
		      0., (trigIt->phi()>0 ? (r+size)*fabs(sin(theta)) : -(r+size)*fabs(sin(theta))), (r+size)*cos(theta) );
      container->AddElement( marker );
      container->SetRnrSelf(     iItem->defaultDisplayProperties().isVisible() );
      container->SetRnrChildren( iItem->defaultDisplayProperties().isVisible() );
     tList->AddElement(container);

   }// end loop over muon particle objects

}

REGISTER_FWRPZ2DDATAPROXYBUILDER(L1MuonTrigProxyRhoPhiZ2DBuilder,l1extra::L1MuonParticleCollection,"L1-Muons");
