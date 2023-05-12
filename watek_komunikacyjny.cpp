#include "main.h"
#include "watek_komunikacyjny.h"
#include<iostream>
// #include "queue.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */


void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while ( stan!=InFinish ) {
	debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        pthread_mutex_lock( &clockMut );
        clockVar = clockVar > pakiet.ts ? clockVar +1 : pakiet.ts +1;
        pthread_mutex_unlock( &clockMut );
        switch ( status.MPI_TAG ) {
	    case REQUEST: 
        {
                packet_t *tmpPacket = new packet_t;
                if (pakiet.typeGuide == 1) {
                    guidesQueue.push(pakiet); // Dodanie proszącego procesu do kolejki do przewodników
                    tmpPacket->typeGuide = 1;
                    sendPacket(tmpPacket, status.MPI_SOURCE, ACK ); 
                    
                } else {
                    auto it = hotels.begin();
                    while(it != hotels.end()){
                        if(it - hotels.begin() == pakiet.hotelIndex){
                            it->push(pakiet);
                            break;
                        }
                        ++it;
                    }                
                    debug("Ktoś coś prosi. A niech ma!")
                    tmpPacket->typeGuide = 0;
                    sendPacket(tmpPacket, status.MPI_SOURCE, ACK ); 
                }
                free(tmpPacket);
	            break;
        }
	    case ACK: 
        {
                if (pakiet.typeGuide == 1) {
                   // pthread_mutex_lock(&wantMut);
                    ackGuides++;
                    // if(ackGuides == size - guides){
                    //     pthread_mutex_unlock(&wantMut);
                    //     pthread_cond_signal(&cond);
                    // } else {pthread_mutex_unlock(&wantMut);} 
                } else {
                    debug("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount);
                    // pthread_mutex_lock(&wantMut);
                    ackCount++;
                    // if(ackCount == size - 1){
                    //     pthread_mutex_unlock(&wantMut);
                    //     pthread_cond_signal(&cond);
                    // } else {pthread_mutex_unlock(&wantMut);} 
                }

            break;
        }
        case RELEASE:
        {
            if (pakiet.typeGuide == 1) {
                guidesQueue.removeBySrc(pakiet.src); // Usunięcie wychodzącego procesu do kolejki do przewodników
            } else {
                auto it = hotels.begin();
                while(it != hotels.end()){
                    if(it - hotels.begin() == pakiet.hotelIndex){
                        it->removeBySrc(pakiet.src); // Usunięcie wychodzącego procesu z kolejki hotelu
                        it->setHotelState(Int2ProcessType(pakiet.processType));
                        break;
                    }
                    ++it;
                }  
            }
            debug("DOSTALEM RELEASE OD %d", status.MPI_SOURCE);
            debug("USUWAM proces %d z kolejki %d\n", pakiet.src, rank);
            break;
        }
	    default:
	    break;
        }
    }
}
