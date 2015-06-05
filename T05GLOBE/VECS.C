/* FILE NAME: T05GLOBE.C
 * PROGRAMMER: MR3
 * DATE: 05.06.2015
 * PURPOSE: vector functions.
 */
#include "VECS.h"
#include "DEF.h"
#include "image1.h"

#define N 100
#define M 100

VEC Grid[N][M];
/* Функция поворота точки вокруг оси X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */

/* Функция векторного произведения */
VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* Функция вычитания векторов */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

/* Рисование четырехугольника */
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  VEC Norm = VecCrossVec(VecSubVec(P3, P0), VecSubVec(P1, P0));
  POINT pnts[4];

  /* back-face culling */
  

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;

  if((pnts[0].x - pnts[1].x) * (pnts[0].y + pnts[1].y) +
      (pnts[1].x - pnts[2].x) * (pnts[1].y + pnts[2].y) +
      (pnts[2].x - pnts[3].x) * (pnts[2].y + pnts[3].y) +
      (pnts[3].x - pnts[0].x) * (pnts[3].y + pnts[0].y) < 0)
    return;


  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

VOID BuildGlobe(INT H, INT W)
{
  INT i, j;
  DOUBLE phi, the, rot = PI/100 * clock()/100, R = 200;

  for(i = 0; i < N; i++)
  {
    the = i / (N - 1.0) * PI;
    for(j = 0; j < N; j++)
    {
      phi = j / (M - 1.0) * 2 * PI + rot;
      Grid[i][j].X = R * sin(the) * sin(phi);      
      Grid[i][j].Y = R * cos(the);               
      Grid[i][j].Z = R * sin(phi) * cos(the);
      Grid[i][j] = RotateX(Grid[i][j], 0); 
    }
  }
} /* End of 'BuildGlobe' function */

VOID DrawGlobe(HDC hDC, INT H, INT W, IMAGE Img)
{
  INT i, j;
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
    {
      SelectObject(hDC, GetStockObject(NULL_PEN));
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hDC, RGB(ImageGetP(Img, i * Img.H / (N - 1),
                                              j * Img.W / (M - 1))));
      DrawQuad(hDC,Grid[i][j],Grid[i+1][j],
                   Grid[i+1][j+1], Grid[i][j+1], H, W); 
    }
} /* End of 'DrawGlobe' function */
