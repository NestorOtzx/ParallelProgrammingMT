#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

void A(int* result) {
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *result = sleepTime;
    std::cout<<"A finished "<<sleepTime/1000000.0<<" seconds.\n";
}

void B(int* result) {
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *result = sleepTime;
    std::cout<<"B finished "<<sleepTime/1000000.0<<" seconds.\n";
}

void C(int* result) {
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *result = sleepTime;
    std::cout<<"C finished "<<sleepTime/1000000.0<<" seconds.\n";
}

void D(int* result) {
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *result = sleepTime;
    std::cout<<"D finished "<<sleepTime/1000000.0<<" seconds.\n";
}

void E(int* result) {
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *result = sleepTime;
    std::cout<<"E finished "<<sleepTime/1000000.0<<" seconds.\n";
}

int main() {
    srand(time(0));

    int resultA, resultB, resultC, resultD, resultE;

    #pragma omp parallel
    {
        #pragma omp single
        {
            // Ejecuta A en paralelo
            #pragma omp task
            A(&resultA);

            #pragma omp task
            {
                // Ejecuta B y C en paralelo
                #pragma omp task
                B(&resultB);

                #pragma omp task
                C(&resultC);

                // Espera a que B y C terminen para ejecutar D
                #pragma omp taskwait

                #pragma omp task
                D(&resultD);

                //Espera a que D termine   
                #pragma omp taskwait
            }

            // Espera a que A y D estén listos para ejecutar E
            
            #pragma omp taskwait
            E(&resultE);
        }
    }

    // El resultado final muestra variantes en el orden de ejecución de A, B, C, pero E termina siempre de último
    std::cout << "Fin del programa" << std::endl;

    return 0;
}
