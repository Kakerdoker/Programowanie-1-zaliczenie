#include <stdlib.h>
#include "structures.h"

/*
    Wypisuje wszystkie informacje o mieście
    Offset dodaje inta do wypisanego numeru dzielnicy, używane przy przegladaniu i wybieraniu opcji w menu
*/
void printCityInfo( struct provinceArray* city,  int offset){
    printf("The city has %i provinces.\n\n", city->size);

    for(int i = 0; i < city->size; i++){
        printf("%i) Province number %i of the city - size: %i\n",i+offset,i+1,city->provinces[i]->amountOfStreets);
    }
}

/*
    Wypisuje wszystkie informacje o dzielnicy
    Offset dodaje inta do wypisanego numeru ulicy, używane przy przegladaniu i wybieraniu opcji w menu
*/
void printProvinceInfo( struct province* province,  int offset){
    printf("The province has %i streets and it's levels are:\n  Crime:%i\n  Greenery:%i\n  Job Market:%i\n\n",province->amountOfStreets,province->crimeLevel,province->greeneryLevel,province->jobMarketLevel);

    for(int i = 0; i < province->amountOfStreets; i++){
        printf("%i) %s street - size: %i\n", i+offset , province->streets[i]->streetName, province->streets[i]->size);
    }
}

/*
    Wypisuje wszystkie informacje o ulicy
    Offset dodaje inta do wypisanego numeru domu, używane przy przegladaniu i wybieraniu opcji w menu
*/
void printStreetInfo( struct houseArray* street,  int offset){
    for(int i = 0; i < street->size; i++){
        printf("%i) %s house worth %i.\n",i+offset,street->houses[i]->isOwned == 'y' ? "Owned" : "Empty",street->houses[i]->value);
    }
}

/*
    Wypisuje wszystkie informacje o domu
*/
void printHouseInfo( struct house* house){
    printf("> %s house on %s with %i rooms. Costs: %i and it's levels are:\n   Garden size: %i\n   Comfort: %i\n   Wall width: %i\n\n", house->isOwned == 'y' ? "Owned" : "Empty", house->parent->streetName, house->numberOfRooms, house->value, house->gardenSizeLevel, house->comfortLevel, house->wallWidthLevel);
}

/*
    Funkcja używana do nawigacji przez menu.
    Wyczytuje od użytkownika odpowiedź gdzie chce przejść, a następnie sprawdza czy jest poprawna.
    Jeśli jest, to dana warstwa w menu zostaje zamieniona na wybór użytkownika.
*/
void menuNavigateInCurrentLayer( int amountOfChoices, int *nthMenuLayer,  char* beginningMessage){
    printf(beginningMessage);

    int currentMenuLayer = *nthMenuLayer;
    if(scanf("%i", nthMenuLayer) != 1, *nthMenuLayer >= amountOfChoices || *nthMenuLayer < 0){
        scanf( "%*[^\n]%*c" );//Usuń niepoprawny input
        system("cls");
        printf("Incorrect choice, please try again:\n\n");
        *nthMenuLayer = currentMenuLayer;
    }
    else{system("cls");}
}

/*
    Spytaj użytkownika pytanie i sprawdź czy podana odpowiedź jest równa y lub n, jeśli tak to zwróć odpowiedź.
*/
char askUserYesOrNo( char* question){
    printf("%s? (y/n)\n",question);
    scanf( "%*[^\n]%*c" );//Wyczyść input przed zapytaniem bo czasami może się wywalić.
    char answer;
    if(scanf(" %c%*[^\n]%*c", &answer) != 1, answer != 'y' && answer != 'n'){
        scanf( "%*[^\n]%*c" );//Usuń niepoprawny input
        system("cls");
        printf("Not a valid input!!!\n\n");
        return askUserYesOrNo(question);
    }

    system("cls");
    return answer;
}

/*
    Spytaj użytkownika o inta i sprawdź jego poprawność.
*/
int askUserForIntProperty( char* question){
    printf("%s?\n",question);
    int property;
    scanf( "%*[^\n]%*c" );//Wyczyść input przed zapytaniem bo czasami może się wywalić.
    if(scanf(" %i%*[^\n]%*c", &property) != 1, property < 0 || property > 100){
        scanf( "%*[^\n]%*c" );//Usuń niepoprawny input
        system("cls");
        printf(property > 100 ? "Max int is 100!\n\n" : "Not a valid input!!!\n\n");
        return askUserForIntProperty(question);
    }
    return property;
}

/*
    Pyta użytkownika o wszystkie właściwości dzielnicy, a potem ją tworzy i dodaje do miasta.
*/
void askUserForProvinceDetailsAndAddIt(struct provinceArray** cityMemAddr){
    int amountOfStreets = askUserForIntProperty("How many streets should the province have");
    int greeneryLevel = askUserForIntProperty("How green should it be");
    int jobMarketLevel = askUserForIntProperty("How well is the job market doing");
    int crimeLevel = askUserForIntProperty("How much crime is there");

    struct province* newProvince = malloc(sizeof(struct province));

    struct houseArray** streets = malloc(sizeof(struct houseArray)*amountOfStreets);
    for (int i = 0 ; i < amountOfStreets ; i ++){
        streets[i] = createARandomStreet(random(6,27),"\0", newProvince);
    }

    initializeProvince(&newProvince, streets, amountOfStreets, greeneryLevel, jobMarketLevel, crimeLevel);

    //Wyświetl użytkownikowi informacje o stworzonej dzielnicy i spytaj czy chce ją utrwalić, jeśli odpowiedź to nie to usuń dzielnice z pamięci.
    printProvinceInfo(newProvince,0);
    if(askUserYesOrNo("\nAre you sure you want to add this province") == 'y'){
        addProvinceToCity(cityMemAddr,newProvince);
    }
    else{
        throwProvinceIntoGarbage(&newProvince);
    }
}

/*
    Pyta użytkownika o wszystkie właściwości ulicy, a potem ją tworzy i dodaje do miasta.
*/
void askUserForStreetDetailsAndAddIt(struct provinceArray* city,  int provinceNum){
    int size = askUserForIntProperty("How many houses should the street have");

    struct houseArray* newStreet = createARandomStreet(size,"\0",city->provinces[provinceNum]);

    //Wyświetl użytkownikowi informacje o stworzonej ulicy i spytaj czy chce ją utrwalić, jeśli odpowiedź to nie to usuń ulice z pamięci.
    printStreetInfo(newStreet,0);
    if(askUserYesOrNo("\nAre you sure you want to add this street") == 'y'){
        addStreetToProvince(&city->provinces[provinceNum], newStreet);
    }
    else{
        deleteStreetFromProvince(&city->provinces[provinceNum], city->provinces[provinceNum]->amountOfStreets-1);
    }
}

/*
    Pyta użytkownika o wszystkie właściwości domu, a potem go tworzy i dodaje do miasta.
*/
void askUserForHouseDetailsAndAddIt(struct provinceArray* city,  int provinceNum,  int streetNum){
    int gardenSizeLevel = askUserForIntProperty("How big should the garden be");
    int numberOfRooms = askUserForIntProperty("How many rooms should the house have");
    int wallWidthLevel = askUserForIntProperty("How wide should the walls be");
    int comfortLevel = askUserForIntProperty("How comfortable should the house be");
    char isOwned = askUserYesOrNo("Should the house be owned");

    struct house* newHouse = malloc(sizeof(struct house));

    initializeHouse(&newHouse, gardenSizeLevel, numberOfRooms, wallWidthLevel, comfortLevel, isOwned, city->provinces[provinceNum]->streets[streetNum],NULL);

    //Wyświetl użytkownikowi informacje o stworzonym domu i spytaj czy chce go utrwalić, jeśli odpowiedź to nie to usuń dom z pamięci.
    printHouseInfo(newHouse);
    if(askUserYesOrNo("\nAre you sure you want to add this house") == 'y'){
        if(isOwned == 'y'){addHumanParentsToHouse(&newHouse,findHomelessHuman(city));};
        addHouseToStreet(&city->provinces[provinceNum]->streets[streetNum], newHouse);
    }
    else{
        throwHouseIntoGarbage(&newHouse);
    }
}