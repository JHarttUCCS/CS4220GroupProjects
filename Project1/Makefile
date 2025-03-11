# Jacob Hartt, Derin Rawson, Lea Karsanbhai
# Serena Sullivan
# CS4220.002
# 04/17/2025

CC = gcc
CFLAGS = -O2 -Wall -I. -lssl -lcrypto

outdir = ../out

all: http tls

http: httpserver httpclient

httpserver: server.c
	${CC} ${CFLAGS} $^ -o ${outdir}/$@

httpclient: client.c
	${CC} ${CFLAGS} $^ -o ${outdir}/$@

tls: tlsserver tlsclient
	
tlsserver: tls/tls-server.c
	${CC} ${CFLAGS} $^ -o ${outdir}/$@

tlsclient: tls/tls-client.c
	${CC} ${CFLAGS} $^ -o ${outdir}/$@

.PHONY: clean

clean:
	rm -rf ${outdir}/*

