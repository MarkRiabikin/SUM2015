/* FILENAME: RENDER.H
 * PROGRAMMER: MR3
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "vec.h"

/* ������� */
extern MATR
  MR3_RndMatrWorld,
  MR3_RndMatrView,
  MR3_RndMatrWorldView,
  MR3_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR MR3_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  MR3_RndWp, MR3_RndHp,    /* ������� ������� ������������� */
  MR3_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT MR3_RndProg;

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

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagmr3MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} mr3MATERIAL;

/* ���������� ������� ���������� */
#define MR3_MATERIAL_MAX 300
extern INT MR3_MtlLibSize;
extern mr3MATERIAL MR3_MtlLib[MR3_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       mr3MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT MR3_MtlAdd( mr3MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT MR3_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT MR3_MtlLoad( CHAR *FileName );


/***
 * ������ � �����������
 ***/

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
    NumOfI,      /* ���������� �������� ��� ������ ��������� */
    MtlNo;       /* ����� ��������� �� ���������� */
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

/* �������������� ������ - ����� ���������� */
typedef struct tagmr3GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  mr3PRIM *Prims;      /* ������ ���������� */
} mr3GEOM;

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

#endif /* __RENDER_H_ */