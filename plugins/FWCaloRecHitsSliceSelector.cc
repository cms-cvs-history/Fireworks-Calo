// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWCaloTowerSliceSelector
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Wed Jun  2 17:36:23 CEST 2010
// $Id: FWEcalRecHitSliceSelector.cc,v 1.3 2010/12/01 21:40:31 amraktad Exp $
//

// system include files

// user include files
#include "TH2F.h"
#include "TMath.h"

#include "Fireworks/Core/interface/FWModelChangeManager.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Calo/plugins/FWCaloRecHitsSliceSelector.h"
#include "Fireworks/Core/interface/FWGeometry.h"
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"

bool
FWCaloRecHitsSliceSelector::matchCell(const TEveCaloData::CellId_t& iCell, const CaloRecHit* hit) const
{
   using namespace TMath;

   const static float upPhiLimit = Pi() -10*DegToRad() -1e-5;

   // int nBinsX = m_hist->GetXaxis()->GetNbins() + 2;
   
   const FWGeometry *geom = m_item->getGeom();
   const float* corners = geom->getCorners(hit->detid().rawId() );
   
   std::vector<TEveVector> front( 4 );
   float eta[4], phi[4];
   bool plusSignPhi  = false;
   bool minusSignPhi = false;
   int j = 0;
   for( int i = 0; i < 4; ++i )
   {	 
      front[i] = TEveVector( corners[j], corners[j + 1], corners[j + 2] );
      j += 3;
 
      eta[i] = front[i].Eta();
      phi[i] = front[i].Phi();

      // make sure sign around Pi is same as sign of fY
      phi[i] = Sign( phi[i], front[i].fY );
      ( phi[i] >= 0 ) ? plusSignPhi = true :  minusSignPhi = true;
   }

   // check for cell around phi and move up edge to negative side
   if( plusSignPhi && minusSignPhi ) 
   {
      for( int i = 0; i < 4; ++i )
      {
         if( phi[i] >= upPhiLimit ) 
         {
            //  printf("over phi max limit %f \n", phi[i]);
            phi[i] -= TwoPi();
         }
      }
   }
  
   float etaM = -10;
   float etam =  10;   float phiM = -4;
   float phim =  4;
   for( int i = 0; i < 4; ++i )
   { 
      etam = Min( etam, eta[i] );
      etaM = Max( etaM, eta[i] );
      phim = Min( phim, phi[i] );
      phiM = Max( phiM, phi[i] );
   }
   float ceta = (etam+etaM)*0.5;
   float cphi = (phim+phiM)*0.5;
   //==============================================================================

   bool match = false;
   int idx = m_hist->FindBin(ceta, cphi);
   int nBinsX = m_hist->GetXaxis()->GetNbins() + 2;

   int etaBin, phiBin, w, newPhiBin;
   m_hist->GetBinXYZ(idx, etaBin, phiBin, w);

   if (Abs(ceta) > 4.716 )
   {
      newPhiBin =  ((phiBin + 1) / 4) * 4 - 1;
      if (newPhiBin <= 0) newPhiBin = 71;

      idx = etaBin + newPhiBin*nBinsX;
      match |= (idx == iCell.fTower);

      idx += nBinsX;
      match |= (idx == iCell.fTower);

      idx += nBinsX;
      if (newPhiBin == 71)
         idx = etaBin + 1*nBinsX;
      match |= (idx == iCell.fTower);

      idx += nBinsX;
      match |= (idx == iCell.fTower);
   } 
   else if (abs(ceta) > 1.740)
   {
      newPhiBin =  ((phiBin  + 1) / 2) * 2 -1;
      idx = etaBin + newPhiBin*nBinsX;
      match = ( idx == iCell.fTower ||  idx + nBinsX == iCell.fTower);
   }
   else
   {
      match = ( idx == iCell.fTower);
   }
   return match;
}

void
FWCaloRecHitsSliceSelector::doSelect(const TEveCaloData::CellId_t& iCell)
{
   //  printf("doSelect BEGIN --------------------------------------------\n");
   if (!m_item) return;

   FWChangeSentry sentry(*(m_item->changeManager()));

   size_t size = m_item->size();
   for (int index = 0; index < static_cast<int>(size); ++index)
   {
      if (m_item->modelInfo(index).m_displayProperties.isVisible() && !m_item->modelInfo(index).isSelected())
      {
         const void* modelData = m_item->modelData(index);
         if (matchCell(iCell, static_cast<const CaloRecHit*>(modelData)))
         {
            m_item->select(index);
            //             pintf("FWCaloRecHitsSliceSelector::doSelect entry %d value %f \n", index, 0.);
         }
      }
   }
   //   printf("doSelect END ---------------------------------------------\n");
}

void
FWCaloRecHitsSliceSelector::doUnselect(const TEveCaloData::CellId_t& iCell)
{
   if (!m_item) return;
  
   //  std::cout <<"  doUnselect "<<std::endl;

   FWChangeSentry sentry(*(m_item->changeManager()));  
   size_t size = m_item->size();
   for (int index = 0; index < static_cast<int>(size); ++index)
   {
      if ( m_item->modelInfo(index).m_displayProperties.isVisible() &&
           m_item->modelInfo(index).isSelected()) {
         const void* modelData = m_item->modelData(index);
         if (matchCell(iCell, static_cast<const CaloRecHit*>(modelData)))
         {
            //  std::cout <<"  doUnselect "<<index<<std::endl;
            m_item->unselect(index);
         }
      }
   }
}


