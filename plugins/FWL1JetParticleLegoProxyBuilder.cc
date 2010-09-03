/*
 *  FWL1JetParticleLegoProxyBuilder.cc
 *  FWorks
 *
 *  Created by Ianna Osborne on 9/3/10.
 *  Copyright 2010 FNAL. All rights reserved.
 *
 */

#include "Fireworks/Core/interface/FWSimpleProxyBuilderTemplate.h"
#include "Fireworks/Calo/interface/CaloUtils.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"

class FWL1JetParticleLegoProxyBuilder : public FWSimpleProxyBuilderTemplate<l1extra::L1JetParticle>
{
public:
   FWL1JetParticleLegoProxyBuilder() {}
   virtual ~FWL1JetParticleLegoProxyBuilder() {}
   
   REGISTER_PROXYBUILDER_METHODS();
   
private:
   FWL1JetParticleLegoProxyBuilder(const FWL1JetParticleLegoProxyBuilder&);    // stop default
   const FWL1JetParticleLegoProxyBuilder& operator=(const FWL1JetParticleLegoProxyBuilder&);    // stop default
   
   virtual void build( const l1extra::L1JetParticle& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext*);
};

void
FWL1JetParticleLegoProxyBuilder::build( const l1extra::L1JetParticle& iData, unsigned int iIndex, TEveElement& oItemHolder , const FWViewContext*) 
{
   fireworks::addCircle( iData.eta(), iData.phi(), 0.5, 6, &oItemHolder, this );
}

REGISTER_FWPROXYBUILDER( FWL1JetParticleLegoProxyBuilder, l1extra::L1JetParticle, "L1JetParticle", FWViewType::kLegoBit );