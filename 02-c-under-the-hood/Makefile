EXERCISES := exercises/ex1-pointer-puzzles exercises/ex2-function-pointers exercises/ex3-stack-frames

test:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex test || true; done

clean:
	@for ex in $(EXERCISES); do $(MAKE) -C $$ex clean || true; done

.PHONY: test clean
