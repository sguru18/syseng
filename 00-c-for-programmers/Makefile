EXERCISES := exercises/ex1-hello-c exercises/ex2-strings-and-arrays exercises/ex3-structs-and-pointers

.PHONY: test clean

test:
	@for ex in $(EXERCISES); do \
		$(MAKE) -C $$ex test || true; \
	done

clean:
	@for ex in $(EXERCISES); do \
		$(MAKE) -C $$ex clean || true; \
	done
