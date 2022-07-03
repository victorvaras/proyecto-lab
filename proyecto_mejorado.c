#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LARGO_TIPO 20
#define LARGO_ENTRADA 60
#define CANTIDAD_MAPAS 2

#define COLUMNAS_MATRIZ 35
#define FILAS_MATRIZ 20


typedef struct Dron
{
	char tipo[LARGO_TIPO];
	int estanque;
	int estanque_max;
	int bateria;
	int bateria_max;
	int rapidez;
}Dron;

typedef struct UBICACION_PAJAROS{
	int pajaro_x;
	int pajaro_y;

}Ubicacion_Pajaros;

typedef struct SENSOR{
	
	int sensor_x; 
	int sensor_y;
	int temperatura;
	int humedad; 

}Sensor;

typedef struct PUNTO{
	
	int punto_x;
	int punto_y;
	int boton_x;
	int boton_y;
	int cantidad_pasos; 
	
}Punto;

typedef struct EXTRAS{

	Sensor* sensores;	
	Punto* puntos;
	
}Extras;	

typedef struct COMPONENTES_MAPA{
	
	int mapa[FILAS_MATRIZ][COLUMNAS_MATRIZ];
	int dron_x;
	int dron_y;
	int salida_x;
	int salida_y;
	int cantidad_regar; 
	int cantidad_regada;
	
	int cantidad_pajaros;
	int cantidad_sensores;
	int cantidad_sensores_max;
	int cantidad_puntos;
	int estado_punto;	
	
	Dron dron;
	Ubicacion_Pajaros* pajaros;
	Sensor* sensor;	
	Punto* punto;
	
}Conponentes_Mapa;


typedef struct BEE_PAJARO{

	int mapa[FILAS_MATRIZ][COLUMNAS_MATRIZ];
	int dron_x;
	int dron_y;
	int pajaro_x;
	int pajaro_y;
	char* transicion ;
	int estado_Actual;
	int estado_Anterior;
}bee_Pajaro;

int correlativo;



bee_Pajaro crear_Estado(int anterior, char* t){

	bee_Pajaro nuevo_Estado;

	nuevo_Estado.estado_Actual = correlativo;
	nuevo_Estado.estado_Anterior = anterior;	
	nuevo_Estado.transicion=t;
	correlativo = correlativo + 1;

	return nuevo_Estado;
}

bee_Pajaro * agregar_Estado(bee_Pajaro * lista, int * elementos, bee_Pajaro estado){

	bee_Pajaro * nuevaLista = (bee_Pajaro *)malloc(sizeof(bee_Pajaro)*(*elementos+1));

	int i;
	for (i = 0; i < *elementos; ++i){
		nuevaLista[i] = lista[i];
	}
	nuevaLista[*elementos] = estado;
	*elementos = *elementos+1;
	free(lista);
	return nuevaLista;
}

bee_Pajaro* sacar_Estado(bee_Pajaro * lista, int * elementos){

	bee_Pajaro* nuevaLista = (bee_Pajaro*)malloc(sizeof(bee_Pajaro)*(*elementos-1));
	for (int i = 1; i < *elementos; ++i){
		nuevaLista[i-1]=lista[i];
	}
	*elementos = *elementos-1;
	free(lista);
	return nuevaLista;
}

int estado_Final(bee_Pajaro estado){

	if(estado.dron_x==estado.pajaro_x && estado.dron_y==estado.pajaro_y){
		return 1;
	}
	else{
		return 0;
	}
}

void  mostrar_Tablero(bee_Pajaro estado);
bee_Pajaro copiar_Tableros(bee_Pajaro nuevoEstado,bee_Pajaro estado){

	int i,j;

	for (int i = 0; i < FILAS_MATRIZ; ++i){
		for (int j = 0; j < COLUMNAS_MATRIZ; ++j){

			nuevoEstado.mapa[i][j]=estado.mapa[i][j];
		}
	}
	//copio las coordenadas del caballo de "estado" 
	
	nuevoEstado.pajaro_x=estado.pajaro_x;
	nuevoEstado.pajaro_y=estado.pajaro_y;
	nuevoEstado.dron_x=estado.dron_x;
	nuevoEstado.dron_y=estado.dron_y;
	//retorno nuevoEstado ya que copie el tablero y la posicion del caballo

	return nuevoEstado;
}

int comparar_Tableros(bee_Pajaro a, bee_Pajaro b){//retorno 1 (si son distintas) y 0 (si son iguales)
	int i,j;

	for ( i = 0; i < FILAS_MATRIZ; ++i){
		for ( j = 0; j < COLUMNAS_MATRIZ; ++j){

			if(a.mapa[i][j]!=b.mapa[i][j]){
				return 0;//si encuentra dos valores que sean distintos entonces los tableros no son los mismos
			}
		}
	}
	return 1;//si termina el for y no encuentro un valor distinto dentro de los tableros , entonces retorno 1, son iguales los tableros
}

int revisar_Estado(bee_Pajaro* lista,int elementos,bee_Pajaro estado){
	
	int i;
	for (i=0;i<elementos;i++){
		if((comparar_Tableros(lista[i],estado))==1){
			return 1;
		}
	}
	return 0;
}

void  mostrar_Tablero(bee_Pajaro estado){
	int i,j;//creo estas variables para copiar los tableros mediante dos ciclos fors
	printf("\n");
	for (int i = 0; i < FILAS_MATRIZ; ++i){
		for (int j = 0; j < COLUMNAS_MATRIZ; ++j){
			
			printf("%c", estado.mapa[i][j]);			
		}
		printf("\n");
	}
}

bee_Pajaro mostrarSolucion(bee_Pajaro* lista, int ultimo){

	//printf("Los movimientos a realizar son los siguientes (desde el ultimo al primero):%d \n");
	int penultimo;
	while(lista[ultimo].estado_Actual != 0){
		//printf("%d,%d\n", lista[ultimo].dron_x,lista[ultimo].dron_y);
		//printf("Transicion %s\n",lista[ultimo].transicion);
		//mostrar_Tablero(lista[ultimo]);
		
		penultimo=ultimo;
		ultimo = lista[ultimo].estado_Anterior;

		lista[ultimo].mapa[lista[ultimo].pajaro_y][lista[ultimo].pajaro_x]='6';
	}
	//lista[ultimo].mapa[lista[ultimo].pajaro_y][lista[ultimo].pajaro_x]='8';
	//mostrar_Tablero(lista[penultimo]);

	return lista[penultimo];
}

bee_Pajaro creacion_Mapa_Estado(bee_Pajaro pajaro_Inicial,int alfa){

	int a,b;

	for(a=(pajaro_Inicial.pajaro_y-alfa);a<=pajaro_Inicial.pajaro_y+alfa;a++){
		for(b=(pajaro_Inicial.pajaro_x-alfa);b<=(pajaro_Inicial.pajaro_x+alfa);b++){

			pajaro_Inicial.mapa[a][b]='1';
		}
	}
	
	return pajaro_Inicial;
}

int comprobar_Movimiento_1(bee_Pajaro estado){

	if ((estado.pajaro_y-1)>=0  && (estado.mapa[estado.pajaro_y-1][estado.pajaro_x]=='1' && estado.pajaro_y>estado.dron_y)){
		
		return 1;
	}
	else {
		return 0;
	}

}

bee_Pajaro movimiento_1(bee_Pajaro estado){

	bee_Pajaro nuevo_Estado;
	nuevo_Estado= crear_Estado(estado.estado_Actual,"arriba");

	nuevo_Estado=copiar_Tableros(nuevo_Estado,estado);

	nuevo_Estado.mapa[nuevo_Estado.pajaro_y][nuevo_Estado.pajaro_x]='0';
	nuevo_Estado.mapa[nuevo_Estado.pajaro_y-1][nuevo_Estado.pajaro_x]='2';

	nuevo_Estado.pajaro_x=nuevo_Estado.pajaro_x;
	nuevo_Estado.pajaro_y=nuevo_Estado.pajaro_y-1;
	return nuevo_Estado;

}

int comprobar_Movimiento_2(bee_Pajaro estado){

	if ((estado.pajaro_x+1)<(COLUMNAS_MATRIZ)&& (estado.mapa[estado.pajaro_y][estado.pajaro_x+1]=='1' && estado.pajaro_x<estado.dron_x)){
		return 1;
	}
	else {
		return 0;
	}

}

bee_Pajaro movimiento_2(bee_Pajaro estado){

	bee_Pajaro nuevo_Estado;

	nuevo_Estado= crear_Estado(estado.estado_Actual,"derecha");
	nuevo_Estado=copiar_Tableros(nuevo_Estado,estado);

	nuevo_Estado.mapa[nuevo_Estado.pajaro_y][nuevo_Estado.pajaro_x]='0';
	nuevo_Estado.mapa[nuevo_Estado.pajaro_y][nuevo_Estado.pajaro_x+1]='2';

	nuevo_Estado.pajaro_x=nuevo_Estado.pajaro_x+1;
	nuevo_Estado.pajaro_y=nuevo_Estado.pajaro_y;
	
	return nuevo_Estado;
}

int comprobar_Movimiento_3(bee_Pajaro estado){
	
	if ((estado.pajaro_y+1)<(FILAS_MATRIZ)&& (estado.mapa[estado.pajaro_y+1][estado.pajaro_x]=='1' && estado.pajaro_y<estado.dron_y)){
		return 1;
	}
	else {
		return 0;
	}
}

bee_Pajaro movimiento_3(bee_Pajaro estado){

	bee_Pajaro nuevo_Estado;
	nuevo_Estado= crear_Estado(estado.estado_Actual,"abajo");

	nuevo_Estado=copiar_Tableros(nuevo_Estado,estado);

	nuevo_Estado.mapa[nuevo_Estado.pajaro_y][nuevo_Estado.pajaro_x]='0';
	nuevo_Estado.mapa[nuevo_Estado.pajaro_y+1][nuevo_Estado.pajaro_x]='2';

	nuevo_Estado.pajaro_x=nuevo_Estado.pajaro_x;
	nuevo_Estado.pajaro_y=nuevo_Estado.pajaro_y+1;
	return nuevo_Estado;
}

int comprobar_Movimiento_4(bee_Pajaro estado){

	if ((estado.pajaro_x-1)>=0 && (estado.mapa[estado.pajaro_y][estado.pajaro_x-1]=='1' && estado.pajaro_x>estado.dron_x)){
		return 1;
	}
	else {
		return 0;
	}

}

bee_Pajaro movimiento_4(bee_Pajaro estado){

	bee_Pajaro nuevo_Estado;
	nuevo_Estado= crear_Estado(estado.estado_Actual,"izquierda");

	nuevo_Estado=copiar_Tableros(nuevo_Estado,estado);

	nuevo_Estado.mapa[nuevo_Estado.pajaro_y][nuevo_Estado.pajaro_x]='0';
	nuevo_Estado.mapa[nuevo_Estado.pajaro_y][nuevo_Estado.pajaro_x-1]='2';

	nuevo_Estado.pajaro_x=nuevo_Estado.pajaro_x-1;
	nuevo_Estado.pajaro_y=nuevo_Estado.pajaro_y;
	return nuevo_Estado;
}


Conponentes_Mapa mover_pajaros(Conponentes_Mapa componentes){

	
	int i,j,a,b,salir;
	int can_Abiertos;
	int can_Cerrados;
	bee_Pajaro pajaro_act, pajaro_sig;
	bee_Pajaro* abiertos;
	bee_Pajaro* cerrados;
	bee_Pajaro pajaro_Inicial;
	//printf("Pajaro 3 X %d  Y %d \n\n\n",componentes.pajaros[2].pajaro_x,componentes.pajaros[2].pajaro_y);
	//printf("can pajaros %d",componentes.cantidad_pajaros);
	//componentes.cantidad_pajaros=1;
	for(i=0;i<componentes.cantidad_pajaros;i++){
		
		correlativo=0;
		can_Abiertos = 0;
		can_Cerrados = 0;
		salir=0;
	
		abiertos=(bee_Pajaro *)malloc(sizeof(bee_Pajaro)*can_Abiertos);
		cerrados=(bee_Pajaro *)malloc(sizeof(bee_Pajaro)*can_Cerrados);

		pajaro_Inicial = crear_Estado(correlativo,"");

		//se copia el mapa del programa a bee del pajaro
		for ( a = 0; a < FILAS_MATRIZ; ++a){
			for ( b = 0; b < COLUMNAS_MATRIZ; ++b){

				pajaro_Inicial.mapa[a][b]=componentes.mapa[a][b];
			}
		}

		//mostrar_Tablero(pajaro_Inicial);

		//copia de informacion fundamental

		pajaro_Inicial.dron_x=componentes.dron_x;
		pajaro_Inicial.dron_y=componentes.dron_y;
		//pajaro_Inicial.dron_x=24;
		//pajaro_Inicial.dron_y=1;
		
		pajaro_Inicial.pajaro_x=componentes.pajaros[i].pajaro_x;
		pajaro_Inicial.pajaro_y=componentes.pajaros[i].pajaro_y;
		//printf("alfa %d     %d\n",componentes.pajaros[i].pajaro_y,pajaro_Inicial.pajaro_y);
	
		//pajaro_Inicial.pajaro_x=3;
		//pajaro_Inicial.pajaro_y=2;
		

		//Rango de seguimiento del dron
		int alfa= 4;
		
		//pajaro_Inicial= creacion_Mapa_Estado(pajaro_Inicial,alfa);
		
		
		for(a=(componentes.pajaros[i].pajaro_y-alfa);a<=(componentes.pajaros[i].pajaro_y+alfa);a++){
			for(b=(pajaro_Inicial.pajaro_x-alfa);b<=(pajaro_Inicial.pajaro_x+alfa);b++){

				if(pajaro_Inicial.mapa[a][b]!='#'){

					pajaro_Inicial.mapa[a][b]='1';
				}				
			}
		}
		
		pajaro_Inicial.pajaro_x=componentes.pajaros[i].pajaro_x;
		pajaro_Inicial.pajaro_y=componentes.pajaros[i].pajaro_y;
		
		pajaro_Inicial.mapa[pajaro_Inicial.pajaro_y][pajaro_Inicial.pajaro_x]='2';


		//mostrar_Tablero(pajaro_Inicial);
		//printf("pajaro X: %d   Y: %d\n",pajaro_Inicial.pajaro_x,pajaro_Inicial.pajaro_y);
		//printf("X: %d   Y: %d\n",pajaro_Inicial.dron_x,pajaro_Inicial.dron_y);
		//mostrar_Tablero(pajaro_Inicial);
		abiertos = agregar_Estado(abiertos, &can_Abiertos, pajaro_Inicial);
		
		time_t ini,fin;
		
		while (can_Abiertos>0 && salir==0){
			
			pajaro_act=abiertos[0];
			
			abiertos=sacar_Estado(abiertos, &can_Abiertos);
			cerrados= agregar_Estado(cerrados, &can_Cerrados, pajaro_act);

		
			if(estado_Final(pajaro_act)==1){
				
				//mostrar_Tablero(pajaro_act);
				pajaro_act=mostrarSolucion(cerrados,can_Cerrados-1);					
				componentes.pajaros[i].pajaro_x=pajaro_act.pajaro_x;
				componentes.pajaros[i].pajaro_y=pajaro_act.pajaro_y;
				salir++;				
			}

			else{

				
				if(comprobar_Movimiento_1(pajaro_act)==1){

					pajaro_sig= movimiento_1(pajaro_act);
					//mostrar_Tablero(pajaro_sig);
					if(revisar_Estado(abiertos,can_Abiertos,pajaro_sig)==0 && revisar_Estado(cerrados,can_Cerrados,pajaro_sig)==0){

						abiertos=agregar_Estado(abiertos,&can_Abiertos,pajaro_sig);
						//printf("Movimiento 1 Posible  \n");
					}
					else{
						correlativo = correlativo - 1;
						//printf("Movimiento 1  no posible  \n");
					}
				}

				if(comprobar_Movimiento_2(pajaro_act)==1){

					pajaro_sig= movimiento_2(pajaro_act);
					//mostrar_Tablero(pajaro_sig);

					if(revisar_Estado(abiertos,can_Abiertos,pajaro_sig)==0 && revisar_Estado(cerrados,can_Cerrados,pajaro_sig)==0){

						abiertos=agregar_Estado(abiertos,&can_Abiertos,pajaro_sig);
						//printf("Movimiento 2 Posible  \n");
					}
					else{
						correlativo = correlativo - 1;
						//printf("Movimiento 2  no posible  \n");
					}
				}
				
				if(comprobar_Movimiento_3(pajaro_act)==1){

					pajaro_sig= movimiento_3(pajaro_act);
					//mostrar_Tablero(pajaro_sig);

					if(revisar_Estado(abiertos,can_Abiertos,pajaro_sig)==0 && revisar_Estado(cerrados,can_Cerrados,pajaro_sig)==0){

						abiertos=agregar_Estado(abiertos,&can_Abiertos,pajaro_sig);
						//printf("Movimiento 3 Posible  \n");
					}
					else{
						correlativo = correlativo - 1;
						//printf("Movimiento 3  no posible  \n");
					}
				}

				if(comprobar_Movimiento_4(pajaro_act)==1){

					pajaro_sig= movimiento_4(pajaro_act);
					//mostrar_Tablero(pajaro_sig);

					if(revisar_Estado(abiertos,can_Abiertos,pajaro_sig)==0 && revisar_Estado(cerrados,can_Cerrados,pajaro_sig)==0){

						abiertos=agregar_Estado(abiertos,&can_Abiertos,pajaro_sig);
						//printf("Movimiento 4 Posible  \n");
					}
					else{
						correlativo = correlativo - 1;
						//printf("Movimiento 4 no posible  \n");
					}
				}
				
			}
		}
		//printf("\nCantidad abiertos %d  cantidad cerrados %d\n",can_Abiertos,can_Cerrados);
		//printf("Mover pajaro %d\n\n\n",i);
	}
	return componentes;
}




Dron* leer_drones(char* nombre_Archivo)
{
	FILE* puntero_archivo;
	
	// use appropriate location if you are using MacOS or Linux
	puntero_archivo = fopen(nombre_Archivo,"r");
	if(puntero_archivo == NULL)
	{
	  printf("Error!  Falta archivo");   
	  exit(1);             
	}
	char str[LARGO_ENTRADA];
	//Aqui se debe leer a cantidad de drones
	
	int cantidad_Drones;
	fscanf(puntero_archivo, "%d", &cantidad_Drones);	
	Dron* arreglo_Drones = (Dron*)malloc(sizeof(Dron)*(cantidad_Drones));
	
	int i;
	char * token;
	for (i = 0; i < 5; ++i)
	{
		memset(str, '\0', LARGO_ENTRADA);
		//Se lee el tipo de dron
		fscanf(puntero_archivo, "%s", arreglo_Drones[i].tipo);
		fgets(str, 60, puntero_archivo);
		//Se lee la bateria
		fgets(str, 60, puntero_archivo);;
		// Extract the first token
		token = strtok(str, " ");
		token = strtok(NULL, " ");
		token = strtok(NULL, " ");
		arreglo_Drones[i].bateria = atoi(token);
		arreglo_Drones[i].bateria_max = atoi(token);
		//Se lee el estanque
		fgets(str, 60, puntero_archivo);
		token = strtok(str, " ");
		token = strtok(NULL, " ");
		token = strtok(NULL, " ");
		arreglo_Drones[i].estanque = atoi(token);
		arreglo_Drones[i].estanque_max = atoi(token);
		//Se lee la rapidez
		fgets(str, 60, puntero_archivo);
		token = strtok(str, " ");
		token = strtok(NULL, " ");
		token = strtok(NULL, " ");
		arreglo_Drones[i].rapidez = atoi(token);
	}
	fclose(puntero_archivo);
	
return arreglo_Drones;
}

void imprimir_Drones(Dron* arreglo_Drones){

	printf("Cantidad de drones es: %d\n\n", 5);
	//arreglo_Drones[0].numero_Drones
	for (int i = 0; i <5 ; ++i)
	{
		printf("Tipo dron: %s\n", arreglo_Drones[i].tipo);
		printf("La bateria del dron es: %d\n", arreglo_Drones[i].bateria);
		printf("El estanque del dron es: %d\n", arreglo_Drones[i].estanque);
		printf("La rapidez del dron es: %d\n", arreglo_Drones[i].rapidez);
		printf("\n\n\n");
	}
	return;
}

Dron eleccion_Dron(Conponentes_Mapa total_Mapas,Dron* arreglo_Drones){
	int i;
	int salir=0;
	char eleccion[100];
	Dron dron_util;
	while(salir==0){
	
		printf("\n\nSeleccione dron que desea ocupar: \n");
			for (i=0;i< 5; i++){
				printf("%d.- Tipo dron: %s\n",i+1, arreglo_Drones[i].tipo);
		}
		fflush(stdin);		
		scanf("%s",&eleccion);
		
		if(0==strcmp(eleccion,"1")){
		
			dron_util=arreglo_Drones[0];
			salir++;
		}
		else if(0==strcmp(eleccion,"2")){
			
			dron_util=arreglo_Drones[1];
			salir++;
		}
		else if(0==strcmp(eleccion,"3")){
			
			dron_util=arreglo_Drones[2];
			salir++;
		}
		else if(0==strcmp(eleccion,"4")){
			
			dron_util=arreglo_Drones[3];
			salir++;
		}
		else if(0==strcmp(eleccion,"5")){
			
			dron_util=arreglo_Drones[4];
			salir++;
		}
	}
return dron_util;
}

Sensor* leer_Sensores(Sensor* sensores,char* nombre_Archivo){

	FILE* puntero_archivo;
	puntero_archivo = fopen(nombre_Archivo,"r");

	if(puntero_archivo == NULL)
	{
	  printf("Error! No existe el archivo");   
	  exit(1);             
	}

	char str[LARGO_ENTRADA];
	int cantidad_Sensores,cantidad_Puntos;	
	
	fscanf(puntero_archivo, "%d",&cantidad_Sensores);	
	sensores = (Sensor*)malloc(sizeof(Sensor)*(cantidad_Sensores));

	fscanf(puntero_archivo, "%d",&cantidad_Puntos);		

	//Lectura salto de linea
	fgets(str, 60, puntero_archivo);

	int i;
	char * token;	
	
	for(i=0;i<cantidad_Sensores;i++){
			
		
		//Lectura sensores
		fgets(str, 60, puntero_archivo);
		
		//Cordenada y
		token = strtok(str, ".");
		sensores[i].sensor_y=atoi(token);
		
		//Cordenada X
		token = strtok(NULL, " ");
		sensores[i].sensor_x=atoi(token);
		
		//Temperatura
		token = strtok(NULL, " ");
		sensores[i].temperatura=atoi(token);
		
		//Humedad
		token = strtok(NULL, " ");
		sensores[i].humedad=atoi(token);	
		
	}
	
	fclose(puntero_archivo);	
	return sensores;
}

Punto* leer_Puntos(Punto* puntos,char* nombre_Archivo){

	FILE* puntero_archivo;
	puntero_archivo = fopen(nombre_Archivo,"r");

	if(puntero_archivo == NULL)
	{
	  printf("Error! No existe el archivo");   
	  exit(1);             
	}

	char str[LARGO_ENTRADA];
	int cantidad_Sensores,cantidad_Puntos;	
	
	fscanf(puntero_archivo, "%d",&cantidad_Sensores);	
	
	fscanf(puntero_archivo, "%d",&cantidad_Puntos);	
	puntos = (Punto*)malloc(sizeof(Punto)*(cantidad_Puntos));	

	//Lectura salto de linea
	fgets(str, 60, puntero_archivo);

	int i;
	char * token;
	
	//Se lee los sensores, para llegar hasta los puntos.
	for(i=0;i<cantidad_Sensores;i++){		
		fgets(str, 60, puntero_archivo);		
	}

	for(i=0;i<cantidad_Puntos;i++){		

		fgets(str, 60, puntero_archivo);
		
		//Cordenada Y punto
		token = strtok(str, ".");
		puntos[i].punto_y=atoi(token);		
		//Cordenada X punto
		token = strtok(NULL, " ");
		puntos[i].punto_x=atoi(token);
		
		token = strtok(NULL, ".");
		puntos[i].boton_y=atoi(token);		
		//Cordenada X punto
		token = strtok(NULL, " ");
		puntos[i].boton_x=atoi(token);
		//Cantidad pasos
		token = strtok(NULL, " ");
		puntos[i].cantidad_pasos=atoi(token);

	}
	
	fclose(puntero_archivo);
	return puntos;
}

void imprimir_mapa(Conponentes_Mapa componente){
	int i,j,a,b, con=0;

	
	for( a=0; a<20; a++){

    		for(b=0;b<37;b++){

 				for(i=0;i<componente.cantidad_pajaros;i++){

 					if(componente.pajaros[con].pajaro_x==b && componente.pajaros[con].pajaro_y==a){
 						componente.mapa[a][b]='P';
 						con++;
 					}
 				}
 		}
 	}
	 

	for (int i = 0; i < FILAS_MATRIZ; ++i){
		for (int j = 0; j < COLUMNAS_MATRIZ; ++j){
			
			if(componente.dron_x==j && componente.dron_y==i){   			
    			printf("D");
    		}
			else{
				printf("%c", componente.mapa[i][j]);
			}			
		}
		printf("\n");
	}
	printf("\n\n");	
}

void leer_Mapa(char* nombre_mapa, Conponentes_Mapa* componente){

	FILE* puntero_archivo;
	puntero_archivo = fopen(nombre_mapa,"r");

	if(puntero_archivo == NULL)
	{
	  printf("Error! No existe el archivo");   
	  exit(1);             
	}

	char str[60];
	int i,j;
	char caracter;
	for (int i = 0; i < FILAS_MATRIZ; ++i)
	{
		for (int j = 0; j < COLUMNAS_MATRIZ; ++j)
		{
			caracter = fgetc(puntero_archivo);      
			componente->mapa[i][j]=caracter;
		}
		fgets(str, 60, puntero_archivo);
		//caracter = fgets(puntero_archivo);
	}

	
	fclose(puntero_archivo);
	return;



}

Conponentes_Mapa eleccion_mapa(Conponentes_Mapa* total_Mapas,char* nombres[2]){
	
	int i;
	int salir=0;
	char eleccion[100];
	while(salir==0){
	
		printf("\n\nSeleccione mapa que desea ocupar: \n");
		
		for(i=0;i<CANTIDAD_MAPAS;i++){
		
			printf("%d.- %s\n",i+1,nombres[i]);
		}
		fflush(stdin);	
		scanf("%s",&eleccion);
		
		if(0==strcmp(eleccion,"1")){
		
			return total_Mapas[0];
		}
		else if(0==strcmp(eleccion,"2")){
			
			return total_Mapas[1];
		}
	}
}

Conponentes_Mapa ubicacion_objetos(Conponentes_Mapa componentes){
	
	int a,b,i,con;
	componentes.cantidad_regar=0;
	componentes.cantidad_puntos=0;
	componentes.cantidad_sensores=0;
	componentes.cantidad_sensores_max=0;
	componentes.cantidad_pajaros=0;
	componentes.estado_punto=-1;

	for(a=0;a<20;a++){
   		for(b=0;b<37;b++){
   		
   			if(componentes.mapa[a][b]=='E'){
   			
	   			componentes.dron_x=b;
	   			componentes.dron_y=a;	   			
   			}
   			else if(componentes.mapa[a][b]=='R'){
   				
   				componentes.cantidad_regar=componentes.cantidad_regar+1;
   			}
			else if(componentes.mapa[a][b]=='*'){
								
				componentes.mapa[a][b]=' ';
				printf("");
			}
   			else if(componentes.mapa[a][b]=='P'){
   			
   				componentes.cantidad_pajaros=componentes.cantidad_pajaros+1;
   			}
			else if(componentes.mapa[a][b]=='X'){
   			
   				componentes.cantidad_sensores=componentes.cantidad_sensores+1;
				componentes.cantidad_sensores_max=componentes.cantidad_sensores_max+1;
   			}
			else if(componentes.mapa[a][b]=='B'){
   			
   				componentes.cantidad_puntos=componentes.cantidad_puntos+1;
   			}
   			else if(componentes.mapa[a][b]=='S'){
   				
   				componentes.salida_x=b;
	   			componentes.salida_y=a;   				
   			}
   		}
   	}	
	
	componentes.cantidad_regada=0;
   	//Reserva de memoria ubicacion de pajaros
   	componentes.pajaros=(Ubicacion_Pajaros*)malloc(sizeof(Ubicacion_Pajaros)*(componentes.cantidad_pajaros));   	
   	   	
   	con=0;
   	for(a=0;a<20;a++){
   		for(b=0;b<37;b++){
   		
   			if(componentes.mapa[a][b]=='P'){
   				
   				componentes.pajaros[con].pajaro_x=b;
   				componentes.pajaros[con].pajaro_y=a;   				
   				//Borrar pajaros, para ser escritos desde impresion mapa 
   				componentes.mapa[a][b]=' ';
				//printf("Pajaro %d  X: %d  Y: %d\n",con,b,a);
   				con++;
   			}
   		}
   	}   	
return componentes;
}

void imprimir_estadisticas(Conponentes_Mapa componente,int velocidad){

	int porcentaje;
	
	porcentaje=(componente.cantidad_regada*100)/componente.cantidad_regar;		
	
	printf("Bateria restante: %d\n",componente.dron.bateria);
	printf("Agua restante: %d\n",componente.dron.estanque);
	printf("Porcentaje zonas regadas: %d\n",porcentaje);
	printf("Rapidez actual: %d", velocidad);
}

//Comprobacion de alguna accion a realizar(regar, sensor, recargar)
Conponentes_Mapa comprobar_accion(Conponentes_Mapa componente, int *fin){

	int i,a,b,salir,porcentaje;

	//Comprobacion movimientos restantes, activacion punto.
	if (componente.estado_punto != -1){

		componente.punto[componente.estado_punto].cantidad_pasos=componente.punto[componente.estado_punto].cantidad_pasos-1;
		printf("Movimientos restantes para reactivar punto: %d\n",componente.punto[componente.estado_punto].cantidad_pasos);

		if(componente.punto[componente.estado_punto].cantidad_pasos==-1){

			printf("Punto no reactivado en movimientos nesesarios, vuelva a intentarlo\n");
			componente.mapa[componente.punto[componente.estado_punto].boton_y][componente.punto[componente.estado_punto].boton_x]=' ';
			componente.mapa[componente.punto[componente.estado_punto].punto_y][componente.punto[componente.estado_punto].punto_x]='B';
			
			componente.estado_punto = -1;
		}		
	}

	//Comprobar si zona debe regarse
	if(componente.mapa[componente.dron_y][componente.dron_x]=='R'){
		
		if(componente.dron.estanque>=5){
			printf("Zona regada. \n");	
			componente.cantidad_regada++;	
			componente.dron.estanque=componente.dron.estanque-5;
			componente.mapa[componente.dron_y][componente.dron_x]=' ';
		}
		else{
			printf("Se requiere recargar estanque, agua insuficiente.\n");

			printf("Se requiere recargar estanque, agua insuficiente.\n");			
		}
	}

	//Comprobacion si se debe recargar estanque
	else if(componente.mapa[componente.dron_y][componente.dron_x]=='A'){

		componente.dron.estanque=componente.dron.estanque_max;
		printf("Estanque recargado con exito\n");

	}

	//Comprobacion si se debe recargar bateria
	else if(componente.mapa[componente.dron_y][componente.dron_x]=='W'){

		componente.dron.bateria=componente.dron.bateria_max;
		printf("Bateria recargada con exito\n");

	}

	//Comprobacion de sensor
	else if(componente.mapa[componente.dron_y][componente.dron_x]=='X'){

		for (i=0;i<=componente.cantidad_sensores_max;i++){
			
			if(componente.sensor[i].sensor_x==componente.dron_x && componente.sensor[i].sensor_y==componente.dron_y){

				if (componente.sensor[i].humedad<40 && componente.sensor[i].temperatura>50){
					
					for(a=(componente.dron_y-2) ; a<=(componente.dron_y+2) ; a++){

						for(b=(componente.dron_x-2);b<=(componente.dron_x+2);b++){

							componente.mapa[a][b]='R';							
						}
					}					
					printf("Zona apta para regarse\n");
					componente.cantidad_regar=componente.cantidad_regar+24;
					componente.cantidad_sensores=componente.cantidad_sensores-1;
				}
				
				else{
					printf("Zona no apta para regar\n");
					componente.cantidad_sensores=componente.cantidad_sensores-1;
				}

				componente.mapa[componente.dron_y][componente.dron_x]=' ';				
			}
		}
		
	}

	//Comprobacion de punto a reactivar
	else if(componente.mapa[componente.dron_y][componente.dron_x]=='B'){
				
		for (i=0;i<=componente.cantidad_puntos;i++){
			
			if(componente.punto[i].punto_x==componente.dron_x && componente.punto[i].punto_y==componente.dron_y){

				componente.mapa[componente.punto[i].boton_y][componente.punto[i].boton_x]='F';
				componente.estado_punto=i;
				componente.mapa[componente.dron_y][componente.dron_x]=' ';
				printf("Debe activar el punto demarcado como 'F' en un maximo de %d movimientos\n\n",componente.punto[i].cantidad_pasos);
			}
		}		
	}

	//Punto a ser reactivado
	else if(componente.mapa[componente.dron_y][componente.dron_x]=='F'){

		printf("Punto reactivado con exito\n");
		componente.estado_punto=-1;
		componente.mapa[componente.dron_y][componente.dron_x]=' ';
	}

	else if(componente.mapa[componente.dron_y][componente.dron_x]=='S'){
		
		salir=0;
		if(componente.cantidad_sensores==0){
			salir++;			
		}
		else{
			printf("Falta comprobar %d sensores para poder completar el mapa\n",componente.cantidad_sensores);
		}

		porcentaje=(componente.cantidad_regada*100)/componente.cantidad_regar;
		if(porcentaje>=90){
			salir++;
		}
		else{
			printf("Cantidad total regada menor a 90 porciento\n");
		}

		if(salir==2){

			printf("\nMapa completado con exito\n");
			*fin=*fin+1;
		}
	}
	
	//mensaje condicion de bateria
	if (componente.dron.bateria<30){
		printf("Bateria baja, debe recargarla en los puntos W\n");
	}

	if(componente.dron.bateria==0){
		printf("\n\n Dron sin bateria. Mapa no completado\n\n");
		*fin=*fin+1;
	}
return componente;
}

void mover_dron(Conponentes_Mapa componente){
	
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	printf("Instrucciones \n");
	printf("Movimiento con teclas W-A-S-D \n");
	printf("Letra V cambio de velocidad \n");
	printf("Si se realizaran 2 movimientos, deben estar separados por coma\n");
	printf("EJEMPLO  d,w\n");
	printf("Letra K forzar salida \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	int fin=0,i,verificador;
	char movimiento[10];
	int velocidad=1;
	char delimitador[] = ",";
    char *token;
	
	
	printf("Rapidez maxima dron seleccionado: %d\n\n",componente.dron.rapidez);
	
	while (fin==0){
		
		//Activar parametro de rapidez
		imprimir_estadisticas(componente,velocidad);
		
		printf("\n\nIngrese movimientos deseado o modifique velocidad deseada  ");
		fflush(stdin);
		scanf("%s",&movimiento);
		token = strtok(movimiento, delimitador);
		
		verificador=0;
		for(i=1;i<=velocidad;i++){
		
			if(0==strcmp(token,"v")){
				if(velocidad<componente.dron.rapidez){
					
					velocidad++;				
				}
				else{
					
					velocidad=1;					
				}
				printf("Nueva velocidad: %d\n",velocidad);
			}

			else if(0==strcmp(token,"w")){
					
					if (componente.mapa[componente.dron_y-1][componente.dron_x]=='#'){
						printf("Movimiento no valido, obstaculo \n");
					}
					else if(componente.dron_x==0){
						printf("Movimiento no valido, fuera de limites\n");
					}
					else{
						componente.dron_y--;
						componente.dron.bateria=componente.dron.bateria-1;
						verificador++;
						componente=comprobar_accion(componente,&fin);
							
					}
				}
				
			else if(0==strcmp(token,"d")){
					if (componente.mapa[componente.dron_y][componente.dron_x+1]=='#'){
						printf("Movimiento no valido, obstaculo \n");
					}
					else if(componente.dron_x==34){
						printf("Movimiento no valido, fuera de limites\n");
					}
					else{
						componente.dron_x++;
						componente.dron.bateria=componente.dron.bateria-1;
						verificador++;
						componente=comprobar_accion(componente,&fin);	
					}
				}
				
			else if(0==strcmp(token,"s")){
					if (componente.mapa[componente.dron_y+1][componente.dron_x]=='#'){
						printf("Movimiento no valido, obstaculo \n");
					}
					else if(componente.dron_x==19){
						printf("Movimiento no valido, fuera de limites\n");
					}
					else{
						componente.dron_y++;
						componente.dron.bateria=componente.dron.bateria-1;
						verificador++;
						componente=comprobar_accion(componente,&fin);
							
					}
				}
								
			else if(0==strcmp(token,"a")){
					if (componente.mapa[componente.dron_y][componente.dron_x-1]=='#'){
						printf("Movimiento no valido, obstaculo \n");
					}
					else if(componente.dron_x==0){
						printf("Movimiento no valido, fuera de limites\n");
					}
					else{
						componente.dron_x--;
						componente.dron.bateria=componente.dron.bateria-1;
						verificador++;
						componente=comprobar_accion(componente,&fin);
						
							
					}
				}
								
			else if(0==strcmp(token,"k")){
					
					fin++;
				}
								
			token = strtok(NULL, delimitador);
			if(token == NULL){
					//En dado caso que se ingresen menos movimientos de los seleccionados
					//se sale de los movimientos, evitando caida del programa
							i=10;
				}
		}
		
		if (verificador!=0){
			mover_pajaros(componente);
		}
		imprimir_mapa(componente);
		
	}
}

void menu(Conponentes_Mapa* totalidad_Mapas,Dron* arreglo_Drones){

	int salir=0;
	char eleccion[10];

	printf("Bienvenido a SUCCESSFUL IRRIGATION \n\n");

	while(salir==0){

		int i;
		char* nombres[CANTIDAD_MAPAS];
		nombres[0]="Mapa_00.in";
		nombres[1]="Mapa_01.in";

		//Struct en la cual se guardara mapa a utilizar por el usuario
		Conponentes_Mapa mapa_Util;
		Dron dron_Util;

		printf("~~~~~ Menu de opciones ~~~~\n");
		printf("Indique valor numerico correspondiente a su preferencia \n");
		printf("1.- Drones disponibles.\n");
		printf("2.- Mapas disponibles.\n");
		printf("3.- Manejo dron de manera manual.\n");
		printf("4.- Manejo dron de manera automatica.\n");
		printf("5.- Salir\n");

		fflush(stdin);
		scanf("%s",eleccion);

		if(0==strcmp(eleccion,"1")){

			imprimir_Drones(arreglo_Drones);

		}

		else if(0==strcmp(eleccion,"2")){

			for	(i=0;i<CANTIDAD_MAPAS;i++){

				printf("\n~~~~~~~~~~~ %s ~~~~~~~~~~~ \n",nombres[i]);
				imprimir_mapa(totalidad_Mapas[i]);
				printf("\n\n");
			}
		}

		else if(0==strcmp(eleccion,"3")){

			mapa_Util=eleccion_mapa(totalidad_Mapas,nombres);		
			//mapa_Util=ubicacion_objetos(mapa_Util);

			dron_Util=eleccion_Dron(mapa_Util,arreglo_Drones);
			
			mapa_Util.dron=dron_Util;

			mover_dron(mapa_Util);

		}

		else if(0==strcmp(eleccion,"4")){


		}

		else if(0==strcmp(eleccion,"5")){
			
			printf("Vuelva pronto a SUCCESSFUL IRRIGATION \n\n");
			salir++;

		}

		else{
			
			printf("Eleccion no valida.\n");

		}
	}
}

int main(){

	char* nombres[CANTIDAD_MAPAS];
	nombres[0]="Mapa_00.in";
	nombres[1]="Mapa_01.in";

	char* nombre_puntos[CANTIDAD_MAPAS];
	nombre_puntos[0]="Puntos.in";
	nombre_puntos[1]="Puntos.in";

	int i;
	
	//Lectura de drones 
	Dron* arreglo_Drones;
	arreglo_Drones = leer_drones("Drones.in");


	// //Lectura de mapas disponibles
	Conponentes_Mapa totalidad_Mapas[CANTIDAD_MAPAS];

	Sensor* sensores;	
	Punto* puntos;

	for(i=0;i<CANTIDAD_MAPAS;i++){

	 	leer_Mapa(nombres[i], &totalidad_Mapas[i]);
		totalidad_Mapas[i]=ubicacion_objetos(totalidad_Mapas[i]);
		
	 	totalidad_Mapas[i].sensor=leer_Sensores(sensores,nombre_puntos[i]);
	 	totalidad_Mapas[i].punto =leer_Puntos(puntos,nombre_puntos[i]);	
	 		
	}
	

	

	//Conponentes_Mapa m=totalidad_Mapas[0];

	//m=mover_pajaros(m);
	
	
	
	//imprimir_mapa(m);

	
	//printf("Prueba\n");
	// //Llamado a menu de aplicacion.
	menu(totalidad_Mapas,arreglo_Drones);

	

	free(arreglo_Drones);


	return 0;
}
