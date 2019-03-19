.PHONY:all
all:server client
server:serv.cc
	gcc $^ -o $@
client:cli.cc
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf server client
