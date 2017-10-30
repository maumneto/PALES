#include "contiki.h"
#include "net/rime.h"
#include <stdio.h>
#include "Pacotes.h"
#include "sys/rtimer.h"
#include <stdint.h>

#define ONE_HOP_CH  11
#define LEITURAS 5

//funcoes
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from);
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
void calcula_vetor_delta();
void printfloat(float x);
void calcula_D();

static uint16_t heathy1[LEITURAS];
static uint16_t heathy2[LEITURAS];
static uint16_t damg21[LEITURAS];
static uint16_t damg22[LEITURAS];
static uint16_t delta1[LEITURAS];
static uint16_t delta2[LEITURAS];
static float A1[] = {0.05,0.06,0.23,0.32,0.34};
static float D1;
static float D2;

static uint8_t round_atual;


PROCESS(ch_process, "Processo do CH");
AUTOSTART_PROCESSES(&ch_process);

static uint8_t nos_coletores[] = { 1, 2 }; //rime address 1.0  e 2.0
int len = sizeof(nos_coletores) / sizeof(uint8_t);


//**broadcast**//
static const struct broadcast_callbacks broadcast_call = { broadcast_recv };
static struct broadcast_conn broadcast;

//**unicast**//
static const struct unicast_callbacks unicast_callbacks = { recv_uc };
static struct unicast_conn uc;

/*
 * Recebe uma msg broadcast do sink e depois repassa essa msg por unicast pra cada
 * um de seus membros
 */
static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from) {
	static struct etimer et;
	static rimeaddr_t end;
	uint8_t i;
	round_atual = ((PacoteSink) packetbuf_dataptr())->round;
	printf("ROUND: %d\n", round_atual);
	for (i = 0; i <= 1; i++) {
		PacoteSink pac = ((PacoteSink) packetbuf_dataptr());
		end.u8[0] = nos_coletores[i];
		end.u8[1] = 0;
		packetbuf_copyfrom(pac, sizeof(uint8_t)); //1 byte
		unicast_send(&uc, &end);
		//rtimer_clock_t  end_time = RTIMER_NOW() + 15000UL*RTIMER_ARCH_SECOND/1000; //15 milliseconds
		//while RTIMER_CLOCK_LT(RTIMER_NOW(), end_time) {;}
	}

}

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	if (round_atual == 0) {
		uint8_t i;
		//armazena as leituras do no 1
		if (from->u8[0] == 1) {
			heathy1[0] = ((DataMsg) packetbuf_dataptr())->omega1;
			heathy1[1] = ((DataMsg) packetbuf_dataptr())->omega2;
			heathy1[2] = ((DataMsg) packetbuf_dataptr())->omega3;
			heathy1[3] = ((DataMsg) packetbuf_dataptr())->omega4;
			heathy1[4] = ((DataMsg) packetbuf_dataptr())->omega5;
		} else {
			heathy2[0] = ((DataMsg) packetbuf_dataptr())->omega1;
			heathy2[1] = ((DataMsg) packetbuf_dataptr())->omega2;
			heathy2[2] = ((DataMsg) packetbuf_dataptr())->omega3;
			heathy2[3] = ((DataMsg) packetbuf_dataptr())->omega4;
			heathy2[4] = ((DataMsg) packetbuf_dataptr())->omega5;

		}
	}
	//round 1
	else {
		if (from->u8[0] == 1) {
			damg21[0] = ((DataMsg) packetbuf_dataptr())->omega1;
			damg21[1] = ((DataMsg) packetbuf_dataptr())->omega2;
			damg21[2] = ((DataMsg) packetbuf_dataptr())->omega3;
			damg21[3] = ((DataMsg) packetbuf_dataptr())->omega4;
			damg21[4] = ((DataMsg) packetbuf_dataptr())->omega5;
		} else {
			damg22[0] = ((DataMsg) packetbuf_dataptr())->omega1;
			damg21[1] = ((DataMsg) packetbuf_dataptr())->omega2;
			damg21[2] = ((DataMsg) packetbuf_dataptr())->omega3;
			damg21[3] = ((DataMsg) packetbuf_dataptr())->omega4;
			damg21[4] = ((DataMsg) packetbuf_dataptr())->omega5;
		}
	}

	//printf("unicast message received from %d.%d: [%d,%d,%d,%d,%d]\n",from->u8[0], from->u8[1],((DataMsg)packetbuf_dataptr())->omega1 ,((DataMsg)packetbuf_dataptr())->omega2,((DataMsg)packetbuf_dataptr())->omega3,((DataMsg)packetbuf_dataptr())->omega4,((DataMsg)packetbuf_dataptr())->omega5);
	//printfloat(((float)( (DataMsg)packetbuf_dataptr())->omega5)/100);
}

PROCESS_THREAD(ch_process, ev, data) {
	PROCESS_EXITHANDLER(unicast_close(&uc);)
	PROCESS_BEGIN()
		broadcast_open(&broadcast, 129, &broadcast_call);
		unicast_open(&uc, 146, &unicast_callbacks);
		//**seguindo os experimentos da atstlands, esses ids sao todos fixos**//
		rimeaddr_t endereco;
		endereco.u8[0] = 10;
		endereco.u8[1] = 0;
		rimeaddr_set_node_addr(&endereco);

		//**espera um tempo definido para que o ch receba as leituras dos seus coletores**//
		//**a cada isntante ele verifica as leituras saudaveis com as leiturasde danos**//
		static struct etimer et;
		etimer_set(&et, CLOCK_SECOND * 12);
		while(1){
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
			printf("*****verifica dano******\n");
			calcula_vetor_delta();
			calcula_D();
			//fuzzifica cada um dos D
			etimer_set(&et, CLOCK_SECOND * 10);
		}

	PROCESS_END();
}

void calcula_vetor_delta()
{
	uint8_t i;
	for(i = 0; i<=LEITURAS-1;i++)
	{
		delta1[i] = heathy1[i]-damg21[i];
		delta2[i] = heathy2[i]-damg22[i];
	}
}
void calcula_D()
{
	uint8_t i;
	for(i = 0; i<=LEITURAS-1;i++)
	{
		D1 = D1 + A1[i]*delta1[i];
		D2 = D2 + A1[i]*delta2[i];
	}
}

void printfloat(float x) {
	int integerPart = (int) x;
	int decimalPart = (x - integerPart) * 100;
	//if (decimalPart < 0)
	//decimalPart = decimalPart * (-1);
	printf("Numero: %d.%d\n", integerPart, decimalPart);
}

/*---------------------------------------------------------------------------*/
