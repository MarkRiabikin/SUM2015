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

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "model" - сам объект анимации:
 *       MR3UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitInit( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  MR3_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'MR3_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "model" - сам объект анимации:
 *       MR3UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID MR3_AnimUnitClose( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  MR3_RndGObjFree(&Uni->Model);
} /* End of 'MR3_AnimUnitClose' function */

/* Функция создания объекта анимации.
 *   - указатель на "model" - сам объект анимации:
 *       MR3UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет. 
 */
static VOID MR3_AnimUnitRender( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  MR3_RndMatrView = MatrView(VecSet(30, 30, 30),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  MR3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
      MatrScale(0.1, 0.1, 0.1)),
      MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
      MatrTranslate(0, 0, 100 * Ani->JZ));
  MR3_RndGObjDraw(&Uni->Model);
} /* End of 'MR3_AnimUnitRender' function */

/* creating of model.
 * АРГУМЕНТЫ: no
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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