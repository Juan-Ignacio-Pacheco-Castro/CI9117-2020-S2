#include <iostream>
#include <omp.h>


int main()
{
    std::cout << "Ejemplo: max. threads = "
              << omp_get_max_threads() << std::endl;

    #pragma omp parallel
    {
        #pragma omp master
        {
            #pragma omp critical(crit_cout)
            {
                std::cout << "Ejemplo: num. threads = "
                          << omp_get_num_threads() << std::endl;
            }
        }
        #pragma omp barrier
    }

    omp_set_num_threads( omp_get_max_threads() + 1 );
    std::cout << "Ejemplo: max. threads = "
              << omp_get_max_threads() << std::endl;

    #pragma omp parallel
    {
        #pragma omp master
        {
            #pragma omp critical(crit_cout)
            {
                std::cout << "Ejemplo: num. threads = "
                          << omp_get_num_threads() << std::endl;
            }
        }
        #pragma omp barrier
    }

    return 0;
}
