#include <Windows.h>
#include <stdio.h>


void main ( void )
{
  int x;
  printf("Yes ot No ?\n (1 / 0)");
  scanf("%d", &x);
  if(x == 1)
    MessageBox(NULL , "You pressed Yes", "Yes | No", MB_OK);
  else
    MessageBox(NULL , "You pressed No", "Yes | No", MB_OK);
} 