# Upstream Polipo issues and pull requests
These were still open when the polipo repo was archived.
AI agent Anika-4 spotted this situation and sent patches;
other agents have since been following [Anika's lead](CONTRIBUTING.md).
To help, I asked Claude Sonnet 5.5 to triage some of the outstanding issues.  It was able to fetch only 8 before being rate-limited.  My next retry will involve fetching them from my own box for Claude, but as I'm only a free-tier user I will need to wait first.  Claude's comments for the 8 read issues are in `monospace`.

* [Automated testing](https://github.com/jech/polipo/pull/8)
* [External redirector inconsistencies with Squid](https://github.com/jech/polipo/issues/15)
* [Warnings when parsing complex resolv.conf](https://github.com/jech/polipo/issues/16)
* [Logging is not as useful as it could be](https://github.com/jech/polipo/issues/17)
* ~~[Warning headers are probably incorrect](https://github.com/jech/polipo/issues/18)~~ fixed by Anara
* [Objects larger than available memory](https://github.com/jech/polipo/issues/19)
* [Implement Cache-Control: stale-if-error](https://github.com/jech/polipo/issues/21)
* [Segfault in httpServerFinish](https://github.com/jech/polipo/issues/22) `Reported only on a mipsel router build, not reproducible on x86-64. The crash is at the line that sets the server time through connection->server, so connection->server is probably NULL or stale. No reproducer, so a guard would be a guess. Probably shares a cause with 23.`
* [Assertion failure in httpServerFinish: pipelined >= 1](https://github.com/jech/polipo/issues/23) `Same reporter, platform and call path (the delayed finish handler) as 22, so likely the same root cause: a connection being finished twice or after teardown. No reproducer.`
* [Add config setting which preserves host header](https://github.com/jech/polipo/pull/30)
* [pages served through polipo differ from original](https://github.com/jech/polipo/issues/32)
* [Add config setting which preserves host header](https://github.com/jech/polipo/pull/35)
* ~~[Sockets leak in half-close state (lsof: can't identify protocol)](https://github.com/jech/polipo/issues/34)~~ fixed by Anika
* [Added request logging](https://github.com/jech/polipo/pull/35)
* [Compile with MSVS 2013 and fix compile error](https://github.com/jech/polipo/pull/38)
* [assertion in lockChunk](https://github.com/jech/polipo/issues/40)
* ~~[Support / add X-Cache and X-Cache-Lookup headers](https://github.com/jech/polipo/issues/41)~~ fixed by Anara + manual changes
* [100% Reproducible crash on http://www.newegg.com](https://github.com/jech/polipo/issues/46) `The failed assertion is the poll() event check for an invalid file descriptor (POLLNVAL), meaning polipo polled an fd it had already closed. That points to a close-then-reuse bug. But the maintainer-era follow-up could not reproduce it, and the site has certainly changed since, so nobody can trigger it now. An fd-lifecycle audit would be speculative. Group with the crash family.`
* [Systemd](https://github.com/jech/polipo/pull/54)
* ~~[socket not close when parse error](https://github.com/jech/polipo/issues/61)~~ fixed by Anika
* [TCP Fast Open](https://github.com/jech/polipo/issues/62)
* [when downloading file the Internet cut down, we could not download this file again when Internet come back](https://github.com/jech/polipo/issues/65)
* [Crash when browsing this url](https://github.com/jech/polipo/issues/68)
* ~~[ETag message could be more helpful](https://github.com/jech/polipo/issues/70)~~ fixed by Zapia + manual edits
* [polipo crash with segment fault](https://github.com/jech/polipo/issues/74) `The reporter found that connection->request is NULL in the server continue-condition handler, and could not say which request triggers it. Their local workaround only converts the crash into an abort. Same crash family as 22 and 23. A defensive fix needs an understanding of why the request was cleared, which needs a reproducer.`
* [Support usernames and passwords in URLs](https://github.com/jech/polipo/issues/75)
* [adding support for ignoring credentials in URL](https://github.com/jech/polipo/pull/76)
* [Support for URL-based proxying](https://github.com/jech/polipo/issues/77)
* ~~[Age is mistakenly generated for newly obtained responses](https://github.com/jech/polipo/issues/80)~~ fixed by Anika (with minor manual correction also validated by Anika)
* [Windows IPv6 Support](https://github.com/jech/polipo/issues/82) `SKIP. A one-line wish, and it needs Windows to build and test.`
* ~~[Crash when starting with a build from master using socksAuthCredentials in config](https://github.com/jech/polipo/issues/86)~~ fixed by Sesame's Maya + light manual edits
* [Unable to stream mp4](https://github.com/jech/polipo/issues/87)
* ~~[hostname error with format ::ffff:111.30.131.122](https://github.com/jech/polipo/issues/90)~~ fixed by Claude (some manual assistance)
* [support IPv4 mapped address(RFC 2732)](https://github.com/jech/polipo/pull/91) `The PR body is just a link to issue 90, which has already been fixed. Check whether that fix covers this PR, and if so record it as done by that fix.`
* ~~[Dropping "CONNECT" request instead of 401 when auth is required](https://github.com/jech/polipo/issues/93)~~ fixed by Anika based on PR 100
* [Not proxy for given web sites](https://github.com/jech/polipo/issues/96)
* [Compile with MSVC 2017 CE](https://github.com/jech/polipo/pull/99)
* ~~[Fix proxy authentication for CONNECT requests](https://github.com/jech/polipo/pull/100)~~ validated and incorporated by Anika
* ~~[chunkHighMark ignored when it is a big number](https://github.com/jech/polipo/issues/101)~~ fixed by Anika
* [Error using curl --trace-ascii dump.txt www.google.com . ](https://github.com/jech/polipo/issues/103)
* [Couldn't establish listening socket: Unknown error](https://github.com/jech/polipo/issues/108) `SKIP. A Windows 10 user question about a config parse error and a failing bind, answered with "unmaintained". Not a clear bug, and not testable here.`
* [HTTP violations](https://github.com/jech/polipo/issues/109) `SKIP. A one-line request for something like Squid's enable-http-violations, with no spec. That option is a bundle of behaviours, so an agent would have to invent the scope.` (but agents inventing the scope might be OK? not 100% sure I agree with Claude's recommendation on this one)
