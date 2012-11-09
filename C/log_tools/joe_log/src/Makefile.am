bin_PROGRAMS=hello

hello_SOURCES=hello.c

LOG4CCONFIG= \
			 $(srcdir)/userloc_formatter.c

if USE_LOG4C
hello_SOURCES += $(LOG4CCONFIG)
endif

hello_LDFLAGS=$(LOG4C_LDFLAGS)
hello_CFLAGS=$(LOG4C_CFLAGS)
