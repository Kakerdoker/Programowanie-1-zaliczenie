#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "garbage_collection.h"
#include "events.h"

/*
    human_manager.c zawiera w sobie wszystkie funkcje dotyczące ludzi - tworzenie, usuwanie, zamienianie, itp.
*/

/*
    Funkcja przechodzi przez petle 1/3 wszystkich ludzi.
    Wewnatrz petli wybiera losowo czy człowiek jest z tablicy normalnych, czy kupców.
    Po wybraniu przechodzi przez ludzi aż nie znajdzie jakiegoś kawalera, po czym dodaje go do tymczasowej tablicy pary.
    Jeśli już ktoś jest w tablicy pary to dodaj adresy obu ludzi nawzajem do ich struktur.
*/
void matchmakeRandomHumans( struct humanArray* humanArray){
    struct human** couple = malloc(sizeof(struct human*)*2);

    int coupleNum = 0;

    int normalHumanIndex = 0;
    int traderHumanIndex = 0;

    int amountOfHumans = humanArray->normalSize+humanArray->traderSize;

    for(int i = 0; i < amountOfHumans/3; i++){//Przejdź 1/3 wszystkich ludzi razy
        if(random(0,amountOfHumans)<= humanArray->normalSize){//Wybierz losowo zwykłą osobę, lub sprzedawce z szansą proporcjonalną do proporcji sprzedawców i normalnych ludzi.
            while(normalHumanIndex < humanArray->normalSize){//Przejdź przez wszystkich normalnych ludzi dopóki nie dojdziesz do końca
                if(humanArray->humans[0][normalHumanIndex]->spouse == NULL){//Jeśli człowiek nie ma partnera
                    couple[coupleNum] = humanArray->humans[0][normalHumanIndex];//Przetrzymaj partnera tymczasowo w tablicy
                    if(coupleNum == 1){//Jesli tablica juz ma w sobie partnera to ustaw obu ludzi jako wlasnych partnerow
                        couple[coupleNum]->spouse = couple[coupleNum == 0 ? 1 : 0];
                        couple[coupleNum == 0 ? 1 : 0]->spouse = couple[coupleNum];
                    }
                    coupleNum = coupleNum == 0 ? 1 : 0;
                    break;
                }
                normalHumanIndex++;
            }
        }
        else{//Jeśli wylosowało sprzedawcę to zrób to samo co na górze tylko że przechodząc przez wszystkich sprzedawców
            while(traderHumanIndex < humanArray->traderSize){
                if(humanArray->humans[1][traderHumanIndex]->spouse == NULL){
                    couple[coupleNum] = humanArray->humans[1][traderHumanIndex];
                    if(coupleNum == 1){
                        couple[coupleNum]->spouse = couple[coupleNum == 0 ? 1 : 0];
                        couple[coupleNum == 0 ? 1 : 0]->spouse = couple[coupleNum];
                    }
                    coupleNum = coupleNum == 0 ? 1 : 0;
                    break;
                }
                traderHumanIndex++;
            }
        }
        if(normalHumanIndex == humanArray->normalSize || traderHumanIndex == humanArray->traderSize){
            break;
        }
    }

    free(couple);
}


/*
    Funkcja dodaje ludzkiego rodzica do domu pierw upewniając się, że nie są oni NULL'em.
*/
void addHumanParentsToHouse(struct house** houseMemAddr,  struct human* human){
    //Nie wykonuj funkcji jeśli dom albo człowniek nie istnieje
    if((*houseMemAddr) == NULL || human == NULL){return;}

    //Jeśli dom nie ma struktury parentHumans to na szybko stwórz mu pustą strukture.
    if( (*houseMemAddr)->parentHumans == NULL){
        struct humanParentArray* parentHumans = malloc(sizeof(struct humanParentArray));
        struct human** humans = malloc(0);

        parentHumans->size = 0;
        parentHumans->humans = humans;

        (*houseMemAddr)->parentHumans = parentHumans;
    }

    (*houseMemAddr)->parentHumans->size++;
    (*houseMemAddr)->parentHumans->humans = realloc((*houseMemAddr)->parentHumans->humans, sizeof(struct human*)*(*houseMemAddr)->parentHumans->size);
    (*houseMemAddr)->parentHumans->humans[(*houseMemAddr)->parentHumans->size-1] = human;
}

/*
    Funkcja wybiera losowy prefix, pare infixow, i sufix sklejając je w imie.
*/
char namePrefix[30][3] = { "Ad\0","Al\0","Ap\0","Ar\0","Bo\0","Co\0","Do\0","Da\0","Di\0","De\0","Fo\0","Fu\0","Ge\0","Ni\0","Mo\0","Re\0","Ta\0","Ti\0","Re\0","Mi\0","Lo\0","Pu\0","Pi\0","Le\0","Re\0","Me\0","He\0","We\0","Um\0","Ro\0" };
char nameMidfix[40][4] = { "bon\0","rze\0","rzo\0","zin\0","zun\0","zen\0","szo\0","szy\0","czy\0","czo\0","cze\0","czi\0","czu\0","bud\0","pro\0","var\0","cno\0","mon\0","dor\0","wid\0","kol\0","sol\0","por\0","umn\0","bol\0","hon\0","chu\0","chi\0","che\0","chm\0","lul\0","ber\0","gej\0","hok\0","win\0","dud\0","bak\0","zyd\0","rag\0","nog\0" };
char nameEndfix[30][3] = { "is\0","in\0","un\0","er\0","us\0","ub\0","im\0","on\0","uc\0","wu\0","op\0","ic\0","yn\0","ow\0","ur\0","cu\0","uz\0","oz\0","iz\0","in\0","ig\0","uf\0","of\0","if\0","eb\0","ej\0","ek\0","el\0","uk\0","ul\0" };

void createRandomName(char *name) {
    strcpy(name, namePrefix[random(0, 30)]);
    for (int i = 0; i < random(0, 4); i++) {
        strcat(name, nameMidfix[random(0, 40)]);
    }
    strcat(name, nameEndfix[random(0, 30)]);
}

/*
    Funkcja tworzy i zwraca strukture do przechowywania ludzi. Pierw losowo tworzy zwykłych ludzi, a następnie sprzedawców.
*/
struct humanArray* generateNRandomHumans( int amountOfNormal,  int amountOfTraders, struct provinceArray* city) {
    struct human*** allHumans = malloc(sizeof(struct human**)*2);

    //Zwykli
    allHumans[0] = malloc(sizeof(struct human*)*amountOfNormal);
    for (int i = 0; i < amountOfNormal; i++) {
        allHumans[0][i] = malloc(sizeof(struct human));
        initializeHuman(&allHumans[0][i], random(18,90), random(20,100)*100,random(1,50)*2000,random(31,51), findNEmptyHouses(random(0,10)<8?1:0,city,allHumans[0][i]), NULL, "\0");
    }

    //Sprzedawcy
    allHumans[1] = malloc(sizeof(struct human*)*amountOfTraders);
    for (int i = 0; i < amountOfTraders; i++) {
        allHumans[1][i] = malloc(sizeof(struct human));
        initializeHuman(&allHumans[1][i], random(18,90), random(1,70)*300,5000*random(15,50),random(1,21), findNEmptyHouses(random(2,6),city,allHumans[1][i]), NULL, "\0");
    }


    struct humanArray* humans = malloc(sizeof(struct humanArray));
    humans->humans = allHumans;
    humans->normalSize = amountOfNormal;
    humans->traderSize = amountOfTraders;
    return humans;
}

/*
    Funkcja usuwa podany pointer osoby z struktury przechowywającej rodziców domu.
*/
static int deleteHumanFromHumanParentArray(struct humanParentArray** parentArrayMemAddr,  struct human* humanToDelete){
    bool inside = false;

    for(int i = 0; i < (*parentArrayMemAddr)->size; i++){//Przejdź przez wszystkich rodziców
        if((*parentArrayMemAddr)->humans[i] == humanToDelete){//Jeśli pętla znajduje się aktualnie na osobie do usunięcia
            (*parentArrayMemAddr)->size--;//Zmniejsz wielkość tablicy
            inside = true;
        }
        else{
            (*parentArrayMemAddr)->humans[i] = (*parentArrayMemAddr)->humans[inside? i+1 : i];//Przekopiuj tablice na samą siebie, indeksując o jeden do przodu jeśli w pętli rodziców została znaleziona osoba do usunięcia.
        }
    }

    if(inside){//Jeśli osoba do usunięcia była w tablicy rodziców zmniejdz wielkość tablicy
        (*parentArrayMemAddr)->humans = realloc((*parentArrayMemAddr)->humans, sizeof(struct human*)*(*parentArrayMemAddr)->size);
    }
    return (*parentArrayMemAddr)->size;//Zwróc nową wielkość, potrzebne do sprawdzenia w niektórych funkcjach czy nowa wielkość jest równa 0.
}

/*
    Dodaj podany dom do tablicy domów człowieka.
*/
void addHouseOwnershipToHuman(struct house** houseMemAddr, struct human** humanMemAddr){
    addHumanParentsToHouse(&*(houseMemAddr),*humanMemAddr);
    (*humanMemAddr)->houses->size++;
    (*humanMemAddr)->houses->houses = realloc((*humanMemAddr)->houses->houses, sizeof(struct house*)*(*humanMemAddr)->houses->size);
    (*humanMemAddr)->houses->houses[(*humanMemAddr)->houses->size-1] = (*houseMemAddr);
}

/*
    Usuń dom o podanym indeksie z tablicy domów człowieka.
*/
void removeHouseOwnershipFromHuman(struct human** humanMemAddr,  int houseToDeleteIndex,  bool spouseToo){
    if(houseToDeleteIndex < 0 || houseToDeleteIndex > (*humanMemAddr)->houses->size){
        printf("Tried to remove ownership of house using index out of range.");
        exit(1);
    };

    //Usuń człowieka z tablicy rodziców tego domu
    if(deleteHumanFromHumanParentArray(&(*humanMemAddr)->houses->houses[houseToDeleteIndex]->parentHumans, (*humanMemAddr)) == 0){//Jeśli nowa wielkość tablicy jest równa 0 to zmień isOwned domu na n
        (*humanMemAddr)->houses->houses[houseToDeleteIndex]->isOwned = 'n';
    }

    (*humanMemAddr)->houses->size--;
    //Przekopiuj tablice na samą siebie, pomijając dom do usunięcia.
    for(int i = 0; i < (*humanMemAddr)->houses->size; i++){
        (*humanMemAddr)->houses->houses[i] = (*humanMemAddr)->houses->houses[i < houseToDeleteIndex ? i : i+1];
    }

    (*humanMemAddr)->houses->houses = realloc((*humanMemAddr)->houses->houses, sizeof(struct house*)*(*humanMemAddr)->houses->size);
}

/*
    Przejdź przez wszystkich ludzi dopóki nie znajdziesz jakiegoś który nie posiada żadnych domów i go zwróć
*/
struct human* findHomelessHuman( struct humanArray* allHumans){
    if(random(0,allHumans->normalSize+allHumans->traderSize) < allHumans->normalSize){//Wylosuj czy człowiek będzie zwykłą osobą czy sprzedawcą
        for(int i = 0; i < allHumans->normalSize;i++){
            if(allHumans->humans[0][i]->houses != NULL){//Upewnij się że tablica domów istnieje
                if(allHumans->humans[0][i]->houses->size == 0){//Jeśli nie ma domów to zwróć osobe
                    return allHumans->humans[0][i];
                }
            }
        }
    }
    for(int i = 0; i < allHumans->traderSize;i++){
        if(allHumans->humans[1][i]->houses != NULL){//Upewnij się że tablica domów istnieje
            if(allHumans->humans[1][i]->houses->size == 0){//Jeśli nie ma domów to zwróć osobe
                return allHumans->humans[1][i];
            }
       }
    }
    return NULL;//Jeśli nie znalazło nikogo zwróć null
}

/*
    Usuń podaną osobe
*/
void deleteHuman(struct humanArray **everyHumanMemAddr, struct human** humanMemAddr){
    
    //napraw przemek ze jak jedno jest size = 0 to sie psuje, dodaj jeszcze consty i wczytywanie z pliku
    printf("\n%i %i\n", (*everyHumanMemAddr)->normalSize, (*everyHumanMemAddr)->traderSize);
    int i = 0, j = 0;
    //Znajdź jaki indeks ma ta osoba przechodząc przez każdego człowieka dopóki go nie znajdą.
    while((*everyHumanMemAddr)->humans[j][i] != NULL && (*everyHumanMemAddr)->humans[j][i] != (*humanMemAddr) && j != 2){

        if(j == 0 && (*everyHumanMemAddr)->normalSize == i){
            i = -1; j = 1;
        }
        if((*everyHumanMemAddr)->traderSize == i && j == 1){
                i = -1; j = 2;
        }
        i++;
    }
    //Jeśli j == 2 to oznacza że osoba nie została znaleziona
    if(j == 2 || (*everyHumanMemAddr)->humans[j][i] == NULL){return;};

    //Usuń człowieka
    throwHumanIntoGarbage(humanMemAddr);

    //Ustal i zmniejsz wielkość zależnie od tego jakim typem człowieka był.
    int size;
    if(j == 0){
        (*everyHumanMemAddr)->normalSize--;
        size = (*everyHumanMemAddr)->normalSize;
    }
    else{
        (*everyHumanMemAddr)->traderSize--;
        size = (*everyHumanMemAddr)->traderSize;
    }

    //Kopiuj tablice wszystkich ludzi na samą siebie z wyjątkiem indeksu osoby która jest usuwana.
    for(int k = 0; k < size; k++){
        (*everyHumanMemAddr)->humans[j][k] = (*everyHumanMemAddr)->humans[j][k < i ? k : k+1];
    }

    (*everyHumanMemAddr)->humans[j] = realloc((*everyHumanMemAddr)->humans[j], sizeof(struct human*)*size);
}

/*
    Oblicz wartość osoby szukającej partnera, potem przejdź przez wszystkie pozostałe osoby, jeśli wartość jednej z przeglądanych osób jest bliska wartości przeglądającej osoby to
    dodaj sobie nawzajem domy posiadane przez obie osoby i ustal im partnerów jako samych siebie.
*/
void lookForSpouse(struct humanArray** humanArrayMemAddr, struct human** singleHumanMemAddr){
    int singleHumanValue = ((100-(*singleHumanMemAddr)->age)*10) + (*singleHumanMemAddr)->salary/10 + (*singleHumanMemAddr)->happiness*5;//Oblicz wartość osoby szukającej partnera

    for(int i = 0; i < (*humanArrayMemAddr)->normalSize; i++){//Przejdź przez wszystkich ludzi
        if((*humanArrayMemAddr)->humans[0][i]->spouse == NULL && (*humanArrayMemAddr)->humans[0][i] != (*singleHumanMemAddr)){//Jeśli człowiek nie ma partnera i nie jest tą samą osobą co szuka partnera
            struct human** potentialSpouseMemAddr = &(*humanArrayMemAddr)->humans[0][i];
            int potentialSpouseValue = ((100-(*potentialSpouseMemAddr)->age)*10) + (*potentialSpouseMemAddr)->salary/10 + (*potentialSpouseMemAddr)->happiness*5;//Oblicz wartość znalezionej osoby

            if(abs(singleHumanValue - potentialSpouseValue) < 10*random(1,20)){//Jeśli wartość szukającej osoby i znalezionej osoby są bliskie siebie
                (*potentialSpouseMemAddr)->spouse = (*singleHumanMemAddr);//Ustaw ich nawzajem jako własnych partnerów
                (*singleHumanMemAddr)->spouse = (*potentialSpouseMemAddr);

                struct houseArray* tempSpouseHouseArray = (*potentialSpouseMemAddr)->houses;
                for(int j = 0; j < (*singleHumanMemAddr)->houses->size; j++){//Dodaj osobie znalezionej domy szukającej
                    addHouseOwnershipToHuman(&(*singleHumanMemAddr)->houses->houses[j],potentialSpouseMemAddr);
                }
                for(int j = 0; j < tempSpouseHouseArray->size; j++){//Dodaj osobie szukajacej domy znalezionej
                    addHouseOwnershipToHuman(&tempSpouseHouseArray->houses[j],singleHumanMemAddr);
                }

                eventsInADay++;
                //printf("\n%i> Day %i) %s looked for a SPOUSE and found %s.",eventsInADay, day, (*singleHumanMemAddr)->name,(*singleHumanMemAddr)->spouse->name);
                return;
            }
        }
    }
    eventsInADay++;
    //printf("\n%i> Day %i) %s looked for a SPOUSE and couldn't find one :(.",eventsInADay, day, (*singleHumanMemAddr)->name);
}

