CFLAGS+=-Wall -D NUMBER_OF_MODEMS=1

OBJS=app.c modem_uart.c uart_posix.c

all = gl_modem

$(all):
	$(CC) $(CFLAGS) -o gl_modem $(OBJS)

.PHONY:clean
clean:
	rm -rf *.o *.*~ *~ *.swap $(all)


