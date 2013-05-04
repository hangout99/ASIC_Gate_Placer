#include "grid.h"

static int halfPerimiterWireLength(gate_t* gateList, int gate);

inline int grid_routeLen(gate_t* gateList, int gate) {
   return halfPerimiterWireLength(gateList,gate);
}
int swap(grid_t* grid, int x1, int y1, int x2, int y2) {
   gate_t* temp = grid[x1+XLEN*y1];
   grid[x1+XLEN*y1] = grid[x2+XLEN*y2];
   grid[x2+XLEN*y2] = temp;
   if (grid[x1+XLEN*y1]) {
      grid[x1+XLEN*y1]->col = x1;
      grid[x1+XLEN*y1]->row = y1;
   }
   if (grid[x2+XLEN*y2]) {
      grid[x2+XLEN*y2]->col = x2;
      grid[x2+XLEN*y2]->row = y2;
   }
   return PASS;
}
inline int grid_partialLen(grid_t* grid, gate_t* gates, int x1, int y1, int x2, int y2) {
   int len = 0;
   if (grid[x1+XLEN*y1]) {
      len += grid_routeLen(gates,grid[x1+XLEN*y1]->gateNum);
      for (int i =0; i < grid[x1+XLEN*y1]->netFromCount; i++) {
         if (grid[x1+XLEN*y1]->netsFrom[i] != grid[x1+XLEN*y1]->gateNum) {
            len += grid_routeLen(gates,grid[x1+XLEN*y1]->netsFrom[i]);
         }
      }
   }
   if (grid[x2+XLEN*y2]) {
      len += grid_routeLen(gates,grid[x2+XLEN*y2]->gateNum);
      for (int i =0; i < grid[x2+XLEN*y2]->netFromCount; i++) {
         if (grid[x2+XLEN*y2]->netsFrom[i] != grid[x2+XLEN*y2]->gateNum) {
            len += grid_routeLen(gates,grid[x2+XLEN*y2]->netsFrom[i]);
         }
      }
   }
   return len;
}
static int halfPerimiterWireLength(gate_t* gateList, int gate) {
   if (!((gateList+gate)->gateNum)) {
      return 0;
   }
   int minX = (gateList+gate)->col;
   int maxX = (gateList+gate)->col;
   int minY = (gateList+gate)->row;
   int maxY = (gateList+gate)->row;
   int* toGates = (gateList+gate)->netsTo;

   int curX = 0;
   int curY = 0;
   for ( int i = 0; i < (gateList+gate)->netToCount; i++) {
      curX = (gateList+toGates[i])->col;
      curY = (gateList+toGates[i])->row;
      if (curX > maxX) {
         maxX = curX;
      } else if (curX < minX) {
         minX = curX;
      }
      if (curY > maxY) {
         maxY = curY;
      } else if (curY < minY) {
         minY = curY;
      }
   }
   return (maxX-minX)+(maxY-minY);
}

int grid_init(grid_t** grid, gate_t** gates) {
   char * buffer = malloc(BUFFER_SIZE);
   char * tok;
   *grid = calloc(XLEN*YLEN,sizeof(grid_t));
   //*gates = calloc(GATE_COUNT,sizeof(gate_t));
   if ( fgets(buffer,BUFFER_SIZE,stdin) ) {
      tok = strtok( buffer, (const char*) " " );
      GATE_COUNT = atoi(tok)+3;
      printf("Gates: %d\n",GATE_COUNT);
      *gates = calloc(GATE_COUNT,sizeof(gate_t));
      tok = strtok( NULL, (const char*) " " );
   }
   free(buffer);
   if (!grid || !gates) {
      return FAIL;
   }
   return PASS;
}
int grid_fill(grid_t* grid, gate_t* gates) {
   char * buffer = malloc(BUFFER_SIZE);
   int* pointerBuf = 0; 
   char*tok = 0;
   int curGateNum = 0; 
   int pointGateNum = 0;
   int count;
   int xPlace = 0;
   int yPlace = 0;

   while (1) {
      if ( fgets(buffer,BUFFER_SIZE,stdin) ) {
         tok = strtok( buffer, (const char*) " " );
         curGateNum = atoi(tok);
         tok = strtok( NULL, (const char*) " " );
         pointerBuf = malloc(sizeof(int)*atoi(tok));
         gates[curGateNum].gateNum = curGateNum;
         do {
            rand_rdrand(&xPlace,XLEN);
            gates[curGateNum].col = xPlace;
            rand_rdrand(&yPlace,YLEN);
            gates[curGateNum].row = yPlace;
         } while ( grid[xPlace + XLEN*yPlace] );
         grid[xPlace + XLEN*yPlace] = gates+curGateNum;
         //pointerBuf = malloc(BUFFER_SIZE);
         count = 0;
         while (1) {
            tok = strtok( NULL,(const char*)" " );
            if ( !tok || (tok[0] == '/') ) {
               break;
            }
            pointGateNum = atoi(tok);
            pointerBuf[count++] = pointGateNum;
            gates[pointGateNum].netFromCount += 1;
            gates[pointGateNum].netsFrom = realloc(gates[pointGateNum].netsFrom, gates[pointGateNum].netFromCount*sizeof(void*));
            gates[pointGateNum].netsFrom[gates[pointGateNum].netFromCount-1] = curGateNum;
         }
         gates[curGateNum].netToCount = count;
         gates[curGateNum].netsTo = pointerBuf;
         //gates[curGateNum].netsTo = realloc(pointerBuf,count*sizeof(void*));
      } else if (feof(stdin)) {
         break;
      }
   }
   free(buffer);
   return PASS;
}
int grid_free(grid_t* grid, gate_t* gates)
{
   int count = 0;
   while (count < GATE_COUNT) {
      if (gates[count].netsFrom) {
         free(gates[count].netsFrom);
      }
      if (gates[count].netsTo) {
         free(gates[count].netsTo);
      }
      count++;
   }
   free(gates);
   free(grid);
   return PASS;
}
int grid_dumpGrid(grid_t* grid) {

   for (int i =0; i < XLEN; i++) {
      printf("-");
   }
   printf("\n");
   for (int i = 0; i < XLEN; i++) {
      for (int j =0; j < YLEN; j++) {
         if (grid[i+XLEN*j]) {
            printf("1");
         } else {
            printf(" ");
         }
      }
      printf("\n");
   }
   for (int i =0; i < XLEN; i++) {
      printf("-");
   }
   printf("\n");

   return 0;
}
