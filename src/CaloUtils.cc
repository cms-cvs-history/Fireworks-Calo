#include "TEveBoxSet.h"
#include "TEveCompound.h"
#include "TEveStraightLineSet.h"
#include "Fireworks/Core/interface/FWEventItem.h"

namespace fireworks {

   void drawCaloHit3D(std::vector<TEveVector> &corners, const FWEventItem* iItem, class TEveElement &oItemHolder, Float_t scaleFraction)
   {
      TEveVector centre = corners[0] + corners[1] + corners[2] + corners[3] + corners[4] + corners[5] + corners[6] + corners[7];
      centre.Set(centre.fX / 8.0, centre.fY / 8.0, centre.fZ / 8.0);

      // Coordinates for a scaled version of the original box
      for(size_t i = 0; i < 8; ++i)
	corners[i] = centre + (corners[i]-centre)*scaleFraction;
      
      const Float_t box[8*3] = {corners[3].fX,  corners[3].fY, corners[3].fZ, 	 
				corners[2].fX,  corners[2].fY, corners[2].fZ, 	 
				corners[1].fX,  corners[1].fY, corners[1].fZ, 	 
				corners[0].fX,  corners[0].fY, corners[0].fZ, 	 
				corners[7].fX,  corners[7].fY, corners[7].fZ, 	 
				corners[6].fX,  corners[6].fY, corners[6].fZ, 	 
				corners[5].fX,  corners[5].fY, corners[5].fZ, 	 
				corners[4].fX,  corners[4].fY, corners[4].fZ}; 	 

      // FIXME: We do not need to make a box set per hit
      // but rather one box set per collection...
      TEveBoxSet* recHit = new TEveBoxSet("Rec Hit"); 	 
      recHit->Reset(TEveBoxSet::kBT_FreeBox, kTRUE, 64); 	 
      recHit->AddBox(box);
      recHit->DigitColor(iItem->defaultDisplayProperties().color());
	    
      TEveStraightLineSet* rechitSet = new TEveStraightLineSet("Rec Hit");
      rechitSet->SetLineWidth(1);
      rechitSet->SetMainColor(iItem->defaultDisplayProperties().color());
      rechitSet->SetRnrSelf(iItem->defaultDisplayProperties().isVisible());
      rechitSet->SetRnrChildren(iItem->defaultDisplayProperties().isVisible());

      for(int j = 0; j < 3; ++j)
      {
	 rechitSet->AddLine(corners[j].fX,   corners[j].fY,   corners[j].fZ,
			    corners[j+1].fX, corners[j+1].fY, corners[j+1].fZ);
	 rechitSet->AddLine(corners[j+4].fX, corners[j+4].fY, corners[j+4].fZ,
			    corners[j+5].fX, corners[j+5].fY, corners[j+5].fZ);
	 rechitSet->AddLine(corners[j].fX,   corners[j].fY,   corners[j].fZ,
			    corners[j+4].fX, corners[j+4].fY, corners[j+4].fZ);
      }
      rechitSet->AddLine(corners[3].fX, corners[3].fY, corners[3].fZ,
			 corners[7].fX, corners[7].fY, corners[7].fZ);
   
      rechitSet->AddLine(corners[3].fX,  corners[3].fY, corners[3].fZ,
			 corners[0].fX,  corners[0].fY, corners[0].fZ);

      rechitSet->AddLine(corners[7].fX,  corners[7].fY, corners[7].fZ,
			 corners[4].fX,  corners[4].fY, corners[4].fZ);

      rechitSet->AddElement(recHit);
      oItemHolder.AddElement(rechitSet);
   }
}
