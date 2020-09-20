#include <stdbool.h>
#define WID 6
#define LEN 6
#define RIVAL ((*player == 1) ? 2 : 1)
#define RiVAL ((player == 1) ? 2 : 1)


int refresh(int data[][LEN]);
void reset(int data[][LEN]);
int ask(int data[][LEN], int * player, bool robot);
int calc(int data[][LEN], int count[]);
bool isNear(int data[][LEN], int x, int y, int player);
int set(int data[][LEN], int x, int y, int player);
int artidiot(int data[][LEN], int player);
