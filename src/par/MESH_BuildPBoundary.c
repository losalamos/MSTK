#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "MSTK.h"
#include "MSTK_private.h"

#ifdef __cplusplus
extern "C" {
#endif
  /* 
     This function builds the partition boundary for the submesh.
     All faces(surface mesh) or regions(volume mesh) are marked 
     as PINTERIOR, processor boundary vertices/edges/faces
     are marked as PBOUNDARY. 

     Submesh is a subset of mesh generated by MESH_Partition()
     To add PGHOST entities and mark POVERLAP, call MESH_AddGHOST().
 
     Author(s): Duo Wang, Rao Garimella
  */

int MESH_Surf_BuildPBoundary(Mesh_ptr mesh, Mesh_ptr submesh);
int MESH_Vol_BuildPBoundary(Mesh_ptr mesh, Mesh_ptr submesh);

int MESH_BuildPBoundary(Mesh_ptr mesh, Mesh_ptr submesh ) {
  int nf, nr;
  /* basic mesh information */
  nf = MESH_Num_Faces(mesh);
  nr = MESH_Num_Regions(mesh);
 
  if (nr) 
    MESH_Vol_BuildPBoundary(mesh, submesh);
  else if(nf) 
    MESH_Surf_BuildPBoundary(mesh, submesh);
  else {
    MSTK_Report("MESH_BuildPBoundary()","This is not a valid mstk file",MSTK_ERROR);
    exit(-1);
  }
  return 1;
}
  
int MESH_Surf_BuildPBoundary(Mesh_ptr mesh, Mesh_ptr submesh) {
  int idx;
  MVertex_ptr lmv;
  MEdge_ptr lme;
  MFace_ptr lmf;
  MAttrib_ptr l2gatt;

  l2gatt = MESH_AttribByName(submesh,"Local2Global");

  /* Mark all the faces to be interior */
  idx = 0;
  while((lmf = MESH_Next_Face(submesh,&idx)))  
    MF_Set_PType(lmf,PINTERIOR);
  
  /* Loop through edges and label them as being on a partition
     boundary or partition interior (even if an entity is on the
     domain boundary it is considered as being in the interior of the
     partition if it does not have a copy on another processor) */

  idx = 0;
  while((lme = MESH_Next_Edge(submesh,&idx))) {
    MEdge_ptr gme;
    List_ptr lmefaces, gmefaces;

    MEnt_Get_AttVal(lme,l2gatt,0,0,&gme);
    if (!gme)
      MSTK_Report("MESH_BuildPBoundary","Cannot find global edge of local edge",MSTK_ERROR);
      

    lmefaces = ME_Faces(lme);
    gmefaces = ME_Faces(gme);

    /* if the number of neighbor faces is different, it is a boundary edge */
    if(List_Num_Entries(lmefaces) != List_Num_Entries(gmefaces)) {
      ME_Set_PType(lme,PBOUNDARY);

      /* By definition, its vertices must also be on the partition boundary */

      int i;
      for (i = 0; i < 2; i++) {
        lmv = ME_Vertex(lme,i);
        MV_Set_PType(lmv,PBOUNDARY);
      }
    }
    else
      ME_Set_PType(lme,PINTERIOR);
    List_Delete(lmefaces);
    List_Delete(gmefaces);
  }




  /* Loop through vertices and label them as being on a partition
     boundary or partition interior. This catches cases where the
     vertex is to be of PBOUNDARY but none of its connected edges are
     (because they are on a model edge and hence labeled as
     PINTERIOR) */

  idx = 0;
  while((lmv = MESH_Next_Vertex(submesh,&idx))) {
    MVertex_ptr gmv;
    List_ptr lmvedges, gmvedges;

    if (MV_PType(lmv) == PBOUNDARY) continue; /* It has already been labeled */

    MEnt_Get_AttVal(lmv,l2gatt,0,0,&gmv);
    if (!gmv)
      MSTK_Report("MESH_BuildPBoundary","Cannot find global vertex of local edge",MSTK_ERROR);

    lmvedges = MV_Edges(lmv);
    gmvedges = MV_Edges(gmv);

    /* If the number of neighbor faces is different, it is boundary vertex */

    if (List_Num_Entries(lmvedges) != List_Num_Entries(gmvedges))
      MV_Set_PType(lmv,PBOUNDARY);
    else
      MV_Set_PType(lmv,PINTERIOR);

    List_Delete(lmvedges);
    List_Delete(gmvedges);
  }

  return 1;
}



int MESH_Vol_BuildPBoundary(Mesh_ptr mesh, Mesh_ptr submesh) {
  int idx;
  MVertex_ptr lmv;
  MEdge_ptr lme;
  MFace_ptr lmf;
  MRegion_ptr lmr;
  MAttrib_ptr l2gatt;


  l2gatt = MESH_AttribByName(submesh,"Local2Global");

  /* Mark all the regions to be interior */
  idx = 0;
  while((lmr = MESH_Next_Region(submesh,&idx)))  
    MR_Set_PType(lmr,PINTERIOR);
  

  /* Loop through faces and label them as being on a partition
     boundary or partition interior (even if an entity is on the
     domain boundary it is considered as being in the interior of the
     partition if it does not have a copy on another processor) */

  idx = 0;
  while((lmf = MESH_Next_Face(submesh,&idx))) {
    MFace_ptr gmf;
    List_ptr lmfregs, gmfregs;

    MEnt_Get_AttVal(lmf,l2gatt,0,0,&gmf);
    if (!gmf)
      MSTK_Report("MESH_BuildPBoundary","Cannot find global vertex of local vertex",MSTK_ERROR);

    lmfregs = MF_Regions(lmf);
    gmfregs = MF_Regions(gmf);

    /* if the number of neighbor regions is different, it is a boundary face */
    if(List_Num_Entries(lmfregs) != List_Num_Entries(gmfregs)) {
      MF_Set_PType(lmf,PBOUNDARY);

      /* By definition all edges of this face are on the boundary */
      List_ptr fedges = MF_Edges(lmf,1,0);
      int idx2 = 0;
      while ((lme = List_Next_Entry(fedges,&idx2))) {
        ME_Set_PType(lme,PBOUNDARY);
        
        int i;
        for (i = 0; i < 2; i++) {
          lmv = ME_Vertex(lme,i);
          MV_Set_PType(lmv,PBOUNDARY);
        }
      }
      List_Delete(fedges);
    }          
    else
      MF_Set_PType(lmf,PINTERIOR);
    List_Delete(lmfregs);
    List_Delete(gmfregs);
  }

  /* Loop through edges. This catches cases where the vertex is to be
     of PBOUNDARY but none of its connected faces are (because they
     are on a model face and hence labeled as PINTERIOR) */

  idx = 0;
  while((lme = MESH_Next_Edge(submesh,&idx))) {
    MEdge_ptr gme;
    List_ptr lmefaces, gmefaces;

    if (ME_PType(lme) == PBOUNDARY) continue;

    MEnt_Get_AttVal(lme,l2gatt,0,0,&gme);

    lmefaces = ME_Faces(lme);
    gmefaces = ME_Faces(gme);
    
    if (List_Num_Entries(lmefaces) != List_Num_Entries(gmefaces)) {
      ME_Set_PType(lme,PBOUNDARY);

      /* By definition a PBOUNDARY edge's vertices are also PBOUNDARY */
      int i;
      for (i = 0; i < 2; i++) {
        lmv = ME_Vertex(lme,i);
        ME_Set_PType(lmv,PBOUNDARY);
      }
    }
    else
      ME_Set_PType(lme,PINTERIOR);

    List_Delete(lmefaces);
    List_Delete(gmefaces);
  }


  /* Loop through vertices. This catches cases where the vertex is to
     be of PBOUNDARY but none of its connected edges are (because they
     are on a model edge and hence labeled as PINTERIOR)  */

  idx = 0;
  while((lmv = MESH_Next_Vertex(submesh,&idx))) {
    MVertex_ptr gmv;
    List_ptr lmvedges, gmvedges;

    if (MV_PType(lmv) == PBOUNDARY) continue; /* It has already been labeled */

    MEnt_Get_AttVal(lmv,l2gatt,0,0,&gmv);
    if (!gmv)
      MSTK_Report("MESH_BuildPBoundary","Cannot find global vertex of local edge",MSTK_ERROR);

    lmvedges = MV_Edges(lmv);
    gmvedges = MV_Edges(gmv);

    /* If the number of neighbor faces is different, it is boundary vertex */

    if (List_Num_Entries(lmvedges) != List_Num_Entries(gmvedges))
      MV_Set_PType(lmv,PBOUNDARY);
    else
      MV_Set_PType(lmv,PINTERIOR);

    List_Delete(lmvedges);
    List_Delete(gmvedges);
  }

  return 1;
}

  
#ifdef __cplusplus
}
#endif

