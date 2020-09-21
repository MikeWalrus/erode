#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include "erode.h"

int main(void)
{
	int player = 1;
	int data[WID][LEN];
	int stat;
	int count[] = {0, 0, 0};
	bool robot = false;
	bool robomatch = false;
	reset(data);
	refresh(data);
	while((stat = ask(data, &player, robot, robomatch)) != 0)
	{
		switch(stat)
		{
			case 1: //standard
				robomatch = false;
				player = RiVAL;
				break;
			case 2: //not near
				printf("Nope! It's not next to any of your pieces.\n");
				break;
			case 3: //not empty
				printf("Nope! It's not empty!\n");
				break;
			case 4: //calc
				calc(data, count);
				robomatch = false;
				break;
			case 5: //rematch
				reset(data);
				count[0]=count[1]=count[2];
				player = 1;
				robot = false;
				refresh(data);
				break;
			case 6: //robot mode
				robot = true;
				player = 1;
				break;
			case 7: //bot vs bot
				robomatch = true;
		}

	}
	return 0;
}


int calc(int data[][LEN], int count[])
{
	int backup[WID][LEN];
	int player = 1;
	int hasAdded[3]; 
	int temp;
	int setReturn;
	int eatCount;  
	bool isDone = true;
	int result;
	for(int i = 0; i < WID; i++)
		for(int j = 0; j < LEN; j++)
			backup[i][j] = data[i][j];

	do
	{
		hasAdded[1] = hasAdded[2] = 0;
		for(int i = 1; i <= 2; i++)
		{
			for(int i = 0; i < WID; i++)
				for(int j = 0; j < LEN && isDone; j++)
				{
					setReturn = set(data, i, j, player, 1, &eatCount);
					hasAdded[player] = (setReturn == 1 && hasAdded[player] == 0) ? 1 : hasAdded[player];
					refresh(data);
					if(eatCount != 0)
						isDone = false;
				}
			player = RiVAL;

		}
	}while((hasAdded[1] ||  hasAdded[2]) && isDone);
	if(isDone)
	{
		refresh(data);
		count[0] = count[1] = count[2] = 0;
		for(int i = 0; i < WID; i++)
			for(int j = 0; j < LEN; j++)
				count[data[i][j]]++;
		if(count[1] != count[2])
		{
			printf("Player %d wins!\n", ((count[1] > count[2]) ? 1 : 2));
			result = ((count[1] > count[2]) ? 1 : 2);
		}
		else
		{
			printf("DRAW!\n");
			result = 0;
		}
		printf("Player 1: %4d\nPlayer 2: %4d\n", count[1], count[2]);
		return result;
	}
	else
	{
		printf("No, I don't think you have finished the game.\n");  
		for(int i = 0; i < WID; i++)
			for(int j = 0; j < LEN; j++)
				data[i][j] = backup[i][j];
		refresh(data);

		return -1;
	}
}

int set(int data[][LEN], int x, int y, int player, int parmN, ...)
{
	va_list ap;
	va_list apcpy;
	va_start(ap, parmN);
	va_copy(apcpy, ap);
	va_end(ap);
	int *eatCount;
	if (parmN >= 0)
	{
		eatCount = va_arg(apcpy, int *);
		*eatCount = 0;
	}
	if(data[x][y] == 0)
	{
		if(isNear(data, x, y, player))
		{
			data[x][y] = player;
			for(int i = x - 1; i <= x + 1; i++)
				for(int j = y - 1; j <= y + 1; j++)
					if(i >= 0 && i < WID && j >= 0 && j < LEN)
						if(data[i][j] == RiVAL)
						{
							data[i][j] = player;
							if(parmN > 0)
								*eatCount += 1;
						}	
			return 1; //successfully set
		}
		else
			return 2; //not near
	}
	else
		return 3; //not empty
}

bool isNear(int data[][LEN], int x, int y, int player)
{
	bool isNear = false;
	for(int i = x - 1; i <= x + 1; i++)
		for(int j = y - 1; j <= y + 1; j++)
			if(i >= 0 && i < WID && j >= 0 && j < LEN)
				if(!(data[i][j] != player && !isNear))
					isNear = true;
	return isNear;
}



int ask(int data[][LEN], int * player, bool robot, bool robomatch)
{
	int ch;
	int x, y;
	int r;
	int count[] = {0, 0, 0};
	int bot[] = {0, 0, 0};
	if (!robomatch)
	{
		if (*player == 1 || (*player == 2 && !robot))
		{
			printf("\nPlayer %d:\nnumber: ", *player);
			while(scanf("%d", &x) != 1 || x < 1 || x > LEN)
			{
				if((ch = getchar()) == 'q')
					return 0;
				else if(ch == 'c')
					return 4; //end and calc
				else if(ch == 'r')
					return 5; //rematch
				else if(ch == 'i')
					return 6; //robot mode
				else if(ch == 'v')
					return 7; //bot vs bot
				else
					getchar();
			}
			getchar();
			printf("letter: ");
			do
			{
				ch = getchar();
				ch = toupper(ch);
			}while(!(ch >= 'A' && ch < 'A' + LEN));
			x -= 1;
			y = ch - 65;
			r = set(data, x, y, *player, 0);
			refresh(data);
			return r;
		}
		else
		{

			artidiot(data, *player);
			return 1;

		}
	}
	else //bot vs bot
	{
		for (int t = 1; t <= BTIMES; t++) 
		{
			while(artidiot(data, *player))
			{
				*player = RIVAL;
			}
			bot[calc(data, count)]++;
			reset(data);
			*player = 1;
		}
		printf("Bot 1:%3d Bot 2:%3d\n", bot[1], bot[2]);
		return 1;
	}
}


void reset(int data[][LEN])
{
	for(int i = 0; i < WID; i++)
		for(int j = 0; j < LEN; j++)
			data[i][j] = 0;
	data[0][LEN - 1] = data[0][0] = 1;
	data[WID - 1][0] = data[WID - 1][LEN -1] = 2;
}


int refresh(int data[][LEN])
{
	system("clear");
	for(int i = -1; i < WID; i++)
	{
		if(i != -1)
			printf("%3d", i+1);
		else
			printf("   ");
		for(int j = 0; j < LEN; j++)
		{
			if(i == -1)
			{
				printf("%c ", j + 65);
				continue;
			}
			switch(data[i][j])
			{
				case 0:
					printf(". ");
					break;
				case 1:
					printf("O ");
					break;
				case 2:
					printf("X ");
					break;
			}
		}
		printf("\n");
	}
	printf("\"q\" to quit.\n\"c\" to end and calculate\n\"r\" for a rematch\n\"i\" to enter robot mode and summon ARTidiot v1\n\"v\" to perform bot vs bot match %d time(s)", BTIMES);
	return 0;
}





