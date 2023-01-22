#ifndef __SIMULATION_H__
#define __SIMULATION_H__

int todaysHuman = 0, salarySum = 0, housesOwned = 0;
void runNDaysOfSimulation(struct humanArray** humanArrayMemAddr,  struct provinceArray* city,  int days);
#endif