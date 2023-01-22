#ifndef __MENU_NAVIGATION_H__
#define __MENU_NAVIGATION_H__

void printCityInfo( struct provinceArray* city,  int offset);
void printProvinceInfo( struct province* province,  int offset);
void printStreetInfo( struct houseArray* street,  int offset);
void printHouseInfo( struct house* house);
void menuNavigateInCurrentLayer( int amountOfChoices, int *nthMenuLayer,  char* beginningMessage);
char askUserYesOrNo( char* question);
int askUserForIntProperty( char* question);
void askUserForProvinceDetailsAndAddIt(struct provinceArray** cityMemAddr);
void askUserForStreetDetailsAndAddIt(struct provinceArray* city,  int provinceNum);
void askUserForHouseDetailsAndAddIt(struct provinceArray* city,  int provinceNum,  int streetNum);

#endif
