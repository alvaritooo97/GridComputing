#include <stdio.h>
#include <math.h> 

#ifdef _OPENMP
#include <omp.h>
#endif

#define N 100
 
float work(int a)
{
  return a+1;
}


int main(void)
{
  float a[N], b[N], x, f, sum, sum_expected, psum;
  int i;
  int num_threads;

  /* Example 1 (this example contains an OpenMP parallelization error) */
  /* --------- */

  a[0] = 0;
# pragma omp parallel for ordered schedule(static) private(i)
  for (i=1; i<N; i++)
  {
    #pragma omp ordered
    a[i] = 2.0*i*(i-1);
    b[i] = a[i] - a[i-1];
    //printf("iteracion %d valores %f %f\n", i, a[i], b[i]);  
  } /* end of omp parallel for */ 

  /* testing the correctness of the numerical result: */
  sum=0; for (i=1; i<N; i++) { sum = sum + b[i]; }
  sum_expected = a[N-1]-a[0];
  printf("Exa.1: sum  computed=%8.1f,  expected=%8.1f,  difference=%8.5f \n", 
                                 sum,  sum_expected, sum-sum_expected); 


  /* Example 2 (this example contains an OpenMP parallelization error) */
  /* --------- */

  a[0] = 0;
# pragma omp parallel
  { 
#   pragma omp for private(i)
    for (i=1; i<N; i++)
    {
      a[i] = 3.0*i*(i+1);
    } /* end of omp for nowait */ 
#   pragma omp for private(i)
    for (i=1; i<N; i++)
    {
      b[i] = a[i] - a[i-1];    } /* end of omp for */ 
  } /* end of omp parallel */ 

  /* testing the correctness of the numerical result: */
  sum=0; for (i=1; i<N; i++) { sum = sum + b[i]; }
  sum_expected = a[N-1]-a[0];
  printf("Exa.2: sum  computed=%8.1f,  expected=%8.1f,  difference=%8.5f \n", 
                                 sum,  sum_expected, sum-sum_expected); 


  /* Example 3 (this example contains an OpenMP parallelization error) */
  /* --------- */

# pragma omp parallel for private(i)
  for (i=1; i<N; i++)
  {
    x = sqrt(b[i]) - 1;
    a[i] = x*x + 2*x + 1;
  } /* end of omp parallel for */ 

  /* testing the correctness of the numerical result: */
  sum=0; for (i=1; i<N; i++) { sum = sum + a[i]; }
  /* sum_expected = same as in Exa.2 */
  printf("Exa.3: sum  computed=%8.1f,  expected=%8.1f,  difference=%8.5f \n", 
                                 sum,  sum_expected, sum-sum_expected); 

  /* Example 4 (this example contains an OpenMP parallelization error) */
  /* --------- */

  f = 2;
# pragma omp parallel for private(i) lastprivate(x)
  for (i=1; i<N; i++)
  {
    x = f * b[i];
    a[i] = x - 7;
  } /* end of omp parallel for */ 
  a[0] = x; 

  /* testing the correctness of the numerical result: */
  printf("Exa.4: a[0] computed=%8.1f,  expected=%8.1f,  difference=%8.5f \n", 
                                a[0],        2*b[N-1],   a[0] - 2*b[N-1] );


  /* Example 5 (this example contains an OpenMP parallelization error) */
  /* --------- */

  sum = 0; 
# pragma omp parallel for private(i) reduction(+:sum)
  for (i=1; i<N; i++)
  {
    sum = sum + b[i];
  } /* end of omp parallel for */ 

  /* testing the correctness of the numerical result: */
  /* sum_expected = same as in Exa.2 */
  printf("Exa.5: sum  computed=%8.1f,  expected=%8.1f,  difference=%8.5f \n", 
                                 sum,  sum_expected, sum-sum_expected); 

}