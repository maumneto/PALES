/*
 * structs utilizadas pelo semantik

 */
#include <stdint.h>
struct variavel_fuzzy_t
{
	float grauA;
	float grauB;
};


struct variavel_fuzzy_t2
{
	float grauA;
	float grauB;
	float grauM;
};

typedef struct PData3
{ //FuzzyficacaoMsg - CH to CH
  uint16_t originCH; //CH que originou a mensagem
  uint16_t originSensor; //sensor que originou a mensagem fuzzyficada
  float dCoeffGrauB;//grau de pertinencia do baixo
  float dCoeffGrauA;//grau de pertinencia do alto
  float nMessagesGrauB;//grau de pertinencia do baixo
  float nMessagesGrauA;//grau de pertinencia do alto
}PData;

typedef struct medicao //datamsg
{
	float w1,w2,w3,w4,w5;
}Medicao;

typedef struct reportmsg
{
	float coeficientes;
	uint8_t qtd_vizinhos;
}ReportMsg;


