#include "PROM2D_router.h"
#include "../../config/extern.h"
#include "stdlib.h"
#include "time.h"

void PROM2D_router::setID(UI id_tile) {
    id = id_tile;
    initialize();
}

UI PROM2D_router::calc_next(UI ip_dir, ULL source_id, ULL dest_id, struct congestion cong_status) {

    float fmax = 4;

    int xco = id / num_cols;
    int yco = id % num_cols;
//  printf("\nTile ID : <%d>; Ip_dir : %d", id, ip_dir);

    int dest_xco = dest_id / num_cols;
    int dest_yco = dest_id % num_cols;
//  printf("\nDestination Coordinates : <%d, %d>", dest_xco, dest_yco);

    int source_xco = source_id / num_cols;
    int source_yco = source_id % num_cols;

    int x = dest_xco - xco;
    int y = dest_yco - yco;

    int m = dest_xco - source_xco;
    int n = dest_yco - source_yco;

    if (x < 0) x = -x;
    if (y < 0) y = -y;

    if (m < 0) m = -m;
    if (n < 0) n = -n;

    if (x == 0 && y == 0) return C;

    float f;
    float p1, p2;
    f = (float)(fmax * (m + 1) * (n + 1)) / (num_rows * num_cols);

    if (id == source_id) {  // if current node is the source node, then ratio is x+f:y+f
        p1 = x;
        p2 = y;
    } else {  // current node is an intermediate node
              // three possibilities:
              // 1. packet arrives on the X-axis ingress,
              // 2. packet arrives on the Y-axis ingress

        if (ip_dir == N || ip_dir == S) {  // 1. X-axis ingress
            if (x == 0)
                p1 = 0;
            else
                p1 = x + f;

            p2 = y;
        } else if (ip_dir == E || ip_dir == W) {  // 2. Y-axis ingress
            p1 = x;

            if (y == 0)
                p2 = y;
            else
                p2 = y + f;
        }
    }

    // compare probabilities and return direction...
    if (p1 >= p2) {
        if (xco < dest_xco)
            return S;
        else
            return N;
    } else {
        if (yco < dest_yco)
            return E;
        else
            return W;
    }
}

void PROM2D_router::initialize() {
    // Any required initializations
}

void PROM2D_router::update_power(int i, double p) {
    // Update power consumption details
}

// for dynamic linking
extern "C"
{
    router *maker()
    {
        return new PROM2D_router;
    }
}
