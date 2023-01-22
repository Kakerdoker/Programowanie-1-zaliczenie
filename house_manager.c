#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "events.h"
#include "simulation.h"
#include "human_manager.h"

/*
    house_manager.c zawiera w sobie wszystkie funkcje dotyczące domów, ulic, dzielnic.
*/

/*
    Funkcja findEmptyHouse() wybiera losową dzielnice, a wniej losową ulice, a w niej losowy dom. Następnie sprawdza czy jest zajęta, jeśli jest to idzie do następnego domu.
    Jeśli zrobi pełne okrążenie wokół całej tablicy domów, to indeksuje tablice ulic o jeden do przodu, i szuka w niej domów w ten sam sposób co w poprzedniej.
    Jeśli przejdzie przez wszystkie ulice dzielnicy i nie znajdzie żadnego pustego domu, to indeksuje tablice dzielnic do przodu.
    Jeśli przejdzie przez wszystkie dzielnice bez znalezienia domu to zwraca NULL.
*/
struct house* findEmptyHouse(struct provinceArray* city){
    if(city->size == 0){return NULL;};

    int currentAmountOfProvinces = city->size-1;
    int initialRandProvince = random(0, currentAmountOfProvinces+1);

    int currentAmountOfStreets = city->provinces[initialRandProvince]->amountOfStreets-1;
    int initialRandStreet = random(0, currentAmountOfStreets+1);

    int currentAmountOfHouses = city->provinces[initialRandProvince]->streets[initialRandStreet]->size-1;
    int initialRandHouse = random(0, currentAmountOfHouses+1);

    int currentRandProvince = initialRandProvince;
    int currentRandStreet = initialRandStreet;
    int currentRandHouse = initialRandHouse;

    struct house* tempHouse = city->provinces[initialRandProvince]->streets[initialRandStreet]->houses[initialRandHouse];
    //printf("%i\n",tempHouse->value);

    while(1){
        if(tempHouse->isOwned == 'y'){
            //Jeśli aktualnie przeglądany dom jest przed pierwszym wybranym (w przypadku gdy pierwszy to 0 to sie zawija i leci na koniec), to przesuń ulice o jeden do przodu w ten sam sposób.
            if(currentRandHouse == (initialRandHouse == 0 ? currentAmountOfHouses : initialRandHouse-1)){
                //Jeśli aktualnie przeglądana ulica jest przed pierwsza wybrana (w przypadku gdy pierwszy to 0 to sie zawija i leci na koniec), to przesuń osiedle o jeden do przodu w ten sam sposób.
                if(currentRandStreet == (initialRandStreet == 0 ? currentAmountOfStreets : initialRandStreet-1)){
                    //Jeśli aktualnie przeglądane osiedle jest przed pierwszym wybranym (w przypadku gdy pierwszy to 0 to sie zawija i leci na koniec), to zwróc null, aby osoba wiedziała, że nie ma żadnych wolnych domów.
                    if(currentRandProvince == (initialRandProvince == 0 ? currentAmountOfProvinces : initialRandProvince-1)){
                        return NULL;
                    }
                    else{
                        //Jeśli aktualnie przeglądane osiedle jest równe albo większa wielkości arraya osiedl, to ustaw go na 0, jeśli nie to indeksuj o 1.
                        if(currentRandProvince >= currentAmountOfProvinces){
                            currentRandProvince = 0;
                        }
                        else{
                            currentRandProvince++;
                        }
                    }
                    currentAmountOfStreets = city->provinces[currentRandProvince]->amountOfStreets-1;
                    initialRandStreet = currentRandStreet = random(0, currentAmountOfStreets+1);
                }
                else{
                    //Jeśli aktualnie przeglądana ulica jest równa albo większa wielkości arraya ulic, to ustaw go na 0, jeśli nie to indeksuj o 1.
                    if(currentRandStreet >= currentAmountOfStreets){
                        currentRandStreet = 0;
                    }
                    else{
                        currentRandStreet++;
                    }
                }
                currentAmountOfHouses = city->provinces[currentRandProvince]->streets[currentRandStreet]->size-1;
                initialRandHouse = currentRandHouse = random(0, currentAmountOfHouses+1);
            }
            else{
                //Jeśli aktualnie przeglądany dom jest równy albo większy wielkości arraya domów, to ustaw go na 0, jeśli nie to indeksuj o 1.
                if(currentRandHouse >= currentAmountOfHouses){
                    currentRandHouse = 0;
                }
                else{
                    currentRandHouse++;
                }
            }
            tempHouse = city->provinces[currentRandProvince]->streets[currentRandStreet]->houses[currentRandHouse];
        }
        else{

            return tempHouse;
        }
    }
}

/*
    Funkcja tworzy tablice dla pustych domów, po czym ją wypełnia i zwraca strukture nowo zajętych domów.
*/
struct houseArray* findNEmptyHouses(int amountOfEmptyHouses, struct provinceArray* city, struct human* human){
    struct houseArray* emptyHousesArray = malloc(sizeof(struct houseArray));
    struct house**  emptyHouses = malloc(sizeof(struct house*)*amountOfEmptyHouses);

    if(amountOfEmptyHouses != 0){
        for(int i = 0 ; i<amountOfEmptyHouses ; i++){
            emptyHouses[i] = findEmptyHouse(city);


            //Jeśli podczas wypełniania skończą się puste domy to zamień ich wielkość i przerwij pętle.
            if(emptyHouses[i] == NULL){
                amountOfEmptyHouses = i;
                break;
            };

            //Upewnij się, że puste domy mają ustalonego rodzica.
            addHumanParentsToHouse(&emptyHouses[i], human);
            if(human->spouse != NULL){addHumanParentsToHouse(&emptyHouses[i], human->spouse);};
            //Upewnij się, że pusty dom jest zaznaczony jako zajęty.
            emptyHouses[i]->isOwned = 'y';
        }
    }
    emptyHousesArray->size=amountOfEmptyHouses;
    emptyHousesArray->houses=emptyHouses;
    return emptyHousesArray;
}

/*
    Funkcja wybiera losowy prefix, pare infixow, i sufix sklejając je w nazwe ulicy.
*/
char streetNamePrefix[15][8] = { "Euro\0","Dwor\0","Aleja\0","Mazo\0","Grun\0","Mari\0","Monte\0", "Jagiel\0","Poczt\0","Wielko\0","Slask\0","Wojsk\0","Emili\0","Rajk\0","Lubin\0"};
char streetNameMidfix[10][4] = { "an\0","ski\0","oln\0","orz\0","pej\0","gut\0","tyn\0","br\0","ock\0","isk\0" };
char streetNameEndfix[10][3] = { "ka\0","no\0","go\0","ry\0","er\0","ow\0","ja\0","ba\0","wa\0","wo\0" };
static void createRandomStreetName(char *name) {
    strcpy(name, streetNamePrefix[random(0, 15)]);
    for (int i = 0; i < random(0, 4); i++) {
        strcat(name, streetNameMidfix[random(0, 10)]);
    }
    strcat(name, streetNameEndfix[random(0, 10)]);
}


/*
    Funkcja tworzy i zwraca ulice wypełnioną losowo wygenerowanymi domami.
*/
char tempStreetString[20];
struct houseArray* createARandomStreet( int amountOfHouses, char* streetName,  struct province* parent){
    //Jeśli streetName jest równy "\0" to stwórz losową nazwę, w przeciwnym razie użyj podanej jako argument.
    strcpy(tempStreetString, streetName);
    if(streetName[0] == '\0'){
        createRandomStreetName(tempStreetString);
    }

    struct houseArray* street = malloc(sizeof(struct houseArray));
    struct house** houses = malloc(sizeof(struct house*)*amountOfHouses);

    //Stwórz podaną ilość losowych domów.
    for(int i = 0; i < amountOfHouses; i++){
        houses[i] = malloc(sizeof(struct house));
        initializeHouse(&houses[i], random(0,100),random(1,7)+random(0,7),random(0,100),random(0,100), 'n', street, NULL);
    }

    street->houses = houses;
    street->size = amountOfHouses;
    street->parent = parent;
    strcpy(street->streetName, tempStreetString);
    return street;
}

/*
    Funkcja tworzy i zwraca dzielnice wypełnioną losowo wygenerowanymi ulicami.
*/
struct province* generateRandomProvince( int amountOfStreets) {
    struct province* province = malloc(sizeof(struct province));

    //Stwórz podaną ilość losowo wygenerowanych ulic.
    struct houseArray** streets = malloc(sizeof(struct houseArray)*amountOfStreets);
    for (int i = 0 ; i < amountOfStreets ; i ++){
        streets[i] = createARandomStreet(random(6,27),"\0", province);
    }

    initializeProvince(&province, streets, amountOfStreets, random(0,100),random(0,100),random(0,100));
    return province;
}

/*
    Funkcja tworzy miasto z podaną ilością dzielnic.
*/
struct provinceArray* generateRandomCity( int amountOfProvinces){
    struct province** provinces = malloc(sizeof(struct province*) *amountOfProvinces);
    for(int i = 0; i < amountOfProvinces; i++){
        provinces[i] = generateRandomProvince(random(10,30));
    }
    struct provinceArray* city = malloc(sizeof(struct provinceArray));
    city->provinces=provinces;
    city->size=amountOfProvinces;
    return city;
}


/*
    Dodaje podaną dzielnice do podanego miasta.
*/
void addProvinceToCity(struct provinceArray** cityMemAddr,  struct province* provinceToAdd){
    (*cityMemAddr)->size++;
    (*cityMemAddr)->provinces = realloc((*cityMemAddr)->provinces, sizeof(struct houseArray*)*(*cityMemAddr)->size);
    (*cityMemAddr)->provinces[(*cityMemAddr)->size-1] = provinceToAdd;
}

/*
    Usuwa dzielnice o podanym indeksie z miasta.
*/
void deleteProvinceFromCity(struct provinceArray** cityMemAddr,  int provinceToDeleteIndex){
    //Jeśli podany indeks jest niepoprawny to wywal program.
    if(provinceToDeleteIndex < 0 || provinceToDeleteIndex > (*cityMemAddr)->size){
        printf("Tried to delete province using index out of range.");
        exit(1);
    }

    throwProvinceIntoGarbage(&(*cityMemAddr)->provinces[provinceToDeleteIndex]);

    //Przekopiuj całą tablice na samą siebie unikając indeksu do usunięcia.
    (*cityMemAddr)->size--;
    for(int i = 0; i < (*cityMemAddr)->size; i++){
        (*cityMemAddr)->provinces[i] = (*cityMemAddr)->provinces[i < provinceToDeleteIndex ? i : i+1];
    }

    (*cityMemAddr)->provinces = realloc((*cityMemAddr)->provinces, sizeof(struct province*)*(*cityMemAddr)->size);
}

/*
    Dodaje podaną ulice do podanej dzielnicy.
*/
void addStreetToProvince(struct province** provinceMemAddr, struct houseArray* streetToAdd){
    (*provinceMemAddr)->amountOfStreets++;
    (*provinceMemAddr)->streets = realloc((*provinceMemAddr)->streets, sizeof(struct houseArray*)*(*provinceMemAddr)->amountOfStreets);
    (*provinceMemAddr)->streets[(*provinceMemAddr)->amountOfStreets-1] = streetToAdd;
}


void deleteStreetFromProvince(struct province** provinceMemAddr,  int streetToDeleteIndex){
    //Jeśli podany indeks jest niepoprawny to wywal program.
    if(streetToDeleteIndex < 0 || streetToDeleteIndex > (*provinceMemAddr)->amountOfStreets){
        printf("Tried to delete street using index out of range.");
        exit(1);
    }

    throwStreetIntoGarbage(&(*provinceMemAddr)->streets[streetToDeleteIndex]);

    //Przekopiuj całą tablice na samą siebie unikając indeksu do usunięcia.
    (*provinceMemAddr)->amountOfStreets--;
    for(int i = 0; i < (*provinceMemAddr)->amountOfStreets; i++){
        (*provinceMemAddr)->streets[i] = (*provinceMemAddr)->streets[i < streetToDeleteIndex ? i : i+1];
    }

    (*provinceMemAddr)->streets = realloc((*provinceMemAddr)->streets, sizeof(struct houseArray*)*(*provinceMemAddr)->amountOfStreets);
}

/*
    Usuwa dom o podanym indeksie z ulicy.
*/
void deleteHouseFromStreet(struct houseArray** streetMemAddr,  int houseToDeleteIndex){
    //Jeśli podany indeks jest niepoprawny to wywal program.
    if(houseToDeleteIndex < 0 || houseToDeleteIndex > (*streetMemAddr)->size){
        printf("Tried to delete house using index out of range.");
        exit(1);
    }

    throwHouseIntoGarbage(&(*streetMemAddr)->houses[houseToDeleteIndex]);

    //Przekopiuj całą tablice na samą siebie unikając indeksu do usunięcia.
    (*streetMemAddr)->size--;
    for(int i = 0; i < (*streetMemAddr)->size; i++){
        (*streetMemAddr)->houses[i] = (*streetMemAddr)->houses[i < houseToDeleteIndex ? i : i+1];
    }

    (*streetMemAddr)->houses = realloc((*streetMemAddr)->houses, sizeof(struct house*)*(*streetMemAddr)->size);
}

/*
    Dodaje podany dom do podanej ulicy.
*/
void addHouseToStreet(struct houseArray** streetMemAddr,  struct house* houseToAdd){
    (*streetMemAddr)->size++;
    (*streetMemAddr)->houses = realloc((*streetMemAddr)->houses, sizeof(struct house*)*(*streetMemAddr)->size);
    (*streetMemAddr)->houses[(*streetMemAddr)->size-1] = houseToAdd;
}

/*
    Funkcja która usuwa wszystkich rodziców domu ze struktury houseParentArray.
*/
void removeAllParentsFromHouse(struct house** houseMemAddr){
    for(int i = 0; i<(*houseMemAddr)->parentHumans->size;i++){//Przejdź przez wszystkich rodziców.
        if((*houseMemAddr)->parentHumans->humans[i]->houses != NULL){//Upewnij się, że człowiek posiada jakiekolwiek domy.
            for(int j = 0; j < (*houseMemAddr)->parentHumans->humans[i]->houses->size; j++){//Przejdź przez wszyskie domy człowieka
                if((*houseMemAddr)->parentHumans->humans[i]->houses->houses[j] == (*houseMemAddr)){//Jeśli znajdziesz dom co ma ten sam pointer, co podany dom w argumencie
                    removeHouseOwnershipFromHuman(&(*houseMemAddr)->parentHumans->humans[i], j,false);//To usuń właścicielstwo tego domu człowiekowi wybranego przez pętle
                };
            }
        }
    }
}

/*
    Przejdź przez wszystkie domy posiadane przez człowieka i ustal który jest najcenniejszy dla osoby niehandlującej domami.
*/
struct house* pickMainHouseForNormalHuman( struct houseArray* houses){
    int currentScore = 0, highestScore = 0, highestScoreHouseIndex = 0;

    for(int i = 0; i < houses->size; i++){
        currentScore += houses->houses[i]->comfortLevel*20;
        currentScore += houses->houses[i]->gardenSizeLevel*5;
        currentScore += houses->houses[i]->numberOfRooms*10;
        currentScore += houses->houses[i]->wallWidthLevel/10;
        currentScore -= houses->houses[i]->value*2;

        //Jeśli bardziej cenny niż poprzedni to ustal nowy najcenniejszy index.
        if(currentScore > highestScore){
            highestScore = currentScore;
            highestScoreHouseIndex = i;
        }
    }
    return houses->houses[highestScoreHouseIndex];
}

/*
    Funkcja która przegląda wszystkie domy sprzedawane przez kupców, ustala cene sprzedaży i jeśli jest mniejsza od budżetu nabywcy, to zamieniają sie domami.
*/
void lookForHouseFromTrader(struct humanArray** humanArrayMemAddr,  int budget, struct human** humanBeingSoldToMemAddr){
    for(int i = 0; i < (*humanArrayMemAddr)->traderSize; i++){//Przejdź przez wszystkich sprzedawców
        if((*humanArrayMemAddr)->humans[1][i]->houses->size > 1){//Jeśli sprzedawca ma więcej niż 1 dom to
            for(int j = 0; j < (*humanArrayMemAddr)->humans[1][i]->houses->size; j++){//Przejdź przez wszystkie domy sprzedawcy
                int negotiatedPrice = (*humanArrayMemAddr)->humans[1][i]->houses->houses[j]->value*random(1,4);//Ustal cene do zapłaty
                if(negotiatedPrice <= budget){//Jeśli cena jest mniejsza od budżetu
                    eventsInADay++;
                    struct house** houseToSellMemAddr = &(*humanArrayMemAddr)->humans[1][i]->houses->houses[j];
                    printf("\n%i> Day %i) %s looked for a house and FOUND ONE! He bought it from %s for %i money.",eventsInADay, day, (*humanBeingSoldToMemAddr)->name, (*humanArrayMemAddr)->humans[1][i]->name, negotiatedPrice);

                    //Zamień się domami
                    //removeAllParentsFromHouse(houseToSellMemAddr);
                    removeHouseOwnershipFromHuman(&(*humanArrayMemAddr)->humans[1][i], j, true);
                    //addHouseOwnershipToHuman(houseToSellMemAddr,humanBeingSoldToMemAddr);

                    //Zamień się pieniędzmi
                    (*humanArrayMemAddr)->humans[1][i]->money+=negotiatedPrice;
                    (*humanBeingSoldToMemAddr)->money-=negotiatedPrice;

                    return;
                }
            }
        }
    }
    eventsInADay++;
    printf("\n%i> Day %i) %s looked for a house but couldn't find one and is now HOMELESS!.",eventsInADay, day, (*humanBeingSoldToMemAddr)->name);
}


/*
    Przejdź przez wszystkie domy i znajdź najdroższy, następnie usuń go z posiadanych podów człowieka i daj mu pieniądze wartości domu.
*/
void sellMostValuableHouse(struct human** humanMemAddr){
    int highestHouseIndex = 0;
    for(int i = 0; i < (*humanMemAddr)->houses->size;i++){
        if((*humanMemAddr)->houses->houses[i]->value > (*humanMemAddr)->houses->houses[highestHouseIndex]->value){
            highestHouseIndex = i;
        }
    }

    (*humanMemAddr)->money += (*humanMemAddr)->houses->houses[highestHouseIndex]->value;
    removeHouseOwnershipFromHuman(humanMemAddr, highestHouseIndex, true);
}

/*
    Znajdź losowy dom i jeśli nabywce stać to go przejmuje i traci pieniądze o wartości nabytego domu.
*/
void buyRandomEmptyHouse( struct provinceArray* city, struct human** traderMemAddr){
    struct house* tempHouse;
    tempHouse = findEmptyHouse(city);

    if(tempHouse != NULL && (*traderMemAddr)->money > tempHouse->value){
        (*traderMemAddr)->money -= tempHouse->value;
        addHumanParentsToHouse(&tempHouse, *traderMemAddr);
        addHouseOwnershipToHuman(&tempHouse, traderMemAddr);
        tempHouse->isOwned = 'y';
        return;
    }
};
