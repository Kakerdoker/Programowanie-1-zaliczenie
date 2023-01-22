#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"

/*
    garbage_collection.c zajmuje się czyszczeniem programu z alokowanej pamięci.
*/

void throwHouseIntoGarbage(struct house** houseMemAddr){
    int lolol = 0;
    if((*houseMemAddr)->parentHumans != NULL){
        removeAllParentsFromHouse(houseMemAddr);
        free((*houseMemAddr)->parentHumans->humans);
    };
    free((*houseMemAddr)->parentHumans);
    free((*houseMemAddr));
}

void throwCityIntoGarbage(struct provinceArray** cityMemAddr){
    for(int i = 0; i < (*cityMemAddr)->size;i++){
        throwProvinceIntoGarbage(&(*cityMemAddr)->provinces[i]);
    }
    free((*cityMemAddr));
}

void throwProvinceIntoGarbage(struct province** provinceMemAddr){
    for(int i = 0; i < (*provinceMemAddr)->amountOfStreets;i++){
        throwStreetIntoGarbage(&(*provinceMemAddr)->streets[i]);
    }
    free((*provinceMemAddr));
}

void throwStreetIntoGarbage(struct houseArray** streetMemAddr){
    for(int i = 0; i < (*streetMemAddr)->size;i++){
        throwHouseIntoGarbage(&(*streetMemAddr)->houses[i]);
    }
    free((*streetMemAddr));
}

void throwHumanIntoGarbage(struct human** humanMemAddr){
    //Usuń posiadanie domu we wszystkich domach tego człowieka
    int amountOfHumanHouses = (*humanMemAddr)->houses->size;
    for(int k = 0; k<amountOfHumanHouses;k++){
        removeHouseOwnershipFromHuman(humanMemAddr,0, false);
    }
    //Jeśli ma partnera to usuń usuń temu partnerowi jego partnera, czyli osobę którą usuwamy
    if((*humanMemAddr)->spouse != NULL){
        (*humanMemAddr)->spouse->spouse = NULL;
    }

    free((*humanMemAddr)->houses);
    free(*humanMemAddr);
}
