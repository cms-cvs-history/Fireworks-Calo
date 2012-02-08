// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWCaloRecHitProxyBuilderBase
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Wed Dec  3 11:28:28 EST 2008
// $Id: FWEveEcalRecHitProxyBuilder.cc,v 1.25 2011/02/23 11:34:52 amraktad Exp $
//

// system includes
#include <math.h>

// user includes
#include "TEveCaloData.h"
#include "TEveCalo.h"
#include "TH2F.h"

#include "Fireworks/Core/interface/Context.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWModelChangeManager.h"
#include "Fireworks/Core/interface/fwLog.h"

#include "Fireworks/Core/interface/FWGeometry.h"

#include "Fireworks/Core/interface/fw3dlego_xbins.h"
#include "Fireworks/Calo/plugins/FWCaloRecHitsProxyBuilder.h"
#include "Fireworks/Calo/plugins/FWCaloRecHitsSliceSelector.h"


#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"

std::string
FWCaloRecHitProxyBuilder::typeOfBuilder()
{
   return std::string("simple#");
}
//______________________________________________________________________________

void
FWCaloRecHitProxyBuilder::itemBeingDestroyed(const FWEventItem* iItem)
{
  
   if(0!=m_hist) {
      m_hist->Reset();
   }
   FWCaloDataProxyBuilderBase::itemBeingDestroyed(iItem);
}

//______________________________________________________________________________

void
FWCaloRecHitProxyBuilder::setCaloData(const fireworks::Context&)
{
  m_caloData = context().getCaloData();
}

//______________________________________________________________________________
bool
FWCaloRecHitProxyBuilder::assertCaloDataSlice()
{
   if (m_hist == 0)
   {
      // add new slice
      Bool_t status = TH1::AddDirectoryStatus();
      TH1::AddDirectory(kFALSE); //Keeps histogram from going into memory
      m_hist = new TH2F("caloHist",
                        "caloHist",
                        fw3dlego::xbins_n - 1, fw3dlego::xbins,
                        72, -M_PI, M_PI);
      TH1::AddDirectory(status);
      TEveCaloDataHist* ch = static_cast<TEveCaloDataHist*>(m_caloData);
      m_sliceIndex = ch->AddHistogram(m_hist);



      m_caloData->RefSliceInfo(m_sliceIndex).Setup(item()->name().c_str(), 0., 
                                                   item()->defaultDisplayProperties().color(),
                                                   item()->defaultDisplayProperties().transparency());

      // add new selector
      FWFromTEveCaloDataSelector* sel = 0;
      if (m_caloData->GetUserData())
      {
         FWFromEveSelectorBase* base = reinterpret_cast<FWFromEveSelectorBase*>(m_caloData->GetUserData());
         assert(0!=base);
         sel = dynamic_cast<FWFromTEveCaloDataSelector*> (base);
         assert(0!=sel);
      }
      else
      {
         sel = new FWFromTEveCaloDataSelector(m_caloData);
         //make sure it is accessible via the base class
         m_caloData->SetUserData(static_cast<FWFromEveSelectorBase*>(sel));
      }
      printf("add slice selector \n");
      sel->addSliceSelector(m_sliceIndex, new FWCaloRecHitsSliceSelector(m_hist, item()));
     
      return true;
   }
   return false;
}

//______________________________________________________________________________
void
FWCaloRecHitProxyBuilder::fillCaloData()
{
   m_hist->Reset();
   m_cellSelelectedSum.clear();


   size_t size = item()->size();
   for (int index = 0; index < static_cast<int>(size); ++index)
   {
      const void* modelData = item()->modelData(index);
      const CaloRecHit* hit = (const CaloRecHit*)modelData;
      // printf("idx %d ene %f",  index, hit->energy() );
      fillTowerForDetId(hit->detid().rawId(), hit->energy(), item()->modelInfo(index));
   }

   for ( std::map<int, float>::iterator i = m_cellSelelectedSum.begin(); i!= m_cellSelelectedSum.end(); ++i)
      m_caloData->GetCellsSelected().push_back(TEveCaloData::CellId_t(i->first,m_sliceIndex, (i->second)/m_hist->GetBinContent(i->first)));
}

//______________________________________________________________________________

namespace { 
double
wrapPi(double val)
{
   using namespace TMath;

   if (val< -Pi())
   {
      return val += TwoPi();
   }
   if (val> Pi())
   {
      return val -= TwoPi();
   }
   return val;
}
}
void
FWCaloRecHitProxyBuilder::fillTowerForDetId( unsigned int rawid, float val , const FWEventItem::ModelInfo& info )
{
   using namespace TMath;

   if ( info.displayProperties().isVisible() == false) return;

   using namespace TMath;
   const static float upPhiLimit = Pi() -10*DegToRad() -1e-5;

   TEveCaloData::vCellId_t cellIds;
   const FWGeometry *geom = item()->getGeom();
   if( ! geom->contains( rawid ))
   {
      fwLog( fwlog::kInfo ) << "Cannot get geometry for DetId: "<< rawid << ". Ignored.\n";
      return;
   }
     
   const float* corners = geom->getCorners( rawid );
   if( ! corners )
   {
      fwLog( fwlog::kInfo ) << "Cannot get corners for DetId: "<< rawid << ". Ignored.\n";
      return;
   }
   
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
   float etam =  10;
   float phiM = -4;
   float phim =  4;
   for( int i = 0; i < 4; ++i )
   { 
      etam = Min( etam, eta[i] );
      etaM = Max( etaM, eta[i] );
      phim = Min( phim, phi[i] );
      phiM = Max( phiM, phi[i] );
   }
   // check if tower is there
   Float_t ceta = (etam+etaM)*0.5;
   Float_t cphi = (phim+phiM)*0.5;

   // std::cout << "fill \n";
   val *= Abs(Sin(TEveCaloData::EtaToTheta(ceta)));


   static float d4 = 11*TMath::Pi()/180;
   static float d2 = 6*TMath::Pi()/180;
   static float d  = 2.5*TMath::Pi()/180;
   if ((phiM-phim) > d4)
   {
      if (Abs(ceta) < 4.716) printf("item %s eta [%f] with \"4\" \n", item()->name().c_str(), ceta);
      m_hist->Fill(ceta,wrapPi(cphi - 3*d), val *0.25);
      m_hist->Fill(ceta,wrapPi(cphi -   d), val *0.25);
      m_hist->Fill(ceta,wrapPi(cphi +   d), val *0.25);
      m_hist->Fill(ceta,wrapPi(cphi + 3*d), val *0.25);        
   }
   else if ((etaM-etam) > d2)
   {
      if (Abs(ceta) < 1.74) printf("item %s eta [%f] with \"2\" \n", item()->name().c_str(), ceta);
      m_hist->Fill(ceta,wrapPi(cphi -   d), val *0.5);
      m_hist->Fill(ceta,wrapPi(cphi +   d), val *0.5);
   }
   else
   {
      m_hist->Fill(ceta, cphi, val);
   }
  
   if (info.isSelected())
   { 
      if ((phiM-phim) > d4)
      { 
         m_cellSelelectedSum[m_hist->FindBin(ceta,wrapPi(cphi - 3*d))] += val*0.25;
         m_cellSelelectedSum[m_hist->FindBin(ceta,wrapPi(cphi - d  ))] += val*0.25;
         m_cellSelelectedSum[m_hist->FindBin(ceta,wrapPi(cphi + d  ))] += val*0.25;
         m_cellSelelectedSum[m_hist->FindBin(ceta,wrapPi(cphi + 3*d))] += val*0.25;
      }
      else if ((etaM-etam) > d2)
      {
         m_cellSelelectedSum[m_hist->FindBin(ceta,wrapPi(cphi - d  ))] += val*0.5;
         m_cellSelelectedSum[m_hist->FindBin(ceta,wrapPi(cphi + d  ))] += val*0.5;
      }
      else
      {
         m_cellSelelectedSum[m_hist->FindBin(ceta, cphi)] += val;
      }
   }
}


// !!!! AMT could be a single proxy builder FWCaloRecHitProxyBuilder for ANY CaloRecHitCollection
// this clashes/confusion are here becuse of clashes with existing PB purposes
//______________________________________________________________________________

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
class FWEveEcalRecHitProxyBuilder: public FWCaloRecHitProxyBuilder
{
public:
  REGISTER_PROXYBUILDER_METHODS();
};
REGISTER_FWPROXYBUILDER(FWEveEcalRecHitProxyBuilder, EcalRecHit ,"Ecal RecHit",FWViewType::k3DBit|FWViewType::kRhoPhi | FWViewType::kRhoZ | FWViewType::kLego);

//______________________________________________________________________________


#include "DataFormats/HcalRecHit/interface/HORecHit.h"
class FWEveHORecHitProxyBuilder: public FWCaloRecHitProxyBuilder
{
public:
  REGISTER_PROXYBUILDER_METHODS();
};
REGISTER_FWPROXYBUILDER(FWEveHORecHitProxyBuilder, HORecHit ,"HO RecHit",FWViewType::k3DBit|FWViewType::kRhoPhi | FWViewType::kRhoZ | FWViewType::kLego);


//______________________________________________________________________________


#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
class FWEveHBHERecHitProxyBuilder: public FWCaloRecHitProxyBuilder
{
public:
  REGISTER_PROXYBUILDER_METHODS();
};
REGISTER_FWPROXYBUILDER(FWEveHBHERecHitProxyBuilder, HBHERecHit ,"HBHE RecHit",FWViewType::k3DBit|FWViewType::kRhoPhi | FWViewType::kRhoZ | FWViewType::kLego);



//______________________________________________________________________________


#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
class FWEveHFRecHitProxyBuilder: public FWCaloRecHitProxyBuilder
{
public:
  REGISTER_PROXYBUILDER_METHODS();
};
REGISTER_FWPROXYBUILDER(FWEveHFRecHitProxyBuilder, HFRecHit ,"HF RecHit",FWViewType::k3DBit|FWViewType::kRhoPhi | FWViewType::kRhoZ | FWViewType::kLego);


