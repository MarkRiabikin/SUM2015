#ifndef __ANIM_H_
#define __ANIM_H_

#include <stdio.h>
#include "def.h"

/* Максимальное количество элементов анимации */
#define MR3_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagMR3UNIT MR3UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */
  BOOL IsPause;        /* значение паузы*/
  INT64  
    GlobalTime,
    GlobalDeltaTime,
    DeltaTime,
    Time,
    FPS;
  BYTE
    Keys[256],       /* Сотояние клавиш клавиатуры и мыши */
    KeysOld[256],    /* Сотояние клавиш на предыдущем кадре */
    KeysClick[256];  /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты курсора мыши */
    MsX, MsY,        /* Абсолютные координаты курсора мыши */
    MsWheel;         /* Состояние колеса мыши */
  BYTE
    JButs[32],       /* Сотояние кнопок джойстика */
    JButsOld[32],    /* Сотояние кнопок джойстика на предыдущем кадре */
    JButsClick[32],  /* Флаги однократного нажатия кнопок джойстика */
    JPOV;            /* Переключатель POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* Оси джойстика (-1.0 .. 1.0*/

  /* Массив элементов анимации и их количество */
  MR3UNIT *Units[MR3_MAX_UNITS]; 
  INT NumOfUnits;

} mr3ANIM;

/* Системный контекст анимации */
extern mr3ANIM MR3_Anim;
/* Глобальная переменная - счетчик прокрутки колеса мыши */
extern INT MR3_MouseWheel;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( MR3UNIT *Unit, mr3ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( MR3UNIT *Unit, mr3ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( MR3UNIT *Unit, mr3ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( MR3UNIT *Unit, mr3ANIM *Ani );
 */
#define MR3_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( MR3UNIT *Unit, mr3ANIM *Ani );       \
  VOID (*Close)( MR3UNIT *Unit, mr3ANIM *Ani );      \
  VOID (*Response)( MR3UNIT *Unit, mr3ANIM *Ani );   \
  VOID (*Render)( MR3UNIT *Unit, mr3ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagMR3UNIT
{
  MR3_UNIT_BASE_FIELDS;
};

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimInit( HWND hWnd );

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       MR3UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimAddUnit( MR3UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (MR3UNIT *) указатель на созданный объект анимации.
 */
MR3UNIT * MR3_AnimUnitCreate( INT Size );

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */                    
VOID MR3_AnimDoExit( VOID );


/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID MR3_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
