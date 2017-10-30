#include <stdint.h>
//-------------------------LISTA DE MEMBROS------------------------------------//
struct lista_membros
{
   uint8_t id;
   float coeficiente;
   struct lista_membros *prox;
};
typedef struct lista_membros *ListaMembros;
ListaMembros inicializa_listamembro (void);
ListaMembros insere_listamembro (ListaMembros, uint8_t,float);
void imprime_listamembro (ListaMembros);
ListaMembros contem_listamembro(ListaMembros,uint8_t);
ListaMembros get_item(ListaMembros,uint8_t);


//--------------------------------------------------------------------------//

//-------------------------LISTA DE VIZINHOS SEMANTICOS------------------------------------//
struct lista_vizinhossematicos
{
	uint8_t CH;
	uint8_t id;
	struct lista_vizinhossematicos *prox;
};
typedef struct lista_vizinhossematicos *ListaVizinhosSemanticos;
ListaVizinhosSemanticos inicializa_vizinhossematicos (void);
ListaVizinhosSemanticos insere_vizinhossematicos (ListaVizinhosSemanticos, uint8_t,uint8_t);
void imprime_listavizinhossemanticos (ListaVizinhosSemanticos);
ListaVizinhosSemanticos get_vizinhossemantico(ListaVizinhosSemanticos, uint8_t,uint8_t);
uint8_t possui_CH(ListaVizinhosSemanticos,uint8_t);


