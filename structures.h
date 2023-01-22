#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

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

void initializeHuman(struct human** ptrPtrHuman, int age, int salary, int money, int happiness, struct houseArray* houses, struct human* spouse, char* name);
void initializeHouse(struct house** ptrPtrHouse, int gardenSizeLevel, int numberOfRooms, int wallWidthLevel, int comfortLevel, char isOwned, struct houseArray* parent, struct humanParentArray* parentHumans);
void initializeProvince(struct province** ptrPtrProvince, struct houseArray** streets, int amountOfStreets, int greeneryLevel, int jobMarketLevel, int crimeLevel);

#endif