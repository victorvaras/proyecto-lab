#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGO_TIPO 20
#define LARGO_ENTRADA 60

typedef struct Sensor
{
	char tipo[LARGO_TIPO];
	//posicion
	int x; //fila
	int y;//columna
	//informacion
	int temperatura;
	int humedad; 

}Sensor;

typedef struct Punto //sensores que deben ser activados
{
	char tipo[LARGO_TIPO];
	//posicion
	int x; //fila
	int y;//columna
	//posicion boton
	int i;
	int j;
	int cantidad_pasos; 
	
}Punto;

typedef struct Componentes_Mapa //tomar cant, dps scanf para obtener ptos
{
	int cantidad_Sensores; 
	int cantidad_Puntos;
	Sensor* Sensores;
	Punto* Puntos;

}Componentes_Mapa;

Sensor* leer_Sensores(char* nombre_Archivo, int* cantidad_Sensores, int* cantidad_Puntos){
	FILE* puntero_archivo;

	puntero_archivo = fopen(nombre_Archivo,"r");

	if(puntero_archivo == NULL)
	{
	  printf("Error! No existe el archivo");   
	  exit(1);             
	}

	char str[LARGO_ENTRADA];

	//Aqui se debe leer a cantidad de drones
	fscanf(puntero_archivo, "%d", cantidad_Sensores);
	printf("Cantidad de sensores es: %d\n", *cantidad_Sensores);

	/*
	fgets(str, 60, puntero_archivo);
	token = strtok(NULL, " ");
		printf( "segundo token %s\n", token);
	obtener cant ptos que activar

	cantidad puntos a volver a activar
	fscanf(puntero_archivo, "%d", cantidad_Puntos);
	printf("Cantidad de sensores por activar es: %d\n", *cantidad_Puntos);
	*/

	Sensor* arreglo_Sensores = (Sensor*)malloc(sizeof(Sensor)*(*cantidad_Sensores));

	int i;
	char * token;
	for (i = 0; i < 9; ++i)
	{
	/*  memset(str, '\0', LARGO_ENTRADA);
		//Se lee el tipo de dron
		fscanf(puntero_archivo, "%s", arreglo_Sensores[i].tipo);
		//printf("Tipo dron leido: %s\n", arreglo_Drones[i].tipo);
		//Se lee el salto de linea 
		fgets(str, 60, puntero_archivo);
		*/

		//Se lee la temperatura
		fgets(str, 60, puntero_archivo);
		printf("string leido: %s", str);
		// Extract the first token
		token = strtok(int, "."); //revisar
		printf( "primer token %s\n", token); //printing each token
		token = strtok(NULL, " ");
		printf( "segundo token %s\n", token);
		/*
		token = strtok(NULL, " ");
		printf( "tercer token %s\n", token);
		token = strtok(NULL, " ");
		printf( "cuarto token %s\n", token);
		*/
		arreglo_Sensores[i].temperatura = atoi(token);
		printf("La temperatura es: %d\n", arreglo_Sensores[i].temperatura);

		//Se lee el humedad
		fgets(str, 60, puntero_archivo); //guarda cadena caract hasta un salto de lienea
		// Extract the first token
		token = strtok(str, ".");
		token = strtok(NULL, " ");
		token = strtok(NULL, " ");

		arreglo_Sensores[i].humedad = atoi(token);
		printf("La humedad es: %d\n", arreglo_Sensores[i].humedad);
	}

	fclose(puntero_archivo);

	return arreglo_Sensores;
}

void imprimir_Sensores(Sensor* arreglo_Sensores, int cantidad_Sensores, int cantidad_Puntos)
{
	printf("La cantidad de sensores es: %d\n\n", cantidad_Sensores);
	printf("Y la cantidad de puntos a volver a activar es: %d\n\n", cantidad_Puntos);
/*
	for (int i = 0; i < cantidad_Drones; ++i)
	{
		printf("Tipo dron: %s\n", arreglo_Drones[i].tipo);
		printf("La bateria del dron es: %d\n", arreglo_Drones[i].bateria);
		printf("El estanque del dron es: %d\n", arreglo_Drones[i].estanque);
		printf("La rapidez del dron es: %d\n", arreglo_Drones[i].rapidez);
		printf("\n\n\n");
	}
*/	return;
}

int main()
{
	int cantidad_Sensores;
	int cantidad_Puntos;
	Sensor* arreglo_Sensores;
	arreglo_Sensores = leer_Sensores("Puntos.in", &cantidad_Sensores, &cantidad_Puntos);
	imprimir_Sensores(arreglo_Sensores, cantidad_Sensores, cantidad_Puntos);
	free(arreglo_Sensores);
	printf("cantidad sensores: %i", cantidad_Sensores);
	return 0;
}
