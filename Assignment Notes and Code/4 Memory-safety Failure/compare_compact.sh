#!/bin/sh
# compare_compact.sh -- prints comparison for screenshot purposes.
# Full ASan traces remain available via `make compare`.

run()
{
    label="$1"
    bin="$2"
    shift 2
    out=$(ASAN_OPTIONS=detect_leaks=0 "$bin" "$@" 2>&1)
    if printf '%s' "$out" | grep -q "heap-buffer-overflow"; then
        verdict="OVERFLOW   (ASan: heap-buffer-overflow)"
    elif printf '%s' "$out" | grep -q "REFUSED"; then
        verdict="safe       (fails closed: REFUSED)"
    else
        verdict="safe       (OK, sizes agree)"
    fi
    printf '  %-32s %s\n' "$label" "$verdict"
}

echo "CVE-2026-42945  --  trigger: capture of eight '+' characters"
echo
echo "Original vulnerability:"
run "rift_vuln      (no fix)"   ./rift_vuln_asan '++++++++'
run "rift_fixed     (upstream)" ./rift_fixed_asan '++++++++'
run "rift_improved  (hardened)" ./rift_improved_asan '++++++++' structural
echo
echo "After a simulated regression reintroduces the desync:"
run "rift_fixed     (upstream)" ./rift_fixed_regressed_asan '++++++++'
run "rift_improved  (hardened)" ./rift_improved_asan '++++++++' upstream