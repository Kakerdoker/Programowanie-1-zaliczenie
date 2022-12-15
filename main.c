#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


struct humanArray {
    struct human*** humans;
    int normalSize;
    int traderSize;
};

struct human {
  char name[16];
  int age;
  int money;
  int salary;
  int happiness;
  struct houseArray* houses;
  struct human* spouse;
};

struct provinceArray{
    struct province** provinces;
    int size;
};

struct province {
    struct houseArray** streets;
    int amountOfStreets;

    int greeneryLevel;
    int jobMarketLevel;
    int crimeLevel;
    int schoolLevel;
};

struct houseArray{
    struct house** houses;
    int size;
    struct province* parent;
};

struct house{
    char streetName[20];
    char isOwned[1];
    int value;
    int gardenSizeLevel;
    int numberOfRooms;
    int wallWidthLevel;
    int comfortLevel;
    struct houseArray* parent;
};


int slopeFunction(double x, double range, int strength){
    double rangeSqrt = range;
    x = -x + 100;
    for (int i = 0; i < strength; i++){
        rangeSqrt = sqrt(rangeSqrt);
        x = sqrt(x);
    }
    return 100-((range/rangeSqrt) * x);
}

int random(int min, int max)
{
    int tmp;
    if (max >= min)
        max -= min;
    else
    {
        tmp = min - max;
        min = max;
        max = tmp;
    }
    return max ? (rand() % max + min) : min;
}

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

char streetNamePrefix[15][8] = { "Euro\0","Dwor\0","Aleja\0","Mazo\0","Grun\0","Mari\0","Monte\0", "Jagiel\0","Poczt\0","Wielko\0","Slask\0","Wojsk\0","Emili\0","Rajk\0","Lubin\0"};
char streetNameMidfix[10][4] = { "an\0","ski\0","oln\0","orz\0","pej\0","gut\0","tyn\0","br\0","ock\0","isk\0" };
char streetNameEndfix[10][3] = { "ka\0","no\0","go\0","ry\0","er\0","ow\0","ja\0","ba\0","wa\0","wo\0" };

void createRandomStreetName(char *name) {
    strcpy(name, streetNamePrefix[random(0, 15)]);
    for (int i = 0; i < random(0, 4); i++) {
        strcat(name, streetNameMidfix[random(0, 10)]);
    }
    strcat(name, streetNameEndfix[random(0, 10)]);
}

void initializeHuman(struct human** ptrPtrHuman, int age, int salary, int money, int happiness, struct houseArray* houses, struct human* spouse, char* name){
    (*ptrPtrHuman)->age=age;
    (*ptrPtrHuman)->salary=salary;
    (*ptrPtrHuman)->money=money;
    (*ptrPtrHuman)->happiness=happiness;
    (*ptrPtrHuman)->houses=houses;
    (*ptrPtrHuman)->spouse=spouse;

    if(name[0] == '\0'){ createRandomName((*ptrPtrHuman)->name);}
    else{strcpy((*ptrPtrHuman)->name,name);}

}

void matchmakeRandomHumans(struct humanArray* humanArray){
    struct human** couple = malloc(sizeof(struct human*)*2);

    int coupleNum = 0;

    int normalHumanIndex = 0;
    int traderHumanIndex = 0;

    int amountOfHumans = humanArray->normalSize+humanArray->traderSize;

    for(int i = 0; i < amountOfHumans/3; i++){
        if(random(0,amountOfHumans)<= humanArray->normalSize){
            while(normalHumanIndex < humanArray->normalSize){
                //humanArray->humans[0][normalHumanIndex]->spouse = NULL;
                //printf("%s\n",humanArray->humans[0][normalHumanIndex]->spouse->name);
                if(humanArray->humans[0][normalHumanIndex]->spouse == NULL){
                    couple[coupleNum] = humanArray->humans[0][normalHumanIndex];
                    if(coupleNum == 1){
                        couple[coupleNum]->spouse = couple[coupleNum == 0 ? 1 : 0];
                        couple[coupleNum == 0 ? 1 : 0]->spouse = couple[coupleNum];
                    }
                    coupleNum = coupleNum == 0 ? 1 : 0;
                    break;
                }
                normalHumanIndex++;
            }
        }
        else{
            while(traderHumanIndex < humanArray->normalSize){
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

}

struct humanArray* generateNRandomHumans(int amountOfNormal, int amountOfTraders) {
    struct human*** allHumans = malloc(sizeof(struct human**)*2);//free

    allHumans[0] = malloc(sizeof(struct human*)*amountOfNormal);//free
    for (int i = 0; i < amountOfNormal; i++) {
        allHumans[0][i] = malloc(sizeof(struct human));//free forloop
        initializeHuman(&allHumans[0][i], random(18,90), random(1,50)*100,random(1,50)*2000,random(31,51), NULL, NULL, "\0");
    }

    allHumans[1] = malloc(sizeof(struct human*)*amountOfTraders);//free
    for (int i = 0; i < amountOfTraders; i++) {
        allHumans[1][i] = malloc(sizeof(struct human));//free forloop
        initializeHuman(&allHumans[1][i], random(18,90), random(1,70)*300,5000*random(15,50),random(1,21), NULL, NULL, "\0");
    }

    struct humanArray* humans = malloc(sizeof(struct humanArray));
    humans->humans = allHumans;
    humans->normalSize = amountOfNormal;
    humans->traderSize = amountOfTraders;

    return humans;
}

void initializeHouse(struct house** ptrPtrHouse, int gardenSizeLevel, int numberOfRooms, float wallWidthLevel, float comfortLevel, char* streetName, char isOwned[1]){
    (*ptrPtrHouse)->gardenSizeLevel = gardenSizeLevel;
    (*ptrPtrHouse)->numberOfRooms = numberOfRooms;
    (*ptrPtrHouse)->wallWidthLevel = wallWidthLevel;
    (*ptrPtrHouse)->comfortLevel = comfortLevel;
    (*ptrPtrHouse)->value = (numberOfRooms*250)+(comfortLevel*20)+(wallWidthLevel*5);
    strcpy((*ptrPtrHouse)->isOwned, isOwned);
    strcpy((*ptrPtrHouse)->streetName, streetName);
}

struct house* findEmptyHouse(struct provinceArray* city){
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

    while(1){
        if(tempHouse->isOwned[0] == 'y'){
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

char tempStreetString[20];
struct houseArray* createARandomStreet(int amountOfHouses, char* streetName, struct province* parent){
    struct house** houses = malloc(sizeof(struct house*)*amountOfHouses); //free
    strcpy(tempStreetString, streetName);
    if(streetName[0] == '\0'){
        createRandomStreetName(tempStreetString);
    }

    for(int i = 0; i < amountOfHouses; i++){
        houses[i] = malloc(sizeof(struct house)); //free forloop
        initializeHouse(&houses[i], slopeFunction(random(0,100),100,3),random(1,7)+random(0,7),slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1),tempStreetString, "n");
    }

    struct houseArray* street = malloc(sizeof(struct houseArray)); //free
    street->houses = houses;
    street->size = amountOfHouses;
    street->parent = parent;
    return street;
}

void initializeProvince(struct province** ptrPtrProvince, struct houseArray** streets, int amountOfStreets, int greeneryLevel, int jobMarketLevel, int crimeLevel, int schoolLevel){
    (*ptrPtrProvince)->streets = streets;
    (*ptrPtrProvince)->amountOfStreets = amountOfStreets;
    (*ptrPtrProvince)->greeneryLevel = greeneryLevel;
    (*ptrPtrProvince)->jobMarketLevel = jobMarketLevel;
    (*ptrPtrProvince)->crimeLevel = crimeLevel;
    (*ptrPtrProvince)->schoolLevel = schoolLevel;
}

struct province* generateRandomProvince(int amountOfStreets) {
    struct province* province = malloc(sizeof(struct province));//free

    struct houseArray** streets = malloc(sizeof(struct houseArray)*amountOfStreets); //free
    for (int i = 0 ; i < amountOfStreets ; i ++){
        streets[i] = createARandomStreet(random(6,27),"\0", province); //free forloop<-forloop
    }

    initializeProvince(&province, streets, amountOfStreets, slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1));
    return province;
}

struct provinceArray* generateRandomCity(int amountOfProvinces){
    struct province** provinces = malloc(sizeof(struct province*) *amountOfProvinces);//free
    for(int i = 0; i < amountOfProvinces; i++){
        provinces[i] = generateRandomProvince(random(10,30));//->free x amount
    }
    struct provinceArray* city = malloc(sizeof(struct provinceArray)); //free
    city->provinces=provinces;
    city->size=amountOfProvinces;
    return city;
}


int main()
{
    srand(3);

    //Create the city
    struct provinceArray* szczecin = generateRandomCity(random(1,4)); //printf("%s\n",szczecin->provinces[0]->streets[0]->houses[0]->isOwned);


    //Create the humans
    struct humanArray* everyHuman = generateNRandomHumans(50,20);
    matchmakeRandomHumans(everyHuman);

    struct house* testHouse = findEmptyHouse(szczecin);
    for(int i = 0; i<everyHuman->normalSize; i++){
        printf("%s\n", everyHuman->humans[1][i]->spouse->name);
    }


    //printf("%s",street->array[0]->streetName);

    //hummus








    //add spouses

    return 0;
}







//przemek musisz pojsc debuggerem od poczatku do konca bo te duchy spousow sa dziwne haj ibe sue oihawua kik

