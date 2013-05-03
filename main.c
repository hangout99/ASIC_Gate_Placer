#include "rand.h"
#include "grid.h"

int main()
{
   gate_t* gates = NULL;
   grid_t* grid = NULL;
   grid_init(&grid,&gates); 
   grid_fill(grid,gates); 

   int totalLen = 0;
   int count = 0;
   int x1,x2,y1,y2;
   int befLen, aftLen;
   int repeats = 0;
   while (count < GATE_COUNT) {
      totalLen += grid_routeLen(gates,count++);
   }
   grid_dumpGrid(grid);
   printf("Manhattan distance: %d\n",totalLen);
   while (1) {
      rdrand(&x1,XLEN);
      rdrand(&x2,XLEN);
      rdrand(&y1,YLEN);
      rdrand(&y2,YLEN);
      befLen = grid_partialLen(grid, gates, x1, y1, x2, y2);
      swap(grid, x1, y1, x2, y2);
      aftLen = grid_partialLen(grid, gates, x1, y1, x2, y2);
      if ( aftLen-befLen < 0 ) {
         repeats = 0;
         totalLen += aftLen-befLen;
      } else {
         swap(grid, x1, y1, x2, y2);
         if ( aftLen == befLen ) {
            repeats++;
         }
         if (repeats > TIMEOUT) {
            break;
         }
      }
   }
   grid_dumpGrid(grid);
   printf("Manhattan Length: %d\n",totalLen);

   grid_free(grid,gates);
   return 0;
}
