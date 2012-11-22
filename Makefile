CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =	src/Server.o src/Network/NetworkManager.o src/Network/NetworkSession.o src/Network/NetworkException.o src/Network/NetworkPacket.o src/Network/Opcode.o

LIBS =	-lsfml-system

TARGET =	Server

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
