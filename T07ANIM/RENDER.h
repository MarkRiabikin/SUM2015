/* FILENAME: RENDER.H
 * PROGRAMMER: MR3
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "vec.h"

/* Матрицы */
extern MATR
  MR3_RndMatrWorld,
  MR3_RndMatrView,
  MR3_RndMatrWorldView,
  MR3_RndMatrWorldViewProj;

/* Матрица изменения примитива при создании */
extern MATR MR3_RndPrimMatrConvert;

/* Параметры проецирования */
extern DBL
  MR3_RndWp, MR3_RndHp,    /* размеры обрасти проецирования */
  MR3_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT MR3_RndProg;

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT MR3_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_ShaderFree( UINT PrgId );

/***
 * Работа с материалами
 ***/

/* Тип задания материала поверхности */
typedef struct tagmr3MATERIAL
{
  /* Имя материала */
  CHAR Name[300];

  /* Коэффициенты отражения */
  VEC Ka, Kd, Ks;
  FLT Kp; /* Степень "бликовости" */

  /* Коэффициент прозрачности */
  FLT Kt;

  /* Текстура поверхности */
  INT TexId;
} mr3MATERIAL;

/* Глобальная таблица материалов */
#define MR3_MATERIAL_MAX 300
extern INT MR3_MtlLibSize;
extern mr3MATERIAL MR3_MtlLib[MR3_MATERIAL_MAX];

/* Функция добавления материала в библиотеку.
 * АРГУМЕНТЫ:
 *   - структура с памаметрами материала:
 *       mr3MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер добавленного материала (0 при ошибке).
 */
INT MR3_MtlAdd( mr3MATERIAL *Mtl );

/* Функция поиска материала по имени.
 * АРГУМЕНТЫ:
 *   - имя материала:
 *       CHAR *Name;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер найденного материала (0 при ошибке).
 */
INT MR3_MtlFind( CHAR *Name );

/* Функция загрузки материала из файла (*.MTL).
 * АРГУМЕНТЫ:
 *   - имя файла материала:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество загруженных материалов.
 */
INT MR3_MtlLoad( CHAR *FileName );


/***
 * Работа с примитивами
 ***/

/* Виды примитивов */
typedef enum tagmr3PRIM_TYPE
{
  MR3_PRIM_TRIMESH,  /* примитив - набор треугольников */
  MR3_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} mr3PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagmr3PRIM
{
  mr3PRIM_TYPE Type; /* Тип примитива (MR3_PRIM_***) */
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfI,      /* количество индексов для вывода примитива */
    MtlNo;       /* номер материала из библиотеки */
} mr3PRIM;

/* Тип хранения текстурных координат */
typedef struct tagmr3UV
{
  FLT U, V; /* текстурные координаты */
} mr3UV;

/* Функция задания текстурных координат.
 * АРГУМЕНТЫ:
 *   - новые значени координат:
 *       FLT U, V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (mr3UV) построенные координаты.
 */
__inline mr3UV UVSet( FLT U, FLT V )
{
  mr3UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagmr3VERTEX
{
  VEC P;    /* позиция вершины */
  mr3UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} mr3VERTEX;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       mr3PRIM *Prim;
 *   - тип примитива (MR3_PRIM_***):
 *       mr3PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       mr3VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_PrimCreate( mr3PRIM *Prim, mr3PRIM_TYPE Type,
                     INT NoofV, INT NoofI, mr3VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       mr3PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_PrimFree( mr3PRIM *Prim );

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       mr3PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_PrimDraw( mr3PRIM *Prim );

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       mr3PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL MR3_PrimCreatePlane( mr3PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       mr3PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL MR3_PrimCreateSphere( mr3PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       mr3PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL MR3_PrimLoad( mr3PRIM *GObj, CHAR *FileName );

/***
 * Работа с геометрическими объектами
 ***/

/* Геометрический объект - набор примитивов */
typedef struct tagmr3GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  mr3PRIM *Prims;      /* массив примитивов */
} mr3GEOM;

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       mr3GEOM *G;
 *   - указатель на добавляемый примитив:
 *       mr3PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT MR3_GeomAddPrim( mr3GEOM *G, mr3PRIM *Prim );

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       mr3GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_GeomFree( mr3GEOM *G );

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       mr3GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_GeomDraw( mr3GEOM *G );

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       mr3GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL MR3_GeomLoad( mr3GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT MR3_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */