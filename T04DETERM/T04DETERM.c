#include <stdio.h>
#include <conio.h>

#define MAX 10

double A[MAX][MAX];
int P[MAX];
int N;
double SUM, PROD;

void Load( char *FileName )
{
  FILE *F;
  int i, j;

  if ((F = fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
}

void swap(int *A, int *B)
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}


int Parity(int *D)
{
  int i, j, par = 0;
  for(i = 0; i < N-1; i++)
    for(j = i + 1; j < N; j++)
      par += (D[i] > D[j]);
  return par % 2;
}

void Determ( void)
{
  int j = 0;
  SUM = 0, PROD = 1;

  for (j = 0; j < N; j++)
    PROD *= A[j][P[j]];
  if (Parity( P) == 0)
    SUM += PROD;
  else
    SUM -= PROD;
}

void perm(int pos)
{
  int i, cnt = 0;
  if(pos == N)
  {
    Determ();
    return;
  }
  else
    for( i = 0; i < N; i++)
    {
      if(i != pos)
      {
        swap( &P[pos], &P[i]);
        perm(pos + 1);
        swap( &P[pos], &P[i]);
      }
    }
}

void main( void)
{
  int i;
  for(i = 0; i < MAX; i++)
    P[i] = i+1;

  Load("m.txt");

  perm(0);
  printf("%f", SUM);
  _getch();
}