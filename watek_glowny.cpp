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
	int leastLoaded;
	
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
			if(processType == CLEANER) sleep(5); //Proces sprzątacza niech uruchamia się rzadziej od fioletowych i niebieskich - co 5 sekund
			perc = random()%100;
			if ( perc < 25 ) {
					debug("Perc: %d", perc);
					println("Ubiegam się o sekcję krytyczną")
					debug("Zmieniam stan na wysyłanie");
					packet_t *pkt = new packet_t;
					pkt->data = perc;
					pkt->processType = processType2Int(processType);
					leastLoaded = size + 1;
					packet_t *tmpPacket = new packet_t{clockVar, rank, perc, processType2Int(processType), 0, -1};
					auto it = hotels.begin();
					if(processType == CLEANER){
						
						randomHotelIndex = ( std::rand() % ( hotelNumber + 1 ) ) - 1;
						println("Wybrałem indeks hotelu %d do sprzątnięcia", randomHotelIndex);
						while(it != hotels.end() && (int)(it - hotels.begin()) != randomHotelIndex){
							++it;
						}
						tmpPacket->hotelIndex = it - hotels.begin();
						it->push(*tmpPacket);
						pkt->hotelIndex = it - hotels.begin();
						hotelChosen = (int)(it - hotels.begin());
						hotel = &(*it);
						println("Wybrałem hotel %d", hotelChosen);	
					}
					else{
						// Przeszukiwanie hotelu, który jest dostępny dla danej fakcji i ma najmniejsze wypełnienie
						while(it != hotels.end()){
							debug("Sprawdzam hotel %ld",it - hotels.begin());
							if(it->isAvailable(*tmpPacket) && it->getQueueSize() < leastLoaded){
									leastLoaded = it->getQueueSize();
									tmpPacket->hotelIndex = it - hotels.begin();
									it->push(*tmpPacket);
									pkt->hotelIndex = it - hotels.begin();
									hotelChosen = (int)(it - hotels.begin());
									hotel = &(*it);
							}
							++it;
						}
					}				
					if(tmpPacket->hotelIndex == -1){
						break; //Jeżeli nie znalazło dostępnego hotelu przerwij
					}
					println("Wybrałem hotel %d, stan -> %s", hotelChosen, hotel->hotelState.c_str());
					ackCount = 0;
					for (int i=0;i<=size-1;i++)
					if (i!=rank)
						sendPacket( pkt, i, REQUEST);
					changeState( InWant );
					free(pkt);
					free(tmpPacket);
				}
				debug("Skończyłem myśleć");
			break;
		}
	    case InWant:
		{
			println("Czekam na wejście do sekcji krytycznej do hotelu %d, ack %d, %d", hotelChosen, ackCount, hotel->isCandidate(rank, hotelCapacity, processType));			
			pthread_mutex_lock(&wantMut);
			while (ackCount != size - 1 && !hotel->isCandidate(rank, hotelCapacity, processType)){
				println("Czekam na wejście do sekcji krytycznej do hotelu %d, ack %d, %d", hotelChosen, ackCount, hotel->isCandidate(rank, hotelCapacity, processType));			
				pthread_cond_wait(&cond, &wantMut);	
			}
			pthread_mutex_unlock(&wantMut);
			changeState(InSection);
			break;
		}
	    case InSection:
		{
			println("Jestem w hotelu %d (sekcji krytycznej)", hotelChosen);
			if (processType == CLEANER) {
				changeState(InSectionGuide);
				break;
			}
			ackGuides = 0;
			packet_t *tmpPacket = new packet_t{clockVar, rank, perc, processType2Int(processType), 1};
			guidesQueue.push(*tmpPacket); // Dodanie siebie do lokalnej kolejki do przewodników
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
			// Czekanie z zwrócenie conajmniej size - guides ACK 
			pthread_mutex_lock(&wantMut);
			while (ackGuides < size - guides && !guidesQueue.isOnFirstNthPlaces(rank, guides)){
				println("Czekam na przewodnika w hotelu %d, ack %d", hotelChosen, ackGuides);			
				pthread_cond_wait(&cond, &wantMut);	
			}
			pthread_mutex_unlock(&wantMut);
			changeState(InSectionGuide);
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
				guidesQueue.removeBySrc(rank); // Usunięcie siebie z lokalnej kolejki do przewodników
				for (int i=0;i<=size-1;i++)
				if (i!=rank)
				{
					sendPacket( pkt, i, RELEASE);
				}
				free(pkt);
				debug("Perc: %d", perc);
			}


		    println("Wychodzę z hotelu %d(sekcji krytycznej)", hotelChosen);
			hotel->setHotelState(processType);
		    debug("Zmieniam stan na wysyłanie");
		    packet_t *pkt_end = new packet_t;
		    pkt_end->data = perc;
			pkt_end->typeGuide = 0;
			pkt_end->hotelIndex = hotelChosen;
			pkt_end->processType = processType2Int(processType);
			hotel->removeBySrc(rank); // Usunięcie siebie z lokalnej kolejki(hotelu)
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
