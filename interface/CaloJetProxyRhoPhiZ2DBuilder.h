#ifndef Fireworks_Calo_CaloJetProxyRhoPhiZ2DBuilder_h
#define Fireworks_Calo_CaloJetProxyRhoPhiZ2DBuilder_h
// -*- C++ -*-
//
// Package:     Calo
// Class  :     CaloJetProxyRhoPhiZ2DBuilder
//
/**\class CaloJetProxyRhoPhiZ2DBuilder CaloJetProxyRhoPhiZ2DBuilder.h Fireworks/Calo/interface/CaloJetProxyRhoPhiZ2DBuilder.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:
//         Created:  Sun Jan  6 23:42:33 EST 2008
// $Id: CaloJetProxyRhoPhiZ2DBuilder.h,v 1.5 2008/09/26 07:40:12 dmytro Exp $
//

// system include files

// user include files
#include "Fireworks/Core/interface/FWRPZ2DDataProxyBuilder.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

// forward declarations

class TEveGeoShapeExtract;
namespace fw {
   class NamedCounter;
}
class CaloJetProxyRhoPhiZ2DBuilder : public FWRPZ2DDataProxyBuilder
{

   public:
      CaloJetProxyRhoPhiZ2DBuilder();
      virtual ~CaloJetProxyRhoPhiZ2DBuilder();

      // ---------- const member functions ---------------------
      REGISTER_PROXYBUILDER_METHODS();

      // ---------- static member functions --------------------
      static std::pair<int,int>        getiEtaRange( const reco::Jet& jet );
      static std::pair<double,double>  getPhiRange( const reco::Jet& jet );
      static double getTheta( double eta ) { return 2*atan(exp(-eta)); }

      static void buildJetRhoPhi(const FWEventItem* iItem,
				 const reco::Jet* jet,
				 TEveElementList* tList,
				 const fw::NamedCounter& counter);

      static void buildJetRhoZ(  const FWEventItem* iItem,
				 const reco::Jet* jet,
				 TEveElementList* tList,
				 const fw::NamedCounter& counter);

      // ---------- member functions ---------------------------

   private:
      virtual void buildRhoPhi(const FWEventItem* iItem,
                               TEveElementList** product);

      virtual void buildRhoZ(const FWEventItem* iItem,
                               TEveElementList** product);

      CaloJetProxyRhoPhiZ2DBuilder(const CaloJetProxyRhoPhiZ2DBuilder&); // stop default

      const CaloJetProxyRhoPhiZ2DBuilder& operator=(const CaloJetProxyRhoPhiZ2DBuilder&); // stop default

      // ---------- member data --------------------------------
};


#endif
