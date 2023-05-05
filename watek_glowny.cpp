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
			if(processType == CLEANER) sleep(5); //Proces sprzątacza niech uruchamia się rzadziej od fioletowych i niebieskich
			perc = random()%100;
			if ( perc < 25 ) {
					debug("Perc: %d", perc);
					println("Ubiegam się o sekcję krytyczną")

					debug("Zmieniam stan na wysyłanie");
					packet_t *pkt = new packet_t;
					pkt->data = perc;
					pkt->processType = processType2Int(processType);
					ackCount = 0;
						packet_t *tmpPacket = new packet_t{clockVar, rank, perc, processType2Int(processType), FALSE};
						bool res = sectionQueue.isAvailable(*tmpPacket);
						if(!res) break;
						sectionQueue.push(*tmpPacket);
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
			println("Czekam na wejście do sekcji krytycznej");			
			// tutaj zapewne jakiś muteks albo zmienna warunkowa
			// bo aktywne czekanie jest BUE
			if ( ackCount == size - 1 &&  sectionQueue.isCandidate(rank, hotelCapacity, processType)
			){
				changeState(InSection);
			}
			break;
	    case InSection:
		{
		// tutaj zapewne jakiś muteks albo zmienna warunkowa

		// std::string sectionState = printVector(sekcja); 
		println("Jestem w sekcji krytycznej");
		// std::cout<< sectionState<<std::endl;
			ackGuides = 0;
			//TODO: wysyłanie REQUEST z polem type=przewodnik do wszystkich i czeka na ackCount = N - P + push do guidesQueue

			packet_t *tmpPacket = new packet_t{clockVar, rank, perc, processType2Int(processType), TRUE};
			bool res = guidesQueue.isAvailable(*tmpPacket);
			if(!res) break;
			printf("available");
			guidesQueue.push(*tmpPacket);
			for (int i=0;i<=size-1;i++){
				if (i!=rank) sendPacket( tmpPacket, i, REQUEST);
			}
			printf("potwierdzenia o przewodnika %d\n", ackGuides);
			if ( ackGuides == size - guides &&  sectionQueue.isOnFirstNthPlaces(rank, guides)){
				println("Jestem z przewodnikiem");
				sleep(5);
			}
			else break;
		    sleep(2);

			println("Kończę wycieczkę z przewodnikiem!");

		    packet_t *pkt = new packet_t;
		    pkt->data = perc;
			pkt->typeGuide = TRUE;
			guidesQueue.removeBySrc(rank);
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			{
			    sendPacket( pkt, i, RELEASE);
			}
			//TODO: wysyłanie RELEASE dla guidesQueue i removeBySrc siebie
		//if ( perc < 25 ) {
		    debug("Perc: %d", perc);

		    println("Wychodzę z sekcji krytycznej")
			
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt_end = new packet_t;
		    pkt_end->data = perc;
			sectionQueue.removeBySrc(rank);
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			{
			    sendPacket( pkt_end, i, RELEASE);
			}
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
