server:nrwserver.c client nread-nwrite.o
	gcc $<  nread-nwrite.o -o server -g  ; ls  

client:nrwclient.c 
	gcc nrwclient.c  nread-nwrite.o -o client -g

nread-nwrite.o:nread-nwrite.c
	gcc nread-nwrite.c -c nread-nwrite.o
.PHONY:clean
clean:
	rm *out server client ; ls 
