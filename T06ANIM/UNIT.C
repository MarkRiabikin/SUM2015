#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       mr3UNIT *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitInit( mr3UNIT *Uni, mr3ANIM *Ani )
{
} /* End of 'MR3_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       mr3UNIT *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitClose( mr3UNIT *Uni, mr3ANIM *Ani )
{
} /* End of 'MR3_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       mr3UNIT *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitResponse( mr3UNIT *Uni, mr3ANIM *Ani )
{
} /* End of 'MR3_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       mr3UNIT *Uni;
 *   - указатель на контекст анимации:
 *       mr3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID MR3_AnimUnitRender( mr3UNIT *Uni, mr3ANIM *Ani )
{
} /* End of 'MR3_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (mr3UNIT *) указатель на созданный объект анимации.
 */
mr3UNIT * MR3_AnimUnitCreate( INT Size )
{
  mr3UNIT *Uni;

  if (Size < sizeof(mr3UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* заполняем поля по-умолчанию */
  Uni->Size = Size;
  Uni->Init = MR3_AnimUnitInit;
  Uni->Close = MR3_AnimUnitClose;
  Uni->Response = MR3_AnimUnitResponse;
  Uni->Render = MR3_AnimUnitRender;
  return Uni;
} /* End of 'MR3_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
