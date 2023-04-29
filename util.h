#ifndef UTILH
#define UTILH
#include "main.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

/* typ pakietu */
typedef struct {
    int ts;       /* timestamp (zegar lamporta */
    int src;  
    int data;     /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
    std::string processType; /* typ procesu = {niebieski, fioletowy, sprzątacz}*/
} packet_t;

extern int clockVar;
/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 3

/* Typy wiadomości */
/* TYPY PAKIETÓW */
#define ACK     1
#define REQUEST 2
#define RELEASE 3
#define APP_PKT 4
#define FINISH  5
#define BLUE_PERCENTAGE 0.4
#define PURPLE_PERCENTAGE 0.4
#define CLEANER_PERCENTAGE 0.2
#define BLUE "niebieski"
#define PURPLE "fioletowy"
#define CLEANER "sprzątacz"

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();
std::string generateTypeForProcess(int rank, int size);
/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(packet_t *pkt, int destination, int tag);

typedef enum {InRun, InMonitor, InWant, InSection, InFinish} state_t;

extern state_t stan;
extern pthread_mutex_t stateMut;
extern pthread_mutex_t clockMut;
extern pthread_mutex_t queueMut;
/* zmiana stanu, obwarowana muteksem */
void changeState( state_t );
extern std::string printVector(const std::vector<int>& v);
extern void removeElement(std::vector<int>& v, int elem);
#endif
