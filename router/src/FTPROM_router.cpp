
/*
 * PROM_router.cpp
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
/// \file PROM_router.cpp
/// \brief Implements routing algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FTPROM_router.h"
#include "../../config/extern.h"
#include "stdlib.h"
#include "time.h"

////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void FTPROM_router::setID(UI id_tile) {
	id = id_tile;
	initialize();
}
int isavailable(int id,int dir)
{
        int id_E=id+1;
        int id_W=id-1;
        int id_S=id+num_cols;
        int id_N=id-num_cols;
        for(int i=0; i<failno ; i++)
        {
                                if(id == fail[i][0])
                                {
                                        if(id_E == fail[i][1])
                                        {
                                                if(dir==E)
                                                        return 0;
                                        }
                                        if(id_S == fail[i][1])
                                        {
                                                if(dir==S)
                                                        return 0;
                                        }
                                }
                                else if(id == fail[i][1])
                                {
                                        if(id_W == fail[i][0])
                                        {
                                                if(dir==W)
                                                        return 0;
                                        }
                                        if(id_N == fail[i][0])
                                        {
                                                if(dir==N)
                                                        return 0;
                                        }
                                }
        }
        return 1;
}

////////////////////////////////////////////////
/// Method that implements routing
/// inherited from base class router
/// Parameters:
/// - input direction from which flit entered the tile
/// - tileID of source tile
/// - tileID of destination tile
/// returns next hop direction
////////////////////////////////////////////////
UI FTPROM_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id,struct congestion cong_status) {
	// insert route logic here
	cout<<"\n ALGORITHM selected is FTPROM" << endl;
	cout<<endl<<"IN FTPROM  "<<id<<endl<<"ip direction "<<ip_dir<<endl;
	int set_avail[2];
	int k=10;
	int xco = id / num_cols;
	int yco = id % num_cols;
	int dest_xco = dest_id / num_cols;
	int dest_yco = dest_id % num_cols;
	int source_xco = source_id / num_cols;
	int source_yco = source_id % num_cols;
	int dirx= ND,diry=ND;
	int id_E = id +1, id_W = id -1, id_N = id - num_cols, id_S = id + num_cols;
	int choose=0; // 0 for X-dir and 1 for Y-dir
	int markx=0, marky=0;

	int x=dest_xco-xco;
	int y=dest_yco-yco;
	int m=dest_xco-source_xco;
	int n=dest_yco-source_yco;
	if(m<0)
		m=0-m;
	if(n<0)
		n=0-n;

	int x1,y1;
	x1=x; y1=y;
	if(x<0)
		x1=0-x;
	if(y<0)
		y1=0-y;
	int min_rec;
	if(m==0)
	{	
		min_rec=n+1;
	}
	else if(n==0)
	{
		min_rec=m+1;
	}
	else
	{
		min_rec=(m+1)*(n+1);
	}
	float f;
	float p1,p2;
	f=(float)(k*min_rec)/(num_rows*num_cols);
	set_avail[0]=set_avail[1]=5;
	if(x1==0 && y1==0)
	{
		p1=p2=1;
		set_avail[0]=C;
		set_avail[1]=C;
		return set_avail[0];
	}
//-------------------------------------------------------------------------
//
	
	if(id==source_id)//if current node is the source node, then ratio is x+f:y+f
	{
		if(y1+f>x1+f)//probability of going in the y direction is more than in the x direction 
		{
			//avail is an array containing directions in order of their probability
			//which direction whether north or south should be selected
			cout<<"\n check1" << endl;
			p1=(y1+f)/(x1+y1+2*f);
			p2=(x1+f)/(x1+y1+2*f);
			if(yco>dest_yco && !borderW(id))
				set_avail[0]=W;
			else if(yco<dest_yco && !borderE(id)&&isavailable(id,E)&&(ip_dir==C||ip_dir==W))
				set_avail[0]=E;
			else if(yco==dest_yco){
				if(xco>dest_xco && !borderN(id)&&isavailable(id,N)&&(ip_dir==C||ip_dir==S))
					set_avail[0]=N;
				else if(xco<dest_xco && !borderS(id)&&isavailable(id,S)&&(ip_dir==C||ip_dir==N))
					set_avail[0]=S;
				else if(xco==dest_xco)
					set_avail[0]=C;}
			if(xco>dest_xco && !borderN(id)&&isavailable(id,N)&&(ip_dir==C||ip_dir==S))
				set_avail[1]=N;
			else if(xco<dest_xco && !borderS(id)&&isavailable(id,S)&&(ip_dir==C||ip_dir==N))
				set_avail[1]=S;
			else
				set_avail[1]=set_avail[0];
		}
		else//probability of going in x direction is more than in the y direction
		{
			p1=(x1+f)/(x1+y1+2*f);
			p2=(y1+f)/(x1+y1+2*f);
			cout<<"\n check2" << endl;
			if(xco>dest_xco && !borderN(id)&&isavailable(id,N)) //&&(ip_dir==C||ip_dir==E))
				{set_avail[0]=N;cout<<"\n checkN" << endl;}
			else if(xco<dest_xco && !borderS(id)&&isavailable(id,S)) //&&(ip_dir==C||ip_dir==N))
				{set_avail[0]=S; cout<<"\n check3" << endl;}
			else if(xco==dest_xco){
				cout<<"\n check4" << endl;
				if(yco>dest_yco && !borderW(id)&&isavailable(id,W)) //&&(ip_dir==C||ip_dir==E))
					set_avail[0]=W;
				else if(yco<dest_yco && !borderE(id)&&isavailable(id,E)) //&&(ip_dir==C||ip_dir==W))
					set_avail[0]=E;
				else if(yco==dest_yco)
					set_avail[0]=C;}
			if(yco>dest_yco && isavailable(id,W)&&!borderW(id)) //&&(ip_dir==C||ip_dir==E))
				{set_avail[1]=W;cout<<"\n checkW" << endl;}	
			else if(yco<dest_yco && !borderE(id)&&isavailable(id,E))//&&(ip_dir==C||ip_dir==W))
				{set_avail[1]=E;cout<<"\n checkE" << endl;}
			else
				set_avail[1]=set_avail[0];
		}
	}
	
	else//current node is an intermediate node
	{
		//two possibilities: 1. packet arrives on the Y-axis ingress, 2.packet arrives on the X-axis ingress
	
		///1. Y-axis ingress
		if(ip_dir==E)//arrives on the Y-axis ingress from the east direction 
		{	
			if(y1==0)
			{
				p1=p2=1;
				if(xco>dest_xco && !borderN(id)&&isavailable(id,N))
				{cout<<"\n checkN0" << endl;
						set_avail[0]=N;
						set_avail[1]=N;
				}
				else if(xco<dest_xco && !borderS(id)&&isavailable(id,S))
				{cout<<"\n checkS0" << endl;
					set_avail[0]=S;
					set_avail[1]=S;
				}
				else
				{
					set_avail[0]=C;	
					set_avail[1]=C;
				}
			}
			else
			{			
				if(y1+f>=x1)
				{
					p1=(y1+f)/(y1+f+x1);
					p2=(x1)/(y1+f+x1);
					cout<<"\n check-0" << endl;
					if(!borderW(id)&&isavailable(id,W))				
					set_avail[0]=W;
					if(xco>dest_xco && !borderN(id)&&isavailable(id,N))
						set_avail[1]=N;
					else if(xco<dest_xco && !borderS(id)&&isavailable(id,S))
						set_avail[1]=S;
					else
						set_avail[1]=set_avail[0];
				}
				else 
				{
					p1=(x1)/(y1+f+x1);
					p2=(y1+f)/(y1+f+x1);
					if(!borderW(id))
					set_avail[1]=W;
					if(xco>dest_xco && !borderN(id)&&isavailable(id,N))
						set_avail[0]=N;
			      		else if(xco<dest_xco && !borderS(id)&&isavailable(id,S))
						set_avail[0]=S;
					else
						set_avail[0]=set_avail[1];
				}
			}
		}
		else if(ip_dir==W)//arrives on the Y-axis ingress from the west direction 
		{
			if(y1==0)
			{
				p1=p2=1;
				if(xco>dest_xco && !borderN(id)&&isavailable(id,N))
				{
						set_avail[0]=N;
						set_avail[1]=N;
				}
				else if(xco<dest_xco && !borderS(id)&&isavailable(id,S))
				{
					set_avail[0]=S;
					set_avail[1]=S;
				}
				else
				{
					set_avail[0]=C;	
					set_avail[1]=C;
				}
			}
			else
			{			
				if(y1+f>=x1)
				{
					p1=(y1+f)/(y1+f+x1);
					p2=(x1)/(y1+f+x1);
					if(!borderE(id))
					set_avail[0]=E;
					if(xco>dest_xco && !borderN(id)&&isavailable(id,N))
						set_avail[1]=N;
					else if(xco<dest_xco && !borderS(id)&&isavailable(id,S))
						set_avail[1]=S;
					else
						set_avail[1]=set_avail[0];
				}
				else 
				{
					p1=(x1)/(y1+f+x1);
					p2=(y1+f)/(y1+f+x1);
					if(!borderE(id))
					set_avail[1]=E;
					if(xco>dest_xco && !borderN(id)&&isavailable(id,N))
						set_avail[0]=N;
				      	else if(xco<dest_xco && !borderS(id)&&isavailable(id,S))
						set_avail[0]=S;
					else
						set_avail[0]=set_avail[1];
			}	}
		}
		
		///2. X-axis ingress 
		else if(ip_dir==N)//arrives on the X-axis ingress from the north direction 
		{
			
			if(x1==0)
			{
				p1=p2=1;
				//ADD
				if(yco>dest_yco && !borderW(id)&&isavailable(id,W))
				{
						set_avail[0]=W;
						set_avail[1]=W;
				}
				else if(yco<dest_yco && !borderE(id)&&isavailable(id,E))
				{
					set_avail[0]=E;
					set_avail[1]=E;
				}
				else
				{
					set_avail[0]=C;	
					set_avail[1]=C;
				}
			}
			else
			{			
				if(x1+f>=y1)
				{
					p1=(x1+f)/(x1+y1+f);
					p2=(y1)/(x1+y1+f);
					if(!borderS(id))
					set_avail[0]=S;
					if(yco>dest_yco && !borderW(id)&&isavailable(id,W))
						set_avail[1]=W;
					else if(yco<dest_yco && !borderE(id)&&isavailable(id,E))
						set_avail[1]=E;
					else 
						set_avail[1]=set_avail[0];
				}
				else
				{
					p1=(y1)/(x1+y1+f);
					p2=(x1+f)/(x1+y1+f);
					if(!borderS(id))
					set_avail[1]=S;
					if(yco>dest_yco && !borderW(id)&&isavailable(id,E))
						set_avail[0]=W;
					else if(yco<dest_yco && !borderE(id))
						set_avail[0]=E;
					else
						set_avail[0]=set_avail[1];
				}
			}
		}
		else if(ip_dir==S)//arrives on the X-axis ingress from the south direction
		{
			if(x1==0)
			{
				p1=p2=1;
				
				if(yco>dest_yco && !borderW(id)&&isavailable(id,W))
				{
						set_avail[0]=W;
						set_avail[1]=W;
				}
				else if(yco<dest_yco && !borderE(id)&&isavailable(id,E))
				{
					set_avail[0]=E;
					set_avail[1]=E;
				}
				else
				{
					set_avail[0]=C;	
					set_avail[1]=C;
				}
			}
			else
			{
				if(x1+f>=y1)
				{
					p1=(x1+f)/(x1+y1+f);
					p2=(y1)/(x1+y1+f);
					if(!borderN(id)&&isavailable(id,N))
					set_avail[0]=N;
					if(yco>dest_yco && !borderW(id)&&isavailable(id,W))
						set_avail[1]=W;
					else if(yco<dest_yco && !borderE(id)&&isavailable(id,E))
						set_avail[1]=E;
					else
						set_avail[1]=set_avail[0];
				}	
				else
				{
					p1=(y1)/(x1+y1+f);
					p2=(x1+f)/(x1+y1+f);
					if(!borderN(id)&&isavailable(id,N))
					set_avail[1]=N;
					if(yco>dest_yco && !borderW(id)&&isavailable(id,W))
						set_avail[0]=W;
					else if(yco<dest_yco && !borderE(id)&&isavailable(id,E))
						set_avail[0]=E;
					else
						set_avail[0]=set_avail[1];
				}
			}
		}		
	}	
	if(p1==p2)
	{
		return set_avail[0];
	}
	srand(time(0));
	float i;
	i=(float)rand()/RAND_MAX+4;
/* 	
	if(set_avail[0]==N)
		p1+=cong_status.DNorth;
	else if(set_avail[0]==S)
		p1+=cong_status.DSouth;
	else if(set_avail[0]==E)
		p1+=cong_status.DEast;
	else if(set_avail[0]==W)
		p1+=cong_status.DWest; */
	if(i<p1)
	{
		return set_avail[0];
	}
	else{	
		return set_avail[1];
	}
}

//-------------------------------------------------------------------
//--------------------------------------------------------

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void FTPROM_router::initialize() {

}

// for dynamic linking
extern "C" {
router *maker() {
	return new FTPROM_router;
}
}
