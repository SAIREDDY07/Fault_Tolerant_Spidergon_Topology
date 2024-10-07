
/*
 * C_router.h
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * Added By: M.K.puthal
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file C_router.h
/// \brief Defines class to implement C routing algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __C_router__
#define __C_router__

#include "../../core/router.h"
#include "../../config/default.h"
#include "../../core/rng.h"
//////////////////////////////////////////////////////////////////////
/// \brief Class to implement C routing algorithm
///
/// inherited from from class router
//////////////////////////////////////////////////////////////////////
class C_router : public router {
	private:
		float **Q;
		RNG *ran_var;
		float alpha;
		UI *minQ;
		UI clusterID, dest_cluster, localID;
		int num_Crows, num_Ccols;
		int ik; /// let cluster size  2^i = 2^4 =16 // later on give it as an input parameter
		int xco, yco, dest_xco, dest_yco;
    	
	public:
		/// Constructor
		C_router() { }
		
		/// returns next hop for a given source and destination. ip_dir is the incoming direction
		UI calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion);
		
		void initialize();	///< any initializations to be done
		void setID(UI tileid);	///< set unique id
		UI get_estimate(UI){}
		void update_estimate(UI,UI,UI,ULL){}
		void update_power(int,double ){}
		
		int Ssc(int Dc, int Y) ;	// Cost to reach destination cluster from source cluster via neighbor.
		int Ssrc(int Dest, int Y) ;	// Estimated cost to reach via source’s neighbor from source to destination.
		
		void UpdateCRT(UI tileid);
		int direction(int , int);
		int UpdateQ(UI tileID, UI ret_dir, int cluster, int inc_cost);
		
		UI chooseRandomNb(UI tileID, ULL dest_id);
		UI chooseRandomClNb(UI tileID, ULL dest_id);
		UI chooseMinNb(UI tileID, ULL dest_id);
		UI chooseMinClNb(UI tileID, ULL dest_id);
		UI deterministicXY( UI tileID, ULL dest_id, int cluster); // cluster = 0 when intra cluster routing
};

#endif

