
#include "main.h"
#include "random.h"

#include <time.h>

void init_random() { srand(GetTickCount()); }
int brandom(int bot,int top) { return (int)((float)(top-bot)*rand()/(RAND_MAX+1.0))+bot; }
int get_random_number(int range) { return rand()%range; }
