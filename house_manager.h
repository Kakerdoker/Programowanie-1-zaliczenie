#ifndef __HOUSE_MANAGER_H__
#define __HOUSE_MANAGER_H__

struct houseArray* findNEmptyHouses(int amountOfEmptyHouses, struct provinceArray* city, struct human* human);
struct houseArray* createARandomStreet(int amountOfHouses, char* streetName, struct province* parent);
struct province* generateRandomProvince(int amountOfStreets);
struct provinceArray* generateRandomCity(int amountOfProvinces);
void addProvinceToCity(struct provinceArray** cityMemAddr, struct province* provinceToAdd);
void deleteProvinceFromCity(struct provinceArray** cityMemAddr, int provinceToDeleteIndex);
void addStreetToProvince(struct province** provinceMemAddr, struct houseArray* streetToAdd);
void deleteStreetFromProvince(struct province** provinceMemAddr, int streetToDeleteIndex);
void removeAllParentsFromHouse(struct house** houseMemAddr);
void deleteHouseFromStreet(struct houseArray** streetMemAddr, int houseToDeleteIndex);
void addHouseToStreet(struct houseArray** streetMemAddr, struct house* houseToAdd);
struct house* pickMainHouseForNormalHuman(struct houseArray* houses);
void lookForHouseFromTrader(struct humanArray** humanArrayMemAddr, int budget, struct human** humanBeingSoldToMemAddr);
void sellMostValuableHouse(struct human** humanMemAddr);
void buyRandomEmptyHouse( struct provinceArray* city, struct human** traderMemAddr);
#endif
