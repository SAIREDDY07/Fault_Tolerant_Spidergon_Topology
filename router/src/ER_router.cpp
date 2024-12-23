
/*
 * ER_router.cpp
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
/// \file OE_router.cpp
/// \brief Implements OE (Odd-Even) routing algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ER_router.h"
#include "../../config/extern.h"

////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void ER_router::setID(UI id_tile)
{
    id = id_tile;
    initialize();
}

////////////////////////////////////////////////
/// Method that implements OE routing
/// inherited from base class router
/// Parameters:
/// - input direction from which flit entered the tile
/// - tileID of source tile
/// - tileID of destination tile
/// returns next hop direction
////////////////////////////////////////////////
UI ER_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status)
{
    bool set_avail[4];
    for (UI i = 0; i < 4; i++)
        set_avail[i] = false;
    int cur_xco = id / num_cols;
    int cur_yco = id % num_cols;
    //int src_xco = source_id / num_cols;
    int src_yco = source_id % num_cols;
    int dst_xco = dest_id / num_cols;
    int dst_yco = dest_id % num_cols;
    int dif_xco = dst_xco - cur_xco;
    int dif_yco = dst_yco - cur_yco;

#define iprint(value)	" " #value "= "<<value
    if (LOG == 10)
        cout<<iprint(cur_xco)<<iprint(cur_yco)<<iprint(src_yco)<<iprint(dst_xco)<<iprint(dst_yco)<<iprint(dif_xco)<<iprint(dif_yco)<<endl;



    if (dif_xco == 0 && dif_yco == 0)	// cur node is dest
        return C;
    if (dif_yco == 0)  	// cur node in same col as dest
    {
        if (dif_xco < 0)
        {
            if (ip_dir != N && !borderN(id))		// 180-degree turn prohibited
                set_avail[N] = true;	// allow to route N
        }
        else
        {
            if (ip_dir != S && !borderS(id))
                set_avail[S] = true;	// allow to route S
        }
    }
    else
    {
        if (dif_yco > 0)  	// E-bound pkt
        {
            if (dif_xco == 0)  	// cur in same row as dest
            {
                if (ip_dir != E)
                    set_avail[E] = true;
            }
            else
            {
                if (cur_yco % 2 != 0 || cur_yco == src_yco)	// N/S turn allowed only in odd col.
                {
                    if (dif_xco < 0 && !borderN(id) && ip_dir != N)
                        set_avail[N] = true;
                    else if (!borderS(id) && ip_dir != S)
                        set_avail[S] = true;
                }
                if (dst_yco % 2 != 0 || dif_yco != 1)  	// allow to go E only if dest is odd col
                {
                    if (ip_dir != E)
                        set_avail[E] = true;	// because N/S turn not allowed in even col.
                }
            }
        }
        else  	// W-bound
        {
            if (ip_dir != W)
                set_avail[W] = true;
            if (cur_yco % 2 == 0)	// allow to go N/S only in even col. because N->W and S->W not allowed in odd col.
                if (dif_xco <= 0 && !borderN(id) && ip_dir != N) // = 0 to allow non minimal path
                    set_avail[N] = true;
                else if (!borderS(id) && ip_dir != S)
                    set_avail[S] = true;
        }
    }

    if (LOG == 10)
        cout<<iprint(id)<<iprint(ip_dir)<<iprint(borderN(id))<<iprint(borderS(id))<<iprint(num_tiles)<<iprint(num_cols)<<iprint(num_rows)<<iprint(sizeof(num_tiles))<<iprint(sizeof(num_cols));

    int count=0; ///indicates the number of possible directions
    int path_len[4];
    int n_row, n_col,dx,dy;
    double power_factor[4];

    //average power of all the neighboring nodes
    double avg_power_nbs=0;

    for (UI i=0; i<4; i++)
    {
        if (set_avail[i])
        {
            avg_power_nbs=avg_power_nbs+power[i];
            count++;
        }
    }
    /*int ss;
    if(id==6){cout<<count;
    cout<<"power: "<<power[0]<<","<<power[2]<<endl;
    for(UI i=0; i<4; i++)
    {
    	if(set_avail[i])
    	{
    		cout<<i<<" ";
    	}

    }
    cin>>ss;}
    if(count==0){cout<<"hi count";cin>>ss;}*/
    if (count!=0)
        avg_power_nbs= avg_power_nbs/count;
    else
    {
        cout<<"no direction avail\n";
        //int b;
        //cin>>b;
    }

    //cout<<"avg+power:"<<avg_power_nbs<<endl;

    //available directions whose power is less than or equal to the avg_power_nbs
    bool set_avail1[4];
    int count1=0;
    for (UI i = 0; i < 4; i++)
        set_avail1[i] = false;

    ///added
    for (UI i=0 ; i < 4; i++)
    {
        if (set_avail[i])
        {

            //checking if its power is less than or equal to avg_pwer_nbs
            if ( power[i] <= avg_power_nbs)
            {
                set_avail1[i]=true;
                count1++;
            }
            if (i==N)
            {
                n_row=cur_xco-1;
                n_col=cur_yco;
            }
            else if (i==S)
            {
                n_row=cur_xco+1;
                n_col=cur_yco;
            }
            else if (i==E)
            {
                n_row=cur_xco;
                n_col=cur_yco+1;
            }
            else if (i==W)
            {
                n_row=cur_xco;
                n_col=cur_yco-1;
            }
            dx=(dst_xco-n_row);
            dy=(dst_yco-n_col);
            if (dx<0)
                dx=0-dx;
            if (dy<0)
                dy=0-dy;
            path_len[i]=dx+dy;
            power_factor[i]=path_len[i]*power[i];
        }

    }

    if (count1<=0)
    {
        cout<<"count1"<<endl;
        int c;
        cin>>c;
    }
    ///final list of available directions
    int k=0,dir[2];
    int a;
    if (count1==1)
    {
        for (UI i=0; i<4 ; i++)
        {
            if (set_avail1[i])
            {
                //cout<<"count=1";
                //cin>>a;
                return i;
            }
        }
    }
    //getting a list of available directions from set_vail1 in sorted order
    int final_dir[4];

    for (UI i=0; i<4 ; i++)
    {
        if (set_avail1[i])
        {
            if (k>1)
            {
                if (power_factor[i]<power_factor[dir[1]])
                    dir[1]=i;
            }
            else
            {
                if (k==0)
                    dir[k]=i;
                else
                {
                    if (power_factor[i]<power_factor[dir[0]])
                    {
                        dir[k]=dir[0];
                        dir[0]=i;
                    }
                    else
                        dir[k]=i;
                }
                k++;
            }
        }
    }
    int t;
    //cout<<"cur_tile:"<<id<<endl<<endl;
//	for(int i=0;i<4;i++)
//	{
//		if(set_avail[i])
//		{
//			//cout<<"i:"<<i<<" path length: "<<path_len[i]<<" power:"<<power[i]<<"avg_pow:"<<avg_power_nbs<<endl;
//			if(set_avail1[i])
//				//cout<<"in set_avail1\n";
//		}
//	}
    //cin>>t;
    if (dir[1]>dir[0])
        return (dir[1]*10+dir[0]);
    else
        return (dir[0]*10+dir[1]);

}

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void ER_router::initialize()
{

    // Added after num_tiles started showing grabage values
    num_tiles = num_cols * num_rows;
}

void ER_router::update_power(int i,double p)
{

    power[i]=p;

}


// for dynamic linking
extern "C"
{
    router *maker()
    {
        return new ER_router;
    }
}
