#ifndef __ANIM_H_
#define __ANIM_H_

#include <stdio.h>
#include "def.h"

/* ������������ ���������� ��������� �������� */
#define MR3_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagMR3UNIT mr3UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */
  BOOL IsPause;        /* �������� �����*/
  INT64  
    GlobalTime,
    GlobalDeltaTime,
    DeltaTime,
    Time,
    FPS;
  BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0*/

  /* ������ ��������� �������� � �� ���������� */
  mr3UNIT *Units[MR3_MAX_UNITS]; 
  INT NumOfUnits;

} mr3ANIM;

/* ��������� �������� �������� */
extern mr3ANIM MR3_Anim;
/* ���������� ���������� - ������� ��������� ������ ���� */
extern INT MR3_MouseWheel;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( MR3UNIT *Unit, mr3ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( MR3UNIT *Unit, mr3ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( MR3UNIT *Unit, mr3ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( MR3UNIT *Unit, mr3ANIM *Ani );
 */
#define MR3_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( mr3UNIT *Unit, mr3ANIM *Ani );       \
  VOID (*Close)( mr3UNIT *Unit, mr3ANIM *Ani );      \
  VOID (*Response)( mr3UNIT *Unit, mr3ANIM *Ani );   \
  VOID (*Render)( mr3UNIT *Unit, mr3ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagMR3UNIT
{
  MR3_UNIT_BASE_FIELDS;
};

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID MR3_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID MR3_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID MR3_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID MR3_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID MR3_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       MR3UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID MR3_AnimAddUnit( mr3UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID MR3_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (MR3UNIT *) ��������� �� ��������� ������ ��������.
 */
mr3UNIT * MR3_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */                    
VOID MR3_AnimDoExit( VOID );


/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID MR3_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
