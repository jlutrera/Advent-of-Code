#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_W 51
#define MAX_M 20001

#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

char warehouse[MAX_W][MAX_W];
char movements[MAX_M] = "";

void print_warehouse(int filas, int columnas) 
{
	for (int i = 0; i < filas; i++) 
	{
		for (int j = 0; j <= columnas; j++) 
		{
			if (warehouse[i][j] == '#')
				printf("%s%c%s", RED, warehouse[i][j], RESET);
			else if (warehouse[i][j] == '@')
				printf("%s%c%s", GREEN, warehouse[i][j], RESET);
			else if (warehouse[i][j] == 'O')
				printf("%s%c%s", YELLOW, warehouse[i][j], RESET);
			else
				printf("%c", warehouse[i][j]);
		}
		printf("\n");
	}
}

bool move_box(int x, int y, char direction, int row_w, int col_w) 
{
	int new_x = x, new_y = y;
	switch (direction) 
	{
		case '^': new_x--; break;
		case '>': new_y++; break;
		case 'v': new_x++; break;
		case '<': new_y--; break;
	}
	if (new_x < 1 || new_x > row_w || new_y < 1 || new_y > col_w || warehouse[new_x][new_y] == '#')
		return false;
	if (warehouse[new_x][new_y] == 'O') 
	{
		if (!move_box(new_x, new_y, direction, row_w, col_w))
			return false;
	}

	warehouse[new_x][new_y] = 'O';
	return true;
}

void move_robot(int x, int y, int row_w, int col_w) 
{
	int n = strlen(movements);

	for (int i = 0; i < n; i++) 
	{
		if (i == 0)
			printf("Movimiento inicial\n");
		else if (i == n - 1)
			printf("Movimiento final\n");
		else
			printf("Movimiento [%c]\n", movements[i]);
		print_warehouse(row_w, col_w);

		int new_x = x, new_y = y;
		switch (movements[i]) 
		{
			case '^': new_x--; break;
			case '>': new_y++; break;
			case 'v': new_x++; break;
			case '<': new_y--; break;
		}
		if (new_x < 1 || new_x > row_w || new_y < 1 || new_y > col_w || warehouse[new_x][new_y] == '#')
			continue;
		if (warehouse[new_x][new_y] == 'O') 
		{
			if (!move_box(new_x, new_y, movements[i], row_w, col_w))
				continue;
		}
		warehouse[x][y] = '.';
		warehouse[new_x][new_y] = '@';
		x = new_x;
		y = new_y;
	}
}

int main() {
	int row_w = 0;
	int col_w;
	FILE *file = fopen("warehouse.txt", "r");
    if (file == NULL) 
	{
        printf("Error al abrir el archivo\n");
        return 1;
    }
    while (fgets(warehouse[row_w], MAX_W, file)) 
	{
  	  size_t len = strlen(warehouse[row_w]);
  	  if (len > 0 && (warehouse[row_w][len - 1] == '\n' || warehouse[row_w][len - 1] == '\r')) 
	  {
  	      warehouse[row_w][len - 1] = '\0';
  	      len--;
  	  }
  	  if (len > 0)
	  	  row_w++;
    }
    col_w = strlen(warehouse[0]) - 1;
    fclose(file);
	warehouse[row_w][0] = '\0';

	file = fopen("movements.txt", "r");
	if (file == NULL) 
	{
		printf("Error al abrir el archivo\n");
		return 1;
	}
	char aux[MAX_M];
	while (fgets(aux, MAX_M, file)) 
	{
		aux[strcspn(aux, "\n")] = 0;
		strcat(movements, aux);
	}
	strcat(movements, "\0");
	fclose(file);

	int x = 0, y = 0;
	for (int i = 1; i < row_w; i++) 
	{
		for (int j = 1; j < col_w; j++) 
		{
			if (warehouse[i][j] == '@') 
			{
				x = i;
				y = j;
				break;
			}
		}
	}
	move_robot(x, y, row_w, col_w);

	int gps = 0;
	for (int i = 1; i < row_w; i++) 
	{
		for (int j = 1; j < col_w; j++) 
		{
			if (warehouse[i][j] == 'O')
				gps += 100 * i + j;
		}
	}
	printf("GPS: %d\n", gps);
	return 0;
}