/* FILENAME: UCONTRL.C
 * PROGRAMMER: MR3
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "control.h"

/* ��� ������������� ���� */
typedef struct tagMR3UNIT_CTRL
{
  MR3_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
} MR3UNIT_CTRL;

//#include "vec.h"

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       MR3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MR3_AnimUnitInit( MR3UNIT_CTRL *Uni, mr3ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'MR3_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       MR3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MR3_AnimUnitClose( MR3UNIT_CTRL *Uni, mr3ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'MR3_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       MR3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MR3_AnimUnitResponse( MR3UNIT_CTRL *Uni, mr3ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    MR3_AnimDoExit();
  if (Ani->KeysClick['F'])
    MR3_AnimFlipFullScreen();
} /* End of 'MR3_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       MR3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       mr3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID MR3_AnimUnitRender( MR3UNIT_CTRL *Uni, mr3ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }
} /* End of 'MR3_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (MR3UNIT *) ��������� �� ��������� ������ ��������.
 */
mr3UNIT * MR3_UnitControlCreate( VOID )
{
  MR3UNIT_CTRL *Uni;

  if ((Uni = (VOID *)MR3_AnimUnitCreate(sizeof(MR3UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)MR3_AnimUnitInit;
  Uni->Close = (VOID *)MR3_AnimUnitClose;
  Uni->Response = (VOID *)MR3_AnimUnitResponse;
  Uni->Render = (VOID *)MR3_AnimUnitRender;
  return (mr3UNIT *)Uni;
} /* End of 'MR3_UnitControlCreate' function */

