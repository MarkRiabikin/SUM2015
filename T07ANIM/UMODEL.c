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

  mr3GEOM Model; 
  mr3GEOM Geom; 
  mr3PRIM Pr;
  INT TextId;  
} mr3UNIT_MODEL;


static VOID MR3_AnimUnitInit( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  mr3VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  MR3_PrimCreate(&Uni->Pr, MR3_PRIM_TRIMESH, 4, 6, V, I);

  MR3_TextureLoad("destroy.bmp");
  //MR3_RndPrimMatrConvert = MatrMulMatr(MatrScale(1, 1, 1), MatrTranslate(-50, 0, 50));
  //R3_PrimCreatePlane(&Uni->Pr, VecSet(1000, 0, 0), VecSet(0, 0, 1000), 30, 30);
  MR3_RndPrimMatrConvert = MatrMulMatr(MatrScale(15, 15, 15), MatrTranslate(0, 0, 0));
  MR3_GeomLoad(&Uni->Model, "E:\\Models\\Avent\\avent.G3D" );

} /* End of 'MR3_AnimUnitInit' function */

/* Ôóíêöèÿ äåèíèöèàëèçàöèè îáúåêòà àíèìàöèè.
 * ÀÐÃÓÌÅÍÒÛ:
 *   - óêàçàòåëü íà "model" - ñàì îáúåêò àíèìàöèè:
 *       MR3UNIT_MODEL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       mr3ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
 */

static VOID MR3_AnimUnitClose( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{
  MR3_GeomFree(&Uni->Model);
  MR3_GeomFree(&Uni->Geom);
  MR3_PrimFree(&Uni->Pr);
} /* End of 'MR3_AnimUnitClose' function */


/* Ôóíêöèÿ ñîçäàíèÿ îáúåêòà àíèìàöèè.
 *   - óêàçàòåëü íà "model" - ñàì îáúåêò àíèìàöèè:
 *       MR3UNIT_MODEL *Uni;
 *   - óêàçàòåëü íà êîíòåêñò àíèìàöèè:
 *       mr3ANIM *Ani;
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò. 
 */
static VOID MR3_AnimUnitRender( mr3UNIT_MODEL *Uni, mr3ANIM *Ani )
{  
  INT i, j;
  MR3_RndMatrView = MatrView(VecSet(0, 5, 30),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->KeysClick['P'])
    MR3_AnimSetPause(!Ani->IsPause);

  glEnable(GL_DEPTH_TEST);
 
  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      MR3_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
         MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.1, 0.1, 0.1)),
          MatrRotateY(0/*0.4 * Ani->Time*/)),
          MatrTranslate(j * 1.30, 0, i * 1.30 + 100 * Ani->JZ));
      MR3_GeomDraw(&Uni->Model);
    }
  //MR3_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  MR3_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  MR3_PrimDraw(&Uni->Pr, 0);
} /* End of 'MR3_AnimUnitRender' function */

/* creating of model.
 * ÀÐÃÓÌÅÍÒÛ: no
 * ÂÎÇÂÐÀÙÀÅÌÎÅ ÇÍÀ×ÅÍÈÅ: Íåò.
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