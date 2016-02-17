CC		=	g++
CFLAGS	=	-fPIC -c -Wall -O3 `pkg-config --cflags opencv`
LDFLAGS	=	`pkg-config --libs opencv` -lODFPluginEngine

TARGET	=	libCascadeDetector.so
SOURCES	=	CascadeClassifierDetector.cpp
OBJECTS	=	$(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(TARGET) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

install: $(all)
	cp $(TARGET) /usr/local/lib

clean:
	rm -f $(OBJECTS) $(TARGET)
