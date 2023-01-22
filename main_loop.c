#include "structures.h"
#include "menu_navigation.h"

    int signal = 1;
        int firstMenuLayer = -1;
            int cityMenuLayer = -1;
                int provinceMenuLayer = -1;
                    int streetMenuLayer = -1;
                        int houseMenuLayer = -1;

void doLoop(struct provinceArray** cityMemAddr, struct humanArray** everyHumanMemAddr){
    while(signal == 1){
        switch(firstMenuLayer){
            case -1://Wyswietl pierwsze wybory menu aplikacji.
                menuNavigateInCurrentLayer(3,&firstMenuLayer,"What do you want to do?\n0)Quit\n1)Look through data\n2)Launch a simulation\n");
            break;
            case 0://Wyjdz z aplikacji
                signal = 0;
            break;
            case 1://Druga warstwa menu - dotycząca dzielnic miasta.
                switch(cityMenuLayer){
                    case -1://Wyświetl wybory aktualnej warstwy menu oraz dzielnice miasta.
                        printCityInfo((*cityMemAddr), 2);
                        menuNavigateInCurrentLayer(2+(*cityMemAddr)->size,&cityMenuLayer,"What do you want to do?\n0)Go back\n1)Add a province\n");
                    break;
                    case 0://Cofnij się o jedną warstwe do tyłu i zresetuj aktualną.
                        cityMenuLayer = -1;
                        firstMenuLayer = -1;
                    break;
                    case 1://Zapytaj użytkownika o szczegóły dzielnicy jaką chce dodać, a następnie ją dodaj.
                        askUserForProvinceDetailsAndAddIt(&(*cityMemAddr));
                        cityMenuLayer = -1;
                    break;
                }
                if(cityMenuLayer>=2){
                    switch(provinceMenuLayer){
                        case -1://Wyświetl wybory aktualnej warstwy menu oraz ulice dzielnicy.
                            printProvinceInfo((*cityMemAddr)->provinces[cityMenuLayer-2],3);
                            menuNavigateInCurrentLayer(3+(*cityMemAddr)->provinces[cityMenuLayer-2]->amountOfStreets,&provinceMenuLayer,"What do you want to do?\n0)Go back\n1)Add a street\n2)Delete currently selected province\n");
                        break;
                        case 0://Cofnij się o jedną warstwe do tyłu i zresetuj aktualną.
                            provinceMenuLayer = -1;
                            cityMenuLayer = -1;
                        break;
                        case 1://Zapytaj użytkownika o szczegóły ulicy jaką chce dodać, a następnie ją dodaj.
                            askUserForStreetDetailsAndAddIt((*cityMemAddr), cityMenuLayer-2);
                            provinceMenuLayer = -1;
                        break;
                        case 2://Usuń aktualnie wybraną dzielnice i się cofnij w menu.
                            if(askUserYesOrNo("Do you really want to delete the province") == 'y'){
                                deleteProvinceFromCity(&(*cityMemAddr), cityMenuLayer-2);
                            }
                            provinceMenuLayer = -1;
                            cityMenuLayer = -1;
                        break;
                    }
                    if(provinceMenuLayer >= 3){
                        switch(streetMenuLayer){
                            case -1://Wyświetl wybory aktualnej warstwy menu oraz domy ulicy.
                                printStreetInfo((*cityMemAddr)->provinces[cityMenuLayer-2]->streets[provinceMenuLayer-3], 3);
                                menuNavigateInCurrentLayer(3+(*cityMemAddr)->provinces[cityMenuLayer-2]->streets[provinceMenuLayer-3]->size, &streetMenuLayer,"What do you want to do?\n0)Go back\n1)Add a house\n2)Delete currently selected street\n");
                            break;
                            case 0://Cofnij się o jedną warstwe do tyłu i zresetuj aktualną.
                                streetMenuLayer = -1;
                                provinceMenuLayer = -1;
                            break;
                            case 1://Zapytaj użytkownika o szczegóły domu jaki chce dodać, a następnie go dodaj.
                                askUserForHouseDetailsAndAddIt((*cityMemAddr),cityMenuLayer-2,provinceMenuLayer-3);
                                streetMenuLayer = -1;
                            break;
                            case 2://Usuń aktualnie wybraną ulice i się cofnij w menu.
                                if(askUserYesOrNo("Are you sure you want to delete the street") == 'y'){
                                    deleteStreetFromProvince(&(*cityMemAddr)->provinces[cityMenuLayer-2], provinceMenuLayer-3);
                                }
                                streetMenuLayer = -1;
                                provinceMenuLayer = -1;
                            break;
                        }
                        if(streetMenuLayer >= 3){
                            switch(houseMenuLayer){
                                case -1://Wyświetl wybory aktualnej warstwy menu oraz informacje o wybranym domie.
                                    printHouseInfo((*cityMemAddr)->provinces[cityMenuLayer-2]->streets[provinceMenuLayer-3]->houses[streetMenuLayer-3]);
                                    menuNavigateInCurrentLayer(2,&houseMenuLayer,"What do you want to do?\n0)Go back\n1)Delete currently selected house\n");
                                break;
                                case 0://Cofnij się o jedną warstwe do tyłu i zresetuj aktualną.
                                    houseMenuLayer = -1;
                                    streetMenuLayer = -1;
                                break;
                                case 1://Usuń aktualnie wybrany dom i się cofnij w menu.
                                    if(askUserYesOrNo("Are you completely certain the house should be deleted") == 'y'){
                                        deleteHouseFromStreet(&(*cityMemAddr)->provinces[cityMenuLayer-2]->streets[provinceMenuLayer-3], streetMenuLayer-3);
                                    }
                                    houseMenuLayer = -1;
                                    streetMenuLayer = -1;
                                break;
                            }
                        }
                    }
                }
            break;
            case 2:
            ;int days = askUserForIntProperty("How many days should the simulation run for");
            runNDaysOfSimulation(everyHumanMemAddr, cityMemAddr, days);
            firstMenuLayer = -1;
            break;
        }


    }
}
