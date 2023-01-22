#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "events.h"

int happinessSum = 0, moneySum = 0, happinessDiff = 0, sumOfHappinessSum = 0, sumOfMoneySum = 0;

/*
    Patrzy na różnice wieku, dochodu i szczęścia między człowiekiem a jego parą, i zależnie od tego zmienia jego szczęście.
*/
static void changeHumansHappinessDependingOnTheirSpouse(struct human** humanMemAddr){
    if((*humanMemAddr)->spouse != NULL){
        int ageDiff = (*humanMemAddr)->age - (*humanMemAddr)->spouse->age;
        int salaryDiff = (*humanMemAddr)->salary - (*humanMemAddr)->spouse->salary;
        happinessDiff = (*humanMemAddr)->happiness - (*humanMemAddr)->spouse->happiness;
        happinessSum += (ageDiff/5)-(salaryDiff/500)-(happinessDiff/5);
    }
    else{
        (*humanMemAddr)->happiness -= 10;
    }
}

/*
    Wybiera dom w którym żyje człowiek, a potem zmienia statystki tego człowieka zależnie od właściwości domu.
*/
static void changeHumansPropertiesDependingOnTheirHouse(struct human** humanMemAddr, struct humanArray** humanArrayMemAddr){
    //Upewnia się, że człowiek posiada jakikolwiek dom, jeśli nieposiada to szuka domu u sprzedawcy.
    if((*humanMemAddr)->houses != NULL && (*humanMemAddr)->houses->size != 0){

        struct house* mainHouse = pickMainHouseForNormalHuman((*humanMemAddr)->houses);

        happinessSum += ((int)sqrt((double)(mainHouse->gardenSizeLevel*mainHouse->parent->parent->greeneryLevel)))/5;//Szczęście od wielkości podwórka powiększone przez poziom zieleni dzielnicy
        happinessSum += mainHouse->numberOfRooms*2;
        happinessSum += mainHouse->comfortLevel/4;
        happinessSum -= 100/(((happinessSum*mainHouse->parent->parent->crimeLevel)*2)+1);//Zmniejszenie sumy szczęścia o % poziomu przestępstwa * 2

        moneySum += (*humanMemAddr)->salary+(100/((((*humanMemAddr)->salary*mainHouse->parent->parent->jobMarketLevel)*2)+1));//Dodanie do sumy pieniędzy wypłaty człowieka z dodatkowym % poziomu rynku pracy * 2

        if(happinessDiff < 0 ){//Jeśli różnica szczęścia między człowiekiem a partnerem jest mniejsza od 0 to dodaj od 0% do 20% jej z powrotem, zależnie od grubości scian w mieszkaniu.
            if((100-mainHouse->wallWidthLevel)/20 == 0){
                happinessSum -= happinessDiff;
            }
            else{
                happinessSum -= (happinessDiff/((100-mainHouse->wallWidthLevel)/20));
            }
        }

        //Jeśli człowiek jest dosyć smutny to niech szuka nowego domu do kupna
        if((*humanMemAddr)->happiness < 10+random(0,21)){
            lookForHouseFromTrader(humanArrayMemAddr,((*humanMemAddr)->money/2)+(*humanMemAddr)->salary, humanMemAddr);
        }

        //LOSOWE WYDARZENIE JEEEJ
        void chooseARandomEvent(mainHouse, humanMemAddr, humanArrayMemAddr, day);

        //Płać rachunki za każdy z domów, jeśli ma sie partnera to płacisz tylko połowe
        for(int k = 0; k < (*humanMemAddr)->houses->size; k++){
            (*humanMemAddr)->money -= (*humanMemAddr)->houses->houses[k]->value/((*humanMemAddr)->spouse == NULL ? 1 : 2);
            happinessSum += 2;
        }

        //Jeśli ma się pieniądze poniżej 0 to sprzedawaj swoje domy dopóki nie ma z powrotem pozytywnej sumy
        while((*humanMemAddr)->money < 0 && (*humanMemAddr)->houses->size != 0){
            eventsInADay++;
            printf("\n%i> Day %i) %s went BROKE and had to sell his most valuable house.",eventsInADay, day, (*humanMemAddr)->name);
            sellMostValuableHouse(humanMemAddr);
        }
    }
    else{
        lookForHouseFromTrader(humanArrayMemAddr,((*humanMemAddr)->money/2)+(*humanMemAddr)->salary, humanMemAddr);
    }
}

/*
    Jeśli człowiek ma za mało pieniędzy, jest zbyt smutny, zbyt stary albo za młody to niech umrze.
*/
static void lookForDeathTriggers(struct human** humanMemAddr, struct humanArray** humanArrayMemAddr){

    if((*humanMemAddr)->money < 0){
        eventsInADay++;
        printf("\n%i> Day %i) %s couldn't find any shelter or food and STARVED to death.",eventsInADay, day, (*humanMemAddr)->name);
        deleteHuman(humanArrayMemAddr,humanMemAddr);

        return;
    }

    if((*humanMemAddr)->happiness < 0){
        eventsInADay++;
        printf("\n%i> Day %i) %s doesn't see the point in life anymore and decides to commit SUICIDE.",eventsInADay, day, (*humanMemAddr)->name);
        deleteHuman(humanArrayMemAddr,humanMemAddr);
        return;
    }

    if((*humanMemAddr)->age > 100){
        eventsInADay++;
        printf("\n%i> Day %i) %s kicks the bucket and DIES of old age surrounded by his loved ones.",eventsInADay, day, (*humanMemAddr)->name);
        deleteHuman(humanArrayMemAddr,humanMemAddr);
        return;
    }

    if((*humanMemAddr)->age < 0){
        eventsInADay++;
        printf("\n%i> Day %i) %s deaged themselves to the point of becoming an embryo and DIED.",eventsInADay, day, (*humanMemAddr)->name);
        deleteHuman(humanArrayMemAddr,humanMemAddr);
        return;
    }
}

/*
    Przejdź przez wszystkie właściwości i własności człowieka i zsymuluj jaki będą miały na niego efekt.
*/
static void runAHumansDay(struct human** humanMemAddr, struct humanArray** humanArrayMemAddr){

    (*humanMemAddr)->age++;
    changeHumansHappinessDependingOnTheirSpouse(humanMemAddr);

    changeHumansPropertiesDependingOnTheirHouse(humanMemAddr, humanArrayMemAddr);

    //Jeśli człowiek nie ma partnera i jest smutny to niech szuka jakiegoś
    if((*humanMemAddr)->spouse == NULL && (*humanMemAddr)->happiness<30){
        lookForSpouse(humanArrayMemAddr, humanMemAddr);
    }
}

/*
    Zamień pieniądze/szczęście zdobyte/utracone w dniu przez człowieka.
*/
static void sumUpHumansMoneyAndHappiness( bool isTrader, struct human** humanMemAddr){
    //Oblicz otrzymane szczęście i pieniądze troche inaczej jeśli to sprzedawca.
    if(isTrader){
        happinessSum = happinessSum/3;
        if((*humanMemAddr)->money != 0){
            //Dodaj do szczęścia procent zysku w porównaniu do wszystkich pieniędzy.
            happinessSum += moneySum/(*humanMemAddr)->money*100;
        }
        moneySum = moneySum*2;
    }

    //Dodaj człowiekowi wykalkulowane sumy pieniędzy i szczęścia oraz dodaj je do sum sum a potem wyczyść.
    (*humanMemAddr)->happiness += happinessSum;
    (*humanMemAddr)->money += moneySum;
    sumOfHappinessSum += happinessSum;
    sumOfMoneySum += moneySum;
    happinessSum = 0;
    moneySum = 0;
}

/*
    Przechodzi przez wszystkich ludzi i symuluje im jeden dzień.
*/
static void runOneDayOfTheSimulation(struct humanArray** humanArrayMemAddr, struct provinceArray** cityMemAddr){
    sumOfHappinessSum = 0;
    sumOfMoneySum = 0;
    eventsInADay = 0;
    day++;



    for(int i = 0; i < (*humanArrayMemAddr)->normalSize ;i++){
        runAHumansDay(&(*humanArrayMemAddr)->humans[0][i],humanArrayMemAddr);
        sumUpHumansMoneyAndHappiness(false,&(*humanArrayMemAddr)->humans[0][i]);
        lookForDeathTriggers(&(*humanArrayMemAddr)->humans[0][i], humanArrayMemAddr);
    }

    for(int i = 0; i < (*humanArrayMemAddr)->traderSize ;i++){
        runAHumansDay(&(*humanArrayMemAddr)->humans[1][i], humanArrayMemAddr);
        buyRandomEmptyHouse(*cityMemAddr, &(*humanArrayMemAddr)->humans[1][i]);
        sumUpHumansMoneyAndHappiness(true, &(*humanArrayMemAddr)->humans[1][i]);
        lookForDeathTriggers(&(*humanArrayMemAddr)->humans[1][i], humanArrayMemAddr);
    }

    int amountOfAllHumans = (*humanArrayMemAddr)->normalSize + (*humanArrayMemAddr)->traderSize;
    if(amountOfAllHumans == 0){
        printf("\n\nThere are no people left in the city, everybody is dead.");
        return;
    }
    printf("\nNEW DAY %i) Avrg. happiness: %i | Avrg. money: %i", day,sumOfHappinessSum/amountOfAllHumans, sumOfMoneySum/amountOfAllHumans);
}

/*
    Przechodzi przez podaną ilość dni i je symuluje.
*/
void runNDaysOfSimulation(struct humanArray** humanArrayMemAddr, struct provinceArray** cityMemAddr,  int days){
    for(int i = 0; i < days; i++){
        runOneDayOfTheSimulation(humanArrayMemAddr, cityMemAddr);
    }
}
