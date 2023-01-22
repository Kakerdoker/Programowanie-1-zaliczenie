#include "structures.h"
#include "miscelaneous.h"
#include "human_manager.h"

int eventsInADay = 0, day = 0;

char* returnRandomKillString(){
    eventsInADay++;
    switch(random(0,3)){
        case 0:
            return "\n%i-DEATH-Day %i) %s was a victim of a serial killer roaming the city and died.";
        break;
        case 1:
            return "\n%i-DEATH-Day %i) during a mugging %s decided not to give up their money and got shot as a result.";
        break;
        case 2:
            return "\n%i-DEATH-Day %i) %s died in a fire trying to rescue a child from a burning building.";
        break;
    }
}

char* returnHouseDestroyedString(){
    eventsInADay++;
    switch(random(0,3)){
        case 0:
            return "\n%i-HOUSE DESTROYED-Day %i) %s was a victim of a serial arsonist roaming the city and their house was burned down.";
        break;
        case 1:
            return "\n%i-HOUSE DESTROYED-Day %i) %s had everything stolen from their house, including the roof, the walls and the floor.";
        break;
        case 2:
            return "\n%i-HOUSE DESTROYED-Day %i) %s came back from works and their house is missing.";
        break;
    }
}

char* returnMoneyRobberyString(){
    eventsInADay++;
    switch(random(0,9)){
        case 0:
            return "\n%i-ROBBERY-Day %i) Somebody HACKED into %s bank account and stole %i money from them.";
        break;
        case 1:
            return "\n%i-ROBBERY-Day %i) %s got mugged on the street in front of their house and lost %i money.";
        break;
        case 2:
            return "\n%i-ROBBERY-Day %i) %s left their wallet with %i money in the supermarket, none of the employes know where it went.";
        break;
        case 3:
            return "\n%i-ROBBERY-Day %i) %s invested %i money in crypto, you can guess what happened to that money.";
        break;
        case 4:
            return "\n%i-ROBBERY-Day %i) %s was visited by the local mafia and had to pay them %i money.";
        break;
        case 5:
            return "\n%i-ROBBERY-Day %i) %s put %i money into starting a local business which got destroyed during a riot.";
        break;
        case 6:
            return "\n%i-ROBBERY-Day %i) %s send %i money to a nigerian prince, it was needed to securily transfer gold to america of which they would get a cut of.";
        break;
        case 7:
            return "\n%i-ROBBERY-Day %i) %s put %i money into an atm. The atm quickly plopped onto it's legs and ran away. It turns out it was just a man dressed as an ATM.";
        break;
        case 8:
            return "\n%i-ROBBERY-Day %i) %s lost %i money gambling.";
        break;
    }
}

char* returnSlightHappinessDecreaseString(int decrease){
    eventsInADay++;
    switch(decrease){
        case 0:
            return "\n%i-INTERACTION-Day %i) Somebody told %s today that they STINK! It really affected their mood :(";
        break;
        case 1:
            return "\n%i-INTERACTION-Day %i) %s was kicked in the nuts.";
        break;
        case 2:
            return "\n%i-INTERACTION-Day %i) %s found 5 money, it turned out to be monopoly money.";
        break;
        case 3:
            return "\n%i-INTERACTION-Day %i) %s went to a fortune teller, the drawn tarot cards were: death and the fool.";
        break;
        case 4:
            return "\n%i-INTERACTION-Day %i) %s lost a game of uno.";
        break;
        case 5:
            return "\n%i-INTERACTION-Day %i) %s wanted to but groceries but it was sunday.";
        break;
        case 6:
            return "\n%i-INTERACTION-Day %i) %s stubbed their toe.";
        break;
        case 7:
            return "\n%i-INTERACTION-Day %i) %s is having a really bad day today.";
        break;
    }
}


void chooseARandomEvent(struct house* mainHouse, struct human** humanMemAddr, struct humanArray ** humanArrayMemAddr){

    int randomEvent;
    switch(random(0,6)){
        case 1://crime event
            randomEvent = random(0,mainHouse->parent->parent->crimeLevel);
            if(randomEvent > 85){//Wydarzenie śmierci
                printf(returnRandomKillString(),eventsInADay, day, (*humanMemAddr)->name);
                deleteHuman(humanArrayMemAddr,humanMemAddr);
            }
            else if(randomEvent > 70){//Wydarzenie usunięcia domu
                int houseNum = random(0,(*humanMemAddr)->houses->size);
                printf(returnHouseDestroyedString(),eventsInADay, day, (*humanMemAddr)->name, (*humanMemAddr)->houses->houses[houseNum]->value);
                throwHouseIntoGarbage(&(*humanMemAddr)->houses->houses[houseNum]);
            }
            else if(randomEvent > 65){//Wydarzenie stracenia wszystkich pieniędzy
                printf(returnMoneyRobberyString(),eventsInADay, day, (*humanMemAddr)->name, (*humanMemAddr)->money);
                (*humanMemAddr)->money = 0;
            }
            else if(randomEvent > 55){//Wydarzenie stracenia kawałka pieniędzy
                printf(returnMoneyRobberyString(),eventsInADay, day, (*humanMemAddr)->name, (*humanMemAddr)->money/(random(2,4)));
                (*humanMemAddr)->happiness -= 10;
                (*humanMemAddr)->money = (*humanMemAddr)->money/2;
            }
            else if(randomEvent > 30){//Wydarzenie stracenia szczęścia
                int happinessDecrease = random(0,9);
                printf(returnSlightHappinessDecreaseString(happinessDecrease),eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->happiness -= happinessDecrease;
            }
            else{
                eventsInADay--;
            }

        break;
        case 2://greenery event
            randomEvent = random(0,mainHouse->parent->parent->greeneryLevel);
            if(randomEvent > 85){//decrease aging by 5 years

                printf("\n%i> Day %i) After meditating in their garden %s feels REJUVENATED, as if he had deaged 5 years",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->age -= 5;
            }
            else if(randomEvent > 80){//increase garden level by 5

                printf("\n%i>AAAAAAAAAAAAA Day %i) After tending to their plot, %s's garden has become so BEAUTIFUL that it became the city's park and %s won a prize.",eventsInADay, day, (*humanMemAddr)->name,(*humanMemAddr)->name);
                if(mainHouse->parent->parent->greeneryLevel+=2>100){
                    mainHouse->parent->parent->greeneryLevel = 10;
                    printf("++\n   It looks like the entire province is OVERGROWN! The city is taking action to stop any and all greenery!");
                };
                (*humanMemAddr)->money+=1000;
            }
            else if(randomEvent > 75){//decrease aging by a year

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s took a deep breath in his garden. Enjoying his life he feels YOUNG again.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->age -= 1;
            }
            else if(randomEvent > 65){//get 10 happiness

                printf("+\n%i> Day %i) %s looks out of the window to stare at his yard, he is CONTENT with his life.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->happiness += 5;
            }
            else if(randomEvent > 40){//decrease garden level by 5 in random property

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s has his food stolen by SQUIRRELS!.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->happiness--;
                (*humanMemAddr)->money--;
            }
        break;
        case 3://job market event
            randomEvent = random(0,mainHouse->parent->parent->jobMarketLevel);
            if(randomEvent > 85){//overwork to death

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s has been working overtime for the past 5 months and got a bonus of 10000 moneys! He also DIED from the exhuastion.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->money += 10000;
                deleteHuman(humanArrayMemAddr,humanMemAddr);
            }
            else if(randomEvent > 70){//get a raise

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s gets a salary INCREASE of 200 moneys.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->salary += 200;
            }
            else if(randomEvent > 65){//gain a bonus

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s gets a BONUS of 1000 moneys.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->money += 1000;
            }
            else if(randomEvent > 55){//get a demotion

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s has worked hard, but not hard enough so they got a DEMOTION.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->salary -= 50;
            }
            else if(randomEvent > 30){//lose 1-20 happiness

                printf("\n%i>AAAAAAAAAAAAA Day %i) %s is OVERBURDENED by their work life.",eventsInADay, day, (*humanMemAddr)->name);
                (*humanMemAddr)->happiness -= (random(1,21));
            }
        break;
    }
}
