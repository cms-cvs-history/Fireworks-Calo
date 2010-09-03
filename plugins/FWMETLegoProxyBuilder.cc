/*
 *  FWMETLegoProxyBuilder.cc
 *  FWorks
 *
 *  Created by Ianna Osborne on 9/3/10.
 *  Copyright 2010 FNAL. All rights reserved.
 *
 */

#include "Fireworks/Core/interface/FWSimpleProxyBuilderTemplate.h"
#include "Fireworks/Calo/interface/CaloUtils.h"

#include "DataFormats/METReco/interface/MET.h"

class FWMETLegoProxyBuilder : public FWSimpleProxyBuilderTemplate<reco::MET>
{
public:
   FWMETLegoProxyBuilder( void ) {}
   virtual ~FWMETLegoProxyBuilder( void ) {}
   
   REGISTER_PROXYBUILDER_METHODS();
   
private:
   FWMETLegoProxyBuilder( const FWMETLegoProxyBuilder& );    // stop default
   const FWMETLegoProxyBuilder& operator=( const FWMETLegoProxyBuilder& );    // stop default
   
   virtual void build( const reco::MET& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext* );
};

void
FWMETLegoProxyBuilder::build( const reco::MET& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext* ) 
{
   fireworks::addDoubleLines( iData.phi(), &oItemHolder, this );
}

REGISTER_FWPROXYBUILDER( FWMETLegoProxyBuilder, reco::MET, "recoMET", FWViewType::kLegoBit );
