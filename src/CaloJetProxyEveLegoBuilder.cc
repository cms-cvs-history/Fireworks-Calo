// -*- C++ -*-
//
// Package:     Calo
// Class  :     CaloJetProxyEveLegoBuilder
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: CaloJetProxyEveLegoBuilder.cc,v 1.7 2008/07/16 16:09:23 chrjones Exp $
//

// system include files
#include "TEveElement.h"
#include "TColor.h"
#include "TGeoTube.h"
#include "TEveTrans.h"
#include "TEveGeoNode.h"
#include "TROOT.h"
#include "TEveStraightLineSet.h"

// user include files
#include "Fireworks/Calo/interface/CaloJetProxyEveLegoBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/BuilderUtils.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CaloJetProxyEveLegoBuilder::CaloJetProxyEveLegoBuilder()
{
}

// CaloJetProxyEveLegoBuilder::CaloJetProxyEveLegoBuilder(const CaloJetProxyEveLegoBuilder& rhs)
// {
//    // do actual copying here;
// }

CaloJetProxyEveLegoBuilder::~CaloJetProxyEveLegoBuilder()
{
}

//
// assignment operators
//
// const CaloJetProxyEveLegoBuilder& CaloJetProxyEveLegoBuilder::operator=(const CaloJetProxyEveLegoBuilder& rhs)
// {
//   //An exception safe implementation is
//   CaloJetProxyEveLegoBuilder temp(rhs);
//   swap(rhs);
//
//   return *this;
// }


void
CaloJetProxyEveLegoBuilder::build(const FWEventItem* iItem, TEveElementList** product)
{
   TEveElementList* tList = *product;

   if(0 == tList) {
      tList =  new TEveElementList(iItem->name().c_str(),"JetsLego",true);
      *product = tList;
      tList->SetMainColor(iItem->defaultDisplayProperties().color());
   } else {
      tList->DestroyElements();
   }
   
   const reco::CaloJetCollection* jets=0;
   iItem->get(jets);
   if(0==jets) return;
   
   fw::NamedCounter counter("jet");
   TColor* c = gROOT->GetColor( tList->GetMainColor() );
   Float_t rgba[4] = { 1, 0, 0, 1 };
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }

   const unsigned int nLineSegments = 20;
   const double jetRadius = 0.5;
   for(reco::CaloJetCollection::const_iterator jet = jets->begin(); 
       jet != jets->end(); ++jet, ++counter) {
      const unsigned int kBufferSize = 1024;
      char title[kBufferSize]; 
      snprintf(title,kBufferSize,"Jet %d, Et: %0.1f GeV",counter.index(),jet->et());
      TEveStraightLineSet* container = new TEveStraightLineSet( counter.str().c_str(), title );
      // container->SetLineWidth(4);
      container->SetLineColor(  iItem->defaultDisplayProperties().color() );
      container->SetRnrSelf(iItem->defaultDisplayProperties().isVisible());
      container->SetRnrChildren(iItem->defaultDisplayProperties().isVisible());
      
      for ( unsigned int iphi = 0; iphi < nLineSegments; ++iphi ) {
	 container->AddLine(jet->eta()+jetRadius*cos(2*M_PI/nLineSegments*iphi),
			    jet->phi()+jetRadius*sin(2*M_PI/nLineSegments*iphi),
			    0.1,
			    jet->eta()+jetRadius*cos(2*M_PI/nLineSegments*(iphi+1)),
			    jet->phi()+jetRadius*sin(2*M_PI/nLineSegments*(iphi+1)),
			    0.1);
      }
      tList->AddElement(container);
   }
}

REGISTER_FW3DLEGODATAPROXYBUILDER(CaloJetProxyEveLegoBuilder,reco::CaloJetCollection,"Jets");

