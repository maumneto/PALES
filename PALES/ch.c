#include "contiki.h"
#include "net/rime.h"
#include "semantik.h"
#include "util.h"
#include "powertrace.h"
#define MAX_MEMBROS 3
#define MAX_MEMBROS_SEMANTICOS 9


PROCESS(ch_process, "Processo do CH");
PROCESS(eleicao_process, "Processo da eleicao");
PROCESS(anuncio_vizinho_process, "Processo de anuncio de vizinhos");
PROCESS(enviar_coleta_process, "Processo de envia coleta");
AUTOSTART_PROCESSES(&ch_process,&el eicao_process,&anuncio_vizinho_process,&enviar_coleta_process);


//----------------------------VARIAVEIS---------------------------//
static Medicao med_saudavel,delta_med;
static process_event_t iniciar_fuzzy; // evento que dispara para iniciar processo de fuzzificacao
static process_event_t iniciar_eleicao; // evento que dispara inicio da eleicao semantica
static process_event_t anuncio_vizinho; //evento que troca de informacoes sobre vizinhanca semantica entre os CHs
static uint8_t vizinhos[2];
static uint8_t msg;
static uint8_t flag_local; //flag que indica se oCH ja fez a inferencia local
static ListaMembros lista;
static ListaVizinhosSemanticos lst_vizinhos_semanticos;
static uint8_t destino;
static float coeficientes;
static float total_coeficiente;
static uint16_t taxa_transmissao;
static uint8_t flag_eleicao;
static uint8_t eh_coletor;
static int cont;
//--------------------------------------------------------------//
uint8_t vetor[9];
//----------------------------FUNCOES---------------------------//

void inicializa();
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from);
static void recv_uc2(struct unicast_conn *c, const rimeaddr_t *from);
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
//--------------------------------------------------------------//

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;


//**unicast**//
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc };
static struct unicast_conn uc;

//**unicast2**//
static void recv_uc2(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks2 = { recv_uc2 };
static struct unicast_conn uc2;

PROCESS_THREAD(ch_process, ev, data)
{
	static struct etimer et;
	PROCESS_EXITHANDLER(unicast_close(&uc));
	PROCESS_BEGIN()
	inicializa();
	powertrace_start(CLOCK_SECOND * 5);
	unicast_open(&uc, 146, &unicast_callbacks);
	unicast_open(&uc2, 156, &unicast_callbacks2);
	broadcast_open(&broadcast, 129, &broadcast_call);
	etimer_set(&et, CLOCK_SECOND *1);
	iniciar_fuzzy = process_alloc_event();
	iniciar_eleicao = process_alloc_event();
	anuncio_vizinho = process_alloc_event();
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	while(1)
	{
		PROCESS_WAIT_EVENT();
		if(ev == iniciar_fuzzy && flag_eleicao == 0)
		{

			fuzzificar(contem_listamembro(lista,data)->coeficiente,msg); //(0.54 e 1)
			inferencia_local();
			flag_local = 1;
			PData *pacote =  gera_PData3(rimeaddr_node_addr.u8[0],data);
			uint8_t i;
			rimeaddr_t endereco;
			for(i = 0; i< 2; i++)
			{
				if(vizinhos[i] == 0)break;
				endereco.u8[0] = vizinhos[i];
				endereco.u8[1] = 0;
				packetbuf_copyfrom(pacote,sizeof(PData));
				//printf("Enviou msg de fuzzificacao\n");
				unicast_send(&uc, &endereco);

			}
		}

	}
	PROCESS_END();
}
PROCESS_THREAD(eleicao_process, ev, data)
{
	static struct etimer et;

	PROCESS_BEGIN()
	while(1 && flag_eleicao == 0)
	{
		etimer_set(&et, CLOCK_SECOND *25);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		imprime_listavizinhossemanticos(lst_vizinhos_semanticos);
		//printf("teste\n");
		//etimer_set(&et, CLOCK_SECOND *60);

		//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if(lst_vizinhos_semanticos == NULL) break;
		ListaVizinhosSemanticos p,p2;
		uint8_t valor,quantidade = 0,aux = 0,maior;

		for (p = lst_vizinhos_semanticos; p != NULL; p = p->prox)
		{
			valor = p->CH;
			aux = 0;

			for(p2=lst_vizinhos_semanticos; p2 != NULL; p2 = p2->prox)
			{
				if(p2->CH == valor)
					aux++;
			}
			if(aux > quantidade)
			{
				maior = valor;
				quantidade = aux;
			}

		}

		uint8_t i = 0;
		for (p = lst_vizinhos_semanticos; p != NULL; p = p->prox)
		{
			vetor[i] = p->id;
			i++;
		}
		if(maior == rimeaddr_node_addr.u8[0])
		{
			eh_coletor = 1;
			flag_eleicao = 1;
			//printf("broadcast de eleicao enviado\n");
			packetbuf_copyfrom(&vetor, sizeof(uint8_t[10]));
			broadcast_send(&broadcast);
		}
	}

	PROCESS_END()
}

PROCESS_THREAD(anuncio_vizinho_process, ev, data)
{
	static struct etimer et;
	static uint8_t cont;
	PROCESS_BEGIN()
	while(1 && flag_eleicao==0)
	{
		PROCESS_WAIT_EVENT();
		if(ev == anuncio_vizinho)
		{

			uint8_t i;
			ListaVizinhosSemanticos p;
			rimeaddr_t end;
			for (i = 0; i<2; i++)
			{
				//pega o primeiro CH vizinho (semantico)
				end.u8[0] = vizinhos[i];
				end.u8[1] = 0;
				//printf("Vizinhos %d e %d\n",vizinhos[0],vizinhos[1]);
				if(!possui_CH(lst_vizinhos_semanticos,end.u8[0])) continue;
				for (p = lst_vizinhos_semanticos; p != NULL; p = p->prox)
				{
					//etimer_set(&et, (CLOCK_SECOND * (rimeaddr_node_addr.u8[0]%6) + random_rand() % (CLOCK_SECOND * 4))/6);
					//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
					//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
					//printf("%d enviou %d para o no %d\n",rimeaddr_node_addr.u8[0],p->id,vizinhos[i]);
					packetbuf_copyfrom(p,sizeof(ListaVizinhosSemanticos));
					//printf("enviou anuncio\n");
					unicast_send(&uc2, &end);

				}




			}
		}

	}
	PROCESS_END();
}

PROCESS_THREAD(enviar_coleta_process, ev, data)
{
	static struct etimer et;
	rimeaddr_t end;
	PROCESS_BEGIN()
	etimer_set(&et, CLOCK_SECOND*2);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	while(1)
	{

		if(flag_eleicao)
		{

			if(lst_vizinhos_semanticos == NULL)
			{
				break;
			}
		}
		etimer_set(&et, taxa_transmissao);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		end.u8[0] = destino;
		end.u8[1] = 0;
		coeficientes = coeficientes/2; //a media
		packetbuf_copyfrom(&coeficientes,sizeof(float));
		//printf("Enviou coleta para %d\n",destino);
		//printf("Cont %d\n",cont);
		unicast_send(&uc, &end);
	}
	PROCESS_END()
}
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	//uint8_t *vetor = packetbuf_dataptr();
	if(lst_vizinhos_semanticos == NULL)
	{
		flag_eleicao = 1;
		return;

	}
	destino = from->u8[0];
	//printf("alterou destino para %d\n",destino);
	flag_eleicao = 1;
	//printf("Recebeu broadcast de eleicao do no %d.%d \n",from->u8[0],from->u8[1]);
}

static void recv_uc2(struct unicast_conn *c, const rimeaddr_t *from)
{
	//printf("chegou %d %d\n",((ListaVizinhosSemanticos)packetbuf_dataptr())->CH,((ListaVizinhosSemanticos)packetbuf_dataptr())->id );
	if(lst_vizinhos_semanticos == NULL) return;
	//printf("chegou %d %d\n",((ListaVizinhosSemanticos)packetbuf_dataptr())->CH,((ListaVizinhosSemanticos)packetbuf_dataptr())->id );
	if(get_vizinhossemantico(lst_vizinhos_semanticos,((ListaVizinhosSemanticos)packetbuf_dataptr())->CH,((ListaVizinhosSemanticos)packetbuf_dataptr())->id) == NULL)
	{
		lst_vizinhos_semanticos = insere_vizinhossematicos(lst_vizinhos_semanticos,((ListaVizinhosSemanticos)packetbuf_dataptr())->CH,((ListaVizinhosSemanticos)packetbuf_dataptr())->id);

	}
}

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	if(contem_listamembro(lista,from->u8[0]) != NULL)
	{
		printf("Recebeu do no %d\n",from->u8[0]);
		cont++;
		Medicao *med = ((Medicao*)packetbuf_dataptr());
		coeficientes = coeficientes + med->w1+med->w2+med->w3+med->w4+med->w5;
		if(med_saudavel.w1 == -1)
		{
			med_saudavel.w1 = med->w1;
			med_saudavel.w2 = med->w2;
			med_saudavel.w3 = med->w3;
			med_saudavel.w4 = med->w4;
			med_saudavel.w5 = med->w5;
			//repassa pro sink

		}
		//calcula o delta
		else
		{

			delta_med.w1 = med_saudavel.w1 - med->w1; //0.1
			delta_med.w2 = med_saudavel.w2 - med->w2; //0
			delta_med.w3 = med_saudavel.w3 - med->w3; //0.32
			delta_med.w4 = med_saudavel.w4 - med->w4; //1.08
			delta_med.w5 = med_saudavel.w5 - med->w5; //0.29

			if(delta_med.w1 != 0)
			{
				//printf("tttttt\n");
				//printfloat(delta_med.w4);
				float d = 0.05*delta_med.w1 + 0.06*delta_med.w2 + 0.23*delta_med.w3 + 0.32*delta_med.w4 + 0.34*delta_med.w5; //0.52 aprox
				(contem_listamembro(lista,from->u8[0]))->coeficiente = d;
				//imprime_listamembro(lista);
				//printf("vai comecar o fuzzy para o no %d\n",from->u8[0]);
				process_post(&ch_process, iniciar_fuzzy,from->u8[0]);

			}
		}
	}
	else //recebeu de um CH vizinho
	{


		PData *pacote = (PData*)packetbuf_dataptr();
		//if(pacote->originCH == NULL)
	//	{
			//printf("unicast message received from %d.%d\n",from->u8[0], pacote->originCH);
			//return;
		//}
		if(flag_local && flag_eleicao == 0) //ja fez a inferencia local
		{
			if(get_vizinhossemantico(lst_vizinhos_semanticos,pacote->originCH,pacote->originSensor) == NULL)
			{
				msg++;
			}
			uint8_t vizinhoSemantico = iniciar_distribuido(pacote->dCoeffGrauA,pacote->dCoeffGrauB,pacote->nMessagesGrauA,pacote-> nMessagesGrauB);
			if(vizinhoSemantico >= 79)
			{
				//aqueles que eram candidados, agora sao confirmados como vizinhos semanticos
				ListaMembros p;
				for (p = lista; p != NULL; p = p->prox)
				{
					if(p->coeficiente != 0 && get_vizinhossemantico(lst_vizinhos_semanticos,rimeaddr_node_addr.u8[0],p->id) == NULL)
						lst_vizinhos_semanticos = insere_vizinhossematicos(lst_vizinhos_semanticos,rimeaddr_node_addr.u8[0],p->id);
				}

				if(get_vizinhossemantico(lst_vizinhos_semanticos,pacote->originCH,pacote->originSensor) == NULL)
					lst_vizinhos_semanticos = insere_vizinhossematicos(lst_vizinhos_semanticos,pacote->originCH,pacote->originSensor);
				process_post(&anuncio_vizinho_process, anuncio_vizinho,NULL);
			}
		}
		if(flag_eleicao)
		{
			//total_coeficiente = total_coeficiente
			//printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
		}
	}
}



/*
 * Seta os parametros de acordo com o trab da atslands
 */
void inicializa()
{
	//NETSTACK_MAC.off(0);
	destino = 1;
	//flag de inicializacao
	med_saudavel.w1 = -1;
	lista = inicializa_listamembro();
	lst_vizinhos_semanticos = inicializa_vizinhossematicos();
	if(rimeaddr_node_addr.u8[0] == 2)
	{
		lista = insere_listamembro(lista,6,0);
		lista = insere_listamembro(lista,7,0);
		vizinhos[0] = 3;
		taxa_transmissao = CLOCK_SECOND + 400;
	}
	if(rimeaddr_node_addr.u8[0] == 3)
	{
		lista = insere_listamembro(lista,8,0);
		lista = insere_listamembro(lista,9,0);
		vizinhos[0] = 2;vizinhos[1] = 4;
		taxa_transmissao = CLOCK_SECOND + 500;
	}
	if(rimeaddr_node_addr.u8[0] == 4)
	{
		lista = insere_listamembro(lista,10,0);
		lista = insere_listamembro(lista,11,0);
		vizinhos[0] = 3;vizinhos[1] = 5;
		taxa_transmissao = CLOCK_SECOND + 600;
	}
	if (rimeaddr_node_addr.u8[0] == 5)
	{
		lista = insere_listamembro(lista,12,0);
		lista = insere_listamembro(lista,13,0);
		lista = insere_listamembro(lista,14,0);
		vizinhos[0] = 4;
		taxa_transmissao = CLOCK_SECOND + 700;
	}
}
