#ifndef Fireworks_Calo_HCalCaloTowerProxy3DLegoBuilder_h
#define Fireworks_Calo_HCalCaloTowerProxy3DLegoBuilder_h
// -*- C++ -*-
//
// Package:     Calo
// Class  :     HCalCaloTowerProxy3DLegoBuilder
// 
/**\class HCalCaloTowerProxy3DLegoBuilder HCalCaloTowerProxy3DLegoBuilder.h Fireworks/Calo/interface/HCalCaloTowerProxy3DLegoBuilder.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  
//         Created:  Sun Jan  6 23:42:33 EST 2008
// $Id$
//

// system include files

// user include files
#include "Fireworks/Core/interface/FW3DLegoDataProxyBuilder.h"

// forward declarations

class HCalCaloTowerProxy3DLegoBuilder : public FW3DLegoDataProxyBuilder
{

   public:
      HCalCaloTowerProxy3DLegoBuilder();
      virtual ~HCalCaloTowerProxy3DLegoBuilder();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

   private:
      virtual void build(const FWEventItem* iItem, 
			 TH2F** product);

      HCalCaloTowerProxy3DLegoBuilder(const HCalCaloTowerProxy3DLegoBuilder&); // stop default

      const HCalCaloTowerProxy3DLegoBuilder& operator=(const HCalCaloTowerProxy3DLegoBuilder&); // stop default

      // ---------- member data --------------------------------

};


#endif