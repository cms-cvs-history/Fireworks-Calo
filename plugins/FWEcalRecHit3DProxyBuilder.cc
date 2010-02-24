#include "Fireworks/Core/interface/register_dataproxybuilder_macro.h"
#include "Fireworks/Core/interface/FW3DSimpleProxyBuilderTemplate.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "TEveBoxSet.h"
#include "TEveCompound.h"
#include "TEveStraightLineSet.h"

class FWEcalRecHit3DProxyBuilder : public FW3DSimpleProxyBuilderTemplate<EcalRecHit>
{
public:
   FWEcalRecHit3DProxyBuilder(void) 
    {}
  
   virtual ~FWEcalRecHit3DProxyBuilder(void) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWEcalRecHit3DProxyBuilder(const FWEcalRecHit3DProxyBuilder&); 			// stop default
   const FWEcalRecHit3DProxyBuilder& operator=(const FWEcalRecHit3DProxyBuilder&); 	// stop default

   void build(const EcalRecHit& iData, unsigned int iIndex, TEveElement& oItemHolder) const;
};

void
FWEcalRecHit3DProxyBuilder::build(const EcalRecHit& iData, unsigned int iIndex, TEveElement& oItemHolder) const
{
   // FIXME: This is what I need, but it's not there in the geometry file.
   //
   //      std::vector<TEveVector> corners = item()->getGeom()->getPoints(iData.id());
   //       if( corners.empty() ) {
   //          std::cout << "ERROR: failed get geometry of ECAL rechit with det id: " <<
   //          iData.id() << std::endl;
   //          return;
   //       }
   const TGeoHMatrix* matrix = item()->getGeom()->getMatrix(iData.id());
   if ( !matrix ) {
     std::cout << "ERROR: failed get geometry of ECAL rechit with det id: " <<
       iData.detid().subdetId() << std::endl;
     return;
   }	 

   Double_t localFront1Point[3] = { 0.85,  0.85, 0.0};
   Double_t localFront2Point[3] = {-0.85,  0.85, 0.0};
   Double_t localFront3Point[3] = {-0.85, -0.85, 0.0};
   Double_t localFront4Point[3] = { 0.85, -0.85, 0.0};
   Double_t localBack1Point[3] = { 0.87,  0.87, 1.0};
   Double_t localBack2Point[3] = {-0.87,  0.87, 1.0};
   Double_t localBack3Point[3] = {-0.87, -0.87, 1.0};
   Double_t localBack4Point[3] = { 0.87, -0.87, 1.0};

   Double_t globalFront1Point[3];
   Double_t globalFront2Point[3];
   Double_t globalFront3Point[3];
   Double_t globalFront4Point[3];
   Double_t globalBack1Point[3];
   Double_t globalBack2Point[3];
   Double_t globalBack3Point[3];
   Double_t globalBack4Point[3];

   TEveStraightLineSet* rechitSet = new TEveStraightLineSet("Ecal Rec Hit");
   rechitSet->SetLineWidth(1);
   rechitSet->SetMainColor(item()->defaultDisplayProperties().color());
   rechitSet->SetRnrSelf(item()->defaultDisplayProperties().isVisible());
   rechitSet->SetRnrChildren(item()->defaultDisplayProperties().isVisible());

   Float_t scale = 10.0; 	// FIXME: The scale should be taken form somewhere else
   Float_t energy = iData.energy();
   Float_t eScale = scale * energy;

   if(iData.detid().subdetId() == 1)
   {
     // FIXME: This is Ecal barrel and the rechits transformation
     // is inwards for some reason.
     eScale = -eScale;
   }

   localBack1Point[2] = eScale;
   localBack2Point[2] = eScale;
   localBack3Point[2] = eScale;
   localBack4Point[2] = eScale;

   matrix->LocalToMaster(localFront1Point, globalFront1Point);
   matrix->LocalToMaster(localFront2Point, globalFront2Point);
   matrix->LocalToMaster(localFront3Point, globalFront3Point);
   matrix->LocalToMaster(localFront4Point, globalFront4Point);
   matrix->LocalToMaster(localBack1Point, globalBack1Point);
   matrix->LocalToMaster(localBack2Point, globalBack2Point);
   matrix->LocalToMaster(localBack3Point, globalBack3Point);
   matrix->LocalToMaster(localBack4Point, globalBack4Point);

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

   const Float_t box[8*3] = {globalFront1Point[0], globalFront1Point[1], globalFront1Point[2], 	 
			     globalFront2Point[0], globalFront2Point[1], globalFront2Point[2],
			     globalFront3Point[0], globalFront3Point[1], globalFront3Point[2],
			     globalFront4Point[0], globalFront4Point[1], globalFront4Point[2],
			     globalBack1Point[0], globalBack1Point[1], globalBack1Point[2], 	 
			     globalBack2Point[0], globalBack2Point[1], globalBack2Point[2],
			     globalBack3Point[0], globalBack3Point[1], globalBack3Point[2],
			     globalBack4Point[0], globalBack4Point[1], globalBack4Point[2]
   };

   // FIXME: We do not need to make a box set per hit
   // but rather one box set per collection...
   TEveBoxSet* recHit = new TEveBoxSet("HBHE Rec Hit"); 	 
   recHit->Reset(TEveBoxSet::kBT_FreeBox, kTRUE, 64); 	 
   recHit->AddBox(box);
   recHit->DigitColor(item()->defaultDisplayProperties().color());

   rechitSet->AddElement(recHit);
   oItemHolder.AddElement(rechitSet);
}

REGISTER_FW3DDATAPROXYBUILDER(FWEcalRecHit3DProxyBuilder, EcalRecHit, "Ecal RecHit");
