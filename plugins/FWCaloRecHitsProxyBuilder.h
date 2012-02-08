#ifndef Fireworks_Calo_FWCaloRecHitProxyBuilder_h
#define Fireworks_Calo_FWCaloRecHitProxyBuilder_h
// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWCaloRecHitProxyBuilderBase
//
/**\class FWCaloRecHitProxyBuilderBase FWCaloRecHitProxyBuilderBase.h Fireworks/Calo/interface/FWCaloRecHitProxyBuilderBase.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:  Chris Jones
//         Created:  Wed Dec  3 11:28:08 EST 2008
// $Id: FWCaloRecHitProxyBuilder.h,v 1.16 2010/12/16 11:39:38 amraktad Exp $
//

#include "Rtypes.h"
#include <string>

#include "Fireworks/Calo/interface/FWCaloDataProxyBuilderBase.h"
#include "Fireworks/Calo/src/FWFromTEveCaloDataSelector.h"

class TH2F;

class FWCaloRecHitProxyBuilder : public FWCaloDataProxyBuilderBase {

public:
   FWCaloRecHitProxyBuilder():m_hist(0){}
   virtual ~FWCaloRecHitProxyBuilder(){}

   // REGISTER_PROXYBUILDER_METHODS();
   static std::string typeOfBuilder();

protected:
   virtual void itemBeingDestroyed(const FWEventItem*);

   virtual void setCaloData(const fireworks::Context&);

   void fillCaloData();
   void fillTowerForDetId( unsigned int rawid, float val, const FWEventItem::ModelInfo& info );
   bool assertCaloDataSlice();

   TH2F* m_hist;
   std::map<int, float> m_cellSelelectedSum;

private:
   FWCaloRecHitProxyBuilder(const FWCaloRecHitProxyBuilder&); // stop default
   const FWCaloRecHitProxyBuilder& operator=(const FWCaloRecHitProxyBuilder&); // stop default

   //   virtual void build(const FWEventItem* iItem, TEveElementList* product, const FWViewContext*);
};

#endif
