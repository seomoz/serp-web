# Features

- Add a "Questions/Keywords/Searches" node under each topic, nesting values
- Double-click a node to start a new search from it

# Items from Source

- src/App.re:20: // TODO get a real query-string parser and move that into state
- src/Cytoscape.re:36: // TODO variant type with supported layouts
- src/Graph.re:59: // TODO check for cycles
- src/Graph.re:84: // TODO: double-check for cycles or redundant edges
- src/SearchGraph.re:6: // TODO DRY up with SearchResults
- src/SearchGraph.re:24: // TODO figure out why useEffect is causing 2 renders
- src/SearchGraph.re:41: // TODO DRY up with SearchResults
- src/SearchGraph.re:57: // TODO Catch and return as an error in the manager
