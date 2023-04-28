#include "main.h"
#include "watek_glowny.h"
// #include "queue.h"

void mainLoop()
{
    srandom(rank);
    int tag;
    int perc;

    while (stan != InFinish) {
	switch (stan) {
	    case InRun: 
			perc = random()%100;
			if ( perc < 25 ) {
					debug("Perc: %d", perc);
					println("Ubiegam się o sekcję krytyczną")
					
					debug("Zmieniam stan na wysyłanie");
					packet_t *pkt = new packet_t;
					pkt->data = perc;
					ackCount = 0;
					pthread_mutex_lock( &queueMut );
						packet_t tmpPacket = {clockVar, rank, perc};
						queuePacket.push(tmpPacket);
						// printf("Ubiegam się %d - %d",queuePacket.top().src, queuePacket.empty());
						// printf("PAKIET %d, %d, %d\n", clockVar, rank, perc);
        			pthread_mutex_unlock( &queueMut );
					// queuePacket.push(*pkt);
					//TODO: Dodawanie do kolejki siebie
					for (int i=0;i<=size-1;i++)
					if (i!=rank)
						sendPacket( pkt, i, REQUEST);
					changeState( InWant ); // w VI naciśnij ctrl-] na nazwie funkcji, ctrl+^ żeby wrócić
							// :w żeby zapisać, jeżeli narzeka że w pliku są zmiany
							// ewentualnie wciśnij ctrl+w ] (trzymasz ctrl i potem najpierw w, potem ]
							// między okienkami skaczesz ctrl+w i strzałki, albo ctrl+ww
							// okienko zamyka się :q
							// ZOB. regułę tags: w Makefile (naciśnij gf gdy kursor jest na nazwie pliku)
					free(pkt);
				} // a skoro już jesteśmy przy komendach vi, najedź kursorem na } i wciśnij %  (niestety głupieje przy komentarzach :( )
				debug("Skończyłem myśleć");
			break;
	    case InWant:
			// printf("%d - %d",queuePacket.top().src, queuePacket.empty());
			println("Czekam na wejście do sekcji krytycznej");
			// pthread_mutex_lock( &queueMut );
			// 	showQueue();
			// pthread_mutex_unlock( &queueMut );
			
			// tutaj zapewne jakiś muteks albo zmienna warunkowa
			// bo aktywne czekanie jest BUE
			// println("warunek: %b", ackCount == size - 1 && queuePacket.top().src == rank);
			// printf("%d", rank);
			if ( ackCount == size - 1 && queuePacket.top().src == rank
			) {
				changeState(InSection);
			}
			break;
	    case InSection:
		{
		// tutaj zapewne jakiś muteks albo zmienna warunkowa
		println("Jestem w sekcji krytycznej")
		
		    sleep(5);
		//if ( perc < 25 ) {
		    debug("Perc: %d", perc);
		    println("Wychodzę z sekcji krytycznej")
			
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt = new packet_t;
		    pkt->data = perc;

		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			    sendPacket( pkt, (rank+1)%size, RELEASE);
		    changeState( InRun );
		    free(pkt);
		//}
			break;
		}

	    default:
			break;
            }
        sleep(SEC_IN_STATE);
    }
}
