# OpenSTA architecture notes

OpenSTA is the timing engine. OpenROAD reaches it through `sta::dbSta`
(`src/dbSta` in the parent repo), not through `app/Main.cc`. The standalone
`sta` binary is `app/Main.cc` and the same libraries.

| Note | What it answers |
|---|---|
| [EXECUTION.md](EXECUTION.md) | Search, delay, and report order |
| [DATA.md](DATA.md) | Graph, tags, paths, and what is cached |
| [OPPORTUNITIES.md](OPPORTUNITIES.md) | Where a second search hides |

Parent timing notes: `AGENTS-openroad/architecture/` in the OpenROAD repo.
