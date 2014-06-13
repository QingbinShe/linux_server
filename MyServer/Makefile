all:server client_tcp client_udp

server:server.o
	gcc -o server server.o
server.o:server.c
	gcc -c server.c

client_tcp:client_tcp.o
	gcc -o client_tcp client_tcp.o
client_tcp.o:client_tcp.c
	gcc -c client_tcp.c

client_udp:client_udp.o
	gcc -o client_udp client_udp.o
client_udp.o:client_udp.c
	gcc -c client_udp.c

clean:
	rm server server.o
	rm client_tcp client_tcp.o
	rm client_udp client_udp.o
