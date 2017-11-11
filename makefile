DEST_DIR=build
SRC=main.c menu.c device.c asm.c getch.c hash.c instructions.c
CC=gcc
CFLAGS=-Wall -g -O0
APP_NAME=sim
DBG=lldb

all:
	${CC} ${SRC} ${CFLAGS} -o ${DEST_DIR}/${APP_NAME}

run:
	${CC} ${SRC} ${CFLAGS} -o ${DEST_DIR}/${APP_NAME}
	./${DEST_DIR}/${APP_NAME}

memtest:
	valgrind ./${DEST_DIR}/${APP_NAME}

debug:
	${DBG} ./${DEST_DIR}/${APP_NAME}