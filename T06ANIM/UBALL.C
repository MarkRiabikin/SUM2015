#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagMR3UNIT_BALL
{
  MR3_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift; /* Масштаб времени */
} MR3UNIT_BALL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       MR3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitInit( MR3UNIT_BALL *Uni, mr3ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'MR3_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       MR3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitClose( MR3UNIT_BALL *Uni, mr3ANIM *Ani )
{
} /* End of 'MR3_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       MR3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitResponse( MR3UNIT_BALL *Uni, mr3ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    MR3_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    MR3_AnimFlipFullScreen();
  if (GetAsyncKeyState('P') & 0x8000);

} /* End of 'MR3_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       MR3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitRender( MR3UNIT_BALL *Uni, mr3ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'MR3_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (MR3UNIT *) указатель на созданный объект анимации.
 */
MR3UNIT * MR3_UnitBallCreate( VOID )
{
  MR3UNIT_BALL *Uni;

  if ((Uni = (VOID *)MR3_AnimUnitCreate(sizeof(MR3UNIT_BALL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)MR3_AnimUnitInit;
  Uni->Close = (VOID *)MR3_AnimUnitClose;
  Uni->Response = (VOID *)MR3_AnimUnitResponse;
  Uni->Render = (VOID *)MR3_AnimUnitRender;
  return (MR3UNIT *)Uni;
} /* End of 'MR3_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */