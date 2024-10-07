
/*
 * XY_router.cpp
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
/// \file XY_router.cpp
/// \brief Implements XY routing algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "XY_router.h"
#include "../../config/extern.h"
 
////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void XY_router::setID(UI id_tile) 
{
    id = id_tile;
    num_tiles=num_rows;
    initialize();
}

////////////////////////////////////////////////
/// Method that implements XY routing
/// inherited from base class router
/// Parameters:
/// - input direction from which flit entered the tile
/// - tileID of source tile
/// - tileID of destination tile
/// returns next hop direction
////////////////////////////////////////////////
UI XY_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id)
{
    int xco = id / num_cols;
    int yco = id % num_cols;
    int dest_xco = dest_id / num_cols;
    int dest_yco = dest_id % num_cols;
     int dst=(dest_id-source_id)%num_tiles;
                int diameter=num_tiles/4;
/*
	switch(TOPO)
	{
	case MESH:
    if (dest_yco > yco)
        return E;
    else if (dest_yco < yco)
        return W;
    else if (dest_yco == yco)
    {
        if (dest_xco < xco)
            return N;
        else if (dest_xco > xco)
            return S;
        else if (dest_xco == xco)
            return C;
    }
    return 0;
	break;

	case TORUS:
		if(dest_yco>yco)
		{
			if((dest_yco-yco)<(yco+num_cols-dest_yco))
				return E;
			else
				return W;
		}
		else if(dest_yco<yco)
		{
			if((yco-dest_yco)<(dest_yco+num_cols-yco))
				return W;
			else
				return E;
		}
		else if(dest_yco==yco)
		{
			if(dest_xco>xco)
			{
				if((dest_xco-xco)<(xco+num_rows-dest_xco))
					return S;
				else 
					return N;
			}
			else if(dest_xco<xco)
			{
				if((xco-dest_xco)<(dest_xco+num_rows-dest_xco))
					return N;
				else 
					return S;
			}
			else if(dest_xco==xco)
				return C;
		}
		return 0;
		break;
	}
*/
	UI ret_dir;
	switch(TOPO)
	{
            case SPIDERGON:
                if(id==dest_id)
                    return C;
                else if(dst<=diameter)
                    return S;
                else if(num_tiles-dst<=diameter)
                    return N;
                else
                    return E;
                //printf("SRC: %d DEST: %d ID:%d DIAM: %d DEST: %d DIR:%D ",)
                break;
	case MESH:
		if(dest_yco > yco)
			ret_dir =  E;
		else if(dest_yco < yco)
			ret_dir =  W;
		else if(dest_yco == yco) 
		{
			if(dest_xco < xco)
				ret_dir =  N;
			else if(dest_xco > xco)
				ret_dir =  S;
			else if(dest_xco == xco)
			{
				ret_dir =  C;
			}
		}
		return ret_dir;
		break;

	case TORUS:
		if(dest_yco>yco)
		{
			if((dest_yco-yco)<(yco+num_cols-dest_yco))
				ret_dir = E;
			else
				ret_dir = W;
		}
		else if(dest_yco<yco)
		{
			if((yco-dest_yco)<(dest_yco+num_cols-yco))
				ret_dir = W;
			else
				ret_dir = E;
		}
		else if(dest_yco==yco)
		{
			if(dest_xco>xco)
			{
				if((dest_xco-xco)<(xco+num_rows-dest_xco))
					ret_dir = S;
				else
					ret_dir = N;
			}
			else if(dest_xco<xco)
			{
				if((xco-dest_xco)<(dest_xco+num_rows-xco))
					ret_dir = N;
				else
					ret_dir = S;
			}
			else if(dest_xco==xco)
			{
					ret_dir =C;
			}
		}
		return ret_dir;
		break;
	}

}

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void XY_router::initialize()
{

}

// for dynamic linking
extern "C"
{
    router *maker()
    {
        return new XY_router;
    }
}

