# Data and cost

## Graph

`Graph` vertices are pins. Edges are cell timing arcs or wires.
`Levelize` assigns an integer level. Arrival propagation visits levels in
order, so each vertex is updated after its fanin. That is O(edges) per full
propagation, not O(edges × depth), as long as the invalid set does not
force a rebuild.

## Tags and paths

A path is a `Tag` plus an arrival. Tags carry clock, rise/fall, and
exception state (`search/Tag.cc`, `TagGroup.cc`). Vertices hold a tag group,
not one arrival. `vertexSlack` in OpenROAD walks every path on the vertex
and keeps the minimum. The walk is O(paths on that vertex). It is cheap
next to `findRequired`.

Scenes (`search/Scene.cc`) are the analysis points (corner × mode). A slack
query filters paths to `Sta::scenes()`.

## Invalid sets

Arrivals and requireds have seeded and exists bits plus invalid seeds
(`seedInvalidArrivals`, `seedInvalidRequireds`). A full `findRequireds()`
still runs the backward visitor even when requireds exist. Calling it once
per repair pass is the intended use. Calling it once per vertex is the
pattern these notes exist to prevent.

## Network

`Network` is an abstract pin/net/instance API (`network/Network.cc`).
`ConcreteNetwork` owns library objects. `dbNetwork` in the parent repo
implements the design side with pointer tags. `isDriver` is
`isLeaf && direction->isAnyOutput()` for an instance, and
`isTop && direction->isAnyInput()` for a top port. Power and ground
directions are not drivers.

## Delay tables

A liberty table lookup is a binary search on axis values plus an
interpolation. It is not a scan of every slew in the library. Do not replace
it with a hash of floating-point keys. The axis is ordered and the result
is the interpolated point, not a map hit.

## Checks

`CheckSlews`, `CheckCapacitances`, and `CheckFanouts` walk drivers after
the graph exists. They are reports. They are not inside the arrival visitor.
