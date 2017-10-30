#include "contiki.h"
#include "net/rime.h"
#include "util.h"
#include "semantik.h"
#include "net/netstack.h"
PROCESS(no_process, "Processo do no");
PROCESS(sleep_process, "Processo de sleep");
AUTOSTART_PROCESSES(&no_process,&sleep_process);

#define DELAY 6
//----------------------------VARIAVEIS---------------------------//
uint8_t ch;
Medicao med_saudavel;
Medicao med_danoFL2;
Medicao med_danoFL4;
uint16_t taxa_transmissao;
static process_event_t reduzir_atividades;
//--------------------------------------------------------------//
void inicializa();
void enviar_coleta_saudavel();
void enviar_coleta_danoFL4();
void enviar_coleta_danoFL2();
uint8_t contem(uint8_t id,uint8_t *vetor);
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from);
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
//**unicast**//

static const struct unicast_callbacks unicast_callbacks = { recv_uc };
static struct unicast_conn uc;

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;


static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	//se recebe a msg do seu CH - enviar nova coleta
	//if(from->u8[0] == ch)
	//printfloat()
}

static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
	//printf("Recebeu do no %d.%d o valor %d\n",from->u8[0],from->u8[1], .id);
	uint8_t *vetor = packetbuf_dataptr();
	//printf("")
	uint8_t i;
	for(i = 0; i<9;i++)
	{
		if(*vetor == 0) break;
		//if(!contem(rimeaddr_node_addr.u8[0],vetor))
		if(rimeaddr_node_addr.u8[0] == 6 || rimeaddr_node_addr.u8[0] == 7 || rimeaddr_node_addr.u8[0] == 8 || rimeaddr_node_addr.u8[0] == 10 || rimeaddr_node_addr.u8[0] == 12)
		{
			process_post(&sleep_process, reduzir_atividades,NULL);

			break;
		}
		vetor++;
	}

}

PROCESS_THREAD(sleep_process, ev, data)
{
	static struct etimer et;
	PROCESS_BEGIN()
	PROCESS_WAIT_EVENT();
	if(ev == reduzir_atividades)
	{
		while(1)
		{
			etimer_set(&et, CLOCK_SECOND*20);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			printf("entrou\n");
			NETSTACK_MAC.off(0);
			etimer_set(&et, CLOCK_SECOND*10);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			NETSTACK_MAC.on();
		}
	}
	PROCESS_END();
}
PROCESS_THREAD(no_process, ev, data)
{
	static struct etimer et,et2;
	PROCESS_EXITHANDLER(unicast_close(&uc));
	PROCESS_BEGIN()
	unicast_open(&uc, 146, &unicast_callbacks);
	broadcast_open(&broadcast, 129, &broadcast_call);
	reduzir_atividades = process_alloc_event();
	inicializa();
	//timer de 10 segundos
	etimer_set(&et2, CLOCK_SECOND*10);
	while(1 && !etimer_expired(&et2) )
	{
		//etimer_set(&et, (CLOCK_SECOND * (rimeaddr_node_addr.u8[0]%DELAY) + random_rand() % (CLOCK_SECOND * 4))/DELAY);
		//etimer_set(&et, (CLOCK_SECOND/2 + rimeaddr_node_addr.u8[0]*100));
		etimer_set(&et, taxa_transmissao);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		enviar_coleta_saudavel();
	}

	//envia dano FL4
	etimer_set(&et2, CLOCK_SECOND*10);
	while(1 && !etimer_expired(&et2) )
	{
		etimer_set(&et, taxa_transmissao);
		//etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if(rimeaddr_node_addr.u8[0] == 9 || rimeaddr_node_addr.u8[0] == 11 || rimeaddr_node_addr.u8[0] == 13 || rimeaddr_node_addr.u8[0] == 14)
		//if(rimeaddr_node_addr.u8[0] == 6 || rimeaddr_node_addr.u8[0] == 7 || rimeaddr_node_addr.u8[0] == 9 || rimeaddr_node_addr.u8[0] == 10)
		//if(rimeaddr_node_addr.u8[0] == 7 || rimeaddr_node_addr.u8[0] == 6 || rimeaddr_node_addr.u8[0] == 8)
			enviar_coleta_danoFL2();
		else
			enviar_coleta_saudavel();
	}
	PROCESS_END();
}

void inicializa() {

	med_saudavel.w1 =  2.54;
	med_saudavel.w2 = 7.52;
	med_saudavel.w3 = 12.01;
	med_saudavel.w4 = 15.53;
	med_saudavel.w5 = 17.77;

	med_danoFL2.w1 =  2.34;
	med_danoFL2.w2 = 7.52;
	med_danoFL2.w3 = 11.62;
	med_danoFL2.w4 = 14.45;
	med_danoFL2.w5 = 17.48;

	med_danoFL4.w1 =  2.44;
	med_danoFL4.w2 = 6.93;
	med_danoFL4.w3 = 11.91;
	med_danoFL4.w4 = 15.14;
	med_danoFL4.w5 = 16.99;
	//todo: taxa de transmissao para cada no
	if (rimeaddr_node_addr.u8[0] == 6) {
		ch = 2;
		taxa_transmissao = CLOCK_SECOND/2; //500
	}
	if (rimeaddr_node_addr.u8[0] == 7) {
		ch = 2;
		taxa_transmissao = CLOCK_SECOND/2 + 100; //600
	}
	if (rimeaddr_node_addr.u8[0] == 8) {
		ch = 3;
		taxa_transmissao = CLOCK_SECOND/2 + 200; //700
	}
	if (rimeaddr_node_addr.u8[0] == 9) {
		ch = 3;
		taxa_transmissao = CLOCK_SECOND/2 + 300; //800
	}
	if (rimeaddr_node_addr.u8[0] == 10) {
		ch = 4;
		taxa_transmissao = CLOCK_SECOND/2 + 400; //900
	}
	if (rimeaddr_node_addr.u8[0] == 11) {
		ch = 4;
		taxa_transmissao = CLOCK_SECOND/2 + 500; //1000
	}
	if (rimeaddr_node_addr.u8[0] == 12) {
		ch = 5;
		taxa_transmissao = CLOCK_SECOND/2 + 600; //1100
	}
	if (rimeaddr_node_addr.u8[0] == 13) {
		ch = 5;
		taxa_transmissao = CLOCK_SECOND/2 + 700; //1200
	}
	if (rimeaddr_node_addr.u8[0] == 14) {
		ch = 5;
		taxa_transmissao = CLOCK_SECOND/2 + 800; //1300
	}
}

void enviar_coleta_saudavel()
{
	packetbuf_copyfrom(&med_saudavel,sizeof(Medicao));
	rimeaddr_t endereco;
	endereco.u8[0] = ch;
	endereco.u8[1] = 0;
	unicast_send(&uc, &endereco);
}

void enviar_coleta_danoFL4()
{
	packetbuf_copyfrom(&med_danoFL4,sizeof(Medicao));
	rimeaddr_t endereco;
	endereco.u8[0] = ch;
	endereco.u8[1] = 0;
	unicast_send(&uc, &endereco);
}
void enviar_coleta_danoFL2()
{

	packetbuf_copyfrom(&med_danoFL2,sizeof(Medicao));
	rimeaddr_t endereco;
	endereco.u8[0] = ch;
	endereco.u8[1] = 0;
	//printf("mandou coleta\n");
	unicast_send(&uc, &endereco);
}

uint8_t contem(uint8_t id,uint8_t *vetor)
{
	uint8_t i;
	for(i = 0; i< 9;i++)
	{
		if (id == *vetor)
		{
			return 1;
		}
		vetor++;
	}
	return 0;
}
