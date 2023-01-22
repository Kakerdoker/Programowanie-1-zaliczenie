#ifndef __GARBAGE_COLLECTION_H__
#define __GARBAGE_COLLECTION_H__

void throwHouseIntoGarbage(struct house** houseMemAddr);
void throwCityIntoGarbage(struct provinceArray** cityMemAddr);
void throwProvinceIntoGarbage(struct province** provinceMemAddr);
void throwStreetIntoGarbage(struct houseArray** streetMemAddr);

#endif