// based on https://github.com/drmackay/samplematrixcode.git

// A simple matrix multiply code to show affect of ordering and blocking
// to compile this use gcc -O2 mm.c -lrt or icc -O2 mm.c -lrt

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEASURE_TIME(X) \
{ \
    struct timespec ts1, ts2; \
    clock_gettime (CLOCK_REALTIME, &ts1); \
    X; \
    clock_gettime (CLOCK_REALTIME, &ts2); \
    printf (#X " -> %f\n", (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec) / 1.0e9); \
}


void setmat (int n, int m, double* a)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0 ; j < m; j++)
            a[i * n + j] = 0.0 ;
}

void fillmat (int n, int m, double* a)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0 ; j < m; j++)
            a[i * n + j] = (double)rand() / 3.1e09;
}

void mm_basic (int n, double* a, double* b, double* c)
{
    int i, j, k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++)
                a[i * n + j] += b[i * n + k] * c[k * n + j] ;
}

void mm_opt (int n, double* a, double* b, double* c, int bs)
{
    // int i, j, k;

    // for (k = 0; k < n; k++)
    //     for (i = 0; i < n; i++)
    //         for (j = 0; j < n; j++)
    //             a[i * n + j] += b[i * n + k] * c[k * n + j] ;

    assert(n%bs == 0); // Asegurarnos de que n y bs son compatibles
    int N = n/bs;
    printf("N = %d\n", N);
    int i, j, k;
    int i_block, j_block, k_block;

    for (i_block = 0; i_block < n; i_block+=bs)
        for (j_block = 0; j_block < n; j_block+=bs)
            for (k_block = 0; k_block < n; k_block+=bs)
                for (i = i_block; i < i_block + bs; i++)
                    for (j = j_block; j < j_block + bs; j++)
                        for (k = k_block; k < k_block + bs; k++){
                            a[i * n + j] += b[i * n + k] * c[k * n + j];
                            // printf("i_block: %d j_block: %d k_block: %d i: %d j: %d k: %d\n", i_block, j_block, k_block, i, j, k);
                        }
}

void checkmm (int n, double* a, double* aa)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (a[i * n + j]-aa[i * n + j] != 0.0) {
                printf ("(%d, %d) -> diff: %lf\n", i, j, a[i * n + j] - aa[i * n + j]);
                return;
            }
        }
    printf ("check OK\n");
}

int main (int argc, char *argv[])
{
    double *aa, *a, *b, *c;
    int n, bs;

    if (argc != 3) {
        printf ("Usage: %s <matrix_size> <blocksize>\n", argv[0]) ;
        exit (1);
    }
    n = atoi (argv[1]) ;
    bs = atoi (argv[2]) ;

    printf ("matrix size (%d x %d), blocksize (%d, %d)\n", n, n, bs, bs) ;
    if (n % bs) {
        printf ("matrix size should be multiple of blocksize\n") ;
        exit (2);
    }

    // allocate matrices
    a =  (double*) calloc (n*n, sizeof(double)) ;
    b =  (double*) calloc (n*n, sizeof(double)) ;
    c =  (double*) calloc (n*n, sizeof(double)) ;
    aa = (double*) calloc (n*n, sizeof(double)) ;
    if (aa == NULL) {
        printf ("insufficient memory \n");
        exit(3);
    }

    // fill a matrix
    setmat (n, n, a) ;
    setmat (n, n, aa) ;

    srand (416) ; // set random seed (change to go off time stamp to make it better

    fillmat (n, n, b) ;
    fillmat (n, n, c) ;

    MEASURE_TIME ( mm_basic (n, aa, b, c) );
    MEASURE_TIME ( mm_opt (n, a, b, c, bs) );

    checkmm (n, a, aa) ;
}
