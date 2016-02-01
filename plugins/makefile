CC		=	g++
CFLAGS	=	-fPIC -c -Wall -O3
LDFLAGS	=	-ldl

TARGET	=	libODFPluginEngine.so
SOURCES	=	Plugin.cpp PluginEngine.cpp
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
