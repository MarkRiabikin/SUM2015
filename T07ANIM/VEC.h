#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include "def.h"
#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)

#define  MR3_UNIT_MATR \
  {                    \
    {                  \
      {1, 0, 0, 0},    \
      {0, 1, 0, 0},    \
      {0, 0, 1, 0},    \
      {0, 0, 0, 1}     \
    }                  \
  }

/* базовый вещественный тип */
typedef double DBL;


/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

typedef struct tagCOLOR
{
  FLT R, G, B, A; /* компоненты цвета + прозрачность (0..1) */
} COLOR;

/* Функция получения случайного числа в диапазоне [0.0 .. 1.0].
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (DBL) итоговое случайное число.
 */
__inline DBL Rnd0( VOID )
{
  return (DBL)rand() / RAND_MAX;
} /* End of 'Rnd0' function */

/* Функция получения случайного числа в диапазоне [-1.0 .. 1.0].
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (DBL) итоговое случайное число.
 */
__inline DBL Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function */


/***
 * Функции работы с цветом
 ***/


/* Функция построения цвета.
 * АРГУМЕНТЫ:
 *   - цветовые компоненты:
 *       FLT R, G, B, A;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (COLOR) собранное в один тип значение цвета.
 */
__inline COLOR ColorSet( FLT R, FLT G, FLT B, FLT A )
{
  COLOR c = {R, G, B, A};

  return c;
} /* End of 'ColorSet' function */


//!!!
__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR M =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1}
    }
  };

  return M;
}  /*End of 'MatrTranslate' function*/

__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR M =
  {
    {
      {Sx, 0,  0, 0},
      { 0, Sy,  0, 0},
      { 0,  0, Sz, 0},
      { 0,  0,  0, 1}
    }
  };

  return M;
} /*End of 'MatrScale' function*/

__inline MATR MatrRotateZ( DBL AngleDegree )
{
  DBL co = cos(AngleDegree);
  DBL si = sin(AngleDegree);
  MATR M =
  {
    {
      {co, si,  0, 0},
     {-si, co,  0, 0},
      { 0,  0,  1, 0},
      { 0,  0,  0, 1}
    }
  };
  return M;
} /*End of 'MatrRotateZ' function*/

__inline MATR MatrRotateX( DBL AngleDegree )
{
  DBL co = cos(AngleDegree);
  DBL si = sin(AngleDegree);
  MATR M =
  {
    {
      {1,  0,  0, 0},
      {0, co, si,  0},
      {0,-si, co, 0},
      {0,  0,  0, 1}
    }
  };
  return M;
} /*End of 'MatrRotateX' function*/

__inline MATR MatrRotateY( DBL AngleDegree )
{
  DBL co = cos(AngleDegree);
  DBL si = sin(AngleDegree);
  MATR M =
  {
    {
      {co, 0, -si, 0},
      { 0, 1,   0, 0},
      {si, 0,  co, 0},
      { 0, 0,   0, 1}
    }
  };
  return M;
} /*End of 'MatrRotateY' function*/

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR M;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
} /*End of 'MatrMulMatr' function*/

__inline MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR M;
  AngleDegree /= 2;

  /* correct source parameters */
  a = D2R(AngleDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;

  /* Build quaternion matrix */
  X *= si;
  Y *= si;
  Z *= si;

  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * co * Z;
  M.A[0][2] = 2 * co * Y + 2 * X * Z;
  M.A[0][3] = 0;

  M.A[1][0] = 2 * X * Y + 2 * co * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * co * X;
  M.A[1][3] = 0;

  M.A[2][0] = 2 * X * Z - 2 * co * Y;
  M.A[2][1] = 2 * co * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;

  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
} /*End of 'MatrRotate' function*/


__inline MATR MatrTranspose( MATR M )
{
  MATR N =
  {
    {
      {M.A[1][1], M.A[2][1], M.A[3][1], M.A[4][1]},

      
      {M.A[1][2], M.A[2][2], M.A[3][2], M.A[4][2]},


      {M.A[1][3], M.A[2][3], M.A[3][3], M.A[4][3]},


      {M.A[1][4], M.A[2][4], M.A[3][4], M.A[4][4]}
    }
  };

  return N;
} /*End of 'MatrTranspose' function*/

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /*End of 'MatrDeterm3x3' function*/



__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /*End of 'MatrDeterm' function*/

__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /*End of 'MatrIdentity' function*/

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT i, j;
  INT p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return MatrIdentity();

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)  
      r.A[j][i] = (1 - ((i + j) % 2 *2)) *
        MatrDeterm3x3(M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
                      M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
                     M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;

  return r;
} /*End of 'MatrInverse' function*/

//////////////////////////VECTORS////////////////////////

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
} /*End of 'VecAddVec' function*/

__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
} /*End of 'VecSubVec' function*/

__inline VEC VecMulNum( VEC A, DBL N )
{
  return VecSet(N * A.X, N * A.Y, N * A.Z);
} /*End of 'VecMulNum' function*/

__inline VEC VecDivNum( VEC A, DBL N )
{
  DBL M = 1/N;
  return VecSet(M * A.X, M * A.Y, M * A.Z);   
} /*End of 'VecDivNum' function*/

__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
} /*End of 'VecNeg' function*/

__inline DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
} /*End of 'VecDotVec' function*/

__inline VEC VecCrossVec( VEC A, VEC B )
{
  return VecSet(/*X*/A.Y * B.Z - A.Z * B.Y, /*Y*/A.Z * B.X - A.X * B.Z,
                /*Z*/A.X * B.Y - A.Y * B.X);
} /*End of 'VecCrossVec' function*/

__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
} /*End of 'VecLen2' function*/

__inline DBL VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
} /*End of 'VecLen' function*/

__inline VEC VecNormalize( VEC V ) //приведение к 1-ой длине
{
  return VecSet( V.X / VecLen(V), V.Y / VecLen(V), V.Z / VecLen(V));
} /*End of 'VecNormalize' function*/

__inline VEC PointTransform( VEC V, MATR M )
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];
  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /*End of 'PointTransform' function*/

__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);

}  

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.X, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

__inline MATR MatrFrustum( DBL Left, DBL Right, DBL Top, DBL Bottom, DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };

  return m;
} /* End of 'MatrFrustum' function */

#endif /* __VEC_H_ */
/* END OF 'VEC' FILE */