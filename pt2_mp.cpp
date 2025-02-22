#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int resultA = 0, resultB = 0, resultC = 0, resultD = 0, resultE = 0, resultF = 0;
bool finishedA = false, finishedB = false, finishedC = false, finishedD = false, finishedE = false, finishedF = false;
bool runningA = false,  runningB = false, runningC = false, runningD = false, runningF = false, runningE = false;

void A(); void B(); void C(); void D(); void E(); void F();

void A() {
    int sleepTime = rand() % 1000000;
    #pragma omp critical
    runningA = true;

    usleep(sleepTime);
    resultB = sleepTime;
    #pragma omp critical
    {
        finishedA = true;
        if (finishedB && !runningF) {
            runningF = true;
            #pragma omp task
            F();
        }
    }

    std::cout << "A finished " << sleepTime / 1000000.0 << " segundos.\n";
}

void B() {
    int sleepTime = rand() % 1000000;
    #pragma omp critical
    runningB = true;
    
    usleep(sleepTime);
    resultB = sleepTime;

    std::cout << "B finished " << sleepTime / 1000000.0 << " segundos.\n";

    #pragma omp critical
    {
        finishedB = true;
        if (finishedA && !runningF) { //si A terminó y F no está corriendo
            runningF = true;
            #pragma omp task
            F(); //Ejecutamos F
        }
        if (finishedC && !runningD) { //si C terminó y D no está corriendo
            runningD = true;

            #pragma omp task
            D();  //Ejecutamos D
        }
    }
}

void C() {
    int sleepTime = rand() % 1000000;
    #pragma omp critical

    runningC = true;
    
    usleep(sleepTime);
    resultC = sleepTime;
    std::cout << "C finished " << sleepTime / 1000000.0 << " segundos.\n";
    #pragma omp critical
    {
        finishedC = true;   
        if (finishedB && !runningD) {
            runningD = true;

            #pragma omp task
            D();
        }
    }
}

void D() {
    int sleepTime = rand() % 1000000;

    usleep(sleepTime);
    resultD = sleepTime;
    std::cout << "D finished " << sleepTime / 1000000.0 << " segundos.\n";
    #pragma omp critical
    {
        finishedD = true;
        if (finishedF && !runningE) {
            runningE = true;

            #pragma omp task
            {
                E();
            }
        }
    }
}

void F() {
    int sleepTime = rand() % 1000000;

    usleep(sleepTime);
    resultF = sleepTime;

    std::cout << "F finished " << sleepTime / 1000000.0 << " segundos.\n";
    #pragma omp critical
    {
        finishedF = true;
        if (finishedD && !runningE) {
            runningE = true;
            #pragma omp task
            E();
        }
    }
}

void E() {
    int sleepTime = rand() % 1000000;

    usleep(sleepTime);
    resultE = sleepTime;

    #pragma omp critical
    finishedE = true;

    std::cout << "E finished " << sleepTime / 1000000.0 << " segundos.\n";
}

int main() {
    srand(time(0));
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            A();
            #pragma omp task
            B();
            #pragma omp task
            C();
        }
        #pragma taskwait
    }

    std::cout << "Fin del programa" << std::endl;
    return 0;
}
