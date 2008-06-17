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
// $Id: CaloJetProxyEveLegoBuilder.cc,v 1.4 2008/06/09 19:54:03 chrjones Exp $
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
#include "TEveCompound.h"

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
   if(0==jets) {
      std::cout <<"Failed to get CaloJets"<<std::endl;
      return;
   }
   
   fw::NamedCounter counter("jet");

   for(reco::CaloJetCollection::const_iterator jet = jets->begin(); 
       jet != jets->end(); ++jet, ++counter) {

      TEveCompound* container = new TEveCompound( counter.str().c_str() );
      container->OpenCompound();
      //guarantees that CloseCompound will be called no matter what happens
      boost::shared_ptr<TEveCompound> sentry(container,boost::mem_fn(&TEveCompound::CloseCompound));
      	
      
      TEveStraightLineSet* marker = new TEveStraightLineSet("energy");
      marker->SetLineWidth(1);
      marker->SetLineColor(  iItem->defaultDisplayProperties().color() );
      fw::addStraightLineSegment( marker, &*jet );
      container->AddElement(marker);
      tList->AddElement(container);
      //container->CloseCompound();
   }
}

REGISTER_FW3DLEGODATAPROXYBUILDER(CaloJetProxyEveLegoBuilder,reco::CaloJetCollection,"Jets");

