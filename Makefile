CC	= gcc
CFLAGS	= -Wall
LDFLAGS	= -lcrypto -lz

TARGETS		= coreos_tool fwpkg

COREOS_OBJS	= coreos_tool.o utils.o
FWPKG_OBJS	= aes.o fwpkg.o utils.o zlib.o


all: $(TARGETS)

coreos_tool: $(COREOS_OBJS)
	@echo -e "  LD\t$@"
	@$(CC) $^ -o $@ $(LDFLAGS)

fwpkg: $(FWPKG_OBJS)
	@echo -e "  LD\t$@"
	@$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@echo -e "  CC\t$<"
	@$(CC) $< -c -o $@ $(CFLAGS)

clean:
	@echo -e "Cleaning..."
	@rm -f $(TARGETS) *.o *~
