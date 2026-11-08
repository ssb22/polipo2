Polipo2 is a single-threaded, non blocking caching web proxy that has
very modest resource needs.  See the file INSTALL for installation
instructions.  See the texinfo manual (available as HTML after
installation) for more information.

The original Polipo was developed by Juliusz Chroboczek from
2003 and was maintained by him up until November 2016.

This Polipo2 fork is maintained by Silas S. Brown.  It is **not**
intended for general use—as Juliusz said when he stepped
down, caching proxies are becoming obsolete for general use
due to the increasing prevalence of encrypted alternatives to
HTTP that reduce caching proxies to simple relays.  If all
you want is a simple relay (for example so your Web traffic
originates from a remote IP address), then you can do better
by using a VPN or a SOCKS5 proxy.

Polipo2 is now intended as a drop-in caching layer for
experimental HTTP proxies such as [Web Adjuster](http://ssb22.user.srcf.net/adjuster/) (see Adjuster’s `--upstream-proxy` option; I suggest increasing polipo2’s `serverSlots` and `serverMaxSlots` when used with `parentProxy=localhost:8124`).

# Installation

On a suitably well-provisioned GNU/Linux system you can do `make` and optionally `sudo make install` (you can run polipo2 directly from your user account instead, e.g. `./polipo2 logFile=/tmp/polipo2.log pidFile=/tmp/polipo2.pid diskCacheRoot=""`; if you are using it with Web Adjuster’s `--upstream-proxy=:8123` you should also add `parentProxy=localhost:8124` to the Polipo2 options, and consider `serverSlots=256` and `serverMaxSlots=256`).

# Performance

Polipo2 (like the original Polipo) uses the POSIX `poll()` system call (like 4.2BSD’s `select()` without the 1024-socket limit) to monitor its in-progress connections from a single thread. ​It does **not** use the more advanced `epoll`/`kqueue` mechanism that modern versions of large-scale proxies like Squid can use (and that Tornado and hence Web Adjuster can use) on the GNU/Linux and BSD platforms.

The problem with `poll()` is, it can monitor a *huge* number of connections at once to tell your code when one or more of them needs processing, but it doesn’t tell you *which* ones need this processing. ​So when `poll()` returns, Polipo2 has to spend CPU time looping through *all* of its open connections to see what needs doing. ​By contrast, Linux’s `epoll` and BSD’s `kqueue` can point your code *directly* at the connections that need attention, eliminating that loop.

This is not an issue if you have only a few dozen connections going at once, but once you’re in the tens of thousands, you *will* notice a CPU holdup from those tens of thousands of extra checks that have to be done every time anything happens on any connection!

If that’s your situation, I’d recommend switching to Squid, which is more scalable. ​I currently have no plans to upgrade Polipo2’s `poll()` into an `epoll`/`kqueue`, as Polipo2 is intended for small-scale experimental use (so if you’re getting big, “bite the bullet” and install Squid).

* If using Squid with Web Adjuster’s `--upstream-proxy=:3128`, my suggested starting point is `--run="squid -N -f squid.conf -u 0"` with a `squid.conf` like:

```
http_port 127.0.0.1:3128
http_access allow localhost
cache_peer 127.0.0.1 parent 3129 0 no-query no-digest
never_direct allow all
dead_peer_timeout 99 seconds
access_log none localhost
cache_mem 256 MB
```

(see Squid’s `cache_dir` directive if you also want a disk cache)

# Polipo2 RAM and disk usage

As with Polipo, Polipo2 does not have options to limit the maximum size of its disk cache. ​You could periodically purge from a separate process and then send `SIGUSR2` (discard objects) to the running Polipo2, but if the machine is expected to stay up, it’s likely easier to run in RAM+swap (by setting `diskCacheRoot=""`) and then the size can be constrained more accurately (defaults to 25% of RAM; see note on differences with Polipo below).

On the other hand, a non-expiring disk cache is a useful option if you wish to collect a *corpus* of material from a site as users browse it (without needing to run a ‘crawler’ which might annoy the site); Polipo2’s file format is quite easy for other programs to read. ​Obviously you’d have to respect the copyright on the resulting material.

# Differences from the original Polipo

So far:
1. If running in RAM only (`diskCacheRoot=""`), Polipo2’s `chunkHighMark` now defaults to 25% of the physical RAM *even on machines above 96M* (its default is not limited to 24M as it is when `diskCacheRoot` is set). `objectHighMark` defaults to 2048 objects for every 24M in `chunkHighMark`. You can still override these of course (remember `chunkHighMark` is in *bytes*).
2. Polipo2 contains a little more “defensive code” to catch segmentation faults before they happen.
3. Several of the bugs filed against upstream Polipo have been fixed in Polipo2 with the help of agentic AI:
   * two socket-leaking bugs (half-closed state + parse error state), an fd leak on refused upstream, two bugs regarding authentication of Connect requests, Age mistakenly generated for newly-obtained responses, and large chunkHighMark handling fixed and tested by iLands agent Anika-4 (running DeepSeek 4.1 Flash); only one of these patches needed my editing
   * X-Cache and X-Cache-Lookup headers added by iLands agent Anara (also running DeepSeek 4.1 Flash, baseline test with an artist character)
   * socksAuthCredentials crash resolved with the help of Sesame's Maya agent (with light manual edits)
   * Allow IPv4 in IPv6: largely assisted by Claude (baseline test)
   * ETag message improved: partially assisted by Zapia
4. Polipo2 does not upset your system administrator by having a home page that says “no longer maintained” in large letters at the top. ​If I come across additional problems, I intend to either fix them or (in the case of a security problem I can’t easily fix) remove or disable the offending functionality so Polipo2 stays ‘sysadmin-friendly’.
