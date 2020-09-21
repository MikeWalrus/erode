/*ARTidiot v1 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "erode.h"

int artidiot(int data[][LEN], int player)
{
	int choice[400][2];
	int options = 0;
	int final;
	bool isBetter;
	int prevSurr[] = {0, 0, 0}; 
	int countSurr[] = {0, 0, 0}; /*countSurr[0] for empty spot.
				       [1] for ally. [2] for rival */
	for (int x = 0; x < WID; x++)
		for (int y = 0; y < LEN; y++)
		{
			if (data[x][y] == 0 && isNear(data, x, y, player))
			{
				for (int k = 0; k < 3; k++)
					countSurr[k] = 0;
				for (int i = x - 1; i <= x + 1; i++)
				{
					for (int j = y - 1; j <= y + 1; j++)
					{
						if (i >= 0 && i < WID && j >= 0 && j < LEN)
						{
							if (data[i][j] == player)
								countSurr[1]++;
							else if (data[i][j] == RiVAL)
								countSurr[2]++;
							else if (data[i][j] == 0)
								countSurr[0]++;
						}
					}
				}
				//core
				if (countSurr[2] > prevSurr[2] || (countSurr[2] == prevSurr[2] && countSurr[0] > prevSurr[0]))
					isBetter = true;
				else if (countSurr[2] == prevSurr[2] && countSurr[0] == prevSurr[0])
					isBetter = false; //as good as prev
				else
					continue;
				if(isBetter)
				{
					options = 1;
					choice[options - 1][0] = x;
					choice[options - 1][1] = y; //save current best coordinates as the only choice
					for (int k = 0; k < 3; k++)
					{
						printf("%d ", countSurr[k]);
						prevSurr[k] = countSurr[k];
					}
					printf("\n");
				}
				else
				{
					options++;
					choice[options - 1][0] = x;
					choice[options - 1][1] = y; //add current coordinates to available choices
					for (int k = 0; k < 3; k++)
					{
						printf("%d ", countSurr[k]);
					}
					printf("\n");
				}
			}
		}
	if (options >= 0)
	{
		srand((unsigned)time(NULL));
		final = rand() % options; //ARTidiot says,"My decision is final!"
		system("sleep 1");
		set(data, choice[final][0], choice[final][1], player, 0);
		refresh(data);
		printf("I found a solution in %d ones, and used No. %d, though I am an idiot.\n", options, final + 1);
		printf("I set down my piece at %d%c.\n", choice[final][0] + 1, choice[final][1] + 'A'); 
		return 1;
	}
	else
	{
		printf("I am out of options not because I am an idiot!\n");
		return 0;
	}
}




