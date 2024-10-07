#include "FTXYZ_router.h"
#include "../../config/extern.h"

////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void FTXYZ_router::setID(UI id_tile)
{
    id = id_tile;
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
UI FTXYZ_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status)
{
      /* int i;
        cout<<"\n id = "<<id<<" ipdir = "<<ip_dir<<" source_id = "<<source_id<<" dest_id = "<<dest_id;
        cin>>i;*/
        int xco = (id%slice_size)/num_cols;
	int yco = (id%slice_size)%num_cols;
	int zco = id/slice_size;	
	
	int dest_xco = (dest_id%slice_size)/num_cols;
	int dest_yco = (dest_id%slice_size)%num_cols;
	int dest_zco = dest_id/slice_size;
	
	int id_E = id +1, id_W = id -1, id_N = id - num_cols, id_S = id + num_cols, id_R = id + slice_size, id_F = id - slice_size;
	int markx=0, marky=0, markz=0;
	int dirx= ND,diry=ND,dirz=ND;
	int choose=0; // 0 for X-dir and 1 for Y-dir 2 for Z-dir
	
	if (dest_xco == xco && dest_yco == yco && dest_zco == zco)
            return C;

        if (dest_yco > yco)
             dirx= E;
        else if (dest_yco < yco)
             dirx= W;

        if (dest_xco < xco)
             diry= N;
        else if (dest_xco > xco)
             diry= S;
             
        if (dest_zco < zco)
             dirz= F;
        else if (dest_zco > zco)
             dirz= R;
             
         
             // Mark nodes in the x direction
    if (dirx == E)
    {
        for (int i=0; i<failno ; i++)
        {
            if (id_E == fail[i][0] || id_E == fail[i][1])
            {
                markx = 1;//link fail in east direction
            }

        }
    }
    else if (dirx == W)
    {
        for (int i=0; i<failno ; i++)
        {
            if (id_W == fail[i][0] || id_W == fail[i][1])
            {
                markx = 1;//link fail in west direction
            }

        }
    }

    // Mark nodes in the y direction

    if (diry == N)
    {
        for (int i=0; i<failno ; i++)
        {
            if (id_N == fail[i][0] || id_N == fail[i][1])
            {
                marky = 1;//link fail in north direction
            }
        }
    }
    else if (diry == S)
    {
        for (int i=0; i<failno ; i++)
        {
            if (id_S == fail[i][0] || id_S == fail[i][1])
            {
                marky = 1;//link fail in south direction
            }

        }
    }
    // Mark nodes in the z direction

    if (dirz == R)
    {
        for (int i=0; i<failno ; i++)
        {
            if ( id_R == fail[i][0] || id_R == fail[i][1])
            {
                markz = 1;//link fail in rear direction
            }
        }
    }
    else if (dirz == F)
    {
        for (int i=0; i<failno ; i++)
        {
            if (id_F == fail[i][0] || id_F == fail[i][1])
            {
                markz = 1;//link fail in front direction
            }

        }
    }
    
    if (markx==0 && marky==0 && markz==0)
    {
        if(yco == dest_yco)
        {
            if(xco == dest_xco)
                choose = 2;
            else 
                choose = 1;
        }
        else
           choose = 0;
    }

    if (markx==0 && marky==0 && markz==1)
    {/*either choose x-dir or y-dir*/
    //cout<<" x dir\n";
        if(yco == dest_yco)
        {
            if(xco == dest_xco)
               choose = 0;
            else
               choose = 1;
        }
        else
           choose = 0;
    }
    if (markx==0 && marky==1 && markz==0)
    {/*either choose x-dir or z-dir*/
    
        if(yco == dest_yco)
        {
           if(zco == dest_zco)
              choose = 0;
           else
              choose = 2;
             
        }
        else
        choose = 0;
    }
    if (markx==0 && marky==1 && markz==1)
    {
        choose = 0;
    }
    if (markx==1 && marky==0 && markz==0)
    {/*either choose x-dir or z-dir*/
      
        if(xco == dest_xco)
        {
           if(zco == dest_zco)
              choose = 1;
           else
              choose = 2;
             
        }
        else
        choose = 1;
    }
    if (markx==1 && marky==0 && markz==1)
    {
        choose = 1;
    }
    if (markx==1 && marky==1 && markz==0)
    {
        choose = 2;
    }
    if (markx==1 && marky==1 && markz==1)
    {
        choose = ND;
    }
    switch (choose)
    {
    case 0:
      //  cout<<" dirx = "<<dirx;
        return dirx;
        break;
    case 1:
      //  cout<<" diry = "<<diry;
        return diry;
        break;
    case 2:
      	//cout<<" dirz = "<<dirz;
        return dirz;
        break;
    }
   // return 0;
}

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void FTXYZ_router::initialize()
{
    //  cout<<"initialized\n";
}

// for dynamic linking
extern "C"
{
    router *maker()
    {
        // cout<<"constructor called\n";
        return new FTXYZ_router;
    }
}
