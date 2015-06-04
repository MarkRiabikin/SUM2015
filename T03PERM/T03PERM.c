#include <stdio.h>
#define N 3


void swap(int *A, int *B)
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}

void write(int a)
{
  FILE *F;
  if((F = fopen("a.log", "a")) != NULL)
  {
    if(a == 100)
      fprintf(F, " - odd\n");
    else if(a == 101)
      fprintf(F, " - even\n");
    else
      fprintf(F, "%d", a);
  }
  else
  {
    return;
  }
}


int parit(int *P)
{
  int i, j, par = 0;
  for(i = 0; i < N-1; i++)
    for(j = i + 1; j < N; j++)
      par += (P[i] > P[j]);
  return par % 2;
}

int a[N];

void perm(int pos)
{
  int i, j, cnt = 0;
  if(pos == N)
  {
    for(j = 0; j < N; j++)
      write( a[j]);

    write(100 + parit( a));
    return; 
  }
  else
    for( i = pos; i < N; i++)
    {
      swap( &a[pos], &a[i]);
      perm(pos + 1);
      swap( &a[pos], &a[i]);
    }
}


void main( void)
{
  int i;
  for(i = 0; i < N; i++)
    a[i] = i+1;
  perm(0);
  
}