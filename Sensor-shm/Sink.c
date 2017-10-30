#include "contiki.h"
#include "net/rime.h"
#include <stdio.h>
#include "Pacotes.h"
#include "sys/clock.h"

PROCESS(sink_process, "Processo do Sink");
AUTOSTART_PROCESSES(&sink_process);

static void broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
  printf("broadcast message received from %d.%d\n",from->u8[0], from->u8[1]);
  PacoteSink *pacote = (PacoteSink *)packetbuf_dataptr();
  dump_pacote(pacote);
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sink_process, ev, data)
{
  static struct etimer et;
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();

  static uint8_t round = 0;
  static PacoteSink pacote;

  broadcast_open(&broadcast, 129, &broadcast_call)
  /*Espera 1 segundo antes de enviar primeira msg de round*/;
  etimer_set(&et, CLOCK_SECOND * 1);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  while(1)
  {
	  pacote = novo_pacote();
	  set_conteudo(pacote,round);
	  packetbuf_copyfrom(pacote, sizeof(PacoteSink));
	  broadcast_send(&broadcast);
	  //dump_pacote(pacote);
	  delete_pacote(pacote);
	  round++;
	  //**espera 10 segundos antes do prox round (de acordo com os experimentos da atslands **//
	  etimer_set(&et, CLOCK_SECOND * 10);
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
