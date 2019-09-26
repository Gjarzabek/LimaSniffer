include /usr/local/etc/PcapPlusPlus.mk

CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -g -fexceptions -c -std=c++14 -lpthread
LDFLAGS = -Wall -Wextra -Wpedantic -g -fexceptions -std=c++14 -lpthread


Lima: main.o PacketGainer.o
	$(CC) $(PCAPPP_LIBS_DIR) -static-libstdc++ -o LimaSniffer PacketGainer.o main.o $(PCAPPP_LIBS)


main.o: main.cpp Lima.hpp Connection.h
	$(CC) $(CFLAGS) main.cpp Lima.hpp Connection.h

PacketGainer.o: PacketGainer.cpp PacketGainer.h
	$(CC) $(CFLAGS) $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) PacketGainer.cpp PacketGainer.h


clean:
	-rm *.o *.gch LimaSniffer