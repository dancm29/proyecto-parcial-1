/*-------------------------------------------------------------------------------------------
Archivo: arboles_binarios.cpp
Nombre: Allan Daniel Cruz Matias
Fecha de entrega: 17-09-2024
---------------------------------------------------------------------------------------------
Descrpcion:	Hacer un programa en C90 que incluya las siguientes opciones:
				*Inserción.
				*Eliminación.
				*Búsqueda.
				*Recorrido en Preorden.
				*Recorrido en Inorden.
				*Recorrido en Posorden.
				
			La representación del árbol debe ser necesariamente en forma gráfica:
			- En las primeras dos opciones se puede refrescar la pantalla para actualizar el estado 
			  del árbol y se pueda apreciar gráficamente el resultado.
			- En el resto de las opciones se debe incluir en la representación gráfica una animación
	  		  de cómo es que se está realizando cada uno de los procesos.
				* En la búsqueda se debe hacer algo con la representación a manera de indicar que se
				  encontró el elemento buscado.
				* En los  recorridos, se debe  hacer algo con la representación visual, para indicar 
				  qué  nodo es  el que se está  visitando.  En estas opciones,  además, se deberá ir 
				  generando una lista en pantalla con los valores de los nodos.
------------------------------------------------------------------------------------------------*/
//Codigo a ejecutar
#include <winbgim.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#define TRUE 1
#define FALSE 0
#define MAX_NODOS 100


// Declarar el arreglo global y el índice para el recorrido
int recorridoPreorden[MAX_NODOS];
int recorridoInorden[MAX_NODOS];
int recorridoPosorden[MAX_NODOS];

int indiceRecorridoPreorden = 0;
int indiceRecorridoInorden = 0;
int indiceRecorridoPosorden = 0;

typedef struct _nodo 
{
    struct _nodo *izquierdo;
    int dato;
    struct _nodo *derecho;
} tipoNodo;

struct grafico
{
    int x;
    int y;
    int radio;
};

typedef tipoNodo *pNodo;
typedef tipoNodo *Arbol;

void generarOtroArbol(Arbol a, int dato);
void mostrarArbolRecorrido(Arbol a);
void pintarElemento2(Arbol a, int x, int y, int nivel, int ventanaAncho, int color, int datoResaltar);
void agregarARecorrido(int dato, int tipo);

void preorden(Arbol *a);
void preordenAux(Arbol a, int x, int y, int nivel, int ventanaAncho);

void inorden(Arbol *a);
void inordenAux(Arbol a, int x, int y, int nivel, int ventanaAncho);

void posorden(Arbol *a);
void posordenAux(Arbol a, int x, int y, int nivel, int ventanaAncho);

void margen(int c, int f);
void gotoxy(int x, int y);
int decision();
void dibujarNodo(Arbol a, int x, int y, int nivel, int ventanaAncho);
void mostrarArbol(Arbol a);

/* Funciones con árboles: */
void Insertar(Arbol *a);
void Borrar(Arbol *a);
int Buscar(Arbol *a);
int Vacio(Arbol r);
int EsHoja(pNodo r);
int NumeroNodos(Arbol a, int* c);
int AlturaArbol(Arbol a, int* altura);
int Altura(Arbol a, int dat);

void InOrden(Arbol);

/* Funciones auxiliares: */
void Podar(Arbol *a);
void auxContador(Arbol nodo, int *c);
void auxAltura(pNodo nodo, int a, int *altura);
void pintarElemento(Arbol a, int x, int y, int nivel, int ventanaAncho, int color, int datoResaltar);


int main()
{
    Arbol ArbolInt = NULL;
    int altura;
    int nnodos;
    int c = 13, f = 2;
    int dec;

    do
    {
        margen(c, f); gotoxy(53, 5);
        printf("--|-- Arbol Binario --|--");
        dec = decision();

        switch (dec)
        {
            case 1:
                Insertar(&ArbolInt);
                break;
            case 2:
            	Borrar(&ArbolInt);
				break;
            case 3:
                Buscar(&ArbolInt);
                break;
            case 4:
                preorden(&ArbolInt);
                break;
            case 5:
            	inorden(&ArbolInt);
                break;
            case 6:
            	posorden(&ArbolInt);
            	break;
            case 7:
                system("cls");
                gotoxy(14, 8);printf("Saliendo del programa");
                break;
            default:
                printf("Opcion no valida");
        }
    } while (dec != 7);

    return 0;
}

int decision()
{
    int opc;
    gotoxy(50, 7);  printf("Insertar...................[ 1 ]");
    gotoxy(50, 8);  printf("Eliminar...................[ 2 ]");
    gotoxy(50, 9);  printf("Buscar.....................[ 3 ]");
    gotoxy(50, 10); printf("Preorden...................[ 4 ]");
    gotoxy(50, 11); printf("Inorden....................[ 5 ]");
    gotoxy(50, 12); printf("Posorden...................[ 6 ]");
    gotoxy(50,13);  printf("Salir......................[ 7 ]");
    gotoxy(50, 14); printf("Seleccione una opci%cn......[   ]",162);
    gotoxy(79, 14);scanf("%d", &opc);
    gotoxy(77, 22);system("pause");
    return opc;
}

void gotoxy(int x, int y)
{
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

/* Poda: borrar todos los nodos a partir de uno, incluido */
void Podar(Arbol *a)
{
	int c = 13, f = 2;
	
   /* Algoritmo recursivo, recorrido en postorden */
   if(!Vacio(*a)) {
      Podar(&(*a)->izquierdo); /* Podar izquierdo */
      Podar(&(*a)->derecho);   /* Podar derecho */
      free(*a);                /* Eliminar nodo */
      *a = NULL;
   }
    
}

/* Insertar un dato en el árbol ABB */
void Insertar(Arbol *a)
{
    int dat;
    int c = 13, f = 2;
    int altura = 1;     

    pNodo padre = NULL;
    pNodo actual = *a;

    system("cls");
    margen(c, f);
    gotoxy(53, 5);printf("--|-- Arbol Binario --|--");
    gotoxy(14, 8);printf("Ingrese Dato: ");scanf("%d", &dat);

    /* Buscar el dato en el árbol, manteniendo un puntero al nodo padre */
    while (!Vacio(actual) && dat != actual->dato)
    {
        padre = actual;
        if (dat < actual->dato)
            actual = actual->izquierdo;
        else if (dat > actual->dato)
            actual = actual->derecho;
    }

    /* Si se ha encontrado el elemento, regresar sin insertar */
    if(!Vacio(actual)) return;

    /* Si padre es NULL, entonces el árbol estaba vacío, el nuevo nodo será
       el nodo raiz */
    if (Vacio(padre))
    {
        *a = (Arbol)malloc(sizeof(tipoNodo));
        (*a)->dato = dat;
        (*a)->izquierdo = (*a)->derecho = NULL;
    }
    /* Si el dato es menor que el que contiene el nodo padre, lo insertamos
      en la rama izquierda */
    else if (dat < padre->dato)
    {
        actual = (Arbol)malloc(sizeof(tipoNodo));
        padre->izquierdo = actual;
        actual->dato = dat;
        actual->izquierdo = actual->derecho = NULL;
    }
    /* Si el dato es mayor que el que contiene el nodo padre, lo insertamos
      en la rama derecha */
    else if (dat > padre->dato)
    {
        actual = (Arbol)malloc(sizeof(tipoNodo));
        padre->derecho = actual;
        actual->dato = dat;
        actual->izquierdo = actual->derecho = NULL;
    }

    /* Mostrar la altura del árbol después de la inserción */
    gotoxy(14, 10);printf("Altura del arbol: %d\n", AlturaArbol(*a, &altura));

    /* Mostrar el árbol InOrden */
    gotoxy(14, 9);printf("Arbol: ");
    InOrden(*a);
    printf("\n");
    mostrarArbol(*a); // mostrar graficamente
    gotoxy(77, 22);system("pause");
	system("cls");
}

void dibujarNodo(Arbol a, int x, int y, int nivel, int ventanaAncho) 
{
    // Calcular la posición del nodo en función del nivel
    int nodoX = x;
    int nodoY;
    
    // Ajustar la posición hacia abajo solo a partir del segundo nivel
    if (nivel > 1) {
        nodoY = y + (nivel - 1) * 70; 
    } else {
        nodoY = y; // Mantener la misma posicion para el primer nivel
    }

    // Dibujar el nodo en la posición calculada
    circle(nodoX, nodoY, 20); // Dibujar el círculo (parámetros: "x" coordenadas, "y" centro y radio)
    
    // Convertir el valor del nodo a una cadena y mostrarlo en el centro del círculo
    char valorStr[10];
    itoa(a->dato, valorStr, 10);
    outtextxy(nodoX - textwidth(valorStr) / 2, nodoY - textheight(valorStr) / 2, valorStr);

    // Dibujar las líneas a los nodos izquierdo y derecho (si existen)
    if (a->izquierdo != NULL) {
        line(nodoX, nodoY + 20, nodoX - ventanaAncho / 4, nodoY + 50); // Dibujar línea al nodo izquierdo (parámetros: x1, y1, x2, y2)
        dibujarNodo(a->izquierdo, x - ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2); 
    }
    if (a->derecho != NULL) {
        line(nodoX, nodoY + 20, nodoX + ventanaAncho / 4, nodoY + 50); // Dibujar línea al nodo derecho
        dibujarNodo(a->derecho, x + ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2); 
    }
}

void mostrarArbol(Arbol a) 
{
    initwindow(1366, 768);
    dibujarNodo(a,  getmaxx() / 2, 50, 2,  getmaxx());  
    getch();
    closegraph();
}

/* Eliminar un elemento de un árbol ABB */
void Borrar(Arbol *a)
{
    int dat;
    int c = 13, f = 2;
    pNodo padre = NULL;
    pNodo actual;
    pNodo nodo;
    int aux;
    
    system("cls");
    margen(c, f);
    gotoxy(53, 5);printf("Arbol Binario");
    gotoxy(14, 8);printf("Ingrese Dato: ");scanf("%d", &dat);
    actual = *a;
   /* Mientras sea posible que el valor esté en el árbol */
   while(!Vacio(actual)) 
   {
      if(dat == actual->dato) { /* Si el valor está en el nodo actual */
         if(EsHoja(actual)) { /* Y si además es un nodo hoja: lo borramos */
            if(padre) /* Si tiene padre (no es el nodo raiz) */
               /* Anulamos el puntero que le hace referencia */
               if(padre->derecho == actual) padre->derecho = NULL;
               else if(padre->izquierdo == actual) padre->izquierdo = NULL;
            gotoxy(14, 9);printf("Se ha eliminado el Elemento: %d",dat);
            /* Mostrar el árbol InOrden */
            gotoxy(14, 10);printf("Arbol: ");
            InOrden(*a);
            printf("\n");
            
            free(actual); /* Borrar el nodo */
            actual = NULL;
            mostrarArbol(*a); // mostrar graficamente

            gotoxy(77, 22);system("pause");
            system("cls");
            return;
         }
         else { /* Si el valor está en el nodo actual, pero no es hoja */
            padre = actual;
            /* Buscar nodo más izquierdo de rama derecha */
            if(actual->derecho) {
               nodo = actual->derecho;
               while(nodo->izquierdo) {
                  padre = nodo;
                  nodo = nodo->izquierdo;
               }
            }
            /* O buscar nodo más derecho de rama izquierda */
            else {
               nodo = actual->izquierdo;
               while(nodo->derecho) {
                  padre = nodo;
                  nodo = nodo->derecho;
               }
            }
            /* Intercambiar valores de no a borrar u nodo encontrado
               y continuar, cerrando el bucle. El nodo encontrado no tiene
               por qué ser un nodo hoja, cerrando el bucle nos aseguramos
               de que sólo se eliminan nodos hoja. */
            aux = actual->dato;
            actual->dato = nodo->dato;
            nodo->dato = aux;
            actual = nodo;
         }
      }
      else { /* Todavía no hemos encontrado el valor, seguir buscándolo */
         padre = actual;
         if(dat > actual->dato) actual = actual->derecho;
         else if(dat < actual->dato) actual = actual->izquierdo;
      }

   }

    gotoxy(14, 9);printf("El Elemento %d no se encuentra en el Arbol",dat);
    gotoxy(77, 22);system("pause");
    system("cls");
}

/* Recorrido de árbol en inorden, aplicamos la función func, que tiene
   el prototipo:
   void func(int*);
*/
void InOrden(Arbol a)
{
   if(a->izquierdo) InOrden(a->izquierdo);
   printf("%d, ", a->dato);
   if(a->derecho) InOrden(a->derecho);
}

/* Buscar un valor en el árbol */
int Buscar(Arbol *a)
{
    int dat;
    int c = 13, f = 2;

    pNodo actual = *a;
    system("cls");
    margen(c, f);
    gotoxy(53, 5); printf("Arbol Binario");
    gotoxy(14, 8); printf("Ingrese Dato: ");
    scanf("%d", &dat);

    /* Todavía puede aparecer, ya que quedan nodos por mirar */
    while (!Vacio(actual))
    {
        if (dat == actual->dato)
        {
            gotoxy(14, 9);printf("El dato %d se encuentra en el arbol.\n", dat);

            /* Mostrar el árbol InOrden */
            gotoxy(14, 10); printf("Arbol: ");
            InOrden(*a);
            printf("\n");
			generarOtroArbol(*a, dat);
			

            gotoxy(77, 22);system("pause");
            system("cls");
            return TRUE; /* dato encontrado */
        }
        else if (dat < actual->dato)
            actual = actual->izquierdo; /* Seguir buscando a la izquierda */
        else if (dat > actual->dato)
            actual = actual->derecho; /* Seguir buscando a la derecha */
    }

    gotoxy(14, 9);printf("El dato %d no se encuentra en el arbol.\n", dat);
    gotoxy(77, 22);system("pause");
    system("cls");
    return FALSE; /* No está en árbol */
}
// Vista gráfica del nodo encontrado
void pintarElemento(Arbol a, int x, int y, int nivel, int ventanaAncho, int color, int datoResaltar) {
    // Calcular la posición del nodo en función del nivel
    int nodoX = x;
    int nodoY;

    // Ajusta la posición vertical del nodo dependiendo del nivel en el árbol.
    // Cada nivel adicional mueve el nodo hacia abajo.
    if (nivel > 1) {
        nodoY = y + (nivel - 1) * 70; // La distancia entre niveles es de 70 píxeles
    } else {
        nodoY = y; // Nivel 1 está en la posición inicial
    }

    // Determinar el color del texto
    int colorTexto;
    // Si el dato del nodo es el que debe resaltarse, usa el color especificado; de lo contrario, usa blanco.
    if (a->dato == datoResaltar) {
        colorTexto = color;
    } else {
        colorTexto = WHITE;
    }

    // Dibujar el nodo en la posición calculada
    circle(nodoX, nodoY, 20); // Dibuja un círculo en la posición (nodoX, nodoY) con radio 20 píxeles

    // Convertir el valor del nodo a una cadena
    char valorStr[10];
    itoa(a->dato, valorStr, 10); // Convierte el dato del nodo a una cadena en base decimal

    // Dibujar el texto en el nodo con el color determinado
    setcolor(colorTexto); // Establece el color del texto usando el valor especificado en `colorTexto`
    // Dibuja el texto centrado en el nodo
    outtextxy(nodoX - textwidth(valorStr) / 2, nodoY - textheight(valorStr) / 2, valorStr);

    // Restaurar el color del texto al valor predeterminado (opcional)
    setcolor(WHITE); // Se restablece el color del texto a blanco

    // Dibujar las líneas a los nodos izquierdo y derecho (si existen)
    if (a->izquierdo != NULL) {
        // Dibuja una línea desde el nodo actual al nodo izquierdo
        line(nodoX, nodoY + 20, nodoX - ventanaAncho / 4, nodoY + 50); // Línea al nodo izquierdo
        // Llamada recursiva para pintar el subárbol izquierdo
        pintarElemento(a->izquierdo, x - ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2, color, datoResaltar);
    }
    if (a->derecho != NULL) {
        // Dibuja una línea desde el nodo actual al nodo derecho
        line(nodoX, nodoY + 20, nodoX + ventanaAncho / 4, nodoY + 50); // Línea al nodo derecho
        // Llamada recursiva para pintar el subárbol derecho
        pintarElemento(a->derecho, x + ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2, color, datoResaltar);
    }
}

void generarOtroArbol(Arbol a, int dato){
	/* Mostrar el árbol con el nodo resaltado */
        initwindow(1366, 768);
        pintarElemento(a, getmaxx() / 2, 50, 2, getmaxx(), CYAN , dato); // Resalta el nodo encontrado en color verde
        getch();
        closegraph();
}

/* Calcular la altura del nodo que contiene el dato dat */
int Altura(Arbol a, int dat)
{
	
   int altura = 0;
   pNodo actual = a;

   /* Todavía puede aparecer, ya que quedan nodos por mirar */
   while(!Vacio(actual)) {
      if(dat == actual->dato) return altura; /* encontrado: devolver altura */
      else {
         altura++; /* Incrementamos la altura, seguimos buscando */
         if(dat < actual->dato) actual = actual->izquierdo;
         else if(dat > actual->dato) actual = actual->derecho;
      }
   }
   return -1; /* No está en árbol, devolver -1 */
}

/* Contar el número de nodos */
int NumeroNodos(Arbol a, int *contador)
{
   *contador = 0;

   auxContador(a, contador); /* Función auxiliar */
   return *contador;
}

/* Función auxiliar para contar nodos. Función recursiva de recorrido en
   preorden, el proceso es aumentar el contador */
void auxContador(Arbol nodo, int *c)
{
   (*c)++; /* Otro nodo */
   /* Continuar recorrido */
   if(nodo->izquierdo) auxContador(nodo->izquierdo, c);
   if(nodo->derecho)   auxContador(nodo->derecho, c);
}

/* Calcular la altura del árbol, que es la altura del nodo de mayor altura. */
int AlturaArbol(Arbol a, int *altura)
{
   *altura = 1;

   auxAltura(a, 1, altura); /* Función auxiliar */
   return *altura;
}

/* Función auxiliar para calcular altura. Función recursiva de recorrido en
   postorden, el proceso es actualizar la altura sólo en nodos hojas de mayor
   altura de la máxima actual */
void auxAltura(pNodo nodo, int a, int *altura)
{
   /* Recorrido postorden */
   if(nodo->izquierdo) auxAltura(nodo->izquierdo, a+1, altura);
   if(nodo->derecho)   auxAltura(nodo->derecho, a+1, altura);
   /* Proceso, si es un nodo hoja, y su altura es mayor que la actual del
      árbol, actualizamos la altura actual del árbol */
   if(EsHoja(nodo) && a > *altura) *altura = a;
}

/* Comprobar si un árbol es vacío */
int Vacio(Arbol r)
{
   return r==NULL;
}

/* Comprobar si un nodo es hoja */
int EsHoja(pNodo r)
{	
   return !r->derecho && !r->izquierdo;
}

//////// RECORRIDOS CON VISTA GRAFICA DE CADA NODO //////////
	////////////////// PREORDEN ///////////////////////////


void preorden(Arbol *a) 
{
    initwindow(1366, 768); // Crear una ventana gráfica
    mostrarArbolRecorrido(*a); // Dibuja el árbol completo en la ventana gráfica
    preordenAux(*a, getmaxx() / 2, 50, 2, getmaxx()); // Realiza el recorrido en preorden
    indiceRecorridoPreorden = 0;
    // delay(2000);// Esperar a que el usuario presione una tecla
    getch();
    closegraph(); // Cierra la ventana gráfica
}

/* Función para realizar el recorrido en preorden y pintar el nodo actual */
void preordenAux(Arbol a, int x, int y, int nivel, int ventanaAncho) 
{
	int xLista = 100;
	int yLista = 700;
	
    if (a == NULL) return;

    // Resaltar el nodo actual
    pintarElemento2(a, x, y, nivel, ventanaAncho, CYAN, a->dato);
    delay(1000); // Esperar 1 segundo para mostrar el nodo resaltado
    
    // Agregar el dato al arreglo de recorrido
    agregarARecorrido(a->dato, 1);
    // Imprimir los números del arreglo en la parte inferior de la pantalla
    for (int i = 0; i < indiceRecorridoPreorden; i++) {
        char texto[3];
        itoa( recorridoPreorden[i],texto,10);
        // sprintf(texto, "%d ", recorridoPreorden[i]);
        outtextxy(xLista + (i * 30), yLista, texto); 
    }
    
    // Restaurar el color del nodo
    pintarElemento2(a, x, y, nivel, ventanaAncho, WHITE, a->dato);
	// setcolor(WHITE);
    // Realizar el recorrido en preorden para el subárbol izquierdo
    if (a->izquierdo != NULL) 
        preordenAux(a->izquierdo, x - ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2);
    
    // Realizar el recorrido en preorden para el subárbol derecho
    if (a->derecho != NULL) 
        preordenAux(a->derecho, x + ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2);
}	//dibujarNodo(a->izquierdo, x - ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2); 

void pintarElemento2(Arbol a, int x, int y, int nivel, int ventanaAncho, int color, int datoResaltar) {
    // Calcular la posición del nodo en función del nivel
    int nodoX = x;
    int nodoY;

    // Ajusta la posición vertical del nodo dependiendo del nivel en el árbol.
    // Cada nivel adicional mueve el nodo hacia abajo.
    if (nivel > 1) {
        nodoY = y + (nivel - 1) * 70; // La distancia entre niveles es de 70 píxeles
    } else {
        nodoY = y; // Nivel 1 está en la posición inicial
    }

    int colorNodo;
    if (a->dato == datoResaltar) {
        colorNodo = color;
    } else {
        colorNodo = WHITE;
    }

    // Pintar el nodo actual en la posición calculada
    setcolor(colorNodo); // Establecer el color del nodo

    // Convertir el valor del nodo a una cadena
    char valorStr[10];
    itoa(a->dato, valorStr, 10); // Convierte el dato del nodo a una cadena en base decimal
	
    // Dibujar el texto en el nodo con el color determinado
    setcolor(colorNodo); // Establece el color del texto usando el valor especificado en `colorNodo`
    outtextxy(nodoX - textwidth(valorStr) / 2, nodoY - textheight(valorStr) / 2, valorStr);

    // Restaurar el color del texto al valor predeterminado (opcional)
    setcolor(WHITE); // Se restablece el color del texto a blanco
}

void mostrarArbolRecorrido(Arbol a) {
	dibujarNodo(a,  getmaxx() / 2, 50, 2,  getmaxx());  
}

void agregarARecorrido(int dato, int tipo) {
	
    /*if (indiceRecorrido < MAX_NODOS) {
        recorridoPreorden[indiceRecorrido++] = dato;
        //recorridoInorden[indiceRecorrido++] = dato;
    } */
    
    switch(tipo)
	{
    	case 1:
    		recorridoPreorden[indiceRecorridoPreorden++] = dato;
    		break;
    	case 2:
    		recorridoInorden[indiceRecorridoInorden++] = dato;
    		break;
    	case 3:
    		recorridoPosorden[indiceRecorridoPosorden++] = dato;
    		break;
    	
	}
}


/////////////// INORDEN //////////////////


void inorden(Arbol *a) 
{
    initwindow(1366, 768); // Crear una ventana gráfica
    mostrarArbolRecorrido(*a); // Dibuja el árbol completo en la ventana gráfica
    inordenAux(*a, getmaxx() / 2, 50, 2, getmaxx()); // Realiza el recorrido en preorden
    // delay(2000);// Esperar a que el usuario presione una tecla
    indiceRecorridoInorden = 0;
    getch();
    closegraph(); // Cierra la ventana gráfica
}

/* Función para realizar el recorrido en inorden y pintar el nodo actual */
void inordenAux(Arbol a, int x, int y, int nivel, int ventanaAncho) 
{
    int xLista = 100;
    int yLista = 700;
    
    if (a == NULL) return;

    // Realizar el recorrido en inorden para el subárbol izquierdo
    if (a->izquierdo != NULL) 
        inordenAux(a->izquierdo, x - ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2);
    
    // Resaltar el nodo actual
    pintarElemento2(a, x, y, nivel, ventanaAncho, CYAN, a->dato);
    delay(1000); // Esperar 1 segundo para mostrar el nodo resaltado
    
    // Agregar el dato al arreglo de recorrido
    agregarARecorrido(a->dato, 2);
    // Imprimir los números del arreglo en la parte inferior de la pantalla
    for (int i = 0; i < indiceRecorridoInorden; i++) {
        char texto[3];
        itoa(recorridoInorden[i], texto, 10);
        // sprintf(texto, "%d ", recorridoInorden[i]);
        outtextxy(xLista + (i * 30), yLista, texto); 
    }
    
    // Restaurar el color del nodo
    pintarElemento2(a, x, y, nivel, ventanaAncho, WHITE, a->dato);
    // setcolor(WHITE);
    
    // Realizar el recorrido en inorden para el subárbol derecho
    if (a->derecho != NULL) 
        inordenAux(a->derecho, x + ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2);
}


/////////// POSORDEN //////////////

void posorden(Arbol *a) 
{
    initwindow(1366, 768); // Crear una ventana gráfica
    mostrarArbolRecorrido(*a); // Dibuja el árbol completo en la ventana gráfica
    posordenAux(*a, getmaxx() / 2, 50, 2, getmaxx()); // Realiza el recorrido en preorden
    // delay(2000);// Esperar a que el usuario presione una tecla
    indiceRecorridoPosorden = 0;
    getch();
    closegraph(); // Cierra la ventana gráfica
}

/* Función para realizar el recorrido en posorden y pintar el nodo actual */
void posordenAux(Arbol a, int x, int y, int nivel, int ventanaAncho) 
{
    int xLista = 100;
    int yLista = 700;
    
    if (a == NULL) return;

    // Realizar el recorrido en posorden para el subárbol izquierdo
    if (a->izquierdo != NULL) 
        posordenAux(a->izquierdo, x - ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2);
    
    // Realizar el recorrido en posorden para el subárbol derecho
    if (a->derecho != NULL) 
        posordenAux(a->derecho, x + ventanaAncho / 4, y, nivel + 1, ventanaAncho / 2);
    
    // Resaltar el nodo actual
    pintarElemento2(a, x, y, nivel, ventanaAncho, CYAN, a->dato);
    delay(1000); // Esperar 1 segundo para mostrar el nodo resaltado
    
    // Agregar el dato al arreglo de recorrido
    agregarARecorrido(a->dato, 3); // 3 indica posorden
    
    // Imprimir los números del arreglo en la parte inferior de la pantalla
    for (int i = 0; i < indiceRecorridoPosorden; i++) {
        char texto[3];
        itoa(recorridoPosorden[i], texto, 10);
        // sprintf(texto, "%d ", recorridoPosorden[i]);
        outtextxy(xLista + (i * 30), yLista, texto); 
    }
    
    // Restaurar el color del nodo
    pintarElemento2(a, x, y, nivel, ventanaAncho, WHITE, a->dato);
}

void margen(int c, int f) 
{
    int i, p, q, j, m = 40, l = 4;

    gotoxy(c, f); printf("%c", 201);

    for (i = 0; i < 105; i++) {
        gotoxy(c + i + 1, f); printf("%c", 205);
        for (j = 0; j < 20; j++) {
            gotoxy(c, f + 1 + j); printf("%c", 186);

            gotoxy(c + 105, f + 1 + j);printf("%c", 186);
        }
        gotoxy(c + i + 1, f + 21); printf("%c", 205);
    }

    gotoxy(c, f + 21); printf("%c", 200);

    gotoxy(c + 105, f); printf("%c", 187);

    gotoxy(c + 105, f + 21); printf("%c", 188);

    gotoxy(m, l); printf("%c", 201);

    for (p = 0; p < 50; p++) {
        gotoxy(m + p + 1, l); printf("%c", 205);
        for (q = 0; q < 2; q++) {
            gotoxy(m, l + 1 + q); printf("%c", 186);

            gotoxy(m + 50, l + 1 + q); printf("%c", 186);
        }
        gotoxy(m + p + 1, l + 2); printf("%c", 205);
    }

    gotoxy(m, l + 2); printf("%c", 200);

    gotoxy(m + 50, f + 2); printf("%c", 187);

    gotoxy(m + 50, f + 4); printf("%c", 188);
}
























