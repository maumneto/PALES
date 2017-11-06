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
static uint16_t contador;
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	contador++;
	printf("unicast message received from %d.%d [%d]\n",from->u8[0], from->u8[1],contador);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sink_process, ev, data)
{
  static struct etimer et;
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
  unicast_open(&uc, 146, &unicast_callbacks);
  broadcast_open(&broadcast, 129, &broadcast_call);
  //Espera 5 segundo antes de enviar primeira msg de round;
  etimer_set(&et, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  packetbuf_copyfrom("1", sizeof(uint8_t));
  broadcast_send(&broadcast);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
