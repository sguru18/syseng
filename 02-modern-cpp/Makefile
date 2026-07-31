EXERCISES := exercises/ex1-raii-ownership exercises/ex2-type-safe-primitives exercises/ex3-move-semantics

.PHONY: test clean

test:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex test || true; done

clean:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex clean || true; done
