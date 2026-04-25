EXERCISES := exercises/ex1-bump-allocator exercises/ex2-free-list exercises/ex3-pool-allocator

test:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex test || true; done

clean:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex clean || true; done

.PHONY: test clean
