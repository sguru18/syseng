EXERCISES := ex1-epoll-server ex2-udp-multicast ex3-fix-framer
EXDIRS    := $(addprefix exercises/,$(EXERCISES))

.PHONY: all test clean $(EXDIRS)

all: $(EXDIRS)

test:
	@for d in $(EXDIRS); do $(MAKE) -C $$d test; done

$(EXDIRS):
	$(MAKE) -C $@

clean:
	@for d in $(EXDIRS); do $(MAKE) -C $$d clean; done
