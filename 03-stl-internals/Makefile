EXERCISES := exercises/ex1-container-benchmark \
             exercises/ex2-circular-buffer \
             exercises/ex3-pool-allocator

test:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex test || true; done

clean:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex clean || true; done

.PHONY: test clean
