#include "Fireworks/Core/interface/register_dataproxybuilder_macro.h"
#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Calo/interface/CaloUtils.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"
#include "TEveCompound.h"

class FWPreshowerCluster3DProxyBuilder : public FW3DSimpleProxyBuilderTemplate<reco::PreshowerCluster>
{
public:
   FWPreshowerCluster3DProxyBuilder(void) 
    {}
  
   virtual ~FWPreshowerCluster3DProxyBuilder(void) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWPreshowerCluster3DProxyBuilder(const FWPreshowerCluster3DProxyBuilder&); 			// stop default
   const FWPreshowerCluster3DProxyBuilder& operator=(const FWPreshowerCluster3DProxyBuilder&); 	// stop default

   void build(const reco::PreshowerCluster& iData, unsigned int iIndex, TEveElement& oItemHolder) const;
};

void
FWPreshowerCluster3DProxyBuilder::build(const reco::PreshowerCluster& iData, unsigned int iIndex, TEveElement& oItemHolder) const
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

REGISTER_FW3DDATAPROXYBUILDER(FWPreshowerCluster3DProxyBuilder, reco::PreshowerCluster, "Preshower Cluster");
