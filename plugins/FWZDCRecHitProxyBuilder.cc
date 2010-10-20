/*
 *  FWZDCRecHitProxyBuilder.cc
 *  cmsShow
 *
 *  Created by Ianna Osborne on 7/8/10.
 *  Copyright 2010 FNAL. All rights reserved.
 *
 */
#include "Fireworks/Core/interface/FWDigitSetProxyBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWGeometry.h"
#include "Fireworks/Core/interface/BuilderUtils.h"
#include "DataFormats/HcalRecHit/interface/ZDCRecHit.h"
#include "DataFormats/Common/interface/SortedCollection.h"

class FWZDCRecHitProxyBuilder :  public FWDigitSetProxyBuilder
{
public:
   FWZDCRecHitProxyBuilder( void ) {}  
   virtual ~FWZDCRecHitProxyBuilder( void ) {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWZDCRecHitProxyBuilder( const FWZDCRecHitProxyBuilder& );

   const FWZDCRecHitProxyBuilder& operator=( const FWZDCRecHitProxyBuilder& );
   virtual void build( const FWEventItem* iItem, TEveElementList* product, const FWViewContext* );	
};

void FWZDCRecHitProxyBuilder::build(const FWEventItem* iItem, TEveElementList* product, const FWViewContext*)
{
   const edm::SortedCollection<ZDCRecHit> *collection = 0;
   iItem->get( collection );
   if (! collection)
      return;


   TEveBoxSet* boxSet = addBoxSetToProduct(product);
   for (std::vector<ZDCRecHit>::const_iterator it = collection->begin() ; it != collection->end(); ++it)
   {  
      const float* corners = item()->getGeom()->getCorners((*it).detid());

      std::vector<float> scaledCorners(24);
      if (corners == 0) 
         fireworks::energyTower3DCorners(corners, (*it).energy(), scaledCorners);

      addBox(boxSet, &scaledCorners[0]);
   }
}

REGISTER_FWPROXYBUILDER( FWZDCRecHitProxyBuilder,  edm::SortedCollection<ZDCRecHit> , "ZDC RecHit", FWViewType::kISpyBit );
