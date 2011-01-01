CC	= gcc
CFLAGS	= -Wall
LDFLAGS	= -lcrypto -lz

TARGET	= fwpkg
OBJS	= aes.o fwpkg.o zlib.o


all: $(TARGET)

$(TARGET): $(OBJS)
	@echo -e "  LD\t$@"
	@$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@echo -e "  CC\t$<"
	@$(CC) $< -c -o $@ $(CFLAGS)

clean:
	@echo -e "Cleaning..."
	@rm -f $(OBJS) $(TARGET) *~
