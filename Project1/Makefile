# Jacob Hartt, Derin Rawson, Lea Karsanbhai
# Serena Sullivan
# CS4220.002
# 04/17/2025

CC = gcc
CFLAGS = -O2 -Wall -I. -lssl -lcrypto

outdir = ../out
$(shell mkdir -p ${outdir})

all: server client

server: server.c
	${CC} ${CFLAGS} $^ -o ${outdir}/httpserver

client: client.c
	${CC} ${CFLAGS} $^ -o ${outdir}/httpclient

clean:
	rm -rf ${outdir}/*

