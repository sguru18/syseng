EXERCISES := ex1-cache-profiler ex2-data-layout ex3-hot-path-alloc

.PHONY: all clean $(EXERCISES)

all: $(EXERCISES)

$(EXERCISES):
	$(MAKE) -C exercises/$@

clean:
	for d in $(EXERCISES); do $(MAKE) -C exercises/$$d clean; done
