/* FILENAME: PRIM.C
 * PROGRAMMER: MR3
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */

#include "anim.h"
#include "render.h"

/* ������� ��������� ��������� ��� �������� */
MATR MR3_RndPrimMatrConvert = MR3_UNIT_MATR;


MATR
  MR3_RndMatrWorld = MR3_UNIT_MATR,
  MR3_RndMatrView = MR3_UNIT_MATR,
  MR3_RndMatrWorldView = MR3_UNIT_MATR,
  MR3_RndMatrWorldViewProj = MR3_UNIT_MATR;

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
VOID MR3_PrimDraw( mr3PRIM *Prim )
{
  INT loc;
  MATR M;

  MR3_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(MR3_RndMatrWorld, MR3_RndMatrView), MR3_RndMatrWorldView);

  /* ��������� ��� ��������, ���� ��� �������� */
  glLoadMatrixf(MR3_RndMatrWorldViewProj.A[0]);

  /* ������ ������������ */
  glBindVertexArray(Prim->VA);
  glUseProgram(MR3_RndProg);

  loc = glGetUniformLocation(MR3_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, &(MR3_RndMatrWorld.A[0][0]));
  loc = glGetUniformLocation(MR3_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrView.A[0]);
  loc = glGetUniformLocation(MR3_RndProg, "MatrWorldView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrWorldView.A[0]);
  loc = glGetUniformLocation(MR3_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MR3_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(MR3_RndMatrWorld, MR3_RndMatrView)));
  loc = glGetUniformLocation(MR3_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(MR3_RndMatrWorld, MR3_RndMatrView);
  loc = glGetUniformLocation(MR3_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(MR3_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, MR3_Anim.Time);

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
