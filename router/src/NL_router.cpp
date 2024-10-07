
#include "NL_router.h"
#include "../../config/extern.h"

void NL_router::setID(UI id_tile){
       id=id_tile;
       initialize();
}

UI NL_router::calc_next(UI ip_dir,ULL src_id,ULL dest_id,struct congestion cong_status)
{
    int xco=id/num_cols;
    int yco=id%num_cols;
    int dest_xco=dest_id/num_cols;
    int dest_yco=dest_id%num_cols;
    static int count=0;
    if(count==0){       printf("\n Algorithm selected is NL  "); system("date"); count++; }   
  
    if(dest_xco>xco)
    {
             if(dest_yco>yco)
             {
			if(cong_status.DSouth<cong_status.DEast)
			  	return S;
			else
				return E;
	     }
             else if(dest_yco<yco)
              {
		    if(cong_status.DSouth<cong_status.DWest)
		    	return S;
		    else
			return W;
	     }
             else if(dest_yco==yco)
                   return S;
    }
    else if(dest_xco<xco)
    {
              if(dest_yco>yco)
                   return E;
              else if(dest_yco<yco)
                   return W;
              else if(dest_yco==yco)
                    return N;
    }
    else if(dest_xco==xco)
    {
            if(dest_yco>yco)
                   return E;
            else if(dest_yco<yco)
                   return W;
            else if(dest_yco==yco)
                   return C;
    }

    return 0;   

}

void NL_router::initialize() {
 
}

extern "C" {
    router *maker() {
             return new NL_router;
}
}    
