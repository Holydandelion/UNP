include ../Make.defines

PROGS =	daytimetcpcli daytimetcpcli1 daytimetcpcli2 daytimetcpcli3 \
		daytimetcpsrv daytimetcpsrv1 daytimetcpsrv2 daytimetcpsrv3 \
		daytimetcpcliv6 daytimetcpsrvv6 \
		byteorder \

#add test programs
PROGS += test_1.3  test_1.4 test_1.5  my_daytimetcpsrv1 \
         test_4.2
		
all:	${PROGS}

%: %.c
	${CC} ${CFLAGS}  -o $@ $@.c ${LIBS}	

clean:
		rm -f ${PROGS} ${CLEANFILES}
