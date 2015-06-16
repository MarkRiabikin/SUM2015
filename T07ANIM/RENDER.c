/* FILENAME: RENDER.C
 * PROGRAMMER: MR3
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 10.06.2015
 */

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

MATR
  MR3_RndMatrWorld = MR3_UNIT_MATR,
  MR3_RndMatrView = MR3_UNIT_MATR,
  MR3_RndMatrWorldView = MR3_UNIT_MATR,
  MR3_RndMatrWorldViewProj = MR3_UNIT_MATR;

/* Параметры проецирования */
DBL
  MR3_RndWp = 3, MR3_RndHp = 3,     /* размеры обрасти проецирования */
  MR3_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
UINT MR3_RndProg;

BOOL MR3_PrimLoad( mr3PRIM *GObj, CHAR *FileName )
{
  FILE *F;
  mr3VERTEX *V;
  INT (*Facets)[3];
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(mr3PRIM));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((V = malloc(sizeof(mr3VERTEX) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Facets = (INT (*)[3])(V + nv);
  memset(V, 0, sizeof(mr3VERTEX) * nv + sizeof(INT [3]) * nf);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%f%f%f",
        &V[nv].P.X, &V[nv].P.Y, &V[nv].P.Z);
      V[nv].C = ColorSet(Rnd0(), Rnd0(), Rnd0(), 1);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      Facets[nf][0] = a - 1;
      Facets[nf][1] = b - 1;
      Facets[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  MR3_PrimCreate(GObj, MR3_PRIM_TRIMESH, nv, nf * 3, V, (INT *)Facets);

  
  free(V);
  return TRUE;
} /* End of 'MR3_PrimLoad' function */

INT MR3_TextureLoad( CHAR *FileName )
{
  INT TexId = 0;
  HDC hMemDC;
  BITMAP bm;
  HBITMAP hBm;
  DWORD *Bits;

  
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return 0;

  /* Create compatible context and select image into one */
  hMemDC = CreateCompatibleDC(MR3_Anim.hDC);
  SelectObject(hMemDC, hBm);

  /* Obtain image size */
  GetObject(hBm, sizeof(BITMAP), &bm);
  if ((Bits = malloc(sizeof(DWORD) * bm.bmWidth * bm.bmHeight)) != NULL)
  {
    INT x, y, r, g, b;
    COLORREF c;

    for (y = 0; y < bm.bmHeight; y++)
      for (x = 0; x < bm.bmWidth; x++)
      {
        c = GetPixel(hMemDC, x, y);
        r = GetRValue(c);
        g = GetGValue(c);
        b = GetBValue(c);
        Bits[(bm.bmHeight - 1 - y) * bm.bmWidth + x] = 0xFF000000 | (r << 16) | (g << 8) | b;
      }
      glGenTextures(1, &TexId);
      glBindTexture(GL_TEXTURE_2D, TexId);
      gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bm.bmWidth, bm.bmHeight,
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
      glBindTexture(GL_TEXTURE_2D, 0);
    free(Bits);
  }
  DeleteDC(hMemDC);
  DeleteObject(hBm);
  return TexId;
} /* End of 'MR3_TextureLoadfunction */

/* END OF 'RENDER.C' FILE */

