#ifndef __HUMAN_MANAGER_H__
#define __HUMAN_MANAGER_H__
#include <stdbool.h>

void matchmakeRandomHumans(struct humanArray* humanArray);
void lookForSpouse(struct humanArray** humanArrayMemAddr, struct human** singleHumanMemAddr);
void addHumanParentsToHouse(struct house** houseMemAddr, struct human* human);
void addHouseOwnershipToHuman(struct house** houseMemAddr, struct human** humanMemAddr);
void removeHouseOwnershipFromHuman(struct human** humanMemAddr, int houseToDeleteIndex, bool spouseToo);
struct humanArray* generateNRandomHumans(int amountOfNormal, int amountOfTraders, struct provinceArray* city);
struct human* findHomelessHuman(struct humanArray* allHumans);
void deleteHuman(struct humanArray **everyHumanMemAddr, struct human** humanMemAddr);
void createRandomName(char *name);
#endif
