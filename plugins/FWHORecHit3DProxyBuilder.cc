#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "DataFormats/HcalRecHit/interface/HORecHit.h"
#include "TEveCompound.h"
#include "TEveStraightLineSet.h"

class FWHORecHit3DProxyBuilder : public FW3DSimpleProxyBuilderTemplate<HORecHit>
{
public:
   FWHORecHit3DProxyBuilder(void) 
     : m_maxEnergy(5.0)
    {}
  
   virtual ~FWHORecHit3DProxyBuilder(void) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   void build(const HORecHit& iData, unsigned int iIndex, TEveElement& oItemHolder) const;

   Float_t m_maxEnergy;

   // Disable default copy constructor
   FWHORecHit3DProxyBuilder(const FWHORecHit3DProxyBuilder&);
   // Disable default assignment operator
   const FWHORecHit3DProxyBuilder& operator=(const FWHORecHit3DProxyBuilder&);
};

void
FWHORecHit3DProxyBuilder::build(const HORecHit& iData, unsigned int iIndex, TEveElement& oItemHolder) const
{
   std::vector<TEveVector> corners = item()->getGeom()->getPoints(iData.detid().rawId());
   if( corners.empty() ) {
      return;
   }

   TEveVector centre = corners[0] + corners[1] + corners[2] + corners[3] + corners[4] + corners[5] + corners[6] + corners[7];
   centre.Set(centre.fX / 8.0, centre.fY / 8.0, centre.fZ / 8.0);

   Float_t maxEnergy = m_maxEnergy;
   Float_t energy = iData.energy();
   if(energy > maxEnergy)
   {
     maxEnergy = energy;
   }
   
   Float_t scaleFraction = energy / maxEnergy;
   
   // Coordinates for a scaled version of the original box
   for(size_t i = 0; i < 8; ++i)
      corners[i] = centre + (corners[i]-centre)*scaleFraction;
     
   TEveStraightLineSet* rechitSet = new TEveStraightLineSet("HO Rec Hit");
   rechitSet->SetLineWidth(3);
   rechitSet->SetMainColor(item()->defaultDisplayProperties().color());
   rechitSet->SetRnrSelf(item()->defaultDisplayProperties().isVisible());
   rechitSet->SetRnrChildren(item()->defaultDisplayProperties().isVisible());

   for(int j = 0; j < 3; ++j)
   {
      rechitSet->AddLine(corners[j].fX,   corners[j].fY,   corners[j].fZ,
			 corners[j+1].fX, corners[j+1].fY, corners[j+1].fZ);
      rechitSet->AddLine(corners[j+4].fX, corners[j+4].fY, corners[j+4].fZ,
			 corners[j+5].fX, corners[j+5].fY, corners[j+5].fZ);
      rechitSet->AddLine(corners[j].fX,   corners[j].fY,   corners[j].fZ,
			 corners[j+4].fX, corners[j+4].fY, corners[j+4].fZ);
   }
   rechitSet->AddLine(corners[3].fX, corners[3].fY, corners[3].fZ,
		      corners[7].fX, corners[7].fY, corners[7].fZ);
   
   rechitSet->AddLine(corners[3].fX,  corners[3].fY, corners[3].fZ,
		      corners[0].fX,  corners[0].fY, corners[0].fZ);

   rechitSet->AddLine(corners[7].fX,  corners[7].fY, corners[7].fZ,
		      corners[4].fX,  corners[4].fY, corners[4].fZ);

   oItemHolder.AddElement(rechitSet);
}

REGISTER_FW3DDATAPROXYBUILDER(FWHORecHit3DProxyBuilder, HORecHit, "HO RecHit");
