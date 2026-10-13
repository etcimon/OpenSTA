# Execution path

Tcl commands are registered from `tcl/Sta.tcl` and the `*.tcl` files next to
each library. `sta::define_sta_cmds` is what OpenROAD evals after its own
commands exist.

A report or a slack query goes through `search/Sta.cc`:

1. `searchPreamble` makes sure the graph, clocks, and exceptions exist.
2. `Search::findAllArrivals` (`search/Search.cc`) propagates arrivals
   forward by level. If arrivals are already seeded it only visits invalid
   arrivals, then a parallel level visitor.
3. Required times are not filled by the arrival pass. `findRequireds()`
   seeds every endpoint (`seedRequireds` / `VisitPathEnds`) and propagates
   backward from level 0. `findRequired(vertex)` does that again down to
   that vertex's level, or only `seedRequired` when the vertex is an
   endpoint with no fanout.
4. `slack(vertex)` calls `findRequired(vertex)` and then walks
   `VertexPathIterator`, keeping the worst path whose min/max and scene
   match.
5. `worstSlack` uses `wnsTnsPreamble`, which seeds endpoint requireds and
   does not by itself push requireds back to every driver.
6. `report_checks` enumerates path ends (`PathEnum`, `ReportPath`) after
   the same search.

Delay numbers on an arc come from `dcalc/GraphDelayCalc.cc`. The calculator
is selected from the liberty model: table (`DmpDelayCalc`), CCS, Arnoldi,
Prima, or a lumped cap. Changing calculator changes the slack number. That
is not a free speedup.

Liberty is parsed by `liberty/LibertyLex.ll` into `LibertyCell` /
`TimingArc` / `TableModel`. The graph builder turns timing arcs into
`graph/Graph` edges and wire edges between connected pins.

SDC is `sdc/Sdc.cc`: clocks, input and output delays, exceptions.
`read_sdc` does not time the design. The first slack query does.

Verilog in this library (`verilog/VerilogReader.cc`) builds a
`ConcreteNetwork`. OpenROAD's gate netlist does not use that reader for the
block. `dbNetwork` is the network. This reader still matters for the
standalone `sta` binary and for liberty test cells.
