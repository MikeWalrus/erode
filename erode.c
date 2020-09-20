#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define WID 6 
#define LEN 6
#define RIVAL ((*player == 1) ? 2 : 1)
#define RiVAL ((player == 1) ? 2 : 1)


int refresh(int data[][LEN]);
void reset(int data[][LEN]);
int ask(int data[][LEN], int * player);
int calc(int data[][LEN], int count[]);
bool isNear(int data[][LEN], int x, int y, int player);
int set(int data[][LEN], int x, int y, int player);


int main(void)
{
	int player = 1;
	int data[WID][LEN];
	int stat;
	int count[] = {0, 0, 0};
	reset(data);
	refresh(data);
	while((stat = ask(data, &player)) != 0)
	{
		switch(stat)
		{
			case 1:
				refresh(data);
				player = RiVAL;
				break;
			case 2:
				printf("Nope! It's not next to any of your pieces.\n");
				break;
			case 3:
				printf("Nope! It's not empty!\n");
				break;
			case 4:
				calc(data, count);
				break;
			case 5:
				reset(data);
				count[0]=count[1]=count[2];
				player = 1;
				refresh(data);
				break;


		}
		
	}
	return 0;
}


int calc(int data[][LEN], int count[])
{
	int player = 1;
	int hasAdded[3]; 
	int temp;
	do
	{
		hasAdded[1] = hasAdded[2] = 0;
		for(int i = 1; i <= 2; i++)
		{
			for(int i = 0; i < WID; i++)
				for(int j = 0; j < LEN; j++)
				{
					hasAdded[player] = (set(data, i, j, player) == 1 && hasAdded[player] == 0) ? 1 : hasAdded[player];
					refresh(data);
				}
			player = RiVAL;
				
		}
	}while(hasAdded[1] ||  hasAdded[2]);
	refresh(data);
	for(int i = 0; i < WID; i++)
		for(int j = 0; j < LEN; j++)
			count[data[i][j]]++;
	if(count[1] != count[2])
		printf("Player %d wins!\n", ((count[1] > count[2]) ? 1 : 2));
	else
		printf("DRAW!\n");
	printf("Player 1: %4d\nPlayer 2: %4d\n", count[1], count[2]);
	return 0;
}

int set(int data[][LEN], int x, int y, int player)
{
	if(data[x][y] == 0)
	{
		if(isNear(data, x, y, player))
		{
			data[x][y] = player;
			for(int i = x - 1; i <= x + 1; i++)
				for(int j = y - 1; j <= y + 1; j++)
					if(i >= 0 && i < WID && j >= 0 && j < LEN)
						if(data[i][j] == RiVAL)
							data[i][j] = player;
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



int ask(int data[][LEN], int * player)
{
	int ch;
	int x, y;
	printf("\nPlayer %d:\nnumber: ", *player);
	while(scanf("%d", &x) != 1 || x < 1 || x > LEN)
	{
		if((ch = getchar()) == 'q')
			return 0;
		else if(ch == 'c')
			return 4; //end and calc
		else if(ch == 'r')
			return 5; //rematch
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
	return set(data, x, y, *player);
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
		printf("\"q\" to quit.\n\"c\" to end and calculate\n\"r\" for a rematch\n");
	return 0;
}

		


				
