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
                // println("Ktoś coś prosi. A niech ma!")
                // println("Request przyszedl od %d", pakiet.src);                 
                    sectionQueue.push(pakiet);
                debug("Ktoś coś prosi. A niech ma!")
                sendPacket( 0, status.MPI_SOURCE, ACK ); 
	            break;
        }
	    case ACK: 
                debug("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount);
                // println("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount);
	        ackCount++; /* czy potrzeba tutaj muteksa? Będzie wyścig, czy nie będzie? Zastanówcie się. */
            break;
        case RELEASE:
        {
            // println("DOSTALEM RELEASE OD %d", status.MPI_SOURCE);
            // printf("USUWAM proces %d z kolejki %d\n", pakiet.src, rank);
            sectionQueue.removeBySrc(pakiet.src); //Usuwanie procesu z kolejki
            break;
        }
	    default:
	    break;
        }
    }
}
