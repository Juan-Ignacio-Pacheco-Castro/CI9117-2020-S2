
// Tomadp de https://stackoverflow.com/questions/11017851/openmp-locking-access-to-single-array-elements
#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv)
{
    const int NITEMS = 20;
    int array[NITEMS];
    omp_lock_t lock[NITEMS];

    for (int i = 0; i < NITEMS; ++i)
        omp_init_lock(&(lock[i]));

#pragma omp parallel for shared(array, lock) default(none)
     for (int i = 0; i < NITEMS; ++i) {
        int tid = omp_get_thread_num();
        int item = (i * 7) % NITEMS;

        omp_set_lock(&(lock[item]));
        array[item] = tid;    // only one thread in here at a time; others block at set_lock()
        omp_unset_lock(&(lock[item]));
    }

    for (int i=0; i<NITEMS; i++)
        printf("%3d ", array[i]);
    printf("\n");

    for (int i=0; i<NITEMS; i++)
        omp_destroy_lock(&(lock[i]));

    return 0;
}
