#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 	150
#define CHARS 	1000

#define YELLOW 	"\033[1;33m"
#define RESET 	"\033[0m"
#define GREEN 	"\033[1;32m"
#define RED 	"\033[1;31m"

int 	num_caracteres = 0;
int 	area  = 0;
int 	precio = 0;
int 	filas = 0;
int 	columnas = 0;
int 	caracteres_presentes[CHARS] = {0};
char	mapa[MAX][MAX] = {0};
int 	mapa_b[MAX][MAX] = {0};
int		visitado[MAX][MAX] = {0};
int 	laterales[MAX+2][MAX+2] = {0};

void print_mapa() 
{
    for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
        	printf(" %c ", mapa[i][j]);
		printf("\n");
	}
    printf("\n");
}

void print_mapa_b() 
{
    for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			if (mapa_b[i][j] < 10)
				printf(" ");
			printf("%i ", mapa_b[i][j]);
		}
		printf("\n");
	}
    printf("\n");
}

void print_visited(int caracter) 
{
    for (int i = 0; i < filas + 2; i++) 
	{
        for (int j = 0; j < columnas + 2; j++)
		{
			if (mapa[i-1][j-1] == caracter && i != 0 && j != 0 && i != filas+1 && j != columnas+1)
				printf("%s%i %s", RED, mapa[i-1][j-1], RESET);
			else
			{
				if (laterales[i][j] == 1)
					printf(GREEN);
				else
					printf(YELLOW);
            	printf("%d ", laterales[i][j]);
				printf(RESET);
			}
		}
        printf("\n");
    }
    printf("\n");
}

int is_valid(int i, int j) 
{
    return (i >= 0 && i < filas && j >= 0 && j < columnas);
}

void flood_fill(int x, int y, char original, int value) 
{
	int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    if (!is_valid(x, y) || visitado[x][y] || mapa[x][y] != original)
		return;
    visitado[x][y] = 1;
    mapa_b[x][y] = value;
    for (int i = 0; i < 4; i++) 
	{
        int new_x = x + dir[i][0];
        int new_y = y + dir[i][1];
        flood_fill(new_x, new_y, original, value);
    }
}

void modify_mapa() 
{
	int x = 1; //0 y 1 no los uso al coincidir con valores booleanos
    for (int i = 0; i < filas; i++) 
        for (int j = 0; j < columnas; j++) 
            if (!visitado[i][j]) 
                flood_fill(i, j, mapa[i][j], ++x);
}

void change_map(char *filename) 
{
    FILE *file = fopen(filename, "r");
    while (fgets(mapa[filas], MAX, file)) 
	{
        mapa[filas][strcspn(mapa[filas], "\n")] = 0;
        filas++;
    }
    columnas = strlen(mapa[0]);
    fclose(file);
	
	modify_mapa();

    // printf("Mapa original\n");
    // print_mapa();
    // printf("Mapa modificado\n");
    // print_mapa_b();

}

void encontrar_caracteres_presentes() 
{
	bool existe[CHARS] = {false};
	num_caracteres = 0;
    for (int i = 0; i < filas; i++) 
	{
        for (int j = 0; j < columnas; j++) 
		{
            int caracter = mapa_b[i][j];
			if (num_caracteres > CHARS)
			{
				printf("Demasiados caracteres en el mapa\n");
				exit(1);
			}
			if (!existe[caracter]) 
			{
                existe[caracter] = true;
                caracteres_presentes[num_caracteres++] = caracter;
            }
        }
    }
    caracteres_presentes[num_caracteres] = '\0';
}

void inicializar_matriz_ampliada() 
{
    for (int i = 0; i < filas + 2; i++)
        for (int j = 0; j < columnas + 2; j++)
            laterales[i][j] = 0;
}

void rellenar_columna(int x, int y, int caracter, int d) 
{
    for (int i = x; i < filas+1; i++) 
	{
		if (mapa_b[i-1][y-1] == caracter || mapa_b[i-1][y-d-1] != caracter)
			break;
		laterales[i][y] = 1;
    }
    for (int i = x-1; i > 0; i--) 
	{
		if (mapa_b[i-1][y-1] == caracter || mapa_b[i-1][y-d-1] != caracter)
			break;
    	laterales[i][y] = 1;
    }
}

void rellenar_fila(int x, int y, int caracter, int d) 
{
    for (int j = y; j < columnas+1; j++) 
	{
		if (mapa_b[x-1][j-1] == caracter || mapa_b[x-d-1][j-1] != caracter)
			break;
        laterales[x][j] = 1;
    }
    for (int j = y-1; j > 0; j--) 
	{
		if (mapa_b[x-1][j-1] == caracter || mapa_b[x-d-1][j-1] != caracter)
			break;
        laterales[x][j] = 1;
    }
}

int explorar_mapa(int x, int y, int caracter, char selection)
{
	int lados = 0;
	int d[2] = {1, -1};
    if (mapa_b[x - 1][y - 1] == caracter && !laterales[x][y]) 
	{
		laterales[x][y] = 1;
		if (selection == 'v')
		{
			for (int i = 0; i < 2; i++) 
			{
        		int ny = y + d[i];
				if (!laterales[x][ny] && (ny < 1 || ny > columnas || mapa_b[x-1][ny-1] != caracter)) 
        		{
					lados++;
					rellenar_columna(x, ny, caracter, d[i]);
				}
			}
		}
		else
		{
			++area;
			for (int i = 0; i < 2; i++) 
			{
        		int nx = x + d[i];
        		if (!laterales[nx][y] && (nx < 1 || nx > filas || mapa_b[nx-1][y-1] != caracter)) 
				{
            		lados++;
					rellenar_fila(nx, y, caracter, d[i]);
        		}
    		}
		}
    }
    return lados;
}

int main() 
{
	change_map("data.txt");

   	encontrar_caracteres_presentes();
    for (int k = 0; k < num_caracteres; k++) 
	{ 
        int caracter = caracteres_presentes[k];
		// printf("Caracter %d : ", caracter);
		int lados_v = 0;
		int lados_h = 0;
		area = 0;
		inicializar_matriz_ampliada();
        for (int i = 1; i < filas + 1; i++) 
		{
			for (int j = 1; j < columnas + 1; j++) 
				lados_h += explorar_mapa(i, j, caracter, 'h');
			if (i % 2 == 1)
			{
				// print_visited(caracter);
				// printf("  Lados horizontales = %d\n", lados_h);
				inicializar_matriz_ampliada();
			}
        }
		// printf("  Lados horizontales = %d\n", lados_h);

		inicializar_matriz_ampliada();
        for (int j = 1; j <= columnas + 1; j++) 
		{
        	for (int i = 1; i <= filas + 1; i++) 
            	lados_v += explorar_mapa(i, j, caracter, 'v');
			if (j % 2 == 1)
			{
				// print_visited(caracter);
				// printf("  Lados verticales = %d\n", lados_v);
				inicializar_matriz_ampliada();
			}
        }
		// printf("  Lados verticales = %d\n", lados_v);

		// printf("  %d * %d = %d\n", area, lados_v+lados_h, (lados_h + lados_v) * area);
		precio += (lados_h + lados_v) * area;
    }
	printf("\nPRECIO TOTAL: %d\n", precio);

    return 0;
}
