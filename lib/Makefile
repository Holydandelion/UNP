include ../Make.defines

LIB_OBJS+=my_readn.o my_written.o

all:	${LIB_OBJS}
		ar rv ${LIBUNP_NAME} $?
		${RANLIB} ${LIBUNP_NAME}

clean:
		rm -f ${PROGS} ${CLEANFILES}
