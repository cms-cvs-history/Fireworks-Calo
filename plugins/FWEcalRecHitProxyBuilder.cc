/*
 *  FWEcalRecHitProxyBuilder.cc
 *  FWorks
 *
 *  Created by Ianna Osborne on 5/28/10.
 *  Copyright 2010 FNAL. All rights reserved.
 *
 */

#include "Fireworks/Core/interface/FWSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Calo/interface/CaloUtils.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "TEveCompound.h"

class FWEcalRecHitProxyBuilder : public FWSimpleProxyBuilderTemplate<EcalRecHit>
{
public:
	FWEcalRecHitProxyBuilder(void) 
    {}
	
	virtual ~FWEcalRecHitProxyBuilder(void) 
    {}
	
	REGISTER_PROXYBUILDER_METHODS();
	
private:
	// Disable default copy constructor
	FWEcalRecHitProxyBuilder(const FWEcalRecHitProxyBuilder&);
	// Disable default assignment operator
	const FWEcalRecHitProxyBuilder& operator=(const FWEcalRecHitProxyBuilder&);
	
	void build(const EcalRecHit& iData, unsigned int iIndex, TEveElement& oItemHolder, const FWViewContext*);
};

void
FWEcalRecHitProxyBuilder::build(const EcalRecHit& iData, unsigned int iIndex, TEveElement& oItemHolder, const FWViewContext*) 
{
	std::vector<TEveVector> corners = item()->getGeom()->getPoints(iData.detid());
	if( corners.empty() ) {
		return;
	}
	Float_t scale = 10.0; 	// FIXME: The scale should be taken form somewhere else
	
	fireworks::drawEnergyTower3D(corners, iData.energy() * scale, &oItemHolder, this);
}

REGISTER_FWPROXYBUILDER(FWEcalRecHitProxyBuilder, EcalRecHit, "Ecal RecHit", FWViewType::kISpyBit );
