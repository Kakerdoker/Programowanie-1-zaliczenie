#ifndef __EVENTS_H__
#define __EVENTS_H__

extern int eventsInADay, day;

void chooseARandomEvent(struct house* mainHouse, struct human** humanMemAddr, struct humanArray ** humanArrayMemAddr, int day);

#endif