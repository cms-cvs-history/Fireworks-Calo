// -*- C++ -*-
//
// Package:     Calo
// Class  :     L1JetTrigProxyEveLegoBuilder
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: L1JetTrigProxyEveLegoBuilder.cc,v 1.2 2008/07/04 01:40:36 dmytro Exp $
//

// system include files
#include "TEveElement.h"
#include "TColor.h"
#include "TGeoTube.h"
#include "TEveGeoShapeExtract.h"
#include "TEveTrans.h"
#include "TEveGeoNode.h"
#include "TROOT.h"
#include "TEveStraightLineSet.h"

// user include files
#include "Fireworks/Calo/interface/L1JetTrigProxyEveLegoBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/BuilderUtils.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1JetTrigProxyEveLegoBuilder::L1JetTrigProxyEveLegoBuilder()
{


}

// L1JetTrigProxyEveLegoBuilder::L1JetTrigProxyEveLegoBuilder(const L1JetTrigProxyEveLegoBuilder& rhs)
// {
//    // do actual copying here;
// }

L1JetTrigProxyEveLegoBuilder::~L1JetTrigProxyEveLegoBuilder()
{
}

//
// assignment operators
//
// const L1JetTrigProxyEveLegoBuilder& L1JetTrigProxyEveLegoBuilder::operator=(const L1JetTrigProxyEveLegoBuilder& rhs)
// {
//   //An exception safe implementation is
//   L1JetTrigProxyEveLegoBuilder temp(rhs);
//   swap(rhs);
//
//   return *this;
// }


void
L1JetTrigProxyEveLegoBuilder::build(const FWEventItem* iItem, TEveElementList** product)
{
  TEveElementList* tList = *product;

  // Make the eve element list
  if(0 == tList) {
    tList =  new TEveElementList(iItem->name().c_str(),"L1JetLego",true);
    *product = tList;
    tList->SetMainColor(iItem->defaultDisplayProperties().color());
  } else {
    tList->DestroyElements();
  }
   
  // Get the particle map collection for L1JetParticles
  l1extra::L1JetParticleCollection const * triggerColl=0;
  iItem->get(triggerColl);
  if(0==triggerColl) return;
   
  // make a counter
  fw::NamedCounter counter("l1jettrigs");
  TColor* c = gROOT->GetColor( tList->GetMainColor() );
  Float_t rgba[4] = { 1, 0, 0, 1 };
  if (c) {
    rgba[0] = c->GetRed();
    rgba[1] = c->GetGreen();
    rgba[2] = c->GetBlue();
  }

  // Ready to loop over the triggered objects
  l1extra::L1JetParticleCollection::const_iterator jet = triggerColl->begin(),
  trigEnd = triggerColl->end();
  const unsigned int nLineSegments = 6;
  const double jetRadius = 0.5;
   
  // Loop over triggered objects and make some 4-vectors
  for ( ; jet != trigEnd; ++jet ) {
      char title[1024]; 
      sprintf(title,"L1 Jet %d, Et: %0.1f GeV",counter.index(),jet->et());
      TEveStraightLineSet* container = new TEveStraightLineSet( counter.str().c_str(), title );
      // container->SetLineWidth(4);
      container->SetLineColor(  iItem->defaultDisplayProperties().color() );
      
      for ( unsigned int iphi = 0; iphi < nLineSegments; ++iphi ) {
	 container->AddLine(jet->eta()+jetRadius*cos(2*M_PI/nLineSegments*iphi),
			    jet->phi()+jetRadius*sin(2*M_PI/nLineSegments*iphi),
			    0.1,
			    jet->eta()+jetRadius*cos(2*M_PI/nLineSegments*(iphi+1)),
			    jet->phi()+jetRadius*sin(2*M_PI/nLineSegments*(iphi+1)),
			    0.1);
      }
      tList->AddElement(container);
  }// end loop over em particle objects

  

}

REGISTER_FW3DLEGODATAPROXYBUILDER(L1JetTrigProxyEveLegoBuilder,l1extra::L1JetParticleCollection,"L1-Jets");

