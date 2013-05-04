#ifndef _GATE_H_
#define _GATE_H_ 
#include "commonTypes.h"
#include "rand.h"

//#define GATE_COUNT 100
int GATE_COUNT;
#define BUFFER_SIZE 200

typedef struct gate {
   int gateNum;
   int row;
   int col;
   int netToCount;
   int* netsTo;
   int netFromCount; 
   int* netsFrom; 
} gate_t;

typedef gate_t* grid_t; 

int swap(grid_t* grid, int x1, int y1, int x2, int y2);
inline int grid_partialLen(grid_t* grid, gate_t* gates, int x1, int y1, int x2, int y2);
int grid_init(grid_t** grid, gate_t** gates);
int grid_fill(grid_t* grid, gate_t* gates);
inline int grid_routeLen(gate_t* gateList, int gate);
int grid_free(grid_t* grid, gate_t* gates);
int grid_dumpGrid(grid_t* grid);

#endif
