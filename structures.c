#include "human_manager.h"
/*
    structures.c zawiera w sobie wszystkie struktury i ich inicjalizacje.
*/
struct humanArray {
    struct human*** humans;
    int normalSize;
    int traderSize;
};

struct humanParentArray {
    struct human** humans;
    int size;
};

struct human {
  char name[20];
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
};

struct houseArray{
    struct house** houses;
    int size;
    struct province* parent;
    char streetName[20];
};

struct house{
    char isOwned;
    int value;
    int gardenSizeLevel;
    int numberOfRooms;
    int wallWidthLevel;
    int comfortLevel;
    struct houseArray* parent;
    struct humanParentArray* parentHumans;
};

void initializeHuman(struct human** ptrPtrHuman,  int age,  int salary,  int money,  int happiness,  struct houseArray* houses,  struct human* spouse,  char* name){
    (*ptrPtrHuman)->age=age;
    (*ptrPtrHuman)->salary=salary;
    (*ptrPtrHuman)->money=money;
    (*ptrPtrHuman)->happiness=happiness;
    (*ptrPtrHuman)->houses=houses;
    (*ptrPtrHuman)->spouse=spouse;

    if(name[0] == '\0'){ createRandomName((*ptrPtrHuman)->name);}
    else{strcpy((*ptrPtrHuman)->name,name);}
}

void initializeHouse(struct house** ptrPtrHouse,  int gardenSizeLevel,  int numberOfRooms,  int wallWidthLevel,  int comfortLevel,  char isOwned,  struct houseArray* parent,  struct humanParentArray* parentHumans){
    (*ptrPtrHouse)->gardenSizeLevel = gardenSizeLevel;
    (*ptrPtrHouse)->numberOfRooms = numberOfRooms;
    (*ptrPtrHouse)->wallWidthLevel = wallWidthLevel;
    (*ptrPtrHouse)->comfortLevel = comfortLevel;
    (*ptrPtrHouse)->value = (numberOfRooms*250)+(comfortLevel*20)+(wallWidthLevel*5)+(gardenSizeLevel*50);
    (*ptrPtrHouse)->parent = parent;
    (*ptrPtrHouse)->parentHumans = parentHumans;
    (*ptrPtrHouse)->isOwned = isOwned;
}

void initializeProvince(struct province** ptrPtrProvince,  struct houseArray** streets,  int amountOfStreets,  int greeneryLevel,  int jobMarketLevel,  int crimeLevel){
    (*ptrPtrProvince)->streets = streets;
    (*ptrPtrProvince)->amountOfStreets = amountOfStreets;
    (*ptrPtrProvince)->greeneryLevel = greeneryLevel;
    (*ptrPtrProvince)->jobMarketLevel = jobMarketLevel;
    (*ptrPtrProvince)->crimeLevel = crimeLevel;
}
