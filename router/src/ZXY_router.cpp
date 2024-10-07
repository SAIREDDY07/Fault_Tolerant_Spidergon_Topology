
/*
 * ZXY_router.cpp
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
 * Author: Lavina Jain
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file ZXY_router.cpp
/// \brief Implements ZXY routing algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ZXY_router.h"
#include "../../config/extern.h"

////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void ZXY_router::setID(UI id_tile) {
	id = id_tile;
	initialize();
}

////////////////////////////////////////////////
/// Method that implements ZXY routing
/// inherited from base class router
/// Parameters:
/// - input direction from which flit entered the tile
/// - tileID of source tile
/// - tileID of destination tile
/// ret_dir = s next hop direction
////////////////////////////////////////////////
UI ZXY_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status) {
	//int xco = id / num_cols;
	//int yco = id % num_cols;
	
	int xco = (id%slice_size)/num_cols;
	int yco = (id%slice_size)%num_cols;
	int zco = id/slice_size;	

//	int dest_xco = dest_id / num_cols;
//	int dest_yco = dest_id % num_cols;
	
	int dest_xco = (dest_id%slice_size)/num_cols;
	int dest_yco = (dest_id%slice_size)%num_cols;
	int dest_zco = dest_id/slice_size;	
	
	UI ret_dir;
	
	if(dest_zco > zco)
		ret_dir = R;
	else if(dest_zco < zco)
		ret_dir = F;
	else if(dest_yco > yco)
		ret_dir =  E;
	else if(dest_yco < yco)
		ret_dir =  W;
	else if(dest_yco == yco) {
		if(dest_xco < xco)
			ret_dir =  N;
		else if(dest_xco > xco)
			ret_dir =  S;
		else if(dest_xco == xco)
			ret_dir =  C;
	}
	return ret_dir;
}

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void ZXY_router::initialize() {

}

// for dynamic linking
extern "C" {
router *maker() {
	return  new ZXY_router;
}
}
