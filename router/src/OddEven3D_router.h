

#ifndef __OddEven3D_router__
#define __OddEven3D_router__

#include "../../core/router.h"



class OddEven3D_router : public router {
	public:
		/// Constructor
		OddEven3D_router() { }
		
		/// returns next hop for a given source and destination. ip_dir is the incoming direction
		UI calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion);
		
		void initialize();	///< any initializations to be done
		void setID(UI tileid);	///< set unique id
		UI get_estimate(UI){}
		void update_estimate(UI,UI,UI,ULL){}
		void update_power(int,double ){};
};

#endif

