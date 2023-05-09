#include "main.h"
#include "util.h"
MPI_Datatype MPI_PAKIET_T;

/* 
 * w util.h extern state_t stan (czyli zapowiedź, że gdzieś tam jest definicja
 * tutaj w util.c state_t stan (czyli faktyczna definicja)
 */
state_t stan=InRun;
int clockVar = 0;
/* zamek wokół zmiennej współdzielonej między wątkami. 
 * Zwróćcie uwagę, że każdy proces ma osobą pamięć, ale w ramach jednego
 * procesu wątki współdzielą zmienne - więc dostęp do nich powinien
 * być obwarowany muteksami
 */
pthread_mutex_t stateMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clockMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queueMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wantMut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct tagNames_t{
    const char *name;
    int tag;
} tagNames[] = { { "pakiet aplikacyjny", APP_PKT }, { "finish", FINISH}, 
                { "potwierdzenie", ACK}, {"prośbę o sekcję krytyczną", REQUEST}, {"zwolnienie sekcji krytycznej", RELEASE} };

const char *tag2string( int tag )
{
    for (int i=0; i <sizeof(tagNames)/sizeof(struct tagNames_t);i++) {
	if ( tagNames[i].tag == tag )  return tagNames[i].name;
    }
    return "<unknown>";
}
/* tworzy typ MPI_PAKIET_T
*/
void inicjuj_typ_pakietu()
{
    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    int       blocklengths[NITEMS] = {1,1,1,1,1,1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT,MPI_INT};

    MPI_Aint     offsets[NITEMS]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, data);
    offsets[3] = offsetof(packet_t, processType);
    offsets[4] = offsetof(packet_t, typeGuide);
    offsets[5] = offsetof(packet_t, hotelIndex);


    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PAKIET_T);

    MPI_Type_commit(&MPI_PAKIET_T);
}

/* opis patrz util.h */
void sendPacket(packet_t *pkt, int destination, int tag)
{
    int freepkt=0;
    if (pkt==0) { pkt = new packet_t; freepkt=1;}
    pkt->src = rank;
    pthread_mutex_lock( &clockMut );
    clockVar++;
    pthread_mutex_unlock( &clockMut );
    pkt->ts=clockVar;
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    debug("Wysyłam %s do %d\n", tag2string( tag), destination);
    if (freepkt) free(pkt);
}

void changeState( state_t newState )
{
    pthread_mutex_lock( &stateMut );
    if (stan==InFinish) { 
	pthread_mutex_unlock( &stateMut );
        return;
    }
    stan = newState;
    pthread_mutex_unlock( &stateMut );
}

std::string generateTypeForProcess(int rank, int size){
    if(rank < floor(size*BLUE_PERCENTAGE))
        return BLUE;
    else if(rank < floor(size*(BLUE_PERCENTAGE+PURPLE_PERCENTAGE)))
        return PURPLE;
    return CLEANER;

}
int generateColorCode(std::string processType){
    if(processType == BLUE)
        return 34;
    else if(processType == PURPLE)
        return 35;
    return 37;

}

std::string printVector(const std::vector<int>& v) {
    std::stringstream ss;
    for (int elem : v) {
        ss << elem << " ";
    }
    std::string result = ss.str();
    result.erase(result.find_last_not_of(" ") + 1);
    return result;
}

void removeElement(std::vector<int>& v, int elem) {
    auto it = std::find(v.begin(), v.end(), elem);
    if (it != v.end()) {
        v.erase(it);
    }
}

int colorEnemy(int colorInt){
    std::string color = Int2ProcessType(colorInt);
    if(color == BLUE) return processType2Int(PURPLE);
    else if(color == PURPLE) return processType2Int(BLUE);
    else return processType2Int(CLEANER);
}

extern int processType2Int(std::string processType){
    if(processType == BLUE) return 0;
    else if(processType == PURPLE) return 1;
    else return 2;
}
extern std::string Int2ProcessType(int processType){
    if(processType == 0) return BLUE;
    else if(processType == 1) return PURPLE;
    else return CLEANER;
}
int colorCode;