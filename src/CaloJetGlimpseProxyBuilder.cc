// -*- C++ -*-
//
// Package:     Calo
// Class  :     CaloJetGlimpseProxyBuilder
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: CaloJetGlimpseProxyBuilder.cc,v 1.4 2008/06/27 00:58:34 dmytro Exp $
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
#include "TEveBoxSet.h"

// user include files
#include "Fireworks/Calo/interface/CaloJetGlimpseProxyBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/BuilderUtils.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "Fireworks/Core/interface/FWGlimpseView.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CaloJetGlimpseProxyBuilder::CaloJetGlimpseProxyBuilder()
{
}

// CaloJetGlimpseProxyBuilder::CaloJetGlimpseProxyBuilder(const CaloJetGlimpseProxyBuilder& rhs)
// {
//    // do actual copying here;
// }

CaloJetGlimpseProxyBuilder::~CaloJetGlimpseProxyBuilder()
{
}

//
// assignment operators
//
// const CaloJetGlimpseProxyBuilder& CaloJetGlimpseProxyBuilder::operator=(const CaloJetGlimpseProxyBuilder& rhs)
// {
//   //An exception safe implementation is
//   CaloJetGlimpseProxyBuilder temp(rhs);
//   swap(rhs);
//
//   return *this;
// }


void
CaloJetGlimpseProxyBuilder::build(const FWEventItem* iItem, TEveElementList** product)
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
      TEveBoxSet* cone = new TEveBoxSet(counter.str().c_str());
      cone->SetPickable(kTRUE);
      cone->Reset(TEveBoxSet::kBT_EllipticCone, kTRUE, 64);
      double height = jet->et()*FWGlimpseView::getScale();
      TEveVector dir, pos;
      dir.Set(jet->px()/jet->p(), jet->py()/jet->p(), jet->pz()/jet->p());
      
      dir *= height;
      pos.Set(0.0,0.0,0.0);
      double eta_size = sqrt(jet->etaetaMoment());
      double theta_size = fabs(getTheta(jet->eta()+eta_size)-getTheta(jet->eta()-eta_size));
      double phi_size = sqrt(jet->phiphiMoment());
      cone->AddEllipticCone(pos, dir, theta_size*height, phi_size*height);
      
      cone->DigitColor( iItem->defaultDisplayProperties().color(), 50 );
      // TColor* c = gROOT->GetColor(iItem->defaultDisplayProperties().color());
      // cone->DigitColor( UChar_t(255*c->GetRed()), UChar_t(255*c->GetGreen()), UChar_t(255*c->GetBlue()), 20 );
      cone->SetDrawConeCap(kFALSE);
      cone->SetMainTransparency(50);
      tList->AddElement(cone);
      /*
      TEveStraightLineSet* marker = new TEveStraightLineSet(counter.str().c_str());
      marker->SetLineWidth(1);
      marker->SetLineColor(  iItem->defaultDisplayProperties().color() );
      fw::addStraightLineSegment( marker, &*jet );
      tList->AddElement(marker);
       */
      
   }
}

REGISTER_FWGLIMPSEDATAPROXYBUILDER(CaloJetGlimpseProxyBuilder,reco::CaloJetCollection,"Jets");
