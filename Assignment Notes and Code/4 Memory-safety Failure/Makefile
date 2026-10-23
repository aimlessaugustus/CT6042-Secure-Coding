# Build and test harness for the CVE-2026-42945 lab.
#   make          -- build every binary, including the ASan ones the guard needs
#   make test     -- run the original regression guard (vuln FAIL, fixed PASS)
#   make compare  -- prove the improved fix solves the original and beats the point-fix
#   make clean    -- remove all built binaries
#
# CC defaults to cc, which is clang on macOS and gcc on Linux. Both support
# -fsanitize=address. The static-analysis step uses clang --analyze for
# portability, since -fanalyzer is a real-GCC-only flag.

CC      ?= cc
CFLAGS   = -O0 -g
ASAN     = -fsanitize=address
ASANOPTS = ASAN_OPTIONS=detect_leaks=0

.PHONY: all test compare clean

all: rift_vuln rift_fixed rift_improved \
     rift_vuln_asan rift_fixed_asan rift_improved_asan \
     rift_fixed_regressed_asan regression_test

rift_vuln: rift_vuln.c
	$(CC) $(CFLAGS) $< -o $@

rift_fixed: rift_fixed.c
	$(CC) $(CFLAGS) $< -o $@

rift_improved: rift_improved.c
	$(CC) $(CFLAGS) $< -o $@

rift_vuln_asan: rift_vuln.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

rift_fixed_asan: rift_fixed.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

rift_improved_asan: rift_improved.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

rift_fixed_regressed_asan: rift_fixed_regressed.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

regression_test: regression_test.c
	$(CC) $(CFLAGS) $< -o $@

test: rift_vuln_asan rift_fixed_asan regression_test
	@echo "== guard vs vulnerable build (expect FAIL) =="
	-./regression_test ./rift_vuln_asan
	@echo "== guard vs fixed build (expect PASS) =="
	./regression_test ./rift_fixed_asan

compare: rift_improved_asan rift_fixed_regressed_asan
	@echo "######## PART 1: improved fix solves the ORIGINAL bug ########"
	@echo "-- static analysis (no output below = clean) --"
	-clang --analyze rift_improved.c
	@echo "-- ASan on the original trigger, structural fix (expect silent) --"
	$(ASANOPTS) ./rift_improved_asan '++++++++' structural
	@echo
	@echo "######## PART 2: beats the point-fix under a regression ########"
	@echo "-- rift_fixed + regression, unbounded copy (expect ASan overflow) --"
	-$(ASANOPTS) ./rift_fixed_regressed_asan '++++++++'
	@echo "-- rift_improved + same desync, bounded copy (expect fail-closed) --"
	-$(ASANOPTS) ./rift_improved_asan '++++++++' upstream

clean:
	rm -f rift_vuln rift_fixed rift_improved \
	      rift_vuln_asan rift_fixed_asan rift_improved_asan \
	      rift_fixed_regressed_asan regression_test
	rm -rf *.plist