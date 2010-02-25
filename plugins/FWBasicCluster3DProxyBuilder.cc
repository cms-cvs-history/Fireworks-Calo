#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Calo/interface/CaloUtils.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "TEveCompound.h"

class FWBasicCluster3DProxyBuilder : public FW3DSimpleProxyBuilderTemplate<reco::BasicCluster>
{
public:
   FWBasicCluster3DProxyBuilder(void) 
    {}
  
   virtual ~FWBasicCluster3DProxyBuilder(void) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   // Disable default copy constructor
   FWBasicCluster3DProxyBuilder(const FWBasicCluster3DProxyBuilder&);
   // Disable default assignment operator
   const FWBasicCluster3DProxyBuilder& operator=(const FWBasicCluster3DProxyBuilder&);

   void build(const reco::BasicCluster& iData, unsigned int iIndex, TEveElement& oItemHolder) const;
};

void
FWBasicCluster3DProxyBuilder::build(const reco::BasicCluster& iData, unsigned int iIndex, TEveElement& oItemHolder) const
{
   std::vector<std::pair<DetId, float> > clusterDetIds = iData.hitsAndFractions ();
   for(std::vector<std::pair<DetId, float> >::iterator id = clusterDetIds.begin (), idend = clusterDetIds.end ();
       id != idend; ++id)
   {
      std::vector<TEveVector> corners = item()->getGeom()->getPoints((*id).first);
      if( corners.empty() ) {
	 continue;
      }
      Float_t scale = 10.0; 	// FIXME: The scale should be taken form somewhere else
      Float_t energy = (*id).second;
      Float_t eScale = scale * energy;

      fireworks::drawEcalHit3D(corners, item(), oItemHolder, eScale);
   }
}

REGISTER_FW3DDATAPROXYBUILDER(FWBasicCluster3DProxyBuilder, reco::BasicCluster, "Basic Cluster");
