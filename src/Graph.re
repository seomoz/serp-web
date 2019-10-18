type visibility =
  | Collapsed
  | Expanded;
type kind =
  | Topic
  | Keyword
  | Question
  | Search;

type node = {
  search: string,
  visibility,
  kind,
  children: list(node),
};

let empty = {search: "", visibility: Collapsed, kind: Topic, children: []};

let nodeOfRelated = (kind, search) => {
  search,
  visibility: Collapsed,
  kind,
  children: [],
};

let setSearch = (search, graph) => {...graph, search};

let rec addResult =
        (search: string, response: Response.response, graph: node): node =>
  if (graph.search == search) {
    {
      ...graph,
      visibility: Expanded,
      children:
        response
        |> List.map((entry: Response.entry) =>
             {
               search: entry.topic,
               visibility: Collapsed,
               kind: Topic,
               children:
                 [
                   entry.keywords |> List.map(nodeOfRelated(Keyword)),
                   entry.questions |> List.map(nodeOfRelated(Question)),
                   entry.searches |> List.map(nodeOfRelated(Search)),
                 ]
                 |> List.concat,
             }
           ),
    };
  } else {
    {
      ...graph,
      children: graph.children |> List.map(addResult(search, response)),
    };
  };

let rec expandNode = (search, graph) => {
  // TODO check for cycles
  if (graph.search == search) {
    {...graph, visibility: Expanded};
  } else {
    {...graph, children: graph.children |> List.map(expandNode(search))};
  };
};

let cytoscapeNodes = (graph: node): list(Cytoscape.node) => {
  let rec walk = (acc, node) => {
    [
      [Cytoscape.makeNode(node.search)],
      switch (node.visibility) {
      | Collapsed => []
      | Expanded => node.children |> List.map(walk([])) |> List.concat
      },
      acc,
    ]
    |> List.concat;
  };

  walk([], graph);
};

let cytoscapeEdges = (graph: node): list(Cytoscape.edge) => {
  // TODO: double-check for cycles or redundant edges
  let rec walk = (acc, source): list(Cytoscape.edge) => {
    [
      // From this node to its children
      switch (source.visibility) {
      | Collapsed => []
      | Expanded =>
        source.children
        |> List.map(target => target.search)
        |> List.map(Cytoscape.makeEdge(source.search))
      },
      // Descend into children in case they are expanded
      switch (source.visibility) {
      | Collapsed => []
      | Expanded => source.children |> List.map(walk([])) |> List.concat
      },
      acc,
    ]
    |> List.concat;
  };

  walk([], graph);
};

let cytoscapeElements = (graph): Cytoscape.elements => {
  {nodes: cytoscapeNodes(graph), edges: cytoscapeEdges(graph)};
};
