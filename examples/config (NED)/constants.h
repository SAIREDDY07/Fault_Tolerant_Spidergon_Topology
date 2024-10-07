
/*
 * constants.h
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

///////////////////////////////////////////////////////
/// \file constants.h
/// \brief Defines constant parameters for NIRGAM
///////////////////////////////////////////////////////

#ifndef _TYPES_H_
#define _TYPES_H_

/// current time
#define CURRENT_TIME sc_simulation_time()

/// number of virtual channels
#define NUM_VCS 6

// parameters for topology
/// maximum number of rows
#define MAX_NUM_ROWS 16
/// maximum number of columns
#define MAX_NUM_COLS 16
/// maximum number of slice
#define MAX_NUM_SLICE 4

/// maximum number of tiles
#define MAX_NUM_TILES (MAX_NUM_ROWS * MAX_NUM_COLS * MAX_NUM_SLICE)

// parameters for NWTile
/// number of neighbors of a general tile in mesh/torus
#define NUM_NB 4
/// number of input channels in a general tile in mesh/torus
#define NUM_IC 5
/// number of output channels in a general tile in mesh/torus
#define NUM_OC 5

// parameters for mesh (non-toroidal) topology
/// number of neighbors of a corner tile in mesh
#define NUM_NB_C 2
/// number of input channels in a corner tile in mesh
#define NUM_IC_C 3
/// number of output channels in a corner tile in mesh
#define NUM_OC_C 3

// parameters for mesh (non-toroidal) topology
/// number of neighbors of a border tile in mesh
#define NUM_NB_B 3
/// number of input channels in a border tile in mesh
#define NUM_IC_B 4
/// number of output channels in a border tile in mesh
#define NUM_OC_B 4

//////////////////////// for 3D NOC  ////////////////////////////////////////////////////////////////////
// parameters for NWTile
/// number of neighbors of a general tile in 3D mesh
#define NUM3D_NB 6
/// number of input channels in a general tile in 3D mesh
#define NUM3D_IC 7
/// number of output channels in a general tile in 3D mesh
#define NUM3D_OC 7

// parameters for mesh (non-toroidal) topology
/// number of neighbors of a corner tile in 3D mesh
#define NUM3D_NB_C 3
/// number of input channels in a corner tile in 3D mesh
#define NUM3D_IC_C 4
/// number of output channels in a corner tile in 3D mesh
#define NUM3D_OC_C 4

/// number of neighbors of a Edge border tile in 3D mesh
#define NUM3D_NB_EB 4
/// number of input channels in a Surface border tile in 3D mesh
#define NUM3D_IC_EB 5
/// number of output channels in a Surface border tile in 3D mesh
#define NUM3D_OC_EB 5

// parameters for mesh (non-toroidal) topology
/// number of neighbors of a Surface border tile in 3D mesh
#define NUM3D_NB_SB 5
/// number of input channels in a Surface border tile in 3D mesh
#define NUM3D_IC_SB 6
/// number of output channels in a Surface border tile in 3D mesh
#define NUM3D_OC_SB 6
////// end for 3D NoC ///////////////////////////////////////////////////////////////////////////////////

// direction ids
/// North direction
#define N 0
/// South direction
#define S 1
/// East direction
#define E 2
/// West direction
#define W 3
/// Front slice direction
#define F 4
/// Rear slice direction
#define R 5

/// Core direction
#define C 6
/// not defined
#define ND 7

// buffer parameters
#define	BUF_BITSIZE	16

#define	SBBUF_BITSIZE	64	//zzz
/// maximum buffer depth

//#define PKT_SIZE 8 //zzz

/// maximum buffer depth
#define	MAX_NUM_BUFS	64

#define	NUM_BUFS_SB	128	//zzz

// parameters for bitwidth
#define ADDR_SIZE 32
#define VCS_BITSIZE 6

/////////////////////////////////////////
/// types of flits: HEAD, DATA, TAIL, HDT
////////////////////////////////////////
enum flit_type{
	HEAD,
	DATA,
	TAIL,
	HDT
};

/////////////////////////////////////////
/// types of packets: ANT, NOC
////////////////////////////////////////
enum pkt_type {
	ANT,
	NOC,
	EST
};

/////////////////////////////////////////
/// types of ant packets: FORWARD, BACKWARD
////////////////////////////////////////
enum ant_type {
	FORWARD,
	BACKWARD
};

////////////////////////////////////////////////
/// types of routing algorithms: SOURCE, XY, OE
////////////////////////////////////////////////
enum routing_type {
	SOURCE,
	XY,
	XYX,
	OE,
	QRT,
	DYAD,
	MAXY,
	FTXY,
	PROM,
	ER,
	CRT,
	XYZ,		// for 3D NoC
	XYZR,		// for 3D NoC
	ZXY		// for 3D NoC
	};

/////////////////////////////////////////
/// types of request to controller
////////////////////////////////////////
enum request_type {
	NONE,
	ROUTE,
	UPDATE,
	ESTUPDATE
};

/////////////////////////////////////////
/// types of topology: MESH, TORUS
////////////////////////////////////////
enum topology {
	MESH,
	TORUS,
	MESH3D
};



#define UI  unsigned int
#define UL  unsigned long
#define ULL unsigned long long

#define borderN(ID) ((ID) < num_cols)
#define borderS(ID) ((ID) >= num_tiles - num_cols)
#define borderE(ID) (((ID) + 1) % num_cols == 0)
#define borderW(ID) (((ID) % num_cols) == 0)

#define border(ID) (borderN(ID) || borderS(ID) || borderE(ID) || borderW(ID))

#define cornerNW(ID) ((ID) == 0)
#define cornerNE(ID) ((ID) == num_cols - 1)
#define cornerSW(ID) ((ID) == num_tiles - num_cols)
#define cornerSE(ID) ((ID) == num_tiles - 1)

#define corner(ID) (cornerNW(ID) || cornerNE(ID) || cornerSW(ID) || cornerSE(ID))
#define ESTIMATE_BUFFER_SIZE 20
/////////////////// for 3D NOC  /////////////////////////////////////////////////////////////////////////////////////////
#define EdgeNW(ID) ((ID%slice_size) == 0)
#define EdgeNE(ID) ((ID%slice_size) == num_cols-1)
#define EdgeNF(ID) (ID < num_cols)
#define EdgeNR(ID) ((ID >= num_tiles-slice_size) && (ID < num_tiles-slice_size + num_cols) )

#define EdgeSW(ID) ((ID%slice_size) == slice_size-num_cols)
#define EdgeSE(ID) ((ID%slice_size) == slice_size-1)
#define EdgeSF(ID) (ID >= slice_size-num_cols && ID < slice_size) 
#define EdgeSR(ID) (ID >= num_tiles-num_cols)

#define EdgeFE(ID) ((ID%num_cols) == num_cols-1 && ID < slice_size)
#define EdgeFW(ID) ((ID%num_cols) == 0 && ID < slice_size)
#define EdgeRE(ID) ((ID%num_cols) == num_cols-1 && (ID >= num_tiles-slice_size) )
#define EdgeRW(ID) ((ID%num_cols) == 0 && (ID >= num_tiles-slice_size) )

#define Edge3D(ID) ( EdgeNW(ID) || EdgeNE(ID) || EdgeNF(ID) || EdgeNR(ID) || EdgeSW(ID) || EdgeSE(ID) || EdgeSF(ID) || EdgeSR(ID) || EdgeFE(ID) || EdgeFW(ID) ||EdgeRE(ID)||EdgeRW(ID) )

#define SurfaceborderN(ID) ((ID%slice_size) < num_cols)
#define SurfaceborderS(ID) ((ID%slice_size) >= slice_size - num_cols)
#define SurfaceborderE(ID) ((ID%num_cols)  == num_cols - 1)
#define SurfaceborderW(ID) ((ID % num_cols) == 0)
#define SurfaceborderF(ID) (ID < slice_size)
#define SurfaceborderR(ID) (ID >= num_tiles-slice_size)

#define Surfaceborder3D(ID) (SurfaceborderN(ID) || SurfaceborderS(ID) || SurfaceborderE(ID) || SurfaceborderW(ID) || SurfaceborderF(ID) || SurfaceborderR(ID))

#define cornerNWF(ID) ( EdgeNW(ID) && EdgeNF(ID) )
#define cornerNEF(ID) ( EdgeNE(ID) && EdgeNF(ID) )
#define cornerNWR(ID) ( EdgeNW(ID) && EdgeNR(ID) )
#define cornerNER(ID) ( EdgeNE(ID) && EdgeNR(ID) )
#define cornerSWF(ID) ( EdgeSW(ID) && EdgeSF(ID) )
#define cornerSEF(ID) ( EdgeSE(ID) && EdgeSF(ID) )
#define cornerSWR(ID) ( EdgeSW(ID) && EdgeSR(ID) )
#define cornerSER(ID) ( EdgeSE(ID) && EdgeSR(ID) )

#define corner3D(ID) (cornerNWF(ID) || cornerNEF(ID) || cornerNWR(ID) || cornerNER(ID) || cornerSWF(ID) || cornerSEF(ID) || cornerSWR(ID) || cornerSER(ID))

#define general3D(ID) (!(Edge3D(ID) || Surfaceborder3D(ID) || corner3D(ID) ) )
//################## end 3D NOC
#endif
