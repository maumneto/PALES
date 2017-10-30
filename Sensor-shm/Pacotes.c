#include <stdio.h>
#include "Pacotes.h"



PacoteSink novo_pacote()
{
	PacoteSink pacote=(PacoteSink)malloc(sizeof(struct __pacoteSink));
	bzero(pacote, sizeof(struct __pacoteSink));
	return pacote;
}

void set_conteudo(PacoteSink pacote,uint8_t valor)
{
	if(pacote==NULL) return;
	pacote->round=valor;
}


void dump_pacote(PacoteSink pacote){
	if(pacote==NULL) {printf("Nada\n"); return;};
	printf("Round==%d\n",pacote->round);
}


void delete_pacote(PacoteSink pacote)
{
	free(pacote);
}

DataMsg novo_pacoteMSG()
{
	DataMsg pacote=(DataMsg)malloc(sizeof(struct __dataMsg));
	bzero(pacote, sizeof(struct __dataMsg));
	return pacote;
}

void set_conteudoMSG(DataMsg pacote,uint16_t valor1,uint16_t valor2,uint16_t valor3,uint16_t valor4,uint16_t valor5)
{
	if(pacote==NULL) return;
	pacote->omega1=valor1;
	pacote->omega2=valor2;
	pacote->omega3=valor3;
	pacote->omega4=valor4;
	pacote->omega5= valor5;
}


void dump_pacoteMSG(DataMsg pacote){
	if(pacote==NULL) {printf("Nada\n"); return;};
	printf("Omega1==%d\n",pacote->omega1);
	printf("Omega2==%d\n",pacote->omega2);
	printf("Omega3==%d\n",pacote->omega3);
	printf("Omega4==%d\n",pacote->omega4);
	printf("Omega5==%d\n",pacote->omega5);

}


void delete_pacoteMSG(DataMsg pacote)
{
	free(pacote);
}
