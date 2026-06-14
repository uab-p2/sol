.PHONY: all clean

SUBDIRS := $(sort $(dir $(wildcard quest/*/)))

all:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d all; \
	done

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean; \
	done
	rm -rf cmake-* out
