#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

struct ThreadData {
    int* data;
};

void* A(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"A finished "<<sleepTime/1000000.0 << " segundos.\n";
    return nullptr;
}

void* B(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"B finished "<<sleepTime/1000000.0 << " segundos.\n";
    return nullptr;
}

void* C(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"C finished "<<sleepTime/1000000.0 << " segundos.\n";
    return nullptr;
}

void* D(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"D finished "<<sleepTime/1000000.0 << " segundos.\n";
    return nullptr;
}

void* E(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sleepTime = rand() % 1000000;
    usleep(sleepTime);
    *(data->data) = sleepTime;
    std::cout<<"E finished "<<sleepTime/1000000.0 << " segundos.\n";
    return nullptr;
}

int main() {
    srand(time(0));

    pthread_t threadA, threadB, threadC, threadD, threadE;
    int resultA, resultB, resultC, resultD, resultE;
    ThreadData dataA = {&resultB}, dataB = {&resultB}, dataC = {&resultC}, dataD = {&resultD}, dataE = {&resultE};

    //Se inician en paralelo A, B y C, pues no tienen dependencias
    pthread_create(&threadA, NULL, A, (void*)&dataA);
    pthread_create(&threadB, NULL, B, (void*)&dataB);
    pthread_create(&threadC, NULL, C, (void*)&dataC);

    //Se espera a B y C para ejecutar D mientras A sigue corriendo
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_create(&threadD, NULL, D, (void*)&dataD);

    //Se espera A y D para ejecutar E
    pthread_join(threadA, NULL);
    pthread_join(threadD, NULL);
    pthread_create(&threadE, NULL, E, (void*)&dataE);

    //Se espera a E para terminar la ejecución
    pthread_join(threadE, NULL);

    //El resultado final muestra variantes en el orden de ejecución de A, B, y C pero E termina siempre de último.
    std::cout << "Fin del programa" << std::endl;
    return 0;
}
