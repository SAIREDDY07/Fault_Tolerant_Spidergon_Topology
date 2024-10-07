

#include "WF_router.h"
#include "../../config/extern.h"

void WF_router::setID(UI id_tile){
       id=id_tile;
       initialize();
}

UI WF_router::calc_next(UI ip_dir,ULL src_id,ULL dest_id,struct congestion cong_status) {
    int xco=id/num_cols;
    int yco=id%num_cols;
    int dest_xco=dest_id/num_cols;
    int dest_yco=dest_id%num_cols;
    static int count; 

    if(count==0)
    {   
	    printf("\n Algorithm selected is  WF "); 
	    count++; 
    }   

    if(dest_yco<yco)
         return W;    

    else if(dest_yco>yco)
    {                         
	 if(dest_xco<xco)
         {	if(cong_status.DNorth<=cong_status.DEast)
                   return N;
                else
                   return E;
         }   
	 else if(dest_xco>xco)
	 {	
                 if(cong_status.DEast<=cong_status.DSouth)
                       return E;
                  else 
                       return S;
         }
         else if(dest_xco==xco)
		return E;
    }
    else if(dest_yco==yco)
    {
	if(dest_xco<xco)
             return N;
        else if(dest_xco>xco)
	     return S;
        else if(dest_xco==xco)
		return C;
    }
   

}

void WF_router::initialize() {
 
}

extern "C" {
    router *maker() {
             return new WF_router;
}
}    
