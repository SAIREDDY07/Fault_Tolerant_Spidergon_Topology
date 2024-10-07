#ifndef __my_router__
#define __my_router__
#include "../../core/ranvar.h"
#include "../../core/rng.h"

#include "../../core/router.h"

class PROM2D_router : public router {
    public:
        /// Constructor
        PROM2D_router() { }
        
        /// returns next hop for a given source and destination. ip_dir is the incoming direction
        UI calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion);
        
        void initialize();   ///< any initializations to be done
        void setID(UI tileid);   ///< set unique id
        UI get_estimate(UI){}
        void update_estimate(UI,UI,UI,ULL){}
        void update_power(int,double );
        UniformRandomVariable *prng ;///< uniform random number to get destination
};

#endif
