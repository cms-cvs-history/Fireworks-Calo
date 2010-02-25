#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Calo/interface/CaloUtils.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "TEveCompound.h"

class FWSuperCluster3DProxyBuilder : public FW3DSimpleProxyBuilderTemplate<reco::SuperCluster>
{
public:
   FWSuperCluster3DProxyBuilder(void) 
    {}
  
   virtual ~FWSuperCluster3DProxyBuilder(void) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   // Disable default copy constructor
   FWSuperCluster3DProxyBuilder(const FWSuperCluster3DProxyBuilder&);
   // Disable default assignment operator
   const FWSuperCluster3DProxyBuilder& operator=(const FWSuperCluster3DProxyBuilder&);

   void build(const reco::SuperCluster& iData, unsigned int iIndex, TEveElement& oItemHolder) const;
};

void
FWSuperCluster3DProxyBuilder::build(const reco::SuperCluster& iData, unsigned int iIndex, TEveElement& oItemHolder) const
{
   std::vector<std::pair<DetId, float> > clusterDetIds = iData.hitsAndFractions ();
   for(std::vector<std::pair<DetId, float> >::iterator id = clusterDetIds.begin (), idend = clusterDetIds.end ();
       id != idend; ++id)
   {
      std::vector<TEveVector> corners = item()->getGeom()->getPoints((*id).first);
      if( corners.empty() ) {
	 continue;
      }
      Float_t scale = 10.0; 		// FIXME: The scale should be taken form somewhere else
      Float_t energy = (*id).second; 	// FIXME: Check it. 
      Float_t eScale = scale * energy;

      fireworks::drawEcalHit3D(corners, item(), oItemHolder, eScale);
   }
}

REGISTER_FW3DDATAPROXYBUILDER(FWSuperCluster3DProxyBuilder, reco::SuperCluster, "Super Cluster");
