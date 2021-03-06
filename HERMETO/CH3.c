#include "contiki.h"
#include "net/rime.h"
#include "semantik.h"
#include "util.h"
#include "fuzzyElec.h"

#define MAX_MEMBROS 3
#define MAX_VIZINHOS_SEMANTICOS 9
#define MAX_VIZINHOS 2

PROCESS(dfler_process, "dfler");
PROCESS(anuncio_process, "anuncio");

AUTOSTART_PROCESSES(&dfler_process,&anuncio_process);

//**unicast**//
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc };

//**broadcast**//
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from);
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

static struct unicast_conn uc;
static process_event_t evento;
static process_event_t iniciar_anuncio;
static uint8_t nmsg,flag_local;
static uint8_t vizinhos[MAX_VIZINHOS];
uint8_t flag_anuncio,flag_eleicao;
static float gasto;
static float rssi;
static uint8_t saida;
static float energiaResidual;

void inicializa();
static ListaMembros lista;
static ListaVizinhosSemanticos lst_vizinhos_semanticos;


PROCESS_THREAD(dfler_process, ev, data)
{
	static struct etimer et;
	static rimeaddr_t endereco;
	static PData *pacote;

	PROCESS_BEGIN()
	inicializa();
	evento = process_alloc_event();
	iniciar_anuncio = process_alloc_event();
	unicast_open(&uc, 146, &unicast_callbacks);
	broadcast_open(&broadcast, 129, &broadcast_call);
	etimer_set(&et, CLOCK_SECOND *10);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	//printf("Endereco: %d\n",rimeaddr_node_addr.u8[0]);
	//printf("NMSH: %d\n",nmsg);
	etimer_set(&et, CLOCK_SECOND *15);
	while(!etimer_expired(&et))
	{
		//etimer_set(&et, CLOCK_SECOND *10);
		//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if(rimeaddr_node_addr.u8[0] == 3)
		{

			fuzzificar(0.54,nmsg);
			inferencia_local();
			flag_local = 1;
			pacote =  gera_PData3(rimeaddr_node_addr.u8[0],9);
			uint8_t i;
			endereco.u8[0] = 4;
			endereco.u8[1] = 0;
			etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
			//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			printf("Enviou [%d] msg de fuzzificacao para o no %d\n",pacote->originSensor,endereco.u8[0]);
			packetbuf_copyfrom(pacote,sizeof(PData));
			unicast_send(&uc, &endereco);
			gasto += 0.000001002;
			energiaResidual -= gasto;
		}
		if(rimeaddr_node_addr.u8[0] == 4)
		{
			fuzzificar(0.54,nmsg);
			inferencia_local();
			flag_local = 1;
			pacote =  gera_PData3(rimeaddr_node_addr.u8[0],11);
			endereco.u8[0] = 5;
			endereco.u8[1] = 0;

			printf("Enviou [%d] msg de fuzzificacao para o no %d\n",pacote->originSensor,endereco.u8[0]);
			etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
			//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			packetbuf_copyfrom(pacote,sizeof(PData));
			unicast_send(&uc, &endereco);
			gasto += 0.000001002;
			energiaResidual -= gasto;

			fuzzificar(0.54,nmsg);
			inferencia_local();
			pacote =  gera_PData3(rimeaddr_node_addr.u8[0],11);
			endereco.u8[0] = 3;
			endereco.u8[1] = 0;

			printf("Enviou [%d] msg de fuzzificacao para o no %d\n",pacote->originSensor,endereco.u8[0]);
			etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
			//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			packetbuf_copyfrom(pacote,sizeof(PData));
			unicast_send(&uc, &endereco);
			gasto += 0.000001002;
			energiaResidual -= gasto;
		}
		if(rimeaddr_node_addr.u8[0] == 5)
		{
			fuzzificar(0.54,nmsg);
			inferencia_local();
			flag_local = 1;
			pacote =  gera_PData3(rimeaddr_node_addr.u8[0],13);
			endereco.u8[0] = 4;
			endereco.u8[1] = 0;

			printf("Enviou [%d] msg de fuzzificacao para o no %d\n",pacote->originSensor,endereco.u8[0]);
			etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
			//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			packetbuf_copyfrom(pacote,sizeof(PData));
			unicast_send(&uc, &endereco);
			gasto += 0.000001002;
			energiaResidual -= gasto;

			fuzzificar(0.54,nmsg);
			inferencia_local();
			pacote =  gera_PData3(rimeaddr_node_addr.u8[0],14);
			endereco.u8[0] = 4;
			endereco.u8[1] = 0;

			printf("Enviou [%d] msg de fuzzificacao para o no %d\n",pacote->originSensor,endereco.u8[0]);
			etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
			//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			packetbuf_copyfrom(pacote,sizeof(PData));
			unicast_send(&uc, &endereco);
			gasto += 0.000001002;
			energiaResidual -= gasto;
		}


	}
	printf("SAIU DO DLFER\n");
	PROCESS_WAIT_EVENT();



	PROCESS_END();
}

PROCESS_THREAD(anuncio_process, ev, data)
{
	static struct etimer et;
	static rimeaddr_t endereco;
	static ListaVizinhosSemanticos p;
	static uint8_t i;
	//static uint8_t CH = 0,no = 0;
	static uint8_t idMaior,maior;
	PROCESS_BEGIN()

	etimer_set(&et, CLOCK_SECOND *20);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	flag_anuncio = 1;
	fuzzificar_elec(rssi,(((float)energiaResidual*100)/0.1)/100);
//	/fuzzificar_elec(-18.0,0.91);
	//printf("Entrou no fuzzy %d\n",);
	printfloat(((energiaResidual*100)/0.1)/100);
	inferencia_elec();
	saida = desfuzzyficar_elec();
	printf("*******saida %d\n",saida);
	atualiza_vizinho(lst_vizinhos_semanticos,rimeaddr_node_addr.u8[0],saida);
	for(i = 0; i<MAX_VIZINHOS;i++)
	{
		if(vizinhos[i] == 0)continue;
		endereco.u8[0] = vizinhos[i];
		endereco.u8[1] = 0;
		//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
		etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		packetbuf_copyfrom(&saida,sizeof(uint8_t));
		unicast_send(&uc, &endereco);
		gasto += 0.00000005;
		energiaResidual -= gasto;
	}



	etimer_set(&et, CLOCK_SECOND *15);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	//imprime_listavizinhossemanticos(lst_vizinhos_semanticos);
	//broadcast do lider
	flag_eleicao = 1;
	maior = saida;
	idMaior=rimeaddr_node_addr.u8[0];
	for (p = lst_vizinhos_semanticos; p != NULL; p = p->prox)
	{
		if(p->fuzzySaida > maior)
		{
			maior = p->fuzzySaida;
			idMaior = p->CH;
		}
	}

	if(idMaior != rimeaddr_node_addr.u8[0])
	{
		//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
		etimer_set(&et, CLOCK_SECOND *(rimeaddr_node_addr.u8[0] -1));
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		endereco.u8[0] = idMaior;
		endereco.u8[1] = 0;
		gasto += 0.00000005;
		energiaResidual -= gasto;
		packetbuf_copyfrom(&saida,sizeof(uint8_t));
		unicast_send(&uc, &endereco);
		printf("Mandou para %d\n",idMaior);
	}
	else //sabe que nao eh o maior, manda o broadcast para seus membros aqueles que nao sao vizinhos semanticos ficam adormecidos
	{
		gasto+= 0.00000005;
		energiaResidual -= gasto;
	}
	printf("Maior %d\n",idMaior);

	etimer_set(&et, CLOCK_SECOND *15);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	long x = (float)gasto * 1000000;
	printf("energia eleicao%lu\n",x);

	/*//--------------------3 lider------------------------///
	///estimativa de gasto durante 15 min de coleta
	if(rimeaddr_node_addr.u8[0] == 5)
	{
		//gasto por recebimento dos seus vizihos semanticos
		gasto += 0.000001*562;// recebe do 12 com o dobro da taxa de transmissao
		gasto += 0.000001*1000; // recebe do 13
		gasto += 0.000001*900; //14
		//gasto += 562*(0.000001); //4
		//gasto para enviar para o 3
		gasto += 900*0.000001004;


	}
	if(rimeaddr_node_addr.u8[0] == 4)
	{

		gasto +=  750*0.000001;// 10
		gasto += 1285*0.000001;//11
		//gasto += 600*(0.000001);//3
		//gasto += 562*(0.000001); //5
		//gasto para enviar para o sink
		//gasto += 562*(0.000001004);
		gasto += 1285*0.000001002; //3
	}
	if(rimeaddr_node_addr.u8[0] == 3)
	{
		gasto += 1125*0.000001; //8 com frequencia reduzida
		gasto += 1800*0.000001;//9
		gasto += 1285*(0.000001);// recebe do 4
		gasto += 900*(0.000001);// recebe do 5
		//gasto para enviar para o 4
		//gasto+=600*0.000001002;
		gasto+= 1800*0.000001003;//sink

	}*/



	/*//--------------------4 lider------------------------///
	///estimativa de gasto durante 15 min de coleta
	if(rimeaddr_node_addr.u8[0] == 5)
	{
		//gasto por recebimento dos seus vizihos semanticos
		gasto += 0.000001*562;// recebe do 12 com o dobro da taxa de transmissao
		gasto += 0.000001*1000; // recebe do 13
		gasto += 0.000001*900; //14
		gasto += 900*0.000001002; //enviar pro 4


	}
	if(rimeaddr_node_addr.u8[0] == 4)
	{

		gasto +=  750*0.000001;// 10
		gasto += 1285*0.000001;//11
		gasto += 0.000001*1800;//recebe do 3
		gasto += 0.000001*900;//recebe do 5
		gasto += 1285*0.000001002; //5 enviar
	}
	if(rimeaddr_node_addr.u8[0] == 3)
	{
		gasto += 1125*0.000001; //8 com frequencia reduzida
		gasto += 1800*0.000001;//9
		gasto+= 1800*0.000001002;//4

	}
*/



	//--------------------5 lider------------------------///
		///estimativa de gasto durante 15 min de coleta
		if(rimeaddr_node_addr.u8[0] == 5)
		{
			//gasto por recebimento dos seus vizihos semanticos
			gasto += 0.000001*562;// recebe do 12 com o dobro da taxa de transmissao
			gasto += 0.000001*1000; // recebe do 13
			gasto += 0.000001*900; //14
			gasto += 1285*(0.000001); // receb e do 4
			gasto += 1800*(0.000001); //recebe do 3
			//enviar para osink
			gasto += 900*0.000001006;


		}
		if(rimeaddr_node_addr.u8[0] == 4)
		{

			gasto +=  750*0.000001;// 10
			gasto += 1285*0.000001;//11
			gasto += 1285*0.000001002; //5
		}
		if(rimeaddr_node_addr.u8[0] == 3)
		{
			gasto += 1125*0.000001; //8 com frequencia reduzida
			gasto += 1800*0.000001;//9
			gasto+= 1800*0.000001004;//5
		}

	energiaResidual -= gasto;
	x = (float)gasto * 1000000;
	printf("*********Depois de 15 min de simulacao********\n");
	printf("Gasto:%d\n",x);
	long y = energiaResidual*1000000;
	printf("Energuia restante:%lu\n",y);
	printfloat((energiaResidual*100/0.1));

	//--------------------------------------------///

	PROCESS_END()
}

static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{


	//process_post(&dfler_process,evento,NULL);
	//printf("unicast message received from %d.%d [%d]\n",from->u8[0], from->u8[1],contador);
}

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{

	PData *pacote = (PData*)packetbuf_dataptr();
	//printf("Sensor: %d\n",tempBuf->originSensor);
	//process_post(&receber_process,iniciar_anuncio,from->u8[0]);
	//printf("Recebeu %d do no %d\n",(*(PData *)packetbuf_dataptr())->originSensor,from->u8[0]);

	if(flag_local)
	{

		if(flag_anuncio)
		{
			if(flag_eleicao)
			{
				gasto += 0.00000005;
				energiaResidual -= gasto;
			}
			//vizinhos_semanticos[get_indice()] = (*(uint8_t*)packetbuf_dataptr());
			//printf("Adicionou %d\n", (*(uint8_t*)packetbuf_dataptr()));
			//printf("Chegou do no %d [%d]\n", from->u8[0], *(uint8_t *)packetbuf_dataptr());
			atualiza_vizinho(lst_vizinhos_semanticos,from->u8[0],*(uint8_t *)packetbuf_dataptr());
			//lst_vizinhos_semanticos = insere_vizinhossematicos(lst_vizinhos_semanticos,((ListaVizinhosSemanticos)packetbuf_dataptr())->CH,((ListaVizinhosSemanticos)packetbuf_dataptr())->id);
			gasto += 0.00000005;
			energiaResidual -= gasto;
			imprime_listavizinhossemanticos(lst_vizinhos_semanticos);
			packetbuf_clear();
			return;
		}
		gasto += 0.000001;
		energiaResidual -= gasto;
		uint8_t vizinhoSemantico = iniciar_distribuido(pacote->dCoeffGrauA,pacote->dCoeffGrauB,pacote->nMessagesGrauA,pacote-> nMessagesGrauB);
		//printf("vizinho:%d\n",vizinhoSemantico);
		printf("Recebeu %d do no %d\n",pacote->originSensor, from->u8[0]);
		//printfloat(pacote->dCoeffGrauA);
		if(vizinhoSemantico >= 79)
		{
			//candidatos
			uint8_t i;
			ListaMembros p;
			for (p = lista; p != NULL; p = p->prox)
			{
				if(p->coeficiente != 0 && get_vizinhossemantico(lst_vizinhos_semanticos,rimeaddr_node_addr.u8[0],p->id) == NULL)
					lst_vizinhos_semanticos = insere_vizinhossematicos(lst_vizinhos_semanticos,rimeaddr_node_addr.u8[0],p->id);
			}
			if(get_vizinhossemantico(lst_vizinhos_semanticos,pacote->originCH,pacote->originSensor) == NULL)
			{
				lst_vizinhos_semanticos = insere_vizinhossematicos(lst_vizinhos_semanticos,pacote->originCH,pacote->originSensor);
				nmsg++;
			}
		}
	}

	//process_post(&dfler_process,evento,NULL);
	//packetbuf_clear();*/
	}


void inicializa()
{
	lista = inicializa_listamembro();
	lst_vizinhos_semanticos = inicializa_vizinhossematicos();
	if(rimeaddr_node_addr.u8[0] == 2)
	{
		lista = insere_listamembro(lista,6,0);
		lista = insere_listamembro(lista,7,0.54);
		vizinhos[0] = 3;
		gasto = 100*(0.000001) + 66*(0.000001) + 0.00000005;
		gasto +=  66*0.000001002; //gasto para enviar as msg agrupadas
		energiaResidual -= gasto;
		rssi = 1;
	}
	if(rimeaddr_node_addr.u8[0] == 3)
	{
		energiaResidual = 0.006842;
		lista = insere_listamembro(lista,8,0);
		lista = insere_listamembro(lista,9,0.54);
		vizinhos[0] = 2;vizinhos[1] = 4;
		gasto = 50*(0.000001) + 40*(0.000001) + 0.00000005; //custo recebimento dos membros + broadcast do sink
		gasto+= 40*0.000001003; //gasto para enviar para o sink
		rssi = 1.4;
		energiaResidual -= gasto;
	}
	if(rimeaddr_node_addr.u8[0] == 4)
	{
		energiaResidual = 0.016617;
		lista = insere_listamembro(lista,10,0);
		lista = insere_listamembro(lista,11,0.54);
		vizinhos[0] = 3;vizinhos[1] = 5;
		gasto = 33*(0.000001) + 28*(0.000001) + 0.00000005;
		gasto+=28*0.000001005;
		rssi = 2.2;
		energiaResidual -= gasto;
	}
	if (rimeaddr_node_addr.u8[0] == 5)
	{
		energiaResidual = 0.036328;
		lista = insere_listamembro(lista,12,0);
		lista = insere_listamembro(lista,13,0.54);
		lista = insere_listamembro(lista,14,0.54);
		vizinhos[0] = 4;
		gasto = 25*(0.000001) + 22*(0.000001) + 20*(0.000001) + 0.00000005;
		gasto += 20*0.000001006;
		rssi = 3.1;
		energiaResidual -= gasto;
	}
}



