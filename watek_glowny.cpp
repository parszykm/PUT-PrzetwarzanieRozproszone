#include "main.h"
#include "watek_glowny.h"
// #include "queue.h"
std::vector<int> sekcja;
void mainLoop()
{
    srandom(rank);
    int tag;
    int perc;
	int hotelChosen;
	int randomHotelIndex;
	ProcessQueue *hotel = nullptr;
	int colorPercentage;
	
		if(processType == BLUE)
			colorPercentage = BLUE_PERCENTAGE;
		else if(processType == PURPLE)
			colorPercentage = PURPLE_PERCENTAGE;
		else
			colorPercentage = CLEANER_PERCENTAGE;

    while (stan != InFinish) {

	switch (stan) {
	    case InRun: 
		{
			if(processType == CLEANER) sleep(5); //Proces sprzątacza niech uruchamia się rzadziej od fioletowych i niebieskich
			perc = random()%100;
			if ( perc < 25 ) {
					debug("Perc: %d", perc);
					println("Ubiegam się o sekcję krytyczną")

					debug("Zmieniam stan na wysyłanie");
					packet_t *pkt = new packet_t;
					pkt->data = perc;
					pkt->processType = processType2Int(processType);
					packet_t *tmpPacket = new packet_t{clockVar, rank, perc, processType2Int(processType), 0, -1};
					auto it = sectionQueues.begin();
					if(processType == CLEANER){
						
						randomHotelIndex = ( std::rand() % ( hotelNumber + 1 ) ) - 1;
						println("Wybrałem indeks hotelu %d do sprzątnięcia", randomHotelIndex);
						while(it != sectionQueues.end() && (int)(it - sectionQueues.begin()) != randomHotelIndex){
							++it;
						}
						tmpPacket->hotelIndex = it - sectionQueues.begin();
						it->push(*tmpPacket);
						pkt->hotelIndex = it - sectionQueues.begin();
						hotelChosen = (int)(it - sectionQueues.begin());
						hotel = &(*it);
						println("Wybrałem hotel %d", hotelChosen);	
					}
					else{
						while(it != sectionQueues.end()){

							println("Sprawdzam hotel %ld",it - sectionQueues.begin());
							if(it->isAvailable(*tmpPacket)){
								tmpPacket->hotelIndex = it - sectionQueues.begin();
								it->push(*tmpPacket);
								pkt->hotelIndex = it - sectionQueues.begin();
								hotelChosen = (int)(it - sectionQueues.begin());
								hotel = &(*it);
								println("Wybrałem hotel %d", hotelChosen);
								if(it->getQueueSize() < size*colorPercentage*0.5){
									break;
								}
								// break;
							}
							++it;
						}
					}				
					if(tmpPacket->hotelIndex == -1){
						break;
					}
					ackCount = 0;
						// bool res = sectionQueue.isAvailable(*tmpPacket);
						// if(!res) break;
						// sectionQueue.push(*tmpPacket);
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
					free(tmpPacket);
				} // a skoro już jesteśmy przy komendach vi, najedź kursorem na } i wciśnij %  (niestety głupieje przy komentarzach :( )
				debug("Skończyłem myśleć");
			break;
		}
	    case InWant:
		{
			println("Czekam na wejście do sekcji krytycznej do hotelu %d, ack %d, %d", hotelChosen, ackCount, hotel->isCandidate(rank, hotelCapacity, processType));			
			// tutaj zapewne jakiś muteks albo zmienna warunkowa
			// bo aktywne czekanie jest BUE

			if ( ackCount == size - 1 &&  
			hotel->isCandidate(rank, hotelCapacity, processType)
			// sectionQueue.isCandidate(rank, hotelCapacity, processType)
			){
				changeState(InSection);
			}
			break;
		}
	    case InSection:
		{
			// tutaj zapewne jakiś muteks albo zmienna warunkowa

			// std::string sectionState = printVector(sekcja); 
			println("Jestem w sekcji krytycznej(hotelu) %d", hotelChosen);
			if (processType == CLEANER) {
				changeState(InSectionGuide);
				break;
			}
			// std::cout<< sectionState<<std::endl;
			ackGuides = 0;
			//TODO: wysyłanie REQUEST z polem type=przewodnik do wszystkich i czeka na ackCount = N - P + push do guidesQueue


			packet_t *tmpPacket = new packet_t{clockVar, rank, perc, processType2Int(processType), 1};
			guidesQueue.push(*tmpPacket);
			for (int i=0;i<=size-1;i++){
				if (i!=rank) sendPacket( tmpPacket, i, REQUEST);
			}
			changeState(InWantGuide);
			free(tmpPacket);
			break;
		}
		case InWantGuide:
		{
			println("Czekam na przewodnika.\n");
			if ( ackGuides >= size - guides &&  guidesQueue.isOnFirstNthPlaces(rank, guides)){
				changeState(InSectionGuide);
			}
			break;
		}
		case InSectionGuide:
		{
			if (processType != CLEANER){
				println("Jestem z przewodnikiem!");
				sleep(5);
				println("Kończę wycieczkę z przewodnikiem")
				packet_t *pkt = new packet_t;
				pkt->data = perc;
				pkt->typeGuide = 1;
				guidesQueue.removeBySrc(rank);
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
				{
					sendPacket( pkt, i, RELEASE);
				}
				free(pkt);
				//TODO: wysyłanie RELEASE dla guidesQueue i removeBySrc siebie
				debug("Perc: %d", perc);
			}


		    println("Wychodzę z sekcji krytycznej")
			
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt_end = new packet_t;
		    pkt_end->data = perc;
			pkt_end->typeGuide = 0;
			pkt_end->hotelIndex = hotelChosen;
			hotel->removeBySrc(rank);
		    for (int i=0;i<=size-1;i++)
			if (i!=rank)
			{
			    sendPacket( pkt_end, i, RELEASE);
			}
		    changeState( InRun );
		    free(pkt_end);
			break;
		}
	    default:
			break;
        }
        sleep(SEC_IN_STATE);
    }
}
