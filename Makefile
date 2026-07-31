MODULES := 00-c-for-programmers 01-bits-and-bytes \
           02-modern-cpp 03-stl-internals 04-cache-and-memory \
           05-concurrency 06-order-book 07-networking

.PHONY: test-all clean-all list

test-all:
	@for mod in $(MODULES); do \
		echo ""; \
		echo "========================================"; \
		echo "  Testing $$mod"; \
		echo "========================================"; \
		$(MAKE) -C $$mod test 2>/dev/null || true; \
	done

clean-all:
	@for mod in $(MODULES); do \
		$(MAKE) -C $$mod clean 2>/dev/null || true; \
	done
	@echo "All clean."

list:
	@echo "Available modules:"
	@for mod in $(MODULES); do echo "  $$mod"; done
