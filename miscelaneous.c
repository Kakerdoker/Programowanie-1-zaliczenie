/*
    Funkcja do wygładzania wyników w taki sposób, aby niższe wartości były częstsze, a wyższe żadsze.
    x jest liczbą która będzie przekształcana
    range wskazuje na jakim przedziale ta funkcja się wygładza (wybierając np. 100 oznacza że największy wynik w zakresie dla x=[0,100] będzie równy 100).
    strength oznacza jak żadkie będą wyższe wartości.
*/
int slopeFunction(double x,  double range, int strength){
    double rangeSqrt = range;
    x = -x + 100;
    strength = strength > 4 ? 4 : strength;
    for (int i = 0; i < strength; i++){
        rangeSqrt = sqrt(rangeSqrt);
        x = sqrt(x);
    }
    return 100-((range/rangeSqrt) * x);
}

/*
    Funkcja do losowania w danym zakresie (max to tak na prawde max-1 lol)
*/
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
