APPS = app/udpc.exe \
       app/udps.exe \
       app/tcpc.exe \
       app/tcps.exe \

TESTS = test/test.exe \

DRIVERS = driver/null.o \
          driver/loopback.o \

OBJS=arp.o  ether.o  icmp.o  ip.o  net.o  sock.o  tcp.o  udp.o  util.o

EXAMPLE=ex

OBJ_LINUX=$(shell ls platform/linux/*.c platform/linux/driver/*.c)
LIB_TCP=libtcp.a

CFLAGS := $(CFLAGS) -g -W -Wall -Wno-unused-parameter -I./include -I./driver -I./

MY_DRIVERS_PREFIX=platform/linux

MY_DRIVERS=$(MY_DRIVERS_PREFIX)/intr $(MY_DRIVERS_PREFIX)/sched.o

ifeq ($(shell uname),Linux)
       CFLAGS := $(CFLAGS) -pthread  -I./platform/linux 
       DRIVERS := $(DRIVERS) platform/linux/driver/ether_tap.o platform/linux/driver/ether_pcap.o
       LDFLAGS := $(LDFLAGS) -lrt
       OBJS := $(OBJS) platform/linux/sched.o platform/linux/intr.o
endif

ifeq ($(shell uname),Darwin)
       CFLAGS := $(CFLAGS)
       DRIVERS := $(DRIVERS)
endif

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean

all: $(EXAMPLE) #$(OBJS) #$(EXAMPLE)#$(APPS) $(TESTS)

$(APPS): %.exe : %.o $(OBJS) $(DRIVERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TESTS): %.exe : %.o $(OBJS) $(DRIVERS) test/test.h
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

MOD:
	./utils/mod.sh 1
$(OBJS): %.o : %.c  
ifdef SHARED
	$(CC)  $(CFLAGS) -fPIC -shared $^ -o $(patsubst %.o,%.so,$@)
else 
	$(CC)  $(CFLAGS) -c  $^ -o $(@)
endif 

$(EXAMPLE): $(OBJS)
	gcc  $(CFLAGS) $(OBJS) user/$@.c -o $@

#$(OBJ_LINUX): %.c : %.so
#	$(CC) -fPIC -shared $@ -o $^
#
#obj_lin: $(OBJ_LINUX)
$(LIB_TCP): $(OBJS) 
ifdef SHARED 
	ar rcs   $(patsubst  %.a,lib/%.a,$@) $^
endif
final: MOD $(LIB_TCP) 
	./utils/mod.sh 2 
obj_clean:
	rm -rf $(OBJS) 
	
build: $(LIB_TCP) #obj_clean
	
clean:
	rm -rf  $(OBJS) $(APPS:.exe=.o) $(OBJS) $(DRIVERS) $(TESTS) $(TESTS:.exe=.o)
