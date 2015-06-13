/* FILENAME: RENDER.H
 * PROGRAMMER: MR3
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagmr3GOBJ
{
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfV,      /* ���������� ������ */
    NumOfF;      /* ���������� ������ */
} mr3GOBJ;

typedef struct tagVERTEX
{
  VEC P;   
  COLOR C; 
} VERTEX;

/* ������� */
extern MATR
  MR3_RndMatrWorld,
  MR3_RndMatrView,
  MR3_RndMatrProj;

/* ��������� ������������� */
extern DBL
  MR3_RndWp, MR3_RndHp,    /* ������� ������� ������������� */
  MR3_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� ��������� ��������� ��� �������� */
extern MATR MR3_RndPrimMatrConvert;

/* ������ �� ��������� */
extern UINT MR3_RndProg;

/* ���� ���������� */
typedef enum tagmr3PRIM_TYPE
{
  MR3_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  MR3_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} mr3PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagmr3PRIM
{
  mr3PRIM_TYPE Type; /* ��� ��������� (MR3_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI;      /* ���������� �������� ��� ������ ��������� */
} mr3PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagmr3UV
{
  FLT U, V; /* ���������� ���������� */
} mr3UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (mr3UV) ����������� ����������.
 */
__inline mr3UV UVSet( FLT U, FLT V )
{
  mr3UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagmr3VERTEX
{
  VEC P;    /* ������� ������� */
  mr3UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} mr3VERTEX;

/* �������������� ������ - ����� ���������� */
typedef struct tagmr3GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  mr3PRIM *Prims;      /* ������ ���������� */
} mr3GEOM;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 *   - ��� ��������� (MR3_PRIM_***):
 *       mr3PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       mr3VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID MR3_PrimCreate( mr3PRIM *Prim, mr3PRIM_TYPE Type,
                     INT NoofV, INT NoofI, mr3VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID MR3_PrimFree( mr3PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID MR3_PrimDraw( mr3PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL MR3_PrimCreatePlane( mr3PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL MR3_PrimCreateSphere( mr3PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       mr3PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL MR3_PrimLoad( mr3PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/


/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       mr3PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT MR3_GeomAddPrim( mr3GEOM *G, mr3PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 * ������������ ��������: ���.
 */
VOID MR3_GeomFree( mr3GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 * ������������ ��������: ���.
 */
VOID MR3_GeomDraw( mr3GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL MR3_GeomLoad( mr3GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT MR3_TextureLoad( CHAR *FileName );


/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       mr3GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL MR3_RndGObjLoad( mr3GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       mr3GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID MR3_RndGObjFree( mr3GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       mr3GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID MR3_RndGObjDraw( mr3GOBJ *GObj );

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT MR3_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID MR3_ShaderFree( UINT PrgId );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */