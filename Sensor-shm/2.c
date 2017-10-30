#include "contiki.h"
#include "net/rime.h"
#include <stdio.h>
#include "Pacotes.h"

#define CH 10

/*---------------------------------------------------------------------------*/
PROCESS(example_unicast_process, "Example unicast");
AUTOSTART_PROCESSES(&example_unicast_process);
/*---------------------------------------------------------------------------*/
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
static void
recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
	uint8_t round = ((PacoteSink)packetbuf_dataptr())->round;
	printf("unicast message received from %d.%d: %d\n",from->u8[0], from->u8[1], round);
	//manda a leitura saudavel
	if(round ==0)
	{
		DataMsg msg=novo_pacoteMSG();
		set_conteudoMSG(msg,254,752,1201,1553,1777);
		packetbuf_copyfrom(msg, 10); //10bytes
		rimeaddr_t end;
		end.u8[0]=CH;
		end.u8[1]=0;
		unicast_send(&uc,&end);
		delete_pacoteMSG(msg);
	}



}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_unicast_process, ev, data)
{
	  PROCESS_EXITHANDLER(unicast_close(&uc);)
	  PROCESS_BEGIN();
	  unicast_open(&uc, 146, &unicast_callbacks);
	  rimeaddr_t endereco;
	  endereco.u8[0] = 2;
	  endereco.u8[1] = 0;
	  rimeaddr_set_node_addr(&endereco);
	  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
