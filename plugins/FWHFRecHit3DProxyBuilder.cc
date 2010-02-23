#include "Fireworks/Core/interface/register_dataproxybuilder_macro.h"
#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "TEveCompound.h"
#include "TEveStraightLineSet.h"

class FWHFRecHit3DProxyBuilder : public FW3DSimpleProxyBuilderTemplate<HFRecHit>
{
public:
   FWHFRecHit3DProxyBuilder(void) 
    {}
  
   virtual ~FWHFRecHit3DProxyBuilder(void) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWHFRecHit3DProxyBuilder(const FWHFRecHit3DProxyBuilder&); 			// stop default
   const FWHFRecHit3DProxyBuilder& operator=(const FWHFRecHit3DProxyBuilder&); 	// stop default

   void build(const HFRecHit& iData, unsigned int iIndex, TEveElement& oItemHolder) const;
};

void
FWHFRecHit3DProxyBuilder::build(const HFRecHit& iData, unsigned int iIndex, TEveElement& oItemHolder) const
{
   std::vector<TEveVector> corners = item()->getGeom()->getPoints(iData.detid().rawId());
   if( corners.empty() ) {
      return;
   }

   Double_t globalFront1Point[3] = {corners[0].fX,  corners[0].fY, corners[0].fZ};
   Double_t globalFront2Point[3] = {corners[1].fX,  corners[1].fY, corners[1].fZ};
   Double_t globalFront3Point[3] = {corners[2].fX,  corners[2].fY, corners[2].fZ};
   Double_t globalFront4Point[3] = {corners[3].fX,  corners[3].fY, corners[3].fZ};
   Double_t globalBack1Point[3] = {corners[4].fX,  corners[4].fY, corners[4].fZ};
   Double_t globalBack2Point[3] = {corners[5].fX,  corners[5].fY, corners[5].fZ};
   Double_t globalBack3Point[3] = {corners[6].fX,  corners[6].fY, corners[6].fZ};
   Double_t globalBack4Point[3] = {corners[7].fX,  corners[7].fY, corners[7].fZ};

   TEveStraightLineSet* rechitSet = new TEveStraightLineSet("HF Rec Hit");
   rechitSet->SetLineWidth(3);
   rechitSet->SetMainColor(item()->defaultDisplayProperties().color());
   rechitSet->SetRnrSelf(item()->defaultDisplayProperties().isVisible());
   rechitSet->SetRnrChildren(item()->defaultDisplayProperties().isVisible());

   Float_t scale = 10.0; 	// FIXME: The scale should be taken form somewhere else
   Float_t energy = iData.energy();
   Float_t eScale = scale * energy;

   rechitSet->AddLine(globalFront1Point[0], globalFront1Point[1], globalFront1Point[2],
		      globalFront2Point[0], globalFront2Point[1], globalFront2Point[2]);
   rechitSet->AddLine(globalFront2Point[0], globalFront2Point[1], globalFront2Point[2],
		      globalFront3Point[0], globalFront3Point[1], globalFront3Point[2]);
   rechitSet->AddLine(globalFront3Point[0], globalFront3Point[1], globalFront3Point[2],
		      globalFront4Point[0], globalFront4Point[1], globalFront4Point[2]);
   rechitSet->AddLine(globalFront4Point[0], globalFront4Point[1], globalFront4Point[2],
		      globalFront1Point[0], globalFront1Point[1], globalFront1Point[2]);

   rechitSet->AddLine(globalBack1Point[0], globalBack1Point[1], globalBack1Point[2],
		      globalBack2Point[0], globalBack2Point[1], globalBack2Point[2]);
   rechitSet->AddLine(globalBack2Point[0], globalBack2Point[1], globalBack2Point[2],
		      globalBack3Point[0], globalBack3Point[1], globalBack3Point[2]);
   rechitSet->AddLine(globalBack3Point[0], globalBack3Point[1], globalBack3Point[2],
		      globalBack4Point[0], globalBack4Point[1], globalBack4Point[2]);
   rechitSet->AddLine(globalBack4Point[0], globalBack4Point[1], globalBack4Point[2],
		      globalBack1Point[0], globalBack1Point[1], globalBack1Point[2]);

   rechitSet->AddLine(globalFront1Point[0], globalFront1Point[1], globalFront1Point[2],
		      globalBack1Point[0], globalBack1Point[1], globalBack1Point[2]);
   rechitSet->AddLine(globalFront2Point[0], globalFront2Point[1], globalFront2Point[2],
		      globalBack2Point[0], globalBack2Point[1], globalBack2Point[2]);
   rechitSet->AddLine(globalFront3Point[0], globalFront3Point[1], globalFront3Point[2],
		      globalBack3Point[0], globalBack3Point[1], globalBack3Point[2]);
   rechitSet->AddLine(globalFront4Point[0], globalFront4Point[1], globalFront4Point[2],
		      globalBack4Point[0], globalBack4Point[1], globalBack4Point[2]);     

   oItemHolder.AddElement(rechitSet);
}

REGISTER_FW3DDATAPROXYBUILDER(FWHFRecHit3DProxyBuilder, HFRecHit, "HF RecHit");
