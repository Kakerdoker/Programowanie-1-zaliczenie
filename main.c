#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>

#include "structures.h"
#include "events.h"
#include "garbage_collection.h"
#include "main_loop.h"

            // main loop

int main()
{
    srand(1);//srand(time(NULL));


    //PLIK ZAPIS I WCZYT ZROB Z LOGOW DNIA GURRWAA


    struct provinceArray* szczecin = generateRandomCity(10);
    struct humanArray* everyHuman = generateNRandomHumans(100,100, szczecin);

    matchmakeRandomHumans(everyHuman);

    doLoop(&szczecin, &everyHuman);


    return 0;
}
