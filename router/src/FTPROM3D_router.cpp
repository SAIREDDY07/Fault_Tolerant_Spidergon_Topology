#include "FTPROM3D_router.h"
#include "../../config/extern.h"
#include "stdlib.h"
#include "time.h"

void FTPROM3D_router::setID(UI id_tile) {
    id = id_tile;
    initialize();
}



int isavailable(int id,int dir)
{
        
for( int s = 0; s < slice; s++) {
			for(int i = 0; i < rows; i++) {
				for(int j = 0; j < cols; j++) {
					//UI id = j + i * cols;
					UI id = (s*slice_size) + (i*num_cols) + j;
					UI id_S = (s*slice_size) + j + ((i+1) % rows) * cols;
					UI id_E = (s*slice_size) + (j+1) % cols + i * cols;
					UI id_F = (((s-1)%slice) *slice_size) + j + i * cols;
					
}
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
                                
				else if(id == fail[i][0])
                                {
                                        if(id_F == fail[i][1])
                                        {
                                                if(dir==F)
                                                      return 0;
                                       }
                                       
                                }



        }
        return 1;
}
UI FTPROM3D_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status) {

    float fmax = 4;

	int xco = (id%slice_size)/num_cols;
	int yco = (id%slice_size)%num_cols;
	int zco = id/slice_size;	
//	printf("\nTile ID : <%d>; Ip_dir : %d", id, ip_dir);

	int dest_xco = (dest_id%slice_size)/num_cols;
	int dest_yco = (dest_id%slice_size)%num_cols;
	int dest_zco = dest_id/slice_size;	
//	printf("\nDestination Coordinates : <%d, %d, %d>",dest_xco, dest_yco, dest_zco);

	int dirx= ND, diry=ND, dirz=ND;
	int id_E = id +1, id_W = id -1, id_N = id - num_cols, id_S = id + num_cols,  id_F = id +num_rows, id_R = id -num_rows;

	int source_xco = (source_id%slice_size)/num_cols;
	int source_yco = (source_id%slice_size)%num_cols;
	int source_zco =  source_id/slice_size;

	int x=dest_xco-xco;
	int y=dest_yco-yco;
	int z=dest_zco-zco;

	int m=dest_xco-source_xco;
	int n=dest_yco-source_yco;
	int o=dest_zco-source_zco;

	if (x<0)
        	x=-x;
	if (y<0)
        	y=-y;
	if (z<0)
        	z=-z;

	if (m<0)
        	m=-m;
	if (n<0)
        	n=-n;
	if (o<0)
        	o=-o;


	if (x==0 && y==0 && z==0 )
        return C;

	float f;
	float p1,p2, p3;
	f=(float)(fmax*(m+1)*(n+1)*(o+1))/(num_rows*num_cols*num_slice);

	if( id == source_id) {	//if current node is the source node, then ratio is x+f:y+f:z+f
		p1 = x;
		p2 = y;
		p3 = z;
	} else {	//current node is an intermediate node 
				/*three possibilities:
					1. packet arrives on the X-axis ingress,
					2. packet arrives on the Y-axis ingress,
					3. packet arrives on the Z-axis ingress */

		if(ip_dir == N || ip_dir == S) { 			//1. X-axis ingress
			if( x == 0)
				p1 = 0;
			else
				p1 = x + f;

			p2 = y;
			p3 = z;
		} else if(ip_dir == E || ip_dir == W) {		//2. Y-axis ingress
			p1 = x;

			if(y == 0)
				p2 = y;
			else
				p2 = y + f;

			p3 = z;
		} else if (ip_dir == R || ip_dir == F) {	//3. Z-axis ingress
			p1 = x;
			p2 = y;

			if(z == 0)
				p3 = z;
			else
				p3 = z + f;
		}
	}

	//compare probabilities and return direction...
	if(p1 >= p2) {
		if(p1 >= p3) {
			// if((xco < dest_xco)&&!borderS(id)&&isavailable(id,S))
			if((xco < dest_xco)&&isavailable(id,S))
				return S;
			else
				return N;
		} else {
			// if((zco < dest_zco)&&!borderR(id)&&isavailable(id,R))
			if((zco < dest_zco)&&isavailable(id,R))
				return R;
			else
				return F;
		}
	} else {
		if(p2 >= p3) {
			// if((yco < dest_yco)&&!borderE(id)&&isavailable(id,E))
			if((yco < dest_yco)&&isavailable(id,E))
				return E;
			else
				return W;
		} else {
			if((zco < dest_zco)&&isavailable(id,R))
				return R;
			else
				return F;
		}
	}
}





void FTPROM3D_router::initialize() {

}

void FTPROM3D_router::update_power(int i,double p) {
}

// for dynamic linking
extern "C"
{
    router *maker()
    {
        return new FTPROM3D_router;
    }
}
