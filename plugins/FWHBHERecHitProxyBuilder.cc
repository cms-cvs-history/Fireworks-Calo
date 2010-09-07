#include "Fireworks/Core/interface/FWProxyBuilderBase.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Calo/interface/CaloUtils.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "TEveCompound.h"

class FWHBHERecHitProxyBuilder : public FWProxyBuilderBase
{
public:
   FWHBHERecHitProxyBuilder( void )
     : m_maxEnergy( 0.85 )
    {}
  
   virtual ~FWHBHERecHitProxyBuilder( void ) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   virtual void build( const FWEventItem* iItem, TEveElementList* product, const FWViewContext* );

   Float_t m_maxEnergy;

   // Disable default copy constructor
   FWHBHERecHitProxyBuilder( const FWHBHERecHitProxyBuilder& );
   // Disable default assignment operator
   const FWHBHERecHitProxyBuilder& operator=( const FWHBHERecHitProxyBuilder& );
};

void
FWHBHERecHitProxyBuilder::build( const FWEventItem* iItem, TEveElementList* product, const FWViewContext* )
{
   const HBHERecHitCollection* collection = 0;
   iItem->get( collection );

   if( 0 == collection )
   {
      return;
   }
   std::vector<HBHERecHit>::const_iterator it = collection->begin();
   std::vector<HBHERecHit>::const_iterator itEnd = collection->end();

   const FWGeometry *geom = iItem->getGeom();

   for( ; it != itEnd; ++it )
   {
      if(( *it ).energy() > m_maxEnergy )
         m_maxEnergy = ( *it ).energy();
   }
   
   for( it = collection->begin(); it != itEnd; ++it )
   {
      const float* corners = geom->getCorners(( *it ).detid());
      if( corners == 0 )
      {
	 TEveCompound* compound = createCompound();
	 setupAddElement( compound, product );

	 continue;
      }

      fireworks::drawEnergyScaledBox3D( corners, ( *it ).energy() / m_maxEnergy, product, this, true );
   }
}

REGISTER_FWPROXYBUILDER( FWHBHERecHitProxyBuilder, HBHERecHitCollection, "HBHE RecHit", FWViewType::kISpyBit );
