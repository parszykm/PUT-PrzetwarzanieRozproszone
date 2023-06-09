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
    int processType; /* typ procesu = {0 - niebieski, 1 - fioletowy, 2 - sprzątacz}*/
    int typeGuide; /*pole okreslajace czy ubiegamy sie o zasob jakim jest przewodnik */
    int hotelIndex;  /* indeks hotelu, którego dotyczy pakiet*/
} packet_t;

extern int clockVar;
/* packet_t ma szesc pól, więc NITEMS=5. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 6

/* Typy wiadomości */
/* TYPY PAKIETÓW */
#define ACK     1
#define REQUEST 2
#define RELEASE 3
#define APP_PKT 4
#define FINISH  5

#define ROOT 0
#define BLUE_PERCENTAGE 0.4
#define PURPLE_PERCENTAGE 0.4
#define CLEANER_PERCENTAGE 0.2
#define BLUE "niebieski"
#define PURPLE "fioletowy"
#define CLEANER "sprzątacz"
#define BLUE_INT 0
#define PURPLE_INT 1
#define CLEANER_INT 2
#define hotelNumber 1
extern int colorCode;

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();
std::string generateTypeForProcess(int rank, int size);
int generateColorCode(std::string processType);
/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(packet_t *pkt, int destination, int tag);

typedef enum {InRun, InMonitor, InWant, InSection, InWantGuide, InSectionGuide, InFinish} state_t;

extern state_t stan;
extern pthread_mutex_t stateMut;
extern pthread_mutex_t clockMut;
extern pthread_mutex_t queueMut;
extern pthread_mutex_t wantMut;
extern pthread_cond_t cond;
/* zmiana stanu, obwarowana muteksem */
void changeState( state_t );
extern std::string printVector(const std::vector<int>& v);
extern void removeElement(std::vector<int>& v, int elem);
extern int colorEnemy(int color);
extern int processType2Int(std::string processType);
extern std::string Int2ProcessType(int processType);
#endif
