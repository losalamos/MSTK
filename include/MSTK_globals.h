#ifndef _H_MSTK_GLOBALVARS
#define _H_MSTK_GLOBALVARS

#include "MSTK_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Templates for TETS, PYRAMIDS, PRISMS and HEXS (move to global file?) */

const MRType MSTK_nv2rtype[9] = {RUNKNOWN, RUNKNOWN, RUNKNOWN, RUNKNOWN,
				  TET, PYRAMID, PRISM, RUNKNOWN, HEX};

/* Number of faces for standard regions */
const int MSTK_nrf_template[5] = {0,4,5,5,6};

/* Number of edges for standard regions */
const int MSTK_nre_template[5] = {0,6,8,9,12};

/* Face-vertex templates for standard regions */
const int MSTK_rfv_template[5][6][5] = 
  {{{0,-1,-1,-1,-1},{0,-1,-1,-1,-1},{0,-1,-1,-1,-1},{0,-1,-1,-1,-1},
    {0,-1,-1,-1,-1},{0,-1,-1,-1,-1}},
   {{3,0,1,2,-1},{3,0,1,3,-1},{3,1,2,3,-1},{3,0,3,2,-1},{0,-1,-1,-1,-1},
    {0,-1,-1,-1,-1}},
   {{4,0,1,2,3},{3,0,1,4,-1},{3,1,2,4,-1},{3,2,3,4,-1},{3,3,0,4,-1},
    {0,-1,-1,-1,-1}},
   {{3,0,1,2,-1},{3,3,4,5,-1},{4,0,1,4,3},{4,1,2,5,4},{4,2,0,3,5},
    {0,-1,-1,-1,-1}},
   {{4,0,1,2,3},{4,4,5,6,7},{4,0,1,5,4},{4,1,2,6,5},{4,2,3,7,6},
    {4,3,0,4,7}}
  };

/* Face direction templates for standard regions */
const int MSTK_rfdir_template[5][6] = 
  {{-1,-1,-1,-1,-1,-1},{0,1,1,1,-1,-1},{0,1,1,1,1,-1},{0,1,1,1,1,-1},
{0,1,1,1,1,1}};

/* Face-edge templates for standard regions */
const int MSTK_rfe_template[5][6][5] = 
  {{{0,-99,-99,-99,-99},{0,-99,-99,-99,-99},{0,-99,-99,-99,-99},
{0,-99,-99,-99,-99},{0,-99,-99,-99,-99},{0,-99,-99,-99}},
   {{3,0,1,2,-99},{3,0,4,-3,-99},{3,1,5,-4,-99},{3,2,3,-5,-99},
    {0,-99,-99,-99,-99},{0,-99,-99,-99,-99}},
   {{4,0,1,2,3},{3,0,5,-4,-99},{3,1,6,-5,-99},{3,2,7,-6,-99},
    {3,3,4,-7,-99},{0,-99,-99,-99,-99}},
   {{3,0,1,2,-99},{3,3,4,5,-99},{4,0,7,-3,-6},{4,1,8,-4,-7},{4,2,6,-5,-8},
    {0,-99,-99,-99,-99}},
   {{4,0,1,2,3},{4,4,5,6,7},{4,0,9,-4,-8},{4,1,10,-5,-9},{4,2,11,-6,-10},
    {4,3,8,-7,-11}}};

/* Edge-vertex templates for standard regions */
const int MSTK_rev_template[5][12][2] = 
  {{{-99,-99},{-99,-99},{-99,-99},{-99,-99},{-99,-99},{-99,-99},
    {-99,-99},{-99,-99},{-99,-99},{-99,-99},{-99,-99},{-99,-99}},
   {{0,1},{1,2},{2,0},{0,3},{1,3},{2,3},{-99,-99},{-99,-99},{-99,-99},
    {-99,-99},{-99,-99},{-99,-99}},
   {{0,1},{1,2},{2,3},{3,0},{0,4},{1,4},{2,4},{3,4},{-99,-99},{-99,-99},
    {-99,-99},{-99,-99}},
   {{0,1},{1,2},{2,0},{3,4},{4,5},{5,3},{0,3},{1,4},{2,5},{-99,-99},
    {-99,-99},{-99,-99}},
   {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},
    {3,7}}};

#ifdef __cplusplus
}
#endif


#endif
