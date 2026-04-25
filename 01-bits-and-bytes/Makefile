EXERCISES := exercises/ex1-bitwise-basics exercises/ex2-ieee754 exercises/ex3-bitfield-packing

test:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex test || true; done

clean:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex clean || true; done

.PHONY: test clean
