#include "fuzzyElec.h"
#include <stdio.h>
typedef int bool;
#define TRUE  1
#define FALSE 0

//------------------------VARIAVEIS----------------------//
struct entradaA distancia;
struct entradaB energia;
struct implicacao agregacaoB, agregacaoA;
bool flagConjuntoSaida[9];
uint8_t limitesConjuntoSaida[9] = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
//------------------------------------------------------//

//------------------------FUNCOES----------------------//
float triangulo_elec(float x, float e, float f, float g);
float trapezioDescrescente_elec(float x, float c, float d);
float trapezioCrescente_elec(float x, float a, float b);
float minimo(float a, float b);
uint8_t desfuzzyficar_elec();

//---------------------FUZZYFICAÇÃO----------------------//
void fuzzificar_elec(float d, float n) {
	// A chamada das funções de conjuntos é ( valor crisp, valor do ponto x, valor do ponto y )
	distancia.grauP = trapezioDescrescente_elec(d, 0.5, 1.8);	// São 2 pontos pois é um trapézio
	distancia.grauM = triangulo_elec(d, 0.0, 1.8, 3.6);		// São 3 pontos pois é um triangulo
	distancia.grauL = trapezioCrescente_elec(d, 1.8, 4.1);

	energia.grauB = trapezioDescrescente_elec(n, 0.1, 0.5);
	energia.grauM = triangulo_elec(n, 0.1, 0.5, 0.9);
	energia.grauA = trapezioCrescente_elec(n, 0.5, 0.9);

	printf("A p: ");
	printfloat(distancia.grauP);
	printf("A m: ");
	printfloat(distancia.grauM);
	printf("A L: ");
	printfloat(distancia.grauL);
	//printf("-----------\n");
	printf("B a: ");
	printfloat(energia.grauA);
	printf("B m: ");
	printfloat(energia.grauM);
	printf("B b: ");
	printfloat(energia.grauB);

}

//------------------------INFERENCIA-------------------------//
void inferencia_elec() {
	float aux = 0;
	float ultimoAux = 0;
	uint8_t i;
	// Inicializando a struct
	agregacaoB.Mpequena = 0;
	agregacaoB.Pequena = 0;
	agregacaoB.Ppequena = 0;
	agregacaoB.Pmedia = 0;
	agregacaoB.Media = 0;
	agregacaoB.Mmedia = 0;
	agregacaoB.Pgrande = 0;
	agregacaoB.Grande = 0;
	agregacaoB.Mgrande = 0;

	agregacaoA.Mpequena = 0;
	agregacaoA.Pequena = 0;
	agregacaoA.Ppequena = 0;
	agregacaoA.Pmedia = 0;
	agregacaoA.Media = 0;
	agregacaoA.Mmedia = 0;
	agregacaoA.Pgrande = 0;
	agregacaoA.Grande = 0;
	agregacaoA.Mgrande = 0;

	// Acionamento das regras de acordo com os conjuntos acionados na fuzzyficação
	for (i = 0; i < 9; i++)
		flagConjuntoSaida[i] = FALSE;

	//p b pp
	aux = minimo(distancia.grauP, energia.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Ppequena = limitesConjuntoSaida[2] * aux;
		agregacaoA.Ppequena = aux;
		ultimoAux = aux;
		printf("7\n");
	}
	if ((agregacaoA.Ppequena != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[2] = TRUE;

	}
	//------------------------------------------------------------ regra 7

	//m b p
	aux = minimo(distancia.grauM, energia.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Pequena = limitesConjuntoSaida[1] * aux;
		agregacaoA.Pequena = aux;
		ultimoAux = aux;
		printf("8\n");
	}
	if ((agregacaoA.Pequena != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[1] = TRUE;

	}
	//----------------------------------------------------------- regra 8

	//L b	mp
	aux = minimo(distancia.grauL, energia.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Mpequena = limitesConjuntoSaida[0] * aux;
		agregacaoA.Mpequena = aux;
		ultimoAux = aux;
		printf("9\n");
	}
	if ((agregacaoA.Mpequena != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[0] = TRUE;

	}
	//----------------------------------------------------------- regra 9

	//p m mm
	aux = minimo(distancia.grauP, energia.grauM);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Mmedia = limitesConjuntoSaida[5] * aux;
		agregacaoA.Mmedia = aux;
		ultimoAux = aux;
		printf("4\n");
	}
	if ((agregacaoA.Mmedia != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[5] = TRUE;

	}
	//----------------------------------------------------------- regra 4
	//M	M	Media
	aux = minimo(distancia.grauM, energia.grauM);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Media = limitesConjuntoSaida[4] * aux;
		agregacaoA.Media = aux;
//		printf("agregB: \n");
//		printfloat(agregacaoB.Media);
//		printf("agregA: \n");
//		printfloat(agregacaoA.Media);
		ultimoAux = aux;
		printf("5\n");

	}
	if ((agregacaoA.Media != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[4] = TRUE;

	}
	//----------------------------------------------------------- regra 5

	//l m pm
	aux = minimo(distancia.grauL, energia.grauM);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Pmedia = limitesConjuntoSaida[3] * aux;
		agregacaoA.Pmedia = aux;
		ultimoAux = aux;
		printf("6\n");
	}
	if ((agregacaoA.Pmedia != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[3] = TRUE;

	}
	//----------------------------------------------------------- regra 6

	//p a mg
	aux = minimo(distancia.grauP, energia.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Mgrande = limitesConjuntoSaida[8] * aux;
		agregacaoA.Mgrande = aux;
		ultimoAux = aux;
		printf("1\n");
	}
	if ((agregacaoA.Mgrande != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[8] = TRUE;

	}
	//----------------------------------------------------------- regra 1

	//m a g
	aux = minimo(distancia.grauM, energia.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Grande = limitesConjuntoSaida[7] * aux;
		agregacaoA.Grande = aux;
		ultimoAux = aux;

		printf("2\n");
	}
	if ((agregacaoA.Grande != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[7] = TRUE;

	}
	//----------------------------------------------------------- regra 2

	//l a pg
	aux = minimo(distancia.grauL, energia.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		agregacaoB.Pgrande = limitesConjuntoSaida[6] * aux;
		agregacaoA.Pgrande = aux;
		ultimoAux = aux;
		printf("3\n");

	}
	if ((agregacaoA.Pgrande != 0)) {
		ultimoAux = 0;
		flagConjuntoSaida[6] = TRUE;
	}
	//----------------------------------------------------------- regra 3

}

//----------------- Funções de Acessórios --------------------//
//------------------------------------------------------------//

float triangulo_elec(float x, float e, float f, float g) {
	float res = 0;

	if (x <= e)
		return res = 0;
	if ((e < x) && (x <= f))
		return res = 1 - (f - x) / (f - e);
	if ((f < x) && (x <= g))
		return res = (g - x) / (g - f);
	if (x > g)
		return res = 0;

	return res;
}

//float triangulo(float x, float e, float f, float g) {
//	float res = 0;
//
//	if (x <= e)
//		return res = 0;
//	if ((x > e) && (x < f))
//		return res = 1 - (f - x) / (f - e);
//	if (x = f)
//		return res = 1;
//	if ((x > f) && (x < g))
//		return res = (g - x) / (g - f);
//	if (x >= g)
//		return res = 0;
//
//	return res;
//}

float trapezioDescrescente_elec(float x, float c, float d) {
	float res = 0;

	if (x <= c)
		return res = 1;
	if ((x > c) && (x <= d))
		return res = (d - x) / (d - c);
	if (x > d)
		return res = 0;

	return res;
}

float trapezioCrescente_elec(float x, float a, float b) {
	float res = 0;

	if (x < a)
		return res = 0;
	if ((x >= a) && (x < b))
		return res = 1 - (b - x) / (b - a);
	if (x >= b)
		return res = 1;

	return res;
}

float minimo(float a, float b) {
	if (a <= b)
		return a;
	else
		return b;
}

//----------------------------------------------------------//

//----------------------DEFUZZIFICAÇÃO----------------------//
uint8_t desfuzzyficar_elec() {
	uint8_t i;
	float num_elec = 0.0;
	float dem_elec = 0.0;

	for (i = 0; i < 9; i++)
		if (flagConjuntoSaida[i] == TRUE) {

			if (i == 6)
			{
				num_elec += agregacaoB.Pgrande;
				dem_elec += agregacaoA.Pgrande;
				printf("1:\n");
				printfloat(agregacaoB.Pgrande);
				printf("2\n:");
				printfloat(agregacaoA.Pgrande);
				printf("num:\n");
				printfloat(num_elec);
				printf("dem\n:");
				printfloat(dem_elec);
				printf("D perto + E baixa = Pouco pequena\n");
				//printf("D media + E alta = Pouco Grande\n");
			}
			if (i == 7)
			{
				num_elec += agregacaoB.Grande;
				dem_elec += agregacaoA.Grande;

				printf("D perto + E media = muito media\n");

			}
			if (i == 8)
			{

				num_elec += agregacaoB.Mgrande;
				dem_elec += agregacaoA.Mgrande;

				printf("D perto + E alta = Muito Grande\n");

			}
			if (i == 2)
			{

				num_elec += agregacaoB.Ppequena;
				dem_elec += agregacaoA.Ppequena;

				//printf("D media + E baixa = Pouco Pequena\n");
				printf("D longe + E alta = Pouco grande\n");

			}
			if (i == 1)
			{

				num_elec += agregacaoB.Pequena;
				dem_elec += agregacaoA.Pequena;

				printf("D longe + E media = pouco media\n");

			}
			if (i == 0)
			{

				num_elec += agregacaoB.Mpequena;
				dem_elec += agregacaoA.Mpequena;

				printf("D longe + E baixa = Muito Pequena\n");

			}
			if (i == 3)
			{

				num_elec += agregacaoB.Pmedia;
				dem_elec += agregacaoA.Pmedia;

				printf("D media + E baixa = pequena\n");
				//printfloat(agregacaoB.Media );
				//printfloat(agregacaoA.Media );

			}
			if (i == 4)
			{

				num_elec += agregacaoB.Media;
				dem_elec += agregacaoA.Media;

				printf("D media + E media = Media\n");
				//printfloat(agregacaoB.Media );
				//printfloat(agregacaoA.Media );

			}
			if (i == 5)
			{

				num_elec += agregacaoB.Mmedia;
				dem_elec += agregacaoA.Mmedia;

				printf("D media + E alta = grande\n");
				//printfloat(agregacaoB.Media );
				//printfloat(agregacaoA.Media );

			}
		}
	//printfloat(num/dem);

	return (num_elec/dem_elec);

}
