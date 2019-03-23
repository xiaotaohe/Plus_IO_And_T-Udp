.PHONY:all
all:server_udp client_udp
client_udp:client_udp.cc
	gcc $^ -o $@
server_udp:server_udp.cc
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf server_udp
