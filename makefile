
CC=gcc
LNK=gcc
LFLAGS= -lcrypto -lc
CFLAGS= 



SRCS = main.c aesquick.c

OBJS = ${SRCS:.c=.o}
EXE = lockbox



all: ${EXE}
allc: all clean_obj

${EXE}: ${OBJS}
	${LNK} ${OBJS} -o $@ ${LFLAGS}
%.o: %.c
	${CC} $< -c ${CFLAGS} -o $@

clean_all: clean_obj clean_exe
clean_obj:
	rm ${OBJS}
clean_exe:
	rm ${EXE}

.PHONY: all allc clean_exe clean_obj clean_all