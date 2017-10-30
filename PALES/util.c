#include "util.h"
#include <stdio.h>

ListaMembros inicializa_listamembro (void)
{
   return NULL;
}

ListaMembros insere_listamembro (ListaMembros lista, uint8_t id, float coeficiente)
{
	ListaMembros novo = (ListaMembros) malloc(sizeof(struct lista_membros));
	novo->id = id;
	novo->coeficiente = coeficiente;
	novo->prox = lista;
	return novo;
}

void imprime_listamembro (ListaMembros l)
{
	ListaMembros p;   /* variável auxiliar para percorrer a lista */
	for (p = l; p != NULL; p = p->prox)
	{
	      printf("ID = %d\n", p->id);
	      printfloat(p->coeficiente);
	}
}

ListaMembros contem_listamembro(ListaMembros l, uint8_t id)
{
	ListaMembros p;
	for (p = l; p != NULL; p = p->prox)
	{
		if(p->id == id)
			return p;
	}
	return NULL;
}
ListaMembros get_item(ListaMembros l,uint8_t index)
{
	ListaMembros p;
	uint8_t contador = 0;
	for (p = l; p != NULL; p = p->prox)
	{
			if(contador == index)
				return p;
	}
	return NULL;
}


//--------------------------lista vizinhos semanticos ----------------------------------------//
ListaVizinhosSemanticos inicializa_vizinhossematicos()
{
	return NULL;
}
ListaVizinhosSemanticos insere_vizinhossematicos (ListaVizinhosSemanticos lista, uint8_t CH,uint8_t no)
{
	ListaVizinhosSemanticos novo = (ListaVizinhosSemanticos) malloc(sizeof(struct lista_vizinhossematicos));
	novo->id = no;
	novo->CH = CH;
	novo->prox = lista;
	return novo;
}

void imprime_listavizinhossemanticos (ListaVizinhosSemanticos l)
{
	ListaVizinhosSemanticos p;
	for (p = l; p != NULL; p = p->prox)
	{
		printf("No = %d\ CH = %d\n", p->id,p->CH);
	}
}

ListaVizinhosSemanticos get_vizinhossemantico(ListaVizinhosSemanticos l, uint8_t CH,uint8_t no)
{
	ListaVizinhosSemanticos p;
	for (p = l; p != NULL; p = p->prox)
	{
		if(p->CH == CH && p->id == no)
			return p;
	}
	return NULL;
}

uint8_t possui_CH(ListaVizinhosSemanticos l,uint8_t CH)
{
	ListaVizinhosSemanticos p;
	for (p = l; p != NULL; p = p->prox)
	{
			if(p->CH == CH)
				return 1;
	}
	return 0;
}
