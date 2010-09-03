#include "Fireworks/Core/interface/FWProxyBuilderBase.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Calo/interface/CaloUtils.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "TEveCompound.h"

class FWHBHERecHitEtProxyBuilder : public FWProxyBuilderBase
{
public:
   FWHBHERecHitEtProxyBuilder( void )
     : m_maxEnergy( 0.85 )
    {}
  
   virtual ~FWHBHERecHitEtProxyBuilder( void ) 
    {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   virtual void build( const FWEventItem* iItem, TEveElementList* product, const FWViewContext* );

   Float_t m_maxEnergy;

   // Disable default copy constructor
   FWHBHERecHitEtProxyBuilder( const FWHBHERecHitEtProxyBuilder& );
   // Disable default assignment operator
   const FWHBHERecHitEtProxyBuilder& operator=( const FWHBHERecHitEtProxyBuilder& );
};

void
FWHBHERecHitEtProxyBuilder::build( const FWEventItem* iItem, TEveElementList* product, const FWViewContext* )
{
   const HBHERecHitCollection* collection = 0;
   iItem->get( collection );

   if( 0 == collection )
   {
      return;
   }
   std::vector<HBHERecHit>::const_iterator it = collection->begin();
   std::vector<HBHERecHit>::const_iterator itEnd = collection->end();
   for( ; it != itEnd; ++it )
   {
      if(( *it ).energy() > m_maxEnergy )
         m_maxEnergy = ( *it ).energy();
   }
   
   unsigned int index = 0;
   for( it = collection->begin(); it != itEnd; ++it, ++index )
   {
      const float* corners = iItem->getGeom()->getCorners(( *it ).detid());
      if( corners == 0 )
      {
	 TEveCompound* compound = createCompound();
	 setupAddElement( compound, product );

	 continue;
      }

      Float_t energy = (*it).energy();
      fireworks::drawEtScaledBox3D( corners, energy, m_maxEnergy, product, this, true );
   }
}

REGISTER_FWPROXYBUILDER( FWHBHERecHitEtProxyBuilder, HBHERecHitCollection, "HBHE RecHit Et", FWViewType::kISpyBit );