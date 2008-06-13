#ifndef Fireworks_Calo_L1EmTrigProxyRhoPhiZ2DBuilder_h
#define Fireworks_Calo_L1EmTrigProxyRhoPhiZ2DBuilder_h
// -*- C++ -*-
//
// Package:     Calo
// Class  :     L1EmTrigProxyRhoPhiZ2DBuilder
// 
/**\class L1EmTrigProxyRhoPhiZ2DBuilder L1EmTrigProxyRhoPhiZ2DBuilder.h Fireworks/Calo/interface/L1EmTrigProxyRhoPhiZ2DBuilder.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  
//         Created:  Sun Jan  6 23:42:33 EST 2008
// $Id: L1EmTrigProxyRhoPhiZ2DBuilder.h,v 1.2 2008/06/09 19:54:03 chrjones Exp $
//

// system include files

// user include files
#include "Fireworks/Core/interface/FWRPZ2DDataProxyBuilder.h"


// forward declarations

class TEveGeoShapeExtract;
class L1EmTrigProxyRhoPhiZ2DBuilder : public FWRPZ2DDataProxyBuilder
{

   public:
      L1EmTrigProxyRhoPhiZ2DBuilder();
      virtual ~L1EmTrigProxyRhoPhiZ2DBuilder();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------
      REGISTER_PROXYBUILDER_METHODS();
   
   private:
      virtual void buildRhoPhi(const FWEventItem* iItem,
                               TEveElementList** product);
   
      virtual void buildRhoZ(const FWEventItem* iItem, 
                               TEveElementList** product);

      double getTheta( double eta ) { return 2*atan(exp(-eta)); }
   
      L1EmTrigProxyRhoPhiZ2DBuilder(const L1EmTrigProxyRhoPhiZ2DBuilder&); // stop default

      const L1EmTrigProxyRhoPhiZ2DBuilder& operator=(const L1EmTrigProxyRhoPhiZ2DBuilder&); // stop default

      // ---------- member data --------------------------------
};


#endif
