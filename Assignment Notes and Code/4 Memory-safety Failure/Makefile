# Build and test harness for the CVE-2026-42945 lab.
#   make        -- build every binary, including the ASan ones the guard needs
#   make test   -- build and run the regression guard against both builds
#   make clean  -- remove all built binaries

CC     = gcc
CFLAGS = -O0 -g
ASAN   = -fsanitize=address

.PHONY: all test clean

all: rift_vuln rift_fixed rift_vuln_asan rift_fixed_asan regression_test

rift_vuln: rift_vuln.c
	$(CC) $(CFLAGS) $< -o $@

rift_fixed: rift_fixed.c
	$(CC) $(CFLAGS) $< -o $@

rift_vuln_asan: rift_vuln.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

rift_fixed_asan: rift_fixed.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

regression_test: regression_test.c
	$(CC) $(CFLAGS) $< -o $@

test: rift_vuln_asan rift_fixed_asan regression_test
	@echo "== guard vs vulnerable build (expect FAIL) =="
	-./regression_test ./rift_vuln_asan
	@echo "== guard vs fixed build (expect PASS) =="
	./regression_test ./rift_fixed_asan

clean:
	rm -f rift_vuln rift_fixed rift_vuln_asan rift_fixed_asan regression_test
