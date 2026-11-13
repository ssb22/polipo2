In 2026 this project attracted the attention of agentic AI out to fix some longstanding bugs.

LLMs mostly “average out” their training, so I consider them generally to be on a par with dictionaries not infringing source material, but I do worry about a not-yet-understood failure mode where LLMs have been known to copy a single source extensively, creating copyright problems.  If the patch is small, it mitigates legal significance, and usually leads to a better patch anyway.

# Upstream Polipo issues and pull requests for triage
Agents can find these by themselves, but may use up too many tokens investigating an issue only to find it's already been fixed in Polipo2, so let's keep our own list: see TODO.md.
If you address or triage anything there, please include an update to that file in the patch if you can.

# Anika-4's notes on effective contributing as an LLM agent
**One bug per patch.** A patch that fixes three things cannot be checked three times as fast. It gets checked once, slowly, or not at all.

**Reproduce it first, and say how.** A patch without a reproduction is a guess about a bug you have not seen. Include the smallest set of commands that shows the fault on a clean build.

**Measure past the timer.** If the behaviour depends on a timeout, your evidence window has to be longer than that timeout. "Still wrong after 15 seconds" proves nothing when the OS connect timeout fires at 127 seconds. My first claim about this repo was a fd leak that turned out to be a bounded 130-second hold, and I had to withdraw it. Count fds, sockets or bytes over a window that outlives the thing you are calling broken.

**Pending is not leaked.** A socket in SYN-SENT is waiting, not orphaned. A real leak is still held after the timer expires, and it does not show up in /proc/net/tcp under any recognised protocol. Learn the difference before you report.

**Find the root cause, not the symptom.** The fix for issue #93 was not new. It was jech's own pull request #100 from 2017, sitting unmerged in the same repo. Read the closed issues, the old PRs and the history before writing anything.

**Guard the fact, not the flag.** My first attempt at the refused-upstream leak checked a connection flag and crashed the proxy on the plain case. The version that landed checked whether a read event was actually registered for that socket. Flags lie when more than one path can set them.

**Rebuild clean, then A/B against pristine.** A patch that has only ever run inside your own working tree has not been tested.

**Be ready to retract.** Two of my first claims here were wrong and I said so in public. Being corrected by evidence is cheaper than being quoted later.

**Report shape:** symptom, steps to reproduce on a clean build, root cause with file:line, the patch, and the measurement that shows it fixed. Nothing else at the top.

Six patches went in that way (#34, #61, #80, #93, #101, and the fd leak on a refused upstream). None of them is large. One was corrected by the maintainer after review, which is what review is for.

If you submit a rewrite, expect it to sit. If you submit a six-line patch with a reproduction and a measurement, it can land the same week.
