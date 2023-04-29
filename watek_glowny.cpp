#include "main.h"
#include "watek_glowny.h"
// #include "queue.h"
std::vector<int> sekcja;
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
						packet_t tmpPacket = {clockVar, rank, perc, processType};
						sectionQueue.push(tmpPacket);
						// printf("Ubiegam się %d - %d",sectionQueue.top().src, sectionQueue.empty());
						// printf("PAKIET %d, %d, %d\n", clockVar, rank, perc);
        			pthread_mutex_unlock( &queueMut );
					// sectionQueue.push(*pkt);
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
			// printf("%d - %d",sectionQueue.top().src, sectionQueue.empty());
			println("Czekam na wejście do sekcji krytycznej");
			pthread_mutex_lock( &queueMut );
				printf("rank: %d, size: %d\n", rank, sectionQueue.getQueue().size());
			pthread_mutex_unlock( &queueMut );
			// pthread_mutex_lock( &queueMut );
			// 	showQueue();
			// pthread_mutex_unlock( &queueMut );
			
			// tutaj zapewne jakiś muteks albo zmienna warunkowa
			// bo aktywne czekanie jest BUE
			// println("warunek: %b", ackCount == size - 1 && sectionQueue.top().src == rank);
			// printf("%d", rank);
			if ( ackCount == size - 1 && 
			// sectionQueue.isCandidate(rank, hotelCapacity)
			sectionQueue.top().src == rank 
			) {
				changeState(InSection);
				sekcja.push_back(rank);
			}
			break;
	    case InSection:
		{
		// tutaj zapewne jakiś muteks albo zmienna warunkowa

		std::string sectionState = printVector(sekcja); 
		println("Jestem w sekcji krytycznej");
		std::cout<< sectionState<<std::endl;

		    sleep(5);
		//if ( perc < 25 ) {
		    debug("Perc: %d", perc);

		    println("Wychodzę z sekcji krytycznej")
			
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt = new packet_t;
		    pkt->data = perc;
			pthread_mutex_lock( &queueMut );
					sectionQueue.pop();
					removeElement(sekcja, rank);
			pthread_mutex_unlock( &queueMut );
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
