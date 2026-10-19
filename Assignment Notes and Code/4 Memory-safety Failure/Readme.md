# Memory Safety Failure

## Context and Classification

This laboratory example emulates CVE-2026-42945 ("NGINX Rift"), a heap overflow in NGINX's `ngx_http_rewrite_module`[^NGINXRiftNIST]. NGINX builds rewritten strings in two passes, one sizing the heap buffer and one filling it, so correctness depends on the two agreeing[^depthfirstRift]. A question mark in the rewrite replacement sets a persistent `is_args` flag. The sizing pass, on a zeroed sub-engine, counts the raw capture, whereas the copy pass, on the main engine, escapes it and writes past the allocation. The flaw maps to CWE-131 (Incorrect Calculation of Buffer Size) manifesting as CWE-122 (Heap-based Buffer Overflow)[^CWE131][^CWE122]. NVD rates it 9.2 Critical, tempered by the non-default configuration it requires[^NGINXRiftNIST].

# References

[^NGINXRiftNIST]: <https://nvd.nist.gov/vuln/detail/cve-2026-42945>
