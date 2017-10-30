/**pacote que o sink envia para o CH**/
#include <stdint.h>
struct __pacoteSink
{
	uint8_t round;
};
typedef struct __pacoteSink * PacoteSink;
PacoteSink novo_pacote();
void set_conteudo(PacoteSink,uint8_t);
void dump_pacote(PacoteSink);
void delete_pacote(PacoteSink);

/**pacote que os CH envia para seus membros**/

struct __pacoteCH
{
	uint8_t round;
};


//****************************************************************************//
//**mensagem que contem as leiturasdas frequencias da estrutura**//
struct __dataMsg
{
	uint16_t omega1;
	uint16_t omega2;
	uint16_t omega3;
	uint16_t omega4;
	uint16_t omega5;

};
typedef struct __dataMsg * DataMsg;
DataMsg novo_pacoteMSG();
void set_conteudoMSG(DataMsg,uint16_t,uint16_t,uint16_t,uint16_t,uint16_t);
void dump_pacoteMSG(DataMsg);
void delete_pacoteMSG(DataMsg);


