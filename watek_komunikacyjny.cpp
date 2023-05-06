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
                // println("Czy REQUEST dotyczy guida %d", pakiet.typeGuide);
                //TODO: jeżeli type=przewodnik to push do guidesQueue
                if (pakiet.typeGuide == 1) {
                    guidesQueue.push(pakiet);
                    packet_t *tmpPacket = new packet_t;
                    tmpPacket->typeGuide = 1;
                    sendPacket(tmpPacket, status.MPI_SOURCE, ACK ); 
                    free(tmpPacket);
                } else {
                    // println("Ktoś coś prosi. A niech ma!")
                    // println("Request przyszedl od %d", pakiet.src);                 
                    sectionQueue.push(pakiet);
                    debug("Ktoś coś prosi. A niech ma!")
                    sendPacket( 0, status.MPI_SOURCE, ACK ); 
                }
	            break;
        }
	    case ACK: 
        {
                //TODO: jeżeli type=przewodnik
                //println("Czy pakiet dotyczy guida %d", pakiet.typeGuide);
                if (pakiet.typeGuide == 1) {
                    ackGuides++;
                    // println("stan ack guides %d dla procesu %d\n", ackGuides, rank);
                } else {
                    debug("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount);
                    // println("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount);
	                ackCount++;
                }
                /* czy potrzeba tutaj muteksa? Będzie wyścig, czy nie będzie? Zastanówcie się. */
            break;
        }
        case RELEASE:
        {
             //TODO: jeżeli type=przewodnik removeBySrc do guidesQueue
            if (pakiet.typeGuide == 1) {
                guidesQueue.removeBySrc(pakiet.src);
            } else {
                sectionQueue.removeBySrc(pakiet.src); //Usuwanie procesu z kolejki
            }
            // println("DOSTALEM RELEASE OD %d", status.MPI_SOURCE);
            // printf("USUWAM proces %d z kolejki %d\n", pakiet.src, rank);
            break;
        }
	    default:
	    break;
        }
    }
}
