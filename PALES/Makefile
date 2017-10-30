CONTIKI = /home/rodrigo/contiki/
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
CFLAGS += -ffunction-sections
APPS+=powertrace
LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__
PROJECT_SOURCEFILES += util.c
PROJECT_SOURCEFILES += semantik.c
#TARGET_LIBFILES += -lm
include $(CONTIKI)/Makefile.include

