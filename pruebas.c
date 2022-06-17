#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LARGO_TIPO 20
#define LARGO_ENTRADA 60
#define CANTIDAD_MAPAS 2


typedef struct Dron
{
	int numero_Drones;
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

typedef struct COMPONENTES_MAPA{
	
	int mapa[20][37];
	int dron_x;
	int dron_y;
	int salida_x;
	int salida_y;
	int cantidad_regar; //Corresponde a las R del mapa
	int cantidad_regada;
	int cantidad_sensores;
	int cantidad_pajaros;
	Ubicacion_Pajaros* pajaros;
	Dron dron;
	

}Conponentes_Mapa;


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
	for (i = 0; i < 3; ++i)
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

		arreglo_Drones[i].numero_Drones=cantidad_Drones;
	}
	fclose(puntero_archivo);
	
return arreglo_Drones;
}

void imprimir_Drones(Dron* arreglo_Drones)
{
	
	printf("Cantidad de drones es: %d\n\n", arreglo_Drones[0].numero_Drones);

	for (int i = 0; i < arreglo_Drones[0].numero_Drones; ++i)
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

			for (i=0;i< arreglo_Drones[0].numero_Drones; i++){

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
	}
return dron_util;
}

Conponentes_Mapa leer_Mapa(char* nombre_mapa){
	
	//printf("\n");// hace que funcione el codigo	
	
   	FILE *nombre_Archivo;
   	int c,h,i=0,con=0;  
   	
	Conponentes_Mapa elemento;
  	


	nombre_Archivo = fopen(nombre_mapa,"r");
	if(nombre_Archivo == NULL){
	
		printf("Error! Falta archivo");   
		exit(1);             
	}
		
	while(1){
			c = fgetc(nombre_Archivo);      
			elemento.mapa[con][i]=c;
										
			if(i==36){      
				con++;
				i=-1;
			}
			
			i++;
				
			if( feof(nombre_Archivo) ){
				break;
			}      
	}  
	fclose(nombre_Archivo);
	
return elemento;
}

void imprimir_mapa(Conponentes_Mapa componente){
	
	int a,b,i;
	int con=0;
	
	for(a=0;a<20;a++){
   		for(b=0;b<37;b++){
				for(i=0;i<componente.cantidad_pajaros;i++){

					if(componente.pajaros[con].pajaro_x==b && componente.pajaros[con].pajaro_y==a){

						componente.mapa[a][b]='P';
						con++;
					}
				}

		}
	}

	for(a=0;a<20;a++){
   		for(b=0;b<37;b++){	
   			   			
   			if(componente.dron_x==b && componente.dron_y==a){   			
   				printf("D");
   			}
   			
			else{
				printf("%c", componente.mapa[a][b]);
			}			
		}
	}
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
	componentes.cantidad_pajaros=0;
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

				// Se contabilizan posibles puntos a regar y son borrados al usuario.
				componentes.cantidad_regar=componentes.cantidad_regar+1;
				componentes.mapa[a][b]=' ';

			}
   			else if(componentes.mapa[a][b]=='P'){
   			
   				componentes.cantidad_pajaros=componentes.cantidad_pajaros+1;
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
	printf("Porcentaje zonas regadas: %d%\n",porcentaje);
	printf("Rapidez actual: %d", velocidad);

}


//Comprobacion de alguna accion a realizar(regar, sensor, recargar)
Conponentes_Mapa comprobar_accion(Conponentes_Mapa componente){


	if(componente.mapa[componente.dron_y][componente.dron_x]=='R'){
		
		if(componente.dron.estanque>=5){
			printf("Zona regada. \n");	
			componente.cantidad_regada++;	
			componente.dron.estanque=componente.dron.estanque-5;
			componente.mapa[componente.dron_y][componente.dron_x]=' ';
		}
		else{
			printf("Se requiere recargar estanque, agua insuficiente.\n");			
		}
	}
	

	else if(componente.mapa[componente.dron_y][componente.dron_x]=='A'){
	
		componente.dron.estanque=componente.dron.estanque_max;
		printf("Estanque recargado con exito\n");
	
	}
	

	else if(componente.mapa[componente.dron_y][componente.dron_x]=='W'){
	
		componente.dron.bateria=componente.dron.bateria_max;
		printf("Bateria recargada con exito\n");
	
	}
	
	if (componente.dron.bateria<30){

		printf("Bateria baja, debe recargarla en los puntos W\n");
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

	int fin=0,i;
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
					componente=comprobar_accion(componente);
						
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
					componente=comprobar_accion(componente);	
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
					componente=comprobar_accion(componente);
						
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
					componente=comprobar_accion(componente);
						
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
			mapa_Util=ubicacion_objetos(mapa_Util);
			
			dron_Util=eleccion_Dron(mapa_Util,arreglo_Drones);
			dron_Util.numero_Drones=1;
			mapa_Util.dron=dron_Util;

			mover_dron(mapa_Util);

		}

		else if(0==strcmp(eleccion,"4")){

			
		}

		else if(0==strcmp(eleccion,"5")){
			salir++;
			
		}

		else{

			printf("Vuelva pronto a SUCCESSFUL IRRIGATION \n\n");
			
		}
	}
}

	

int main(){
	char* nombres[CANTIDAD_MAPAS];
	nombres[0]="Mapa_00.in";
	nombres[1]="Mapa_01.in";
	int i;
	Dron* arreglo_Drones;
	arreglo_Drones = leer_drones("Drones.in");
	


	Conponentes_Mapa totalidad_Mapas[CANTIDAD_MAPAS];
	
	//Nombre de mapas
	
	
	for(i=0;i<CANTIDAD_MAPAS;i++){
	
		Conponentes_Mapa nuevo_mapa=leer_Mapa(nombres[i]);
		
		totalidad_Mapas[i]=nuevo_mapa;	
	}

	
	//Seleccion de mapa a usar
	/*
	Conponentes_Mapa mapa_Util;	
	mapa_Util=eleccion_mapa(totalidad_Mapas,nombres);		
	mapa_Util=ubicacion_objetos(mapa_Util);
	imprimir_mapa(mapa_Util);
	
	Dron dron_Util;
	dron_Util=eleccion_Dron(mapa_Util,arreglo_Drones);
	dron_Util.numero_Drones=1;
	mapa_Util.dron=dron_Util;
	//imprimir_Drones(dron_Util);
	

	//Movimiento del dron
	mover_dron(mapa_Util);
	
	*/
	menu(totalidad_Mapas,arreglo_Drones);

	free(arreglo_Drones);


	return 0;
}
