# Memory Safety Failure

## Context and Classification

This laboratory example emulates CVE-2026-42945 ("NGINX Rift"), a heap overflow in NGINX's `ngx_http_rewrite_module`[^NGINXRiftNIST]. NGINX builds rewritten strings in two passes, one sizing the heap buffer and one filling it, so correctness depends on the two agreeing[^depthfirstRift]. A question mark in the rewrite replacement sets a persistent `is_args` flag. The sizing pass on a zeroed sub-engine counts the raw capture, whereas the copy pass on the main engine escapes it and writes past the allocation. The flaw maps to CWE-131 (Incorrect Calculation of Buffer Size) manifesting as CWE-122 (Heap-based Buffer Overflow)[^CWE131][^CWE122]. NVD rates it 9.2 Critical, tempered by the non-default configuration it requires[^NGINXRiftNIST].

## Vulnerable Code and Exploit

The minimal sink is the two-pass build (Listing X). A length function and a copy function share one escape predicate, but each pass sees different engine state. The capture is N URI-escapable bytes, from plus signs padding the request path. The length pass runs on a zeroed sub-engine and sizes the buffer at raw width. The copy pass runs on the main engine, which still carries `is_args` from the rewrite's question mark, so it writes percent-encoded triples (Figure X). A buffer allocated for N bytes therefore receives 3N, overflowing by 2N (Output Figure). In NGINX the overrun crosses into an adjacent request pool. A cleanup handler sprayed there is reached on pool destruction, giving remote code execution with ASLR off, a full loss of confidentiality, integrity and availability[^depthfirstRift]. With ASLR on, the worker only crashes, a denial of service[^RedHatNginxRift].

*Figure: Diagram illustrating the two-pass script engine*

![alt text](Images/riftvulnsuccessful.png)
*Figure: Successful execution of the buffer overflow.*

*Listing: Vulnerable passes within rift_vuln.c*

```c
/* Length pass (le->is_args == 0). */
return raw;                       /* rift_vuln.c:59 */

/* Copy pass (e->is_args == 1). */
*e->pos++ = '%';                  /* rift_vuln.c:81-83 -- 1 byte -> 3 */
*e->pos++ = hex[cap[i] >> 4];
*e->pos++ = hex[cap[i] & 0x0f];
```

# References

[^NGINXRiftNIST]: <https://nvd.nist.gov/vuln/detail/cve-2026-42945>

[^depthfirstRift]: <https://depthfirst.com/research/nginx-rift-achieving-nginx-rce-via-an-18-year-old-vulnerability>
