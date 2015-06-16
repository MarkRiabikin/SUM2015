/* FILENAME: MTLLIB.C
 * PROGRAMMER: MR3
 * PURPOSE: Material library support module.
 * LAST UPDATE: 15.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "render.h"

/* Глобальная таблица материалов */
INT MR3_MtlLibSize = 1;
mr3MATERIAL MR3_MtlLib[MR3_MATERIAL_MAX] =
{
  {
    /* Material #0 - материал по умолчанию */
    "Default",
    {0.1, 0.1, 0.1}, /* Ka */
    {0.9, 0.9, 0.9}, /* Kd */
    {0.5, 0.5, 0.5}, /* Ks */
    30.0,            /* Kp */
    1.0,             /* Kt */
    0                /* Texture id */
  }
};

/* Функция добавления материала в библиотеку.
 * АРГУМЕНТЫ:
 *   - структура с памаметрами материала:
 *       mr3MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер добавленного материала (0 при ошибке).
 */
INT MR3_MtlAdd( mr3MATERIAL *Mtl )
{
  if (MR3_MtlLibSize < MR3_MATERIAL_MAX)
  {
    MR3_MtlLib[MR3_MtlLibSize] = *Mtl;
    return MR3_MtlLibSize++;
  }
  return 0;
} /* End of 'MR3_MtlAdd' function */

/* Функция поиска материала по имени.
 * АРГУМЕНТЫ:
 *   - имя материала:
 *       CHAR *Name;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер найденного материала (0 при ошибке).
 */
INT MR3_MtlFind( CHAR *Name )
{
  INT i;

  for (i = 0; i < MR3_MtlLibSize; i++)
    if (strcmp(Name, MR3_MtlLib[i].Name) == 0)
      return i;
  return 0;
} /* End of 'MR3_MtlFind' function */

/* Функция загрузки материала из файла (*.MTL).
 * АРГУМЕНТЫ:
 *   - имя файла материала:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество загруженных материалов.
 */
INT MR3_MtlLoad( CHAR *FileName )
{
  FILE *F;
  INT cnt = 0, i, j;
  mr3MATERIAL Mtl = MR3_MtlLib[0];
  static CHAR
    Buf[1000], Name[_MAX_PATH],
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    /* пропускаем лидирующие пробелы */
    i = 0;
    while (Buf[i] != 0 && isspace(Buf[i]))
      i++;

    /* обрабатываем команды материалов */
    if (strncmp(Buf + i, "Ka", 2) == 0)
      sscanf(Buf + i + 3, "%f %f %f", &Mtl.Ka.X, &Mtl.Ka.Y, &Mtl.Ka.Z);
    else if (strncmp(Buf + i, "Kd", 2) == 0)
      sscanf(Buf + i + 3, "%f %f %f", &Mtl.Kd.X, &Mtl.Kd.Y, &Mtl.Kd.Z);
    else if (strncmp(Buf + i, "Ks", 2) == 0)
      sscanf(Buf + i + 3, "%f %f %f", &Mtl.Ks.X, &Mtl.Ks.Y, &Mtl.Ks.Z);
    else if (strncmp(Buf + i, "Ns", 2) == 0)
      sscanf(Buf + i + 3, "%f", &Mtl.Kp);
    else if (Buf[i] == 'D' || Buf[i] == 'd')
      sscanf(Buf + i + 2, "%f", &Mtl.Kt);
    else if (strncmp(Buf + i, "newmtl", 6) == 0)
    {
      /* выделяем в строку остаток массива Buf */
      j = 0;
      i += 7;
      while (j < sizeof(Name) - 1 && Buf[i] != 0 && Buf[i] != '\n')
        Name[j++] = Buf[i++];
      Name[j] = 0;

      /* добавляем материал */
      if (cnt++ != 0)
        MR3_MtlAdd(&Mtl);
      /* создаем материал */
      Mtl = MR3_MtlLib[0];
      strcpy(Mtl.Name, Name);
    }
    else if (strncmp(Buf + i, "map_Kd", 6) == 0)
    {
      /* выделяем в строку остаток массива Buf */
      j = 0;
      i += 7;
      while (j < sizeof(Name) - 1 && Buf[i] != 0 && Buf[i] != '\n')
        Name[j++] = Buf[i++];
      Name[j] = 0;

      /* загружаем текстуру */
      _makepath(path_buffer, drive, dir, Name, "");
      Mtl.TexId = MR3_TextureLoad(path_buffer);
    }
  }
  /* добавляем материал */
  if (cnt != 0)
    MR3_MtlAdd(&Mtl);
  fclose(F);
  return cnt;
} /* End of 'MR3_MtlLoad' function */

/* END OF 'MTLLIB.C' FILE */

