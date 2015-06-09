//#include "DEF.h"
////-------------_ANIM:
//  /* ���������� ������������� */
//  DBL
//    Time,            /* ����� � �������� �� ������ �������� */
//    GlobalTime,      /* ����� -"-, �� ��� ����� */
//    DeltaTime,       /* ����������� ����� � �������� */
//    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
//    FPS;             /* ���������� ������ � ������� */
//  BOOL
//    IsPause;         /* ���� ����� */
//
////--------------ANIM.C:
///* ������ ��� ������������� �� ������� */
////static INT64
////  TimeFreq,  /* ������ ��������� � ������� */
////  TimeStart, /* ����� ������ �������� */
////  TimeOld,   /* ����� �������� ����� */
////  TimePause, /* ����� ������� � ����� */
////  TimeFPS;   /* ����� ��� ������ FPS */
////static INT
////  FrameCounter; /* ������� ������ */
//
////AnimInit:
//  LARGE_INTEGER li;
//
//  /* ������������� ������� */
//  /*QueryPerformanceFrequency(&li);
//  TimeFreq = li.QuadPart;
//  QueryPerformanceCounter(&li);
//  TimeStart = TimeOld = TimeFPS = li.QuadPart;
//  MR3_Anim.IsPause = FALSE;
//  FrameCounter = 0;*/
//
////Render:
//  LARGE_INTEGER li;
//  /*** ���������� ������� ***/
//  QueryPerformanceCounter(&li);
//  MR3_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
//  MR3_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;
//
//  if (!MR3_Anim.IsPause)
//    MR3_Anim.DeltaTime = MR3_Anim.GlobalDeltaTime;
//  else
//  {
//    TimePause += li.QuadPart - TimeOld;
//    MR3_Anim.DeltaTime = 0;
//  }
//
//  MR3_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;
//
//  /* ��������� FPS */
//  if (li.QuadPart - TimeFPS > TimeFreq)
//  {
//    static CHAR Buf[100];
//
//    sprintf(Buf, "FPS: %.5f", MR3_Anim.FPS);
//    SetWindowText(MR3_Anim.hWnd, Buf);
//
//    MR3_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
//    TimeFPS = li.QuadPart;
//    FrameCounter = 0;
//  }
//
//  /* ����� "��������" ����� */
//  TimeOld = li.QuadPart;
//
//  FrameCounter++;
//  
//
///* ������� ��������� ����� ��������.
// * ���������:
// *   - ���� �����:
// *       BOOL NewPauseFlag;
// * ������������ ��������: ���.
// */
//VOID MR3_AnimSetPause( BOOL NewPauseFlag )
//{
//  MR3_Anim.IsPause = NewPauseFlag;
//} /* End of 'MR3_AnimSetPause' function */
//
//
