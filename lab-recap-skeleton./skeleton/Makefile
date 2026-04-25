.PHONY: build test test-valgrind clean $(EXERCISES)

EXERCISES = ex1_hot_potato ex2_postfix ex3_bugfix ex4_bst_complete ex5_swap_pairs_dll ex6_merge_k

build:
	@for d in $(EXERCISES); do \
		echo "=== Build $$d ==="; \
		$(MAKE) -C $$d build || exit 1; \
	done

test:
	@total=0; max=0; \
	for d in $(EXERCISES); do \
		echo ""; \
		echo "========================================"; \
		echo "=== Test $$d ==="; \
		echo "========================================"; \
		$(MAKE) -s -C $$d build > /dev/null && \
		./$$d/test$$(echo $$d | sed -E 's/^ex([0-9]+).*/Ex\1/') > test_out.tmp 2>&1; \
		cat test_out.tmp; \
		score=$$(grep "^SCOR " test_out.tmp | grep -oE '[0-9]+\.[0-9]+' | head -1); \
		maxs=$$(grep "^SCOR " test_out.tmp | grep -oE '[0-9]+\.[0-9]+' | tail -1); \
		if [ -n "$$score" ]; then total=$$(echo "$$total + $$score" | bc); fi; \
		if [ -n "$$maxs" ]; then max=$$(echo "$$max + $$maxs" | bc); fi; \
		rm -f test_out.tmp; \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "SCOR TOTAL: $$total / $$max"; \
	echo "========================================"

test-valgrind:
	@for d in $(EXERCISES); do \
		echo ""; \
		echo "=== Valgrind $$d ==="; \
		$(MAKE) -s -C $$d test-valgrind || echo "  FAILED (vezi output de mai sus)"; \
	done

clean:
	@for d in $(EXERCISES); do \
		$(MAKE) -s -C $$d clean; \
	done
