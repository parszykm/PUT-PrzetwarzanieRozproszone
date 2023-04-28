#include "main.h"
#include "watek_komunikacyjny.h"
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
                // println("Ktoś coś prosi. A niech ma!")
                //TODO: Dodawanie do kolejki procesu z requestem
                // printf("request przyszedl od %d - clock: %d", pakiet.src, pakiet.ts);
                pthread_mutex_lock( &queueMut );
                    queuePacket.push(pakiet);
                pthread_mutex_unlock( &queueMut );
                debug("Ktoś coś prosi. A niech ma!")

		sendPacket( 0, status.MPI_SOURCE, ACK );
	    break;
	    case ACK: 
                debug("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount);
                // println("Dostałem ACK od %d, mam już %d.", status.MPI_SOURCE, ackCount, clockVar);
	        ackCount++; /* czy potrzeba tutaj muteksa? Będzie wyścig, czy nie będzie? Zastanówcie się. */
        case RELEASE:
            // debug("Dostałem REALEASE od %d, usuwam %d z kolejki", status.MPI_SOURCE, queuePacket.top().src);
            pthread_mutex_lock( &queueMut );
                queuePacket.pop();
            pthread_mutex_unlock( &queueMut );
            break;
        //TODO: dodac case dla RELEASE
	    break;
	    default:
	    break;
        }
    }
}
