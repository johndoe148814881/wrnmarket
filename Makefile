CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -pedantic
CPPFLAGS ?= -Iinclude -I/usr/local/include
LDFLAGS ?= -L/usr/local/lib -L/usr/local/lib64
RPATH_FLAGS ?= -Wl,-rpath,/usr/local/lib -Wl,-rpath,/usr/local/lib64
PKG_CONFIG ?= pkg-config
WRN_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags wrntui wrnfrac 2>/dev/null)
WRN_LIBS ?= $(shell $(PKG_CONFIG) --libs wrntui wrnfrac 2>/dev/null)
LDLIBS ?= $(WRN_LIBS) -lwrntui -lwrnfrac -lpthread -lm

TARGET := wrnmarket
SRC := src/main.c src/tui.c
OBJ := $(SRC:.c=.o)
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $(OBJ) $(LDLIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(WRN_CFLAGS) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 0755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
