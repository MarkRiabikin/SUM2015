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

/* ������� */
extern MATR
  MR3_RndMatrWorld,
  MR3_RndMatrView,
  MR3_RndMatrProj;

/* ��������� ������������� */
extern DBL
  MR3_RndWp, MR3_RndHp,    /* ������� ������� ������������� */
  MR3_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT MR3_RndProg;

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