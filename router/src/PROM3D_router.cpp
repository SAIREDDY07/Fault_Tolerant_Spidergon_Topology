#include "PROM3D_router.h"
#include "../../config/extern.h"
#include "stdlib.h"
#include "time.h"

void PROM3D_router::setID(UI id_tile) {
    id = id_tile;
    initialize();
}

UI PROM3D_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status) {

    float fmax = 4;

	int xco = (id%slice_size)/num_cols;
	int yco = (id%slice_size)%num_cols;
	int zco = id/slice_size;	
//	printf("\nTile ID : <%d>; Ip_dir : %d", id, ip_dir);

	int dest_xco = (dest_id%slice_size)/num_cols;
	int dest_yco = (dest_id%slice_size)%num_cols;
	int dest_zco = dest_id/slice_size;	
//	printf("\nDestination Coordinates : <%d, %d, %d>",dest_xco, dest_yco, dest_zco);

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
			if(xco < dest_xco)
				return S;
			else
				return N;
		} else {
			if(zco < dest_zco)
				return R;
			else
				return F;
		}
	} else {
		if(p2 >= p3) {
			if(yco < dest_yco)
				return E;
			else
				return W;
		} else {
			if(zco < dest_zco)
				return R;
			else
				return F;
		}
	}
}

void PROM3D_router::initialize() {

}

void PROM3D_router::update_power(int i,double p) {
}

// for dynamic linking
extern "C"
{
    router *maker()
    {
        return new PROM3D_router;
    }
}
