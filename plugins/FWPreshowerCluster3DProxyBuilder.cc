#include "Fireworks/Core/interface/register_dataproxybuilder_macro.h"
#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"
#include "TEveCompound.h"
#include "TEveStraightLineSet.h"

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
   Double_t localFront1Point[3] = { 0.85,  0.85, 0.0};
   Double_t localFront2Point[3] = {-0.85,  0.85, 0.0};
   Double_t localFront3Point[3] = {-0.85, -0.85, 0.0};
   Double_t localFront4Point[3] = { 0.85, -0.85, 0.0};

   Double_t globalFront1Point[3];
   Double_t globalFront2Point[3];
   Double_t globalFront3Point[3];
   Double_t globalFront4Point[3];

   TEveStraightLineSet* rechitSet = new TEveStraightLineSet("Preshower Cluster");
   rechitSet->SetLineWidth(3);
   rechitSet->SetMainColor(item()->defaultDisplayProperties().color());
   rechitSet->SetRnrSelf(item()->defaultDisplayProperties().isVisible());
   rechitSet->SetRnrChildren(item()->defaultDisplayProperties().isVisible());

   std::vector<std::pair<DetId, float> > clusterDetIds = iData.hitsAndFractions ();
   for(std::vector<std::pair<DetId, float> >::iterator id = clusterDetIds.begin (), idend = clusterDetIds.end ();
       id != idend; ++id)
   {
     const TGeoHMatrix* matrix = item()->getGeom()->getMatrix((*id).first);
     if ( !matrix ) {
        std::cout << "ERROR: failed get geometry of ECAL rechit with det id: " <<
	  (*id).first << std::endl;
	continue;
     }	 

     matrix->LocalToMaster(localFront1Point, globalFront1Point);
     matrix->LocalToMaster(localFront2Point, globalFront2Point);
     matrix->LocalToMaster(localFront3Point, globalFront3Point);
     matrix->LocalToMaster(localFront4Point, globalFront4Point);

     rechitSet->AddLine(globalFront1Point[0], globalFront1Point[1], globalFront1Point[2],
			globalFront2Point[0], globalFront2Point[1], globalFront2Point[2]);
     rechitSet->AddLine(globalFront2Point[0], globalFront2Point[1], globalFront2Point[2],
			globalFront3Point[0], globalFront3Point[1], globalFront3Point[2]);
     rechitSet->AddLine(globalFront3Point[0], globalFront3Point[1], globalFront3Point[2],
			globalFront4Point[0], globalFront4Point[1], globalFront4Point[2]);
     rechitSet->AddLine(globalFront4Point[0], globalFront4Point[1], globalFront4Point[2],
			globalFront1Point[0], globalFront1Point[1], globalFront1Point[2]);
   }
   oItemHolder.AddElement(rechitSet);
}

REGISTER_FW3DDATAPROXYBUILDER(FWPreshowerCluster3DProxyBuilder, reco::PreshowerCluster, "Preshower Cluster");
