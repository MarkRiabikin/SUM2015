#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <windows.h>
#include "image1.h"
/* Имя класса окна */
#define WND_CLASS_NAME "My window class"
#define PI       3.14159265358979323846


/* Тип хранения координат точки в пространстве */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC;

VEC RotateX( VEC P, DOUBLE AngleDegree );
VEC VecCrossVec( VEC A, VEC B );
VEC VecSubVec( VEC A, VEC B );
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H );
VOID BuildGlobe(INT H, INT W);
VOID DrawGlobe(HDC hDC, INT H, INT W, IMAGE *Img);