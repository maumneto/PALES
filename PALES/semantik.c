#include "semantik.h"
#include <stdio.h>
typedef int bool;
#define TRUE  1
#define FALSE 0
#define LD 0.54

//------------------------VARIAVEIS----------------------//
struct variavel_fuzzy_t d_coeficiente, n_msg, mostD, mostNM;
struct variavel_fuzzy_t2 semanticoN, semanticoD;
bool centroideSemNeighDecision[3];
uint8_t centroSemNeighDecision[3] = { 22, 50, 79 };
static float countDAlto, countDBaixo, countMAlto, countMBaixo;
static uint8_t countNmsg;
static float auxMostBaixoD, auxMostAltoD, auxMostAltoN, auxMostBaixoN;
//------------------------------------------------------//

//------------------------FUNCOES----------------------//
float triangulo(float x, float e, float f, float g);
float trapezioDescrescente(float x, float c, float d);
float trapezioCrescente(float x, float a, float b);
void printfloat(float x);
float min(float a, float b, float c, float d);
float min2(float a, float b);
void imprimir();
PData gera_PData3(uint8_t CH, uint8_t no);
int iniciar_distribuido(float altoD, float baixoD, float altoM, float baixoM);
void somatorio_d(float alto, float baixo);
void somatorio_m(float alto, float baixo);
void fuzzyficarMostD(float b, float a);
void fuzzyficarMostNM(float b, float a);
void inferenciaDadosDistribuida();
void fuzzificar(float d, float n);
uint8_t desfuzzyficarSemNeighborDecision();
//------------------------------------------------------//

PData gera_PData3(uint8_t CH, uint8_t no) {
	//PData *pd = malloc(sizeof(PData));
	PData pd;
	pd.dCoeffGrauA = d_coeficiente.grauA;
	pd.dCoeffGrauB = d_coeficiente.grauB;
	pd.nMessagesGrauA = n_msg.grauA;
	pd.nMessagesGrauB = n_msg.grauB;
	pd.originCH = CH;
	pd.originSensor = no;
	return pd;
}

int iniciar_distribuido(float altoD, float baixoD, float altoM, float baixoM) {
	countNmsg++;
	somatorio_d(altoD, baixoD);
	somatorio_m(altoM, baixoM);
	auxMostAltoD = countDAlto / countNmsg;
	auxMostBaixoD = countDBaixo / countNmsg;
	auxMostAltoN = countMAlto / countNmsg;
	auxMostBaixoN = countMBaixo / countNmsg;
	fuzzyficarMostD(auxMostBaixoD, auxMostAltoD);
	fuzzyficarMostNM(auxMostBaixoN, auxMostAltoN);

	/*printf("***************local******************");
	 printfloat(d_coeficiente.grauA);
	 printfloat(n_msg.grauA);
	 printf("Alto D:\n");
	 printfloat(mostD.grauA);
	 printf("Baixo D:\n");
	 printfloat(mostD.grauB);
	 printf("Alto NM:\n");
	 printfloat(mostNM.grauA);
	 printf("Baixo NM:\n");
	 printfloat(mostNM.grauB);*/
	inferenciaDadosDistribuida();
	return desfuzzyficarSemNeighborDecision();
}
void somatorio_d(float alto, float baixo) {
	countDBaixo = countDBaixo + baixo;
	countDAlto = countDAlto + alto;
}
void somatorio_m(float alto, float baixo) {
	countMAlto += alto;
	countMBaixo += baixo;
}

void fuzzyficarMostD(float b, float a) {
	if (b <= 0.3)
		mostD.grauB = 0;
	if ((b > 0.3) && (b < 0.8))
		mostD.grauB = 2 * b - 0.6;
	if (b >= 0.8)
		mostD.grauB = 1;

	if (a <= 0.3)
		mostD.grauA = 0;
	if ((a > 0.3) && (a < 0.8))
		mostD.grauA = 2 * a - 0.6;
	if (a >= 0.8)
		mostD.grauA = 1;

}

void fuzzyficarMostNM(float b, float a) {
	if (b <= 0.3)
		mostNM.grauB = 0;
	if ((b > 0.3) && (b < 0.8))
		mostNM.grauB = 2 * b - 0.6;
	if (b >= 0.8)
		mostNM.grauB = 1;

	if (a <= 0.3)
		mostNM.grauA = 0;
	if ((a > 0.3) && (a < 0.8))
		mostNM.grauA = 2 * a - 0.6;
	if (a >= 0.8)
		mostNM.grauA = 1;

}
void fuzzificar(float d, float n) {
	d_coeficiente.grauB = trapezioDescrescente(d, 0.52, LD);
	d_coeficiente.grauA = trapezioCrescente(d, 0.52, LD);
	n_msg.grauB = trapezioDescrescente(n, 1, 3);
	n_msg.grauA = trapezioCrescente(n, 1, 3);

}
void inferencia_local() {
	float aux = 0;
	float ultimoAux = 0;
	uint8_t i;
	semanticoN.grauB = 0;
	semanticoN.grauM = 0;
	semanticoN.grauA = 0;
	semanticoD.grauB = 0;
	semanticoD.grauM = 0;
	semanticoD.grauA = 0;
	for (i = 0; i < 3; i++)
		centroideSemNeighDecision[i] = FALSE;

//B	B	Baixa
	aux = min2(d_coeficiente.grauB, n_msg.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauB = centroSemNeighDecision[0] * aux;
		semanticoD.grauB = aux;
		ultimoAux = aux;

	}

	//B	A	Baixa (Média?)
	aux = min2(d_coeficiente.grauB, n_msg.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauB = centroSemNeighDecision[0] * aux;
		semanticoD.grauB = aux;
		ultimoAux = aux;

	}

	if ((semanticoD.grauB != 0)) {
		ultimoAux = 0;
		centroideSemNeighDecision[0] = TRUE;

	}

	//A	B	Alta (Média?)
	aux = min2(d_coeficiente.grauA, n_msg.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;

	}

	//A	A	Alta
	aux = min2(d_coeficiente.grauA, n_msg.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;

	}

	if ((semanticoD.grauA != 0)) {
		ultimoAux = 0;
		centroideSemNeighDecision[2] = TRUE;
	}
}

void inferenciaDadosDistribuida() {
	uint8_t i;
	float aux = 0;
	float ultimoAux = 0;

	semanticoN.grauB = 0;
	semanticoN.grauM = 0;
	semanticoN.grauA = 0;

	semanticoD.grauB = 0;
	semanticoD.grauM = 0;
	semanticoD.grauA = 0;

	for (i = 0; i < 3; i++)
		centroideSemNeighDecision[i] = FALSE;

	//*****regras para a decisão de vizinhança semantica (com most)

	//B	B B B	Baixa
	aux = min(d_coeficiente.grauB, n_msg.grauB, mostD.grauB, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauB = centroSemNeighDecision[0] * aux;
		semanticoD.grauB = aux;
		ultimoAux = aux;
		//printf("A\n");
	}

	//B	B B A	Baixa
	aux = min(d_coeficiente.grauB, n_msg.grauB, mostD.grauB, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauB = centroSemNeighDecision[0] * aux;
		semanticoD.grauB = aux;
		ultimoAux = aux;
		//printf("B\n");
	}

	//B	B A B 	Baixa
	aux = min(d_coeficiente.grauB, n_msg.grauB, mostD.grauA, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauB = centroSemNeighDecision[0] * aux;
		semanticoD.grauB = aux;
		ultimoAux = aux;
		//printf("C\n");
	}

	//B	A B B 	Baixa
	aux = min(d_coeficiente.grauB, n_msg.grauA, mostD.grauB, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauB = centroSemNeighDecision[0] * aux;
		semanticoD.grauB = aux;
		ultimoAux = aux;
		//printf("D\n");
	}

	if ((semanticoD.grauB != 0)) {
		ultimoAux = 0;
		centroideSemNeighDecision[0] = TRUE;
	}

	//B	B A A 	Média
	aux = min(d_coeficiente.grauB, n_msg.grauB, mostD.grauA, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauM = centroSemNeighDecision[1] * aux;
		semanticoD.grauM = aux;
		ultimoAux = aux;
		//printf("E\n");
	}

	//B	A A B 	Média
	aux = min(d_coeficiente.grauB, n_msg.grauA, mostD.grauA, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauM = centroSemNeighDecision[1] * aux;
		semanticoD.grauM = aux;
		ultimoAux = aux;
		//printf("F\n");
	}

	//B	A A A 	Média
	aux = min(d_coeficiente.grauB, n_msg.grauA, mostD.grauA, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauM = centroSemNeighDecision[1] * aux;
		semanticoD.grauM = aux;
		ultimoAux = aux;
		//printf("G\n");
	}

	//A	B B B 	Média
	aux = min(d_coeficiente.grauA, n_msg.grauB, mostD.grauB, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauM = centroSemNeighDecision[1] * aux;
		semanticoD.grauM = aux;
		ultimoAux = aux;
		//printf("H\n");
	}

	//A	B B A 	Média
	aux = min(d_coeficiente.grauA, n_msg.grauB, mostD.grauB, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauM = centroSemNeighDecision[1] * aux;
		semanticoD.grauM = aux;
		ultimoAux = aux;
		//printf("I\n");
	}

	if ((semanticoD.grauM != 0)) {
		ultimoAux = 0;
		centroideSemNeighDecision[1] = TRUE;
	}

	//A	B A B 	Alta
	aux = min(d_coeficiente.grauA, n_msg.grauB, mostD.grauA, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;
		//printf("J\n");
	}

	//A	B A A 	Alta
	aux = min(d_coeficiente.grauA, n_msg.grauB, mostD.grauA, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;
		//printf("L\n");
	}

	//A	A B B 	Alta
	aux = min(d_coeficiente.grauA, n_msg.grauA, mostD.grauB, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;
		//printf("M\n");
	}

	//A	A B A 	Alta
	aux = min(d_coeficiente.grauA, n_msg.grauA, mostD.grauB, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;
		//printf("N\n");
	}

	//A	A A B 	Alta
	aux = min(d_coeficiente.grauA, n_msg.grauA, mostD.grauA, mostNM.grauB);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;
		//printf("N A\n");

	}

	//A	A A A 	Alta
	aux = min(d_coeficiente.grauA, n_msg.grauA, mostD.grauA, mostNM.grauA);
	if ((aux != 0) && (aux > ultimoAux)) {
		semanticoN.grauA = centroSemNeighDecision[2] * aux;
		semanticoD.grauA = aux;
		ultimoAux = aux;
		/*printf("A N\n");
		printfloat(centroSemNeighDecision[2] * aux);
		printf("A D\n");
		printfloat(aux);*/
	}

	if ((semanticoD.grauA != 0)) {
		ultimoAux = 0;
		centroideSemNeighDecision[2] = TRUE;
	}
}

float triangulo(float x, float e, float f, float g) {
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

float trapezioDescrescente(float x, float c, float d) {
	float res = 0;

	if (x <= c)
		return res = 1;
	if ((x > c) && (x <= d))
		return res = (d - x) / (d - c);
	if (x > d)
		return res = 0;

	return res;
}

float trapezioCrescente(float x, float a, float b) {
	float res = 0;

	if (x <= a)
		return res = 0;
	if ((x > a) && (x <= b))
		return res = 1 - (b - x) / (b - a);
	if (x > b)
		return res = 1;

	return res;
}

void printfloat(float x) {
	int integerPart = (int) x;

	int decimalPart = (x - integerPart) * 100;
	//if (decimalPart < 0)
	//decimalPart = decimalPart * (-1);
	printf("%d.%d\n", integerPart, decimalPart);
}

float min2(float a, float b) {
	if (a <= b)
		return a;
	else
		return b;
}

float min(float a, float b, float c, float d) {
	if ((a <= b) && (a <= c) && (a <= d))
		return a;
	if ((b <= a) && (b <= c) && (b <= d))
		return b;
	if ((c <= a) && (c <= b) && (c <= d))
		return c;

	return d;
}

void imprimir() {
	printfloat(d_coeficiente.grauA);
	printfloat(n_msg.grauA);
}

uint8_t desfuzzyficarSemNeighborDecision() {
	uint8_t i;
	static float num = 0;
	static float dem = 0;

	for (i = 0; i < 3; i++)
		if (centroideSemNeighDecision[i] == TRUE) {

			if (i == 0)
			{
				num += semanticoN.grauB;
				dem += semanticoD.grauB;

			}
			if (i == 1)
			{
				num += semanticoN.grauM;
				dem += semanticoD.grauM;

			}
			if (i == 2)
			{


				num += semanticoN.grauA;
				dem += semanticoD.grauA;


			}
		}

	return num/dem;

}
