#include <stdio.h>
#define N 5


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
    fprintf(F, "%d", a);
  }
  else
  {
    return;
  }
}

int a[N], cnt = 0;
void perm(int pos)
{
  int i, j;
  if(pos == N)
  {
    for(j = 0; j < N; j++)
      write( a[j]);
    
    return; 
  }
  else
    for( i = pos; i < N; i++)
    {
      cnt += (a[pos] > a[i]);
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