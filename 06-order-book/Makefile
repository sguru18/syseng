SUBDIRS := exercises/ex1-price-levels \
           exercises/ex2-limit-order-book \
           exercises/ex3-matching-engine

.PHONY: all run clean

all:
	@for d in $(SUBDIRS); do $(MAKE) -C $$d; done

run:
	@for d in $(SUBDIRS); do \
		echo ""; echo "=== $$d ==="; \
		$(MAKE) -C $$d run; \
	done

clean:
	@for d in $(SUBDIRS); do $(MAKE) -C $$d clean; done
