#include "OddEven3D_router.h"
#include "../../config/extern.h"

////////////////////////////////////////////////
/// Method to set unique id
////////////////////////////////////////////////
void OddEven3D_router::setID(UI id_tile) {
	id = id_tile;
	initialize();
}

UI OddEven3D_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status) 
{
        int i;
        cout<<"\n id = "<<id<<" ipdir = "<<ip_dir<<" source_id = "<<source_id<<" dest_id = "<<dest_id<<"  ";
        cin>>i;
        bool set_avail[6];
	for(UI i = 0; i < 6; i++)
		set_avail[i] = false;
		
	int xco = (id%slice_size)/num_cols;
	int yco = (id%slice_size)%num_cols;
	int zco = id/slice_size;	
	
	int dest_xco = (dest_id%slice_size)/num_cols;
	int dest_yco = (dest_id%slice_size)%num_cols;
	int dest_zco = dest_id/slice_size;
	
	if(dest_xco == xco && dest_yco == yco && dest_zco == zco)
	   return C;
	
	if(dest_yco < yco) // West bound packet
	{
	      if(ip_dir!=W)
	          set_avail[W]=true;
	      
	      if(dest_xco < xco)//North bound packet
	      {
	          if(yco % 2 == 0)//even column
	          {
	              if((ip_dir!=W))//east-->North turn not allowed
	              {
	                 if((ip_dir!=N) && (!SurfaceborderN(id)))
	                     set_avail[N]=true;
	                 else
	                     set_avail[N]=false;
	              }
	          }
	          if(yco % 2 != 0)//odd column
	          {
	               if(ip_dir!=N && ip_dir!=S)
	                   set_avail[W]=true;
	               else
	                   set_avail[W]=false;
	          }
	          if(dest_zco < zco)//Front bound
	          {
	                if(ip_dir!=F)
	                   set_avail[F]=true;
	                if(zco % 2 != 0)//odd slice
	                {
	                     if(ip_dir!=E && ip_dir!=W && ip_dir!=N && ip_dir!=S)
	                         set_avail[F]=true;
	                     else 
	                         set_avail[F]=false;
	                }
	                if(zco % 2 ==0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	          else if(dest_zco >= zco)//Rear Bound
	          {
	                if(dest_zco > zco)
	                {
	                    if(ip_dir!=R)
	                       set_avail[R]=true;
	                }
	                if(zco % 2 == 0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	      }
	      else if(dest_xco >= xco)//South bound packet
	      {
	          if(dest_xco > xco)
	          {
	             if(yco % 2 == 0)//even column
	             {
	                 if((ip_dir!=W))
	                 { 
	                    if((ip_dir!=S) && (!SurfaceborderS(id)))
	                       set_avail[S]=true;
	                    else
	                       set_avail[S]=false;
	                 }
	             }
	             if(yco % 2 != 0)//odd column
	             {
	                  if(ip_dir!=N && ip_dir!=S)
	                     set_avail[W]=true;
	                  else
	                     set_avail[W]=false;
	             }
	          }
	          if(dest_zco < zco)//Front bound
	          {
	                if(ip_dir!=F)
	                   set_avail[F]=true;
	                if(zco % 2 != 0)//odd slice
	                {
	                     if(ip_dir!=E && ip_dir!=W && ip_dir!=N && ip_dir!=S)
	                         set_avail[F]=true;
	                     else 
	                         set_avail[F]=false;
	                }
	                if(zco % 2 ==0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	          else if(dest_zco >= zco)//Rear Bound
	          {
	                if(dest_zco > zco)
	                {
	                   if(ip_dir!=R)
	                      set_avail[R]=true;
	                }
	                if(zco % 2 == 0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	      }	
	}//end of if(dest_yco < yco)
	
	else if(dest_yco >= yco)//East Bound
	{
	    if(dest_yco > yco)
	    {
	       if(ip_dir!=E)
	          set_avail[E]=true;
	    }
	    
	    if(dest_xco < xco)//North bound
	    {
	          if(yco % 2 == 0)//even column
	          {
	              if((ip_dir!=W))
	              {
	                 if((ip_dir!=N) && (!SurfaceborderN(id)))
	                     set_avail[N]=true;
	                 else
	                     set_avail[N]=false;
	              }
	          }
	        
	          if(dest_zco < zco)//Front bound
	          {
	                if(ip_dir!=F)
	                   set_avail[F]=true;
	                if(zco % 2 != 0)//odd slice
	                {
	                     if(ip_dir!=E && ip_dir!=W && ip_dir!=N && ip_dir!=S)
	                         set_avail[F]=true;
	                     else 
	                         set_avail[F]=false;
	                }
	                if(zco % 2 ==0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	          else if(dest_zco >= zco)//Rear Bound
	          {
	                if(dest_zco > zco)
	                {
	                   if(ip_dir!=R)
	                      set_avail[R]=true;
	                }
	                if(zco % 2 == 0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	    }
	    else if(dest_xco >= xco)//South bound
	    {
	          if(dest_xco > xco)
	          {
	             if(yco % 2 == 0)//even column
	             {
	                if((ip_dir!=W))
	                {
	                   if((ip_dir!=S) && (!SurfaceborderS(id)))
	                       set_avail[S]=true;
	                   else
	                       set_avail[S]=false;
	                }
	             }
	          }
	          if(dest_zco < zco)//Front bound
	          {
	                if(ip_dir!=F)
	                   set_avail[F]=true;
	                if(zco % 2 != 0)//odd slice
	                {
	                     if(ip_dir!=E && ip_dir!=W && ip_dir!=N && ip_dir!=S)
	                         set_avail[F]=true;
	                     else 
	                         set_avail[F]=false;
	                }
	                if(zco % 2 ==0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	          else if(dest_zco >= zco)//Rear Bound
	          {
	                if(dest_zco > zco)
	                {
	                    if(ip_dir!=R)
	                        set_avail[R]=true;
	                }
	                if(zco % 2 == 0)//even slice
	                {
	                    if(ip_dir == F)//up(rear) -> E/W/S/F turns are not allowed
	                    {
	                        set_avail[E]=false;
	                        set_avail[N]=false;
	                        set_avail[S]=false;
	                        set_avail[W]=false;
	                    }
	                }
	          }
	    }
	
	}
	
	for(UI i = 0; i < 6; i++) 
	{
		if(set_avail[i])
		{
			return i;
		}
	}
	
}

////////////////////////////////////////////////
/// Method containing any initializations
/// inherited from base class router
////////////////////////////////////////////////
// may be empty
// definition must be included even if empty, because this is a virtual function in base class
void OddEven3D_router::initialize() {

}

// for dynamic linking
extern "C" {
router *maker() {
	return  new OddEven3D_router;
}
}
