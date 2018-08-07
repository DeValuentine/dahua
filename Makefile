CC=g++
CFLAGS = -c -w -std=c++14 -L./client/3rdParty/NetSDK/lib `pkg-config --cflags`
LDFLAGS = -lavcodec -lavutil -lavfilter -lavformat -lswscale -lpthread \
-lavnetsdk -ldhconfigsdk -ldhnetsdk -lInfra -lNetFramework -lStream -lStreamSvr \
-lopencv_core -lopencv_highgui

SOURCES = client/main.cpp \
	client/engine/algorithm/play/decoder/Decoder.cpp \
	client/engine/algorithm/play/play.cpp \
	client/engine/callback/callback.cpp \
	client/engine/client/client.cpp \
	client/engine/exception/exception.cpp \
	client/engine/h264/data/data.cpp \
	client/engine/algorithm/export/exportcenter.cpp \
	client/engine/algorithm/export/local/localstream.cpp \
	client/engine/algorithm/export/web/webstream.cpp \
	client/engine/algorithm/control/ptzcontrol.cpp \
	#client/engine/App.cpp \
	


OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ValueMonitor

all: create_required_dirs $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o build/$@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(OBJECTS)

create_required_dirs:
	@if [ ! -d build ]; then mkdir build; fi
