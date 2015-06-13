#include "anim.h"
#include "render.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Получение значения оси джойстика */
#define MR3_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / \
  (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Системный контекст анимации */
mr3ANIM MR3_Anim;
MATR RndMatrFrustum;

/* Сохраненные мышиные координаты */
static INT
  MR3_MouseOldX, MR3_MouseOldY;

/* переменные таймера*/       
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */


/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
BOOL MR3_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&MR3_Anim, 0, sizeof(mr3ANIM));
  MR3_Anim.hWnd = hWnd;
  MR3_Anim.hDC = GetDC(hWnd);

  MR3_Anim.W = 30;
  MR3_Anim.H = 30;
  MR3_Anim.NumOfUnits = 0;

  /*** Инициализация OpenGL ***/

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(MR3_Anim.hDC, &pfd);
  DescribePixelFormat(MR3_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(MR3_Anim.hDC, i, &pfd);

  
  MR3_Anim.hGLRC = wglCreateContext(MR3_Anim.hDC);
 
  wglMakeCurrent(MR3_Anim.hDC, MR3_Anim.hGLRC);

  
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(MR3_Anim.hGLRC);
    ReleaseDC(MR3_Anim.hWnd, MR3_Anim.hDC);
    memset(&MR3_Anim, 0, sizeof(mr3ANIM));
    return FALSE;
  }

  MR3_RndProg = MR3_ShaderLoad("TEST");

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  MR3_Anim.IsPause = FALSE;
  FrameCounter = 0;
  
  GetCursorPos(&pt);
  ScreenToClient(MR3_Anim.hWnd, &pt);
  MR3_MouseOldX = pt.x;
  MR3_MouseOldY = pt.y;
  GetKeyboardState(MR3_Anim.KeysOld);
  return TRUE;
} /* End of 'MR3_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < MR3_Anim.NumOfUnits; i++)
  {
    MR3_Anim.Units[i]->Close(MR3_Anim.Units[i], &MR3_Anim);
    free(MR3_Anim.Units[i]);
    MR3_Anim.Units[i] = NULL;
  }

   MR3_ShaderFree(MR3_RndProg);
  MR3_RndProg = 0;

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(MR3_Anim.hGLRC);
  /* delete objects of GDI */
  ReleaseDC(MR3_Anim.hWnd, MR3_Anim.hDC);
} /* End of 'MR3_AnimClose' function */

} /* End of 'MR3_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(MR3_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(MR3_Anim.hBmFrame);
  MR3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(MR3_Anim.hDC, MR3_Anim.hBmFrame);

  /* Сохранение размера */
  MR3_Anim.W = W;
  MR3_Anim.H = H;

  ReleaseDC(MR3_Anim.hWnd, hDC);
  if (W > H)
    MR3_RndWp = (DBL)W / H * 3, MR3_RndHp = 3;
  else
    MR3_RndHp = (DBL)H / W * 3, MR3_RndWp = 3;

  RndMatrFrustum = MatrFrustum(-MR3_RndWp / 2, MR3_RndWp / 2, -MR3_RndHp / 2, MR3_RndHp / 2, MR3_RndProjDist, 800);  
} /* End of 'MR3_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  QueryPerformanceCounter(&li);
  MR3_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  MR3_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!MR3_Anim.IsPause)
    MR3_Anim.DeltaTime = MR3_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    MR3_Anim.DeltaTime = 0;
  }

  MR3_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* counting FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    MR3_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  TimeOld = li.QuadPart;

  /*** Обновление input***/

  /* Клавиатура */
  GetKeyboardState(MR3_Anim.Keys);
  for (i = 0; i < 256; i++)
    MR3_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    MR3_Anim.KeysClick[i] = MR3_Anim.Keys[i] && !MR3_Anim.KeysOld[i];
  memcpy(MR3_Anim.KeysOld, MR3_Anim.Keys, sizeof(MR3_Anim.KeysOld));


  MR3_Anim.MsWheel = MR3_MouseWheel;
  MR3_MouseWheel = 0;
  
  GetCursorPos(&pt);
  ScreenToClient(MR3_Anim.hWnd, &pt);
  MR3_Anim.MsX = pt.x;
  MR3_Anim.MsY = pt.y;
  
  MR3_Anim.MsDeltaX = pt.x - MR3_MouseOldX;
  MR3_Anim.MsDeltaY = pt.y - MR3_MouseOldY;
  MR3_MouseOldX = pt.x;
  MR3_MouseOldY = pt.y;

  
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;
    
    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        memcpy(MR3_Anim.JButsOld, MR3_Anim.JButs, sizeof(MR3_Anim.JButs));
        for (i = 0; i < 32; i++)
          MR3_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          MR3_Anim.JButsClick[i] = MR3_Anim.JButs[i] && !MR3_Anim.JButsOld[i];

        MR3_Anim.JX = MR3_GET_AXIS_VALUE(X);
        MR3_Anim.JY = MR3_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          MR3_Anim.JZ = MR3_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          MR3_Anim.JU = MR3_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          MR3_Anim.JV = MR3_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          MR3_Anim.JR = MR3_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            MR3_Anim.JPOV = 0;
          else
            MR3_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  for (i = 0; i < MR3_Anim.NumOfUnits; i++)
    MR3_Anim.Units[i]->Response(MR3_Anim.Units[i], &MR3_Anim);

  glClearColor(0.3, 0.5, 0.7, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(TRUE, TRUE, TRUE, FALSE);


  for (i = 0; i < MR3_Anim.NumOfUnits; i++)
  {
    static DBL time = 5;

    time += MR3_Anim.GlobalDeltaTime;
    if (time > 5)
    {
      time = 0;
      MR3_ShaderFree(MR3_RndProg);
      MR3_RndProg = MR3_ShaderLoad("TEST");
    }

    MR3_RndMatrWorld = MatrIdentity();

    MR3_Anim.Units[i]->Render(MR3_Anim.Units[i], &MR3_Anim);
  }
  glFinish();
  FrameCounter++;
}
 /* End of 'MR3_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimCopyFrame( VOID )
{
  SwapBuffers(MR3_Anim.hDC);
} /* End of 'MR3_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       MR3UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimAddUnit( mr3UNIT *Unit )
{
  if (MR3_Anim.NumOfUnits < MR3_MAX_UNITS)
  {
    MR3_Anim.Units[MR3_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &MR3_Anim);
  }
} /* End of 'MR3_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(MR3_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(MR3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(MR3_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(MR3_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(MR3_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'MR3_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimSetPause( BOOL NewPauseFlag )
{
  MR3_Anim.IsPause = NewPauseFlag;
} /* End of 'MR3_AnimSetPause' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimDoExit( VOID )
{
  PostMessage(MR3_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'MR3_AnimDoExit' function */


/* END OF 'ANIM.C' FILE */
