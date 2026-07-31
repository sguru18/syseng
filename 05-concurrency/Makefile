EXERCISES := ex1-seqlock ex2-spsc-queue ex3-mpmc-false-sharing
EXDIRS    := $(addprefix exercises/,$(EXERCISES))

.PHONY: all clean $(EXDIRS)

all: $(EXDIRS)

$(EXDIRS):
	$(MAKE) -C $@

clean:
	for d in $(EXDIRS); do $(MAKE) -C $$d clean; done
