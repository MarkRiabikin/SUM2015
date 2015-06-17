/* FILENAME: PRIM.C
 * PROGRAMMER: MR3
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */


#include "anim.h"
#include "render.h"
#include "image.h"

/* ������� ��������� ��������� ��� �������� */
MATR MR3_RndPrimMatrConvert = MR3_UNIT_MATR;

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
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
VOID MR3_PrimCreate( mr3PRIM *Prim, mr3PRIM_TYPE Type,
                     INT NoofV, INT NoofI, mr3VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* ������� ������ OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* ������ �������� ������ ������ */
  glBindVertexArray(Prim->VA);
  /* ������ �������� ����� ������ */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* ������� ������ */
  glBufferData(GL_ARRAY_BUFFER, sizeof(mr3VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* ������ �������� ����� �������� */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* ������� ������ */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* ������ ������� ������ */
  /*                    layout,
   *                       ���������� ���������,
   *                          ���,
   *                                    ���� �� �����������,
   *                                           ������ � ������ ������ �������� ������,
   *                                                           �������� � ������ �� ������ ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(mr3VERTEX), (VOID *)0); /* ������� */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(mr3VERTEX), (VOID *)sizeof(VEC)); /* �������� */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(mr3VERTEX), (VOID *)(sizeof(VEC) + sizeof(mr3UV))); /* ������� */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(mr3VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(mr3UV))); /* ���� */

  /* �������� ������ ��������� (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* ����������� �� ������� ������ */
  glBindVertexArray(0);
} /* End of 'MR3_PrimCreate' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID MR3_PrimFree( mr3PRIM *Prim )
{
  /* ������ �������� ������ ������ */
  glBindVertexArray(Prim->VA);
  /* "���������" ������ */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* ������ ���������� ������ ������ */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(mr3PRIM));
} /* End of 'MR3_PrimFree' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       mr3PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID MR3_PrimDraw( mr3PRIM *Prim, INT i )
{
  INT loc;
  MATR M;
  srand(30);
  MR3_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(MR3_RndMatrWorld, MR3_RndMatrView), MR3_RndMatrProj);

  /* ��������� ��� ��������, ���� ��� �������� */
  glLoadMatrixf(MR3_RndMatrWorldViewProj.A[0]);

  /* ������ ������������ */
  glBindVertexArray(Prim->VA);
  glUseProgram(MR3_RndProg);

  loc = glGetUniformLocation(MR3_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(MR3_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrView.A[0]);
  loc = glGetUniformLocation(MR3_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrProj.A[0]);
  loc = glGetUniformLocation(MR3_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(MR3_RndMatrWorld, MR3_RndMatrView)));
  loc = glGetUniformLocation(MR3_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
                                                      
  M = MatrTranspose(MatrInverse(MR3_RndMatrWorld));
  loc = glGetUniformLocation(MR3_RndProg, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(MR3_RndMatrWorld, MR3_RndMatrView);
  loc = glGetUniformLocation(MR3_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(MR3_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, MR3_Anim.Time);

  loc = glGetUniformLocation(MR3_RndProg, "Rand");
  if (loc != -1)
    glUniform1f(loc, 2 * (float)rand() / RAND_MAX - 1);

  /* ���������� ��������� */
  loc = glGetUniformLocation(MR3_RndProg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &MR3_MtlLib[Prim->MtlNo].Ka.X);
  loc = glGetUniformLocation(MR3_RndProg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &MR3_MtlLib[Prim->MtlNo].Kd.X);
  loc = glGetUniformLocation(MR3_RndProg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &MR3_MtlLib[Prim->MtlNo].Ks.X);
  loc = glGetUniformLocation(MR3_RndProg, "Kp");
  if (loc != -1)
    glUniform1f(loc, MR3_MtlLib[Prim->MtlNo].Kp);
  loc = glGetUniformLocation(MR3_RndProg, "Kt");
  if (loc != -1)
    glUniform1f(loc, MR3_MtlLib[Prim->MtlNo].Kt);

  loc = glGetUniformLocation(MR3_RndProg, "IsTextureUse");
  if (MR3_MtlLib[Prim->MtlNo].TexId == 0)
    glUniform1f(loc, 0);
  else
  {
    glUniform1f(loc, 1);
    glBindTexture(GL_TEXTURE_2D, MR3_MtlLib[Prim->MtlNo].TexId);
  }

  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == MR3_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'MR3_PrimDraw' function */

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
BOOL MR3_PrimCreatePlane( mr3PRIM *Prim, VEC Du, VEC Dv, INT N, INT M )
{
  INT i, j;
  VEC norm, Loc;
  INT *Ind, *iptr;
  mr3VERTEX *V, *ptr;

  Loc = VecSet(0, 0, 0);
  memset(Prim, 0, sizeof(mr3PRIM));

  if ((V = malloc(sizeof(mr3VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* ��������� ������� */
  norm = VecNormalize(VecCrossVec(Du, Dv));
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      ptr->P = VecAddVec(Loc,
                 VecAddVec(VecMulNum(Du, j / (M - 1.0)),
                           VecMulNum(Dv, i / (N - 1.0))));
      ptr->C = MR3_RndPrimDefaultColor;
      ptr->N = norm;
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* ��������� ������� */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* ������� */
      *iptr++ = i * M + j + M;
      /* ������ */
      *iptr++ = i * M + j;
    }
    /* ��������� ������ ������� ��������� */
    *iptr++ = 0xFFFFFFFF;
  }

  MR3_PrimCreate(Prim, MR3_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'MR3_PrimCreatePlane' function */

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
BOOL MR3_PrimCreateSphere( mr3PRIM *Prim, VEC C, FLT R, INT N, INT M )
{
 

} /* End of 'MR3_PrimCreateSphere' function */

/* END OF 'PRIM.C' FILE */
