/*
 * C_router.cpp
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
 * Added By: M.K.Puthal
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file C_router.cpp
/// \brief Implements C routing algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "C_router.h"
#include "../../config/extern.h"
#include "../../core/rng.h"
#include "../../core/ranvar.h"

////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void C_router::setID(UI id_tile)
{
    id = id_tile;
    //initialize();	// initialized in NoC.cpp
}

////////////////////////////////////////////////
/// Method that implements C routing
/// inherited from base class router
/// Parameters:
/// - input direction from which flit entered the tile
/// - tileID of source tile
/// - tileID of destination tile
/// returns next hop direction
////////////////////////////////////////////////
UI C_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status)
{
    	xco = id / num_cols;
	yco = id % num_cols;
	dest_xco = dest_id / num_cols;
	dest_yco = dest_id % num_cols;
	//cout<<"\n Curr_id= "<<id;
	// co-ordinate details of the cluster. ie. a node belongs to which cluster co-ordinate.
	int Cx_co = id/(num_rows*ik);					
	int Cy_co = (((id%(num_rows*ik))%num_rows)/ik);
	clusterID = (num_rows/ik * Cx_co) + Cy_co;
	dest_cluster = (num_rows/ik * (dest_id/(num_rows*ik))) + (((dest_id%(num_rows*ik))%num_rows)/ik);
	localID=(id%ik)+(ik*(id/num_cols)) - (Cx_co*cluster_size);
	
    	//cout<<" Cx_co="<<Cx_co<<" clusterID="<<clusterID<<" dest_Cl="<<dest_cluster;
	UI ret_dir;
	// functions & variables for C-routing.
	float r ;		//random no
	float est_time = .05;	// probability of not receiving the estimated cost in estimated time, hence penalty cost
	UniformRandomVariable *rng ;///< uniform random number to get destination

	int XSc = clusterID / num_Crows;	// X-axis address of source cluster.
	int XDc = dest_cluster / num_Crows;	// X-axis address of destination cluster.
	
	if(id == dest_id)
	{
		return C;
	}
if(clusterID != dest_cluster) 	// routing into other cluster by choosing minimum cost neighbour cluster
{
	if(XDc < XSc)	// do deterministic XY routing
    	{
    		ret_dir = deterministicXY( id, dest_id, dest_cluster+cluster_size-1);  // dest_cluster+cluster_size-1 is passed to update Q
	}
	else if(XDc >= XSc)
	{
		rng=new UniformRandomVariable(0, 1);
		r=(float)rng->value(); 
		begining_interCl:
		if( r <= EPSILON ) {// choose a random neighbor of the current sourse node to go to destination cluster
			ret_dir = chooseRandomClNb(id, dest_id);	} 
		else 
		{
			ret_dir = chooseMinClNb(id, dest_id);		
			if(r <= est_time)	// penalize the cost by increaing cost +50 and repeat the routing process.
			{
				UpdateQ(localID, ret_dir-1, dest_cluster+cluster_size-1, 50);	
				r=r+0.1;
				goto begining_interCl;
			}	
		}
	}
}
else if(clusterID == dest_cluster) 	// Routing inside cluster, ie. if source & dest is in same cluster.		
{
	if(dest_xco < xco)	// do deterministic XY routing
    	{ 
    		ret_dir = deterministicXY( id, dest_id, 0);
	}
	else if(dest_xco >= xco)		// do partially adaptive routing.
	{
		rng=new UniformRandomVariable(0, 1);
		r=(float)rng->value(); 
		begining_intraCl:
		if( r <= EPSILON ) {// choose a random neighbor of the current sourse node to go to destination cluster
			ret_dir = chooseRandomNb(id, dest_id);		}
		else  		//Choose a neighbor which has minimum a cost from the routing table
		{
			ret_dir = chooseMinNb(id, dest_id);		
			if(r <= est_time)	// penalize the cost by increaing cost +50 and repeat the routing process.
			{
				UpdateQ(localID, ret_dir-1, dest_cluster+cluster_size-1, 50);	
				r=r+0.1;
				goto begining_intraCl;
			}
		}
	}
}
	// Displaying the updated cost at any perticular tile
	/*if(id==58 || id==57)
	{
		cout<<endl;
		for(int a=0; a<3; a++)
			cout<<Q[localID][a]<<"  ";
	}*/
	//cout<<"\t Moving in ret_dir: "<<ret_dir;
/*////////////////////Printing the rtable for a specific node
     int id_E , id_W, id_N, id_S;
     id_E = id +1; id_W = id -1; id_N = id - num_rows; id_S = id + num_cols;
     if(id==44 || id==52)
     {
	     cout<<"\n     id_E = "<<id_E<<" \tid_S = "<<id_S<<" \tid_W = "<<id_W<<endl;
	     for (int i = 0; i <20 ; i++)
	     {
	     	 printf("\n ");
	         for (int j = 0; j < num_nb; j++)
	         {
	         	cout<<"   Q["<<i<<"]["<<j<<"]= "<<Q[i][j];
	         }
	     }
     }
     /////////////////////////////////end*/
return ret_dir;
}

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void C_router::initialize()
{
    ik=4; /// let cluster size  2^i = 2^4 =16 // later on give it as an input parameter
    num_Crows = num_rows/ik ; num_Ccols = num_cols/ik;	// calculating number of rows and cols of the cluster topology
    int Cx_co = id/(num_rows*ik);
    int Cy_co = (((id%(num_rows*ik))%num_rows)/ik);
    clusterID = (num_rows/ik * Cx_co) + Cy_co;
    int id_E , id_W, id_N, id_S;
    
    UI rtableSize= (num_tiles/num_clusters) + num_clusters + 1;	//Size of routing table;
    ran_var = new RNG((RNG::RNGSources)2,1);
    
    localID=(id%ik)+(ik*(id/num_cols)) - (Cx_co*cluster_size);
    
        
    Q = new float*[rtableSize];
    for (int i = 0; i <rtableSize ; i++)
    {
         Q[i] = new float[num_nb];
         for (int j = 0; j < num_nb; j++)
         {
             Q[i][j] = 0.0;	// Initial Estimation to all its neighbour nodes inside cluster and to neighbour clusters
         }
     } 
     for (int j = 0; j < num_nb; j++)  
     {
     	//Q[id%cluster_size][j] = 0;		// Estimation to id itself = 0;
     	Q[clusterID+cluster_size][j] = 0;	// Estimation inside same cluster = 0;
     }
 /*    ////////////////////Printing the rtable for a specific node
     id_E = id +1; id_W = id -1; id_N = id - num_rows; id_S = id + num_cols;
     if(id==9)
     {
	     cout<<"\n     id_E = "<<id_E<<" \tid_S = "<<id_S<<" \tid_W = "<<id_W<<endl;
	     for (int i = 0; i <rtableSize ; i++)
	     {
	     	 printf("\n ");
	         for (int j = 0; j < num_nb; j++)
	         {
	         	cout<<"   Q["<<i<<"]["<<j<<"]= "<<Q[i][j];
	         }
	     }
     }
     /////////////////////////////////end*/
}
// Choose deterministic XY routing when XDc < XSc
UI C_router::deterministicXY( UI id, ULL dest_id, int cluster)
{
	UI ret_dir;
	if (dest_yco > yco)
        	ret_dir=  E;
	else if (dest_yco < yco)
	       	ret_dir=  W;
	else if (dest_yco == yco)
	{
	       	if (dest_xco < xco)
	       		ret_dir=  N;
	      	else if (dest_xco > xco)
	       		ret_dir=  S;
	       	else if (dest_xco == xco)
		        ret_dir=  C;
	}
	if(ret_dir!=C || ret_dir!=N)
		UpdateQ(localID, ret_dir-1, cluster, 1);// ret_dir retruns 1-3 but subscripts are(0-2) S-0, E-1, W-2
	return ret_dir;
}

//Choose minimun cost neighbour during inter cluster routing
UI C_router::chooseMinClNb(UI id, ULL dest_id)
{
	UI ret_dir;
	UI rand = ran_var->uniform(3000);
	UI minQ;
	UI mindir;
	
	mindir= rand % num_nb;
	minQ = Q[dest_cluster+cluster_size-1][num_nb-1];
	//find minimum Q value
	for (int i = 0; i < num_nb; i++)
	{
		if (minQ > Q[dest_cluster+cluster_size-1][i])
		{
		    minQ = Q[dest_cluster+cluster_size-1][i];
		    mindir = i;
		}
	}
	ret_dir=  direction(id, mindir);
	
	UpdateQ(localID, ret_dir-1, dest_cluster+cluster_size-1, 1);	
	return ret_dir;
}
//Choose minimun cost neighbour during intra cluster routing
UI C_router::chooseMinNb(UI id, ULL dest_id)
{
	UI ret_dir;
	UI rand = ran_var->uniform(3000);
	UI minQ;
	UI mindir;
	
	mindir= rand % num_nb;
	minQ = Q[dest_id%cluster_size][mindir];
	//find minimum Q value
	for (int i = 0; i < num_nb; i++)
	{
		if (minQ > Q[dest_id%cluster_size][i])
		{
		    minQ = Q[dest_id%cluster_size][i];
		    mindir = i;
		}
	}
	ret_dir= direction(id, mindir);
	UpdateQ(localID, ret_dir-1, 0, 1 );// ret_dir retruns 1-3 but subscripts are(0-2) S-0, E-1, W-2
	return ret_dir;
}
// Choose Random neighbour when  r < EPSILON during inter cluster routing
UI C_router::chooseRandomClNb(UI id, ULL dest_id)
{
	UI ret_dir;
	UI rand = ran_var->uniform(3000);
	rand= rand % num_nb;
	ret_dir= direction(id, rand);	
	UpdateQ(localID, ret_dir-1, dest_cluster+cluster_size-1, 1);
	return ret_dir;
}

// Choose Random neighbour when  r < EPSILON during intra cluster routing
UI C_router::chooseRandomNb(UI id, ULL dest_id)
{
	UI ret_dir;
	UI rand = ran_var->uniform(3000);
	rand= rand % num_nb;
	ret_dir =  direction(id, rand);
	UpdateQ(localID, ret_dir-1, 0, 1);// ret_dir retruns 1-3 but subscripts are(0-2) S-0, E-1, W-2
	return ret_dir;
}
int C_router::direction(int id, int d)
{
	
	if(corner(id))
	{
		if(cornerNW(id)) if(d==0) return S; else return E;
		if(cornerNE(id)) if(d==0) return S; else return W;
		if(cornerSW(id)) if(d==0) return E; else return E; // both E to avoid N
		if(cornerSE(id)) if(d==0) return W; else return W;	// both W to avoid N
	}
	else if(border(id))
	{
		if(borderN(id)) if(d==0) return S; else if(d==1) return E; else return W;
		if(borderW(id)) if(d==0) return S; else if(d==1) return S; else return E;
		if(borderE(id)) if(d==0) return S; else if(d==1) return S; else return W;
		if(borderS(id)) if(d==0) return E; else if(d==1) return E; else return W;	
	}
	else
	{
		if (d==0)
			return S;
		else
			return d;
	}
}
int C_router::UpdateQ(UI tileID, UI ret_dir, int cluster, int inc_cost)
{
	if(ret_dir != C )
	{
		if(cluster >= cluster_size)					// Ssc = (1 - Alpha)Ssc + Alpha * q;
		{
			Q[cluster][ret_dir]= (1-ALPHA)*Q[cluster][ret_dir] + ALPHA * (Q[cluster][ret_dir]+inc_cost);		// update for inter cluster movement
		}
		Q[tileID][ret_dir] = (1-ALPHA)*Q[tileID][ret_dir] + ALPHA*(Q[tileID][ret_dir]+inc_cost);	// update for both inter and intra cluster movement
	}
}
// for dynamic linking
extern "C"
{
    router *maker()
    {
        return new C_router;
    }
}
