#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

struct ThreadData {
    int* data;
};


pthread_t threadA, threadB, threadC, threadD, threadE, threadF;
int resultA, resultB, resultC, resultD, resultE, resultF;
ThreadData dataA, dataB, dataC, dataD, dataE, dataF;

bool threadRunning[6] = {false};
bool threadFinished[6] = {false};

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  // for locks

void* A(void * arg); void* B(void * arg); void* C(void * arg); void* D(void * arg); void* E(void * arg); void* F(void * arg);

void* A(void* arg) {
    pthread_mutex_lock(&mtx);
    threadRunning[0] = true;
    pthread_mutex_unlock(&mtx);

    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"A finished "<<sleepTime/1000000.0 << " segundos.\n";

    pthread_mutex_lock(&mtx);
    threadFinished[0] = true;
    if (threadFinished[1] && !threadRunning[5]) //si b terminó y F no está corriendo
    {
        threadRunning[5] = true;
        pthread_create(&threadF, NULL, F, (void*)&dataF); //ejecutamos F
    }
    pthread_mutex_unlock(&mtx);


    return nullptr;
}

void* B(void* arg) {
    pthread_mutex_lock(&mtx);
    threadRunning[1] = true; //marca B como en ejecución
    pthread_mutex_unlock(&mtx);

    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"B finished "<<sleepTime/1000000.0 << " segundos.\n";

    pthread_mutex_lock(&mtx);
    threadFinished[1] = true;
    if (threadFinished[0] && !threadRunning[5]) //si A terminó y F no está corriendo
    {
        threadRunning[5] = true;
        pthread_create(&threadF, NULL, F, (void*)&dataF); //ejecutamos F
    }
    if (threadFinished[2] && !threadRunning[3]) //si C terminó y D no está corriendo
    {
        threadRunning[3] = true;
        pthread_create(&threadD, NULL, D, (void*)&dataD); //ejecutamos D
    }
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

void* C(void* arg) {
    pthread_mutex_lock(&mtx);
    threadRunning[2] = true;
    pthread_mutex_unlock(&mtx);

    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"C finished "<<sleepTime/1000000.0 << " segundos.\n";

    pthread_mutex_lock(&mtx);
    threadFinished[2] = true;
    if (threadFinished[1] && !threadRunning[3]) //si B terminó y D no está corriendo
    {
        threadRunning[3] = true;
        pthread_create(&threadD, NULL, D, (void*)&dataD); //ejecutamos D
    }
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

void* D(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"D finished "<<sleepTime/1000000.0 << " segundos.\n";

    pthread_mutex_lock(&mtx);
    threadFinished[3] = true;
    if (threadFinished[5] && !threadRunning[4]) //si F terminó y E no está corriendo
    {
        threadRunning[4] = true;
        pthread_create(&threadE, NULL, E, (void*)&dataE); //ejecutamos E
    }
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

void* F(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"F finished "<<sleepTime/1000000.0 << " segundos.\n";

    pthread_mutex_lock(&mtx);
    threadFinished[5] = true;
    if (threadFinished[3] && !threadRunning[4]) //si D terminó y E no está corriendo
    {
        threadRunning[4] = true;
        pthread_create(&threadE, NULL, E, (void*)&dataE); //ejecutamos E
    }
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

void* E(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"E finished "<<sleepTime/1000000.0 << " segundos.\n";

    pthread_mutex_lock(&mtx);
    threadFinished[4] = true;
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

int main() {
    srand(time(0));

    dataA = {&resultB}; dataB = {&resultB}; dataC = {&resultC}; dataD = {&resultD}; dataE = {&resultE}; dataF = {&resultF};

    //Se inician en paralelo A, B y C, pues no tienen dependencias
    pthread_create(&threadA, NULL, A, (void*)&dataA);
    pthread_create(&threadB, NULL, B, (void*)&dataB);
    pthread_create(&threadC, NULL, C, (void*)&dataC);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_join(threadD, NULL);
    pthread_join(threadF, NULL);
    pthread_join(threadE, NULL);

    //El resultado final muestra variantes en el orden de ejecución de A, B, y C pero E termina siempre de último.
    std::cout << "Fin del programa" << std::endl;
    return 0;
}
