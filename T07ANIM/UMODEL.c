/* FILENAME: UMODEL.C
 * PROGRAMMER: MR3
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"
#include "render.h"


typedef struct tagmr3UNIT_MODEL
{
  MR3_UNIT_BASE_FIELDS;
  mr3GOBJ Model; 
} mr3UNIT_MODEL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "model" - ��� ������ ��������:
 *       MR3UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MR3_AnimUnitInit( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  MR3_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'MR3_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "model" - ��� ������ ��������:
 *       MR3UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���.
 */

static VOID MR3_AnimUnitClose( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  MR3_RndGObjFree(&Uni->Model);
} /* End of 'MR3_AnimUnitClose' function */

/* ������� �������� ������� ��������.
 *   - ��������� �� "model" - ��� ������ ��������:
 *       MR3UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���. 
 */
static VOID MR3_AnimUnitRender( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{  
  INT i, j;
  MR3_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      MR3_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.1, 0.1, 0.1)),
          MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
          MatrTranslate(j * 1.30, 0, i * 1.30 + 100 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      MR3_RndGObjDraw(&Uni->Model);
    }
} /* End of 'MR3_AnimUnitRender' function */

/* creating of model.
 * ���������: no
 * ������������ ��������: ���.
 */
mr3UNIT * MR3_UnitModelCreate( VOID )
{
  mr3UNIT_MODEL *Uni;

  if ((Uni = (VOID *)MR3_AnimUnitCreate(sizeof(mr3UNIT_MODEL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)MR3_AnimUnitInit;
  Uni->Close = (VOID *)MR3_AnimUnitClose;
  Uni->Render = (VOID *)MR3_AnimUnitRender;
  return (mr3UNIT *)Uni;
} /* End of 'MR3_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */