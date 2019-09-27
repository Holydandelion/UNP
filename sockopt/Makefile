include ../Make.defines

PROGS =	checkopts prdefaults rcvbuf rcvbufset sockopt test_7_1  test_7_2 test_7_3

all:	${PROGS}

sockopt:	sockopt.o
		${CC} ${CFLAGS} -o $@ sockopt.o ${LIBS}

checkopts:	checkopts.o
		${CC} ${CFLAGS} -o $@ checkopts.o ${LIBS}

prdefaults:	prdefaults.o
		${CC} ${CFLAGS} -o $@ prdefaults.o ${LIBS}

rcvbuf:	rcvbuf.o
		${CC} ${CFLAGS} -o $@ rcvbuf.o ${LIBS}

rcvbufset:	rcvbufset.o
		${CC} ${CFLAGS} -o $@ rcvbufset.o ${LIBS}

test_7_1:	test_7_1.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}

test_7_2:	test_7_2.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}

test_7_3:	test_7_3.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}
		
clean:
		rm -f ${PROGS} ${CLEANFILES}
