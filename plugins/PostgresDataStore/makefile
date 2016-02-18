CC		=	g++
CFLAGS	=	-fPIC -c -Wall -O3
LDFLAGS	=	-lpqxx

TARGET	=	libPostgresDataStore.so
SOURCES	=	PostgresDataStore.cpp
OBJECTS	=	$(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(TARGET) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
