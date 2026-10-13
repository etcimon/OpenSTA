# Opportunities

OpenSTA was not edited on `g6lc`. The speed work is in OpenROAD and calls
into this library. These are the levers that stay accurate.

## Already used from outside

- One `findRequireds()` per ranking pass, then read path slack. Do not call
  `Sta::slack(Vertex*)` in that loop. It enters `findRequired` again.
- Endpoint WNS can live on the seed from `worstSlack`. Driver slack cannot.
  Drivers need the backward propagation.

## Worth measuring before editing

- `findRequireds` does not return early when `requireds_exist_` is set and
  the invalid set is empty. An early return there matches the Yosys empty
  dirty-set rule, but only if every editor that changes a required time
  marks the invalid set. If one editor forgets, slack goes stale. Read
  `seedInvalidRequireds` and the graph-change callbacks before adding the
  return.
- `findAllArrivals` already has a seeded fast path. Do not add a second
  arrival cache beside it.
- Tag groups grow with clocks and exceptions. A design with one clock does
  not pay for a per-clock scan inside `getPinArrival` (that scan was in
  OpenROAD and ignored the clock). Do not reintroduce it here.
- Path enumeration (`PathEnum`) is exponential in detours. `report_checks`
  without a path limit is a different cost from slack. Cap paths in the
  benchmark before attributing time to the search.

## Do not do these

- Do not swap the delay calculator to unit delay to make a benchmark faster.
  The slack number changes.
- Do not hash liberty table axes.
- Do not flatten tag groups to one arrival per vertex. Min and max, and
  multiple clocks, are separate paths.
- The files `*/test/regression` and `*/test/save_ok` show as modified on a
  Windows checkout. They are golden lists. Do not commit them with an
  architecture note.
