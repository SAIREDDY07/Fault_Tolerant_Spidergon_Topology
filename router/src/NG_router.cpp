#include "NG_router.h"
#include "../../config/extern.h"

void NG_router::setID(UI id_tile)
{
	id=id_tile;
	initialize();
}

UI NG_router::calc_next(UI ip_dir,ULL src_id,ULL dest_id,struct congestion cong_status)
{
		int xco=id/num_cols;
		int yco=id%num_cols;
		int dest_xco=dest_id/num_cols;
		int dest_yco=dest_id%num_cols;
	        static int count;
                if(count==0){ cout<<"algorithm selected is Negative First ";  count++; }
		if(dest_xco<xco)
		{
			if(dest_yco<yco)
			{
				if(cong_status.DNorth>cong_status.DWest)
					return W;
				else
					return N;
			}
			else
				return N;
		}
		else if(dest_xco>xco)
		{
			if(dest_yco<yco)
			{
				return W;
                        }
			else if(dest_yco>yco)
			{
				if(cong_status.DEast<cong_status.DSouth)
					return E;
				else
					return S;
			}
                        else if(dest_yco==yco)
                        {
				return S;
			}        
		}
		else if(dest_xco==xco)
		{
			if(dest_yco<yco)
				return W;
			else if(dest_yco>yco)
			{
				return E;
			}
			else if(dest_yco==yco)
                              return C;
		}
}
			



void NG_router::initialize() {
 
}

extern "C" {
router *maker(){
		return new NG_router;
}
}
