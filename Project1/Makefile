# Jacob Hartt, Derin Rawson, Lea Karsanbhai
# Serena Sullivan
# CS4220.002
# 05/09/2025

CC = gcc
CFLAGS = -O2 -Wall -I.

outdir = ../out

all: httpserver httpclient

httpserver: server.c
	${CC} ${CFLAGS} $^ -o ${outdir}/$@

httpclient: client.c
	${CC} ${CFLAGS} $^ -o ${outdir}/$@

.PHONY: clean

clean:
	rm -rf ${outdir}/*

