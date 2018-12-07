APP = bsm-shell
all: $(APP)

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y
ifeq ($(DEBUG),y)
  # "-O" is needed to expand inlines
  EXTRA_CFLAGS += -O -g -DDEBUG
else
  EXTRA_CFLAGS += -O2
endif

ifneq (,$(findstring $(BOARD),mk5))
  INSTALLDIR ?= $(CURDIR)/../../../bsp/image/rootfs
  EXTRA_CFLAGS += -DBOARD_$(shell echo $(BOARD) | tr a-z A-Z) --sysroot=$(INSTALLDIR)
endif
ifeq ($(BOARD),"") # PC specific build
EXTRA_CFLAGS += -DNO_SYSINFO
endif
# Fallback defaults (host)
BOARD ?= $(shell uname --m)
INSTALLDIR ?=

# lib1609
LIB1609_SRC_DIR = $(CURDIR)/../../../ieee1609/app/lib1609
LIB1609_APP_DIR = $(CURDIR)/lib1609
LIB1609_APP_BIN = $(LIB1609_APP_DIR)/lib1609.a
# libnl (inside lib1609)
LIBNL_APP_DIR = $(LIB1609_APP_DIR)/libnl
LIBNL_APP_BIN = $(LIBNL_APP_DIR)/lib/libnl.so
# libbsm
LIBBSM_DIR = $(CURDIR)/j2735-lib
LIBBSM_BIN = $(LIBBSM_DIR)/libbsm.a
#asn1c
ASN1C_DIR = $(CURDIR)/../../../tools/asn1c
ASN1C_BIN = $(ASN1C_DIR)/asn1c/asn1c
# GPSD
GPSD_SRC_DIR := $(CURDIR)/../../../bsp/app/gpsd
GPSD_APP_DIR := $(CURDIR)/gpsd
GPSD_APP_BIN := $(GPSD_APP_DIR)/libgps.so
# 1609.2
VIICSEC_DIR := $(CURDIR)/../../../aerolink/$(BOARD)
VIICSEC ?= disable
# hardware accelerator
LIBC2X_DIR := $(CURDIR)/../../../nxp/libc2x
LIBNXDISPATCHER_DIR := $(CURDIR)/../../../nxp/dispatcher/$(BOARD)

# If using yocto/poky toolchain, use CC in environment
ifneq ($(TARGET_PREFIX),arm-poky-linux-gnueabi-)
  CC := ccache $(CROSS_COMPILE)gcc
endif

CFLAGS += -Wall -Werror -MD

EXTRA_CFLAGS += -I. \
                -I$(CURDIR)/../../v2x-lib/include/ \
                -I$(INSTALLDIR)/usr/include \
                -I$(CURDIR)/../../../ieee1609/kernel/include \
                -I$(CURDIR)/../../../ieee1609/kernel/include/linux/ieee1609/dot3 \
                -I$(CURDIR)/../../../cohda/kernel/include \
                -I$(LIBBSM_DIR) \
                -I$(GPSD_APP_DIR) \
                -I$(LIB1609_APP_DIR)/include \
                -I$(LIBNL_APP_DIR)/include

ifeq ($(VIICSEC),disable)
EXTRA_CFLAGS += -DNO_VIICSEC
else
EXTRA_CFLAGS += -I$(VIICSEC_DIR)/include
LDFLAGS += -L$(VIICSEC_DIR)/lib \
           -lviicsec
ifneq (,$(findstring $(BOARD),mk5))
LDFLAGS += -L$(LIBC2X_DIR) \
           -lc2x \
           -L$(LIBNXDISPATCHER_DIR) \
           -lNxDispatcher

endif
endif

LDFLAGS += -L$(LIB1609_APP_DIR) -l1609 \
           -L$(LIBNL_APP_DIR)/lib -lnl \
           -L$(GPSD_APP_DIR) -lgps \
           -lpthread -lrt -lm

LIBS += $(LIB1609_APP_BIN) $(LIBNL_APP_BIN) $(GPSD_APP_BIN) $(LIBBSM_BIN)


SRCS =  bsm-shell.c \
        bsm-msg.c \
        bsm-sec.c \
        bsm-opts.c \
        if-gps.c \
        if-can.c \
        if-p1609.c \
        if-p1609sync.c \
        if-udp.c \
        if-vic.c \
        util-log.c

OBJS = $(SRCS:.c=.o)

DEPDIR = .deps
DEPS   = $(SRCS:%.c=$(DEPDIR)/%.d)

$(APP): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(OBJS) $(LIBS) $(LDFLAGS) -o $@
	@cp $@ .$@_$(BOARD)

%.o: %.c $(LIBS)
	-@mkdir --parents $(shell dirname $(DEPDIR)/$*.d)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@
	@echo $@: Makefile >> $*.d
	@mv -f $*.d $(DEPDIR)/$*.d

$(LIBNL_APP_BIN): $(LIB1609_APP_BIN)
	test -f $@

$(LIB1609_APP_BIN):
	make -C $(LIB1609_SRC_DIR) BOARD=$(BOARD)
	rm -f $(LIB1609_APP_DIR)
	ln -s $(LIB1609_SRC_DIR) $(LIB1609_APP_DIR)
	test -f $@

$(ASN1C_BIN):
	(cd $(ASN1C_DIR); LC_ALL=C autoreconf -iv && env -i PATH=$(PATH) ./configure --host=$(shell uname --m) && make mostlyclean && make)

$(LIBBSM_DIR)/Makefile: $(ASN1C_BIN)
	cd $(LIBBSM_DIR); \
	ASN1PREFIX=DSRC_ \
	ASN1CMDOPTS="-fcompound-names" \
	ASN1MODULES="DSRC_R36_Source.ASN" \
	ASN1PDU=BasicSafetyMessage \
	PROGNAME=dsrc-bsm \
	../sample.makefile.regen; \
	$(MAKE) mangle

$(LIBBSM_BIN): $(LIBBSM_DIR)/Makefile
	$(MAKE) -C $(LIBBSM_DIR)

$(GPSD_APP_BIN):
	make -C $(GPSD_SRC_DIR)
	rm -f $(GPSD_APP_DIR)
	ln -s $(GPSD_SRC_DIR)/$(BOARD) $(GPSD_APP_DIR)
	test -f $@

distclean: clean
	if [ -f $(LIBBSM_DIR)/Makefile ]; then $(MAKE) -C $(LIBBSM_DIR) distclean; fi
	rm -f  $(LIBBSM_DIR)/Makefile

clean:
	find . -name "*.o" -delete
	rm -f $(APP) $(OBJS)
	make -C $(GPSD_SRC_DIR) clean
	rm -f $(GPSD_APP_DIR)
	make -C $(LIB1609_SRC_DIR) clean
	rm -f $(LIB1609_APP_DIR)
	if [ -f $(LIBBSM_DIR)/Makefile ]; then $(MAKE) -C $(LIBBSM_DIR) clean; fi
	rm -rf $(DEPDIR)/*
	rm -rf $(DEPDIR)
	rm -f .$(APP)_*

install: $(APP)
	sudo install -d $(INSTALLDIR)/opt/cohda/bin
	sudo install -c $(APP) $(INSTALLDIR)/opt/cohda/bin

mk5:
	env BOARD=$@ ARCH=arm CROSS_COMPILE="ccache arm-linux-gnueabihf-" $(MAKE)

ifeq (,$(findstring $(BOARD),mk5 $(NATIVE)))
$(BOARD):
	env BOARD=$@ ARCH=arm CROSS_COMPILE="ccache $(CROSS_COMPILE)" $(MAKE)
endif

# Doxygen documentation
doc:
	$(MAKE) -C doc

doc-clean:
	$(MAKE) -C doc clean

-include $(DEPS)

.PHONY: mk5 install clean doc
