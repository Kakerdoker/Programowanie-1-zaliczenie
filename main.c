#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct kidArray {
    struct kid** array;
    int size;
};

struct kid {
    char name[16];
    char isOrhpan[1];
    int age;
    int allowance;
};

struct humanArray {
    struct human*** array;
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
  struct kidArray* kids;
};

struct provinceArray{
    struct province** array;
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
    struct house** array;
    int size;
};

struct house{
    char streetName[20];
    int value;
    int gardenSizeLevel;
    int numberOfRooms;
    int wallWidthLevel;
    int comfortLevel;
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

void createRandomName(char* name) {
    strcpy(name, namePrefix[random(0, 30)]);
    for (int i = 0; i < random(0, 4); i++) {
        strcat(name, nameMidfix[random(0, 40)]);
    }
    strcat(name, nameEndfix[random(0, 30)]);
}

char streetNamePrefix[15][8] = { "Euro\0","Dwor\0","Aleja\0","Mazo\0","Grun\0","Mari\0","Monte\0", "Jagiel\0","Poczt\0","Wielko\0","Slask\0","Wojsk\0","Emili\0","Rajk\0","Lubin\0"};
char streetNameMidfix[10][4] = { "an\0","ski\0","oln\0","orz\0","pej\0","gut\0","tyn\0","br\0","ock\0","isk\0" };
char streetNameEndfix[10][3] = { "ka\0","no\0","go\0","ry\0","er\0","ow\0","ja\0","ba\0","wa\0","wo\0" };

void createRandomStreetName(char* name) {
    strcpy(name, streetNamePrefix[random(0, 15)]);
    for (int i = 0; i < random(0, 4); i++) {
        strcat(name, streetNameMidfix[random(0, 10)]);
    }
    strcat(name, streetNameEndfix[random(0, 10)]);
}

struct kidArray generateNKids(int amountOfKids) {
    struct kid** allKids = malloc(sizeof(struct kid*)*amountOfKids); //free
    for (int i = 0; i < amountOfKids; i++) {
        allKids[i] = malloc(sizeof(struct kid));//free forloop
        createRandomName(&allKids[i]->name);
        strcpy(allKids[i]->isOrhpan, "y");
        allKids[i]->age = random(0,18);
        allKids[i]->allowance = random(0,4);
    }
    struct kidArray kids = { allKids, amountOfKids };
    return kids;
}

int kidIndex=0;
struct kidArray* unorphanRandomKids(struct kidArray* kids){
    int amountOfKidsToAdopt = random(0,4), adoptedKids = 0;
    struct kidArray *unorphanedKids = malloc(sizeof(struct kidArray));//free me

    unorphanedKids->size = 0;
    unorphanedKids->array = malloc(sizeof(struct kid*)*(amountOfKidsToAdopt+1));//free

    while (kidIndex< kids->size){
        if(kids->array[kidIndex]->isOrhpan[0] == 'y'){
            strcpy(kids->array[kidIndex]->isOrhpan,"n");
            //printf("%s\n",kids->array[kidIndex]->name);
            unorphanedKids->array[adoptedKids]=kids->array[kidIndex];

            adoptedKids++;
            unorphanedKids->size=adoptedKids;
            if(adoptedKids == amountOfKidsToAdopt+1){break;}
        }
        kidIndex++;
    }
    return unorphanedKids;
}



void initializeHuman(struct human** ptrPtrHuman, int age, int salary, int money, int happiness, struct houseArray* houses, struct human* spouse, struct kidArray* kids, char* name){
    (*ptrPtrHuman)->age=age;
    (*ptrPtrHuman)->salary=salary;
    (*ptrPtrHuman)->money=money;
    (*ptrPtrHuman)->happiness=happiness;
    (*ptrPtrHuman)->houses=houses;
    (*ptrPtrHuman)->spouse=spouse;
    (*ptrPtrHuman)->kids=kids;

    if(name[0] == '\0'){ createRandomName(&(*ptrPtrHuman)->name);}
    else{strcpy((*ptrPtrHuman)->name,name);}
}

struct humanArray generateNRandomHumans(int amountOfNormal, int amountOfTraders, struct kidArray* kids) {
    struct human*** allHumans = malloc(sizeof(struct human**)*2);//free

    allHumans[0] = malloc(sizeof(struct human*)*amountOfNormal);//free
    for (int i = 0; i < amountOfNormal; i++) {
        allHumans[0][i] = malloc(sizeof(struct human));//free forloop
        initializeHuman(&allHumans[0][i], random(18,90), random(1,50)*100,random(1,50)*2000,30+random(1,21), NULL, NULL, unorphanRandomKids(kids), "\0");
    }

    allHumans[1] = malloc(sizeof(struct human*)*amountOfTraders);//free
    for (int i = 0; i < amountOfTraders; i++) {
        allHumans[1][i] = malloc(sizeof(struct human));//free forloop
        initializeHuman(&allHumans[1][i], random(18,90), random(1,70)*300,5000*random(15,50),random(1,21), NULL, NULL, NULL, "\0");
    }

    struct humanArray humans = { allHumans, amountOfNormal, amountOfTraders};
    return humans;
}

void initializeHouse(struct house** ptrPtrHouse, int gardenSizeLevel, int numberOfRooms, float wallWidthLevel, float comfortLevel, char* streetName){
    (*ptrPtrHouse)->gardenSizeLevel = gardenSizeLevel;
    (*ptrPtrHouse)->numberOfRooms = numberOfRooms;
    (*ptrPtrHouse)->wallWidthLevel = wallWidthLevel;
    (*ptrPtrHouse)->comfortLevel = comfortLevel;
    (*ptrPtrHouse)->value = (numberOfRooms*250)+(comfortLevel*20)+(wallWidthLevel*5);
    strcpy(&(*ptrPtrHouse)->streetName,streetName);

}

char tempStreetString[20];
struct houseArray* createARandomStreet(int amountOfHouses, char* streetName){
    struct house** houses = malloc(sizeof(struct house*)*amountOfHouses); //free
    strcpy(&tempStreetString, streetName);
    if(streetName[0] == '\0'){
        createRandomStreetName(tempStreetString);
    }

    for(int i = 0; i < amountOfHouses; i++){
        houses[i] = malloc(sizeof(struct house)); //free forloop
        initializeHouse(&houses[i], slopeFunction(random(0,100),100,3),random(1,7)+random(0,7),slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1),tempStreetString);
    }

    struct houseArray* street = malloc(sizeof(struct houseArray)); //free
    street->array = houses;
    street->size = amountOfHouses;
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
        streets[i] = createARandomStreet(random(0,21)+6,"\0"); //free forloop<-forloop
    }

    initializeProvince(&province, streets, amountOfStreets, slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1),slopeFunction(random(0,100),100,1));
    return province;
}

struct provinceArray** generateRandomCity(int amountOfProvinces){
    province
    for(int i = 0; i < 0; i++){
        
    }
}


int main()
{
    //printf("%c",'0'+1);

    srand(2);

    //kids
    struct province* province = generateRandomProvince(2);
    struct kidArray everyKid = generateNKids(50);

    //houses

    //generate streets

    //printf("%s",street->array[0]->streetName);

    //hummus

    struct humanArray everyHuman = generateNRandomHumans(50,20, &everyKid);

    




    //add spouses

    return 0;
}


