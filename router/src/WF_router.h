#include "../../core/router.h"

class WF_router : public router
{
     public:
           WF_router() {  }
       
           unsigned int calc_next(unsigned int ip_dir,ULL src_id,ULL dest_id,congestion cong_status);
           
           void initialize();
	   void setID(UI tileid);
           UI get_estimate(UI){}
           void update_estimate(UI,UI,UI,ULL){}
	void update_power(int,double ){};

};	
