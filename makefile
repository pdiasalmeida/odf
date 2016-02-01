CC=			g++
CFLAGS=		-c -g -O0 -Wall -std=c++11 `pkg-config --cflags opencv libconfig++`
LDFLAGS=	`pkg-config --libs opencv libconfig++` -ldl -lODFPluginEngine 

SOURCES=	plugins/Plugin.cpp plugins/PluginEngine.cpp \
			src/ODF.cpp

OBJECTS=	$(SOURCES:.cpp=.o)

all: odf

odf: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -f $(OBJECTS) odf
