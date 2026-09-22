# Memory Safety Failure: NGINX Rift

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

NGINX Open Source is the world's most popular Web Server, high performance Load Balancer, Reverse Proxy, API Gateway and Content Cache,being used in roughly one third of websites today [^NGINXGithub][^depthfirstRift]. NGINX Rift (CVE-2026-42945) is a heap buffer overflow affecting NGINX Open Source versions 0.6.27 - 1.30.1 which enables Remote Code Execution (RCE) on a target machine [^NGINXRiftNIST] [^cve202642945].

The vulnerability exists within the `ngx_http_rewrite_module` when rewrite rules are chained, specifically when the `rewrite`, `set` and `if` directives interact with unnamed regex captures. The following example below (taken from the proof of concept GitHub) shows how rewrite might be implemented legitimately within an NGINX server within `nginx.conf` [^NGINXRiftGitHub].

```nginx
location ~ ^/api/(.*)$ {
    # Rewrite 
    rewrite ^/api/(.*)$ /internal?migrated=true;
    set $original_endpoint $1;
}
```

NGINX Rift is an implementation logic error in the NGINX script engine that produces a deterministic heap overflow when `rewrite` and `set` are chained and the captured substring contains URI-escapable characters. The overflow arises from inconsistent observation of a single mutable flag, `is_args`, between the engine pass that computes a required buffer length and the pass that writes the data. The defect is exploitable to crash workers and, under additional heap-manipulation conditions, to achieve remote code execution.

#### Root cause and mechanics

NGINX compiles directive sequences into bytecode executed by a script engine (`ngx_http_script_engine_t`). Scripted operations that produce string values run in two passes. The length pass computes the exact number of bytes to allocate; the copy pass writes the bytes into the allocated buffer. When a rewrite replacement contains `?`, the compiled code calls `ngx_http_script_start_args_code`, which sets `e->is_args = 1` on the main engine. A subsequent `set` referencing a regex capture uses `ngx_http_script_complex_value_code`. The length pass runs on a fresh, zeroed sub-engine `le` (so `le.is_args == 0`), while the copy pass runs on the main engine `e` (so `e->is_args == 1`). Both passes call functions that consult `is_args` to decide whether to URI-escape captures. The length pass therefore returns the raw capture length; the copy pass performs escaping and writes a larger representation. Each escapable character expands from one byte to three bytes when percent encoded. If the capture contains `N` escapable characters, the copy pass writes `raw + 2*N` bytes into a buffer allocated for `raw` bytes, producing a heap overflow.

#### Exploitation path to code execution

A heap overflow must overwrite a useful target to become code execution. NGINX per-request pools (`ngx_pool_t`) include a cleanup list of `ngx_pool_cleanup_t` entries; each entry contains a function pointer and an argument pointer. When a pool is destroyed, NGINX iterates the cleanup list and calls `handler(data)`. An attacker who can corrupt a cleanup entry so that the handler and argument are attacker controlled can cause the worker to call an attacker-chosen function with attacker-chosen data. The published exploitation technique arranges pool adjacency across connections, overflows from one pool into the next, and then triggers destruction of the corrupted pool before other corrupted fields are dereferenced. Memory spraying of fake cleanup entries via POST bodies supplies raw binary pointers that survive escaping, whilst low-byte overwrites converge on a sprayed target. With ASLR disabled, this results in RCE. With ASLR enabled, the overflow still enables a threat actor to perform a denial of service attack on a target system.

#### Classification and Security Context

- Assigned a CVSS 3.1 score of 8.1 [^NGINXRiftNIST].
- Maps to CWE-131: Incorrect Calculation of Buffer Size, escalating to CWE-119: Improper Restriction of Operations within the Bounds of a Memory Buffer when the vulnerability is exploited for either RCE or DoS [^RedHatNginxRift] [^CWE131] [^CWE119].
- OWASP: Relates to risks from vulnerable components and unsafe memory handling in server-side software; it exemplifies how documented features can combine to create a critical vulnerability.  
- Nature of weakness: Implementation-level state inconsistency in a stateful script engine, not malformed input parsing.

#### Severity Context

CVE-2026-42945 affects NGINX Open Source and NGINX Plus when `rewrite` is followed by `rewrite`, `if`, or `set` and an unnamed PCRE capture is used with a replacement containing `?`. An unauthenticated attacker can trigger the overflow by sending crafted HTTP requests. Because NGINX is so widely used, this affects a swathe of servers with outdated NGINX versions.

### Demonstration of Exploitability

### Identifying the Vulnerability Using Techniques and Tools

### How to Mitigate the Vulnerability

### Retesting For Resilience

## Marking Scheme (For Reference)

| **Mark Range (%)** | **Knowledge & Understanding** | **Analysis** | **Reading & Research** | **Writing / Presentation Style** |
| --- | --- | --- | --- | --- |
| **80+** | Extraordinary and authoritative understanding of the five selected software errors, including root causes, exploit mechanisms, secure mitigations, verification, the Security Development Life Cycle and product risk. | Exceptional critical analysis of vulnerable and mitigated code. Exploitation, appropriate tool‑assisted testing and retesting are interpreted rigorously; security decisions, limitations and residual risks are evaluated convincingly. | Extensive, independent and critical research. Authoritative standards and high‑quality academic or technical sources are integrated throughout to substantiate technical claims, mitigations, lifecycle discussion and risk analysis. | Lively, precise and professional report with exceptional control of argument. Code, screenshots and tool results are clearly captioned, referenced and explained; appendices are complete; virtually error‑free. |
| **70–79** | Excellent and wide‑ranging understanding of the five software errors, their causes, exploitation, mitigation and retesting, with confident understanding of secure development lifecycle and risk principles. | Excellent and consistent analysis of practical evidence. Vulnerable and mitigated code, exploits, security‑tool findings and retests are interpreted critically, with well‑justified controls and thoughtful consideration of limitations and residual risk. | Excellent independent research using a wide range of relevant academic and authoritative technical sources. Evidence is selected critically, integrated effectively and used consistently to support claims. | Excellent, coherent and professional presentation with strong control of argument. Figures, code and test evidence are consistently captioned, referenced and discussed; source‑code appendices are complete; very few errors. |

[^NGINXGithub]: <https://github.com/nginx/nginx>

[^cve202642945]: <https://app.opencve.io/cve/CVE-2026-42530>

[^NGINXRiftNIST]: <https://nvd.nist.gov/vuln/detail/cve-2026-42945>

[^depthfirstRift]: <https://depthfirst.com/research/nginx-rift-achieving-nginx-rce-via-an-18-year-old-vulnerability>

[^RedHatNginxRift]: <https://access.redhat.com/security/cve/cve-2026-42945#cve-cwe>

[^CWE131]: <https://cwe.mitre.org/data/definitions/131.html>

[^CWE119]: <https://cwe.mitre.org/data/definitions/119.html>

[^NGINXRiftGitHub]: <https://github.com/DepthFirstDisclosures/Nginx-Rift/blob/main/Nginx-Rift/env/nginx.conf>
