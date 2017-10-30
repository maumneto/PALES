#include "contiki.h"
#include "net/rime.h"
#include <stdio.h>


PROCESS(sink_process, "Processo do Sink");
AUTOSTART_PROCESSES(&sink_process);

static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
 // printf("broadcast message received from %d.%d\n",from->u8[0], from->u8[1]);
  //PacoteSink *pacote = (PacoteSink *)packetbuf_dataptr();
  //dump_pacote(pacote);*/
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
//**unicast**//
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc };
static struct unicast_conn uc;

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	printf("unicast message received from %d.%d\n",from->u8[0], from->u8[1]);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sink_process, ev, data)
{
  static struct etimer et;
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
  unicast_open(&uc, 146, &unicast_callbacks);
  /*static uint8_t round = 0;
  broadcast_open(&broadcast, 129, &broadcast_call)
  //Espera 1 segundo antes de enviar primeira msg de round;
  etimer_set(&et, CLOCK_SECOND * 1);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  while(1)
  {
	  packetbuf_copyfrom(&round, sizeof(uint8_t));
	  broadcast_send(&broadcast);
	  round++;
	  //espera 20 segundos antes do prox round (de acordo com os experimentos da atslands //
	  etimer_set(&et, CLOCK_SECOND * 20);
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }*/

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
