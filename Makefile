CXXFLAGS = -g -fmessage-length=0 -std=c++11 #-Wall -O2

OBJS =	src/Server.o src/Network/NetworkManager.o src/Network/NetworkSession.o src/Network/NetworkException.o src/Network/NetworkPacket.o src/Network/Opcode.o src/Network/NetworkEncryption.o src/Network/PacketHandler.o

LIBS =	-lsfml-system -lcryptopp

TARGET =	Server

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
