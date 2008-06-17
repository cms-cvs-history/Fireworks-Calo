// -*- C++ -*-
// $Id$
//

// system include files
#include "TEveManager.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "RVersion.h"
#include "TEveCalo.h"
#include "TEveCaloData.h"
#include "TH2F.h"

// user include files
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWRPZDataProxyBuilder.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

#include "Fireworks/Calo/interface/ECalCaloTowerProxy3DBuilder.h"
#include "Fireworks/Core/interface/FW3DLegoDataProxyBuilder.h"

void ECalCaloTowerProxy3DBuilder::build(const FWEventItem* iItem, TEveElementList** product)
{
   if( *product == 0) *product = new TEveElementList();
   TH2F* hist = 0;
   TEveCaloDataHist* data = 0;
   std::string name = "ecal3D";
   if ( m_calo3d ) data = dynamic_cast<TEveCaloDataHist*>( m_calo3d->GetData() );
   if ( data ) {
      for ( Int_t i = 0; i < data->GetNSlices(); ++i ){
	 TH2F* h = data->RefSliceInfo(i).fHist;
	 if ( ! h ) continue;
	 if ( name == h->GetName() ) {
	    hist = h;
	    break;
	 }
      }
   }
   const CaloTowerCollection* towers=0;
   iItem->get(towers);
   if(0==towers) {
      std::cout <<"Failed to get CaloTowers"<<std::endl;
      return;
   }
   bool newHist = false;
   if ( hist == 0 ) {
      hist = new TH2F(name.c_str(),"CaloTower ECAL Et distribution", 82, fw3dlego::xbins, 72, -M_PI, M_PI);
      newHist = true;
   }
   hist->Reset();
   for(CaloTowerCollection::const_iterator tower = towers->begin(); tower != towers->end(); ++tower)
     (hist)->Fill(tower->eta(), tower->phi(), tower->emEt());
   
   if ( ! data ) data = new TEveCaloDataHist();
   if ( newHist ) {
      Int_t s = data->AddHistogram(hist);
      data->RefSliceInfo(s).Setup("ECAL", 0., iItem->defaultDisplayProperties().color());
   }
   
   if ( m_calo3d == 0 ) {
      m_calo3d = new TEveCalo3D(data);
      m_calo3d->SetBarrelRadius(129);
      m_calo3d->SetEndCapPos(310);
      //	 m_calo3d->IncDenyDestroy();
      // gEve->AddElement(m_calo3d);
      //	(*product)->AddElement(m_calo3d);
      gEve->AddElement(m_calo3d, *product);
   }
}

REGISTER_FWRPZDATAPROXYBUILDER(ECalCaloTowerProxy3DBuilder,CaloTowerCollection,"ECal");