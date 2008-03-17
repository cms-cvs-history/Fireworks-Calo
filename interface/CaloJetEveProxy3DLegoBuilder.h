#ifndef Fireworks_Calo_CaloJetEveProxy3DLegoBuilder_h
#define Fireworks_Calo_CaloJetEveProxy3DLegoBuilder_h
// -*- C++ -*-
//
// Package:     Calo
// Class  :     CaloJetEveProxy3DLegoBuilder
// 
/**\class CaloJetEveProxy3DLegoBuilder CaloJetEveProxy3DLegoBuilder.h Fireworks/Calo/interface/CaloJetEveProxy3DLegoBuilder.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  
//         Created:  Sun Jan  6 23:42:33 EST 2008
// $Id: CaloJetEveProxy3DLegoBuilder.h,v 1.3 2008/03/07 09:06:47 dmytro Exp $
//

// system include files

// user include files
#include "Fireworks/Core/interface/FW3DLegoDataProxyBuilder.h"

// forward declarations
class CaloJetEveProxy3DLegoBuilder : public FW3DLegoDataProxyBuilder
{

   public:
      CaloJetEveProxy3DLegoBuilder();
      virtual ~CaloJetEveProxy3DLegoBuilder();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------
   private:
      virtual void build(const FWEventItem* iItem, 
			 TEveElementList** product);

      CaloJetEveProxy3DLegoBuilder(const CaloJetEveProxy3DLegoBuilder&); // stop default

      const CaloJetEveProxy3DLegoBuilder& operator=(const CaloJetEveProxy3DLegoBuilder&); // stop default

      // ---------- member data --------------------------------
};


#endif
