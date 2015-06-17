/* FILENAME: GEOM.C
 * PROGRAMMER: MR3
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       mr3PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT MR3_GeomAddPrim( mr3GEOM *G, mr3PRIM *Prim )
{
  mr3PRIM *new_bulk;

  /* �������� ������ ��� ����� ������ ���������� */
  if ((new_bulk = malloc(sizeof(mr3PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* �������� ������ ��������� */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(mr3PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* ��������� �� ����� ������ ���������� */
  G->Prims = new_bulk;

  /* ��������� ����� ������� */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'MR3_GeomAddPrim' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 * ������������ ��������: ���.
 */
VOID MR3_GeomFree( mr3GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      MR3_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(mr3GEOM));
} /* End of 'MR3_GeomFree' function */

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 * ������������ ��������: ���.
 */
VOID MR3_GeomDraw( mr3GEOM *G )
{
  INT i, loc;

  /* �������� ���������� ������ */
  glUseProgram(MR3_RndProg);
  loc = glGetUniformLocation(MR3_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  /* ������ ������������ ������� */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (MR3_MtlLib[G->Prims[i].MtlNo].Kt == 1)
    {
      /* �������� ����� ������� ����� */
      glUseProgram(MR3_RndProg);
      loc = glGetUniformLocation(MR3_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      
      MR3_PrimDraw(&G->Prims[i], i);
    }

  /* ������ ���������� ������� */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (MR3_MtlLib[G->Prims[i].MtlNo].Kt != 1)
    {
      /* �������� ����� ������� ����� */
      glUseProgram(MR3_RndProg);
      loc = glGetUniformLocation(MR3_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      MR3_PrimDraw(&G->Prims[i], 1);
    }
} /* End of 'MR3_GeomDraw' function */

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       mr3GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL MR3_GeomLoad( mr3GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];
  static CHAR
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  memset(G, 0, sizeof(mr3GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(MR3_RndPrimMatrConvert));

  /* ������ ��������� */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* ������ ���������� ���������� � ������� */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* ������ � ��������� ���������� ���������� */
  _makepath(path_buffer, drive, dir, MtlName, "");
  MR3_MtlLoad(path_buffer);

  /* ������ ��������� */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    mr3VERTEX *Vert;
    mr3PRIM P;

    /* ������ ���������� ������ � �������� */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* ������ ��� ��������� �������� ��������� */
    fread(MtlName, 1, 300, F);

    /* �������� ������ ��� ������� � ������� */
    if ((Vert = malloc(sizeof(mr3VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* ������ ������ */
    fread(Vert, sizeof(mr3VERTEX), nv, F);
    /* ������������ ��������� */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = PointTransform(Vert[j].P, MR3_RndPrimMatrConvert);
      Vert[j].N = VectorTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* ������� � �������� */
    MR3_PrimCreate(&P, MR3_PRIM_TRIMESH, nv, ni, Vert, Ind);
    P.MtlNo = MR3_MtlFind(MtlName);

    free(Vert);

    /* ��������� �������� � ������� */
    MR3_GeomAddPrim(G, &P);
  }
  fclose(F);
  MR3_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'MR3_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
