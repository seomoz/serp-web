[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

let style = ReactDOMRe.Style.make(~width="100%", ~height="800px", ());
let layout = Cytoscape.Encode.layout("breadthfirst");

// TODO DRY up with SearchResults
type status =
  | NotStarted
  | InFlight
  | Error(string)
  | Returned(string)
  | Done;

type fetchManager = {
  status,
  graph: Graph.node,
};

[@react.component]
let make = (~search) => {
  let (manager, updateManager) =
    React.useState(() => {status: NotStarted, graph: Graph.empty});

  // TODO figure out why useEffect is causing 2 renders
  /*
   React.useEffect1(
     () => {
       Js.log(search);
       switch (search) {
       | Some(_) =>
         updateManager(_ => {status: NotStarted, graph: Graph.empty})
       | None => ()
       };

       None;
     },
     [|search|],
   );
   */

  // TODO DRY up with SearchResults
  switch (search, manager.status) {
  | (Some(search), NotStarted) =>
    updateManager(manager =>
      {status: InFlight, graph: Graph.setSearch(search, manager.graph)}
    );
    Js.Promise.(
      Fetch.fetch("/api/search?keyword=" ++ encodeURIComponent(search))
      |> then_(Fetch.Response.text)
      |> then_(results => {
           updateManager(manager => {...manager, status: Returned(results)});
           resolve();
         })
    )
    |> ignore;
  | (Some(search), Returned(results)) =>
    // TODO  Catch and return as an error in the manager
    let data = results |> Json.parseOrRaise |> Response.Decode.response;
    let graph = Graph.addResult(search, data, manager.graph);
    updateManager(_ => {status: Done, graph});
  | (_, _) => ()
  };

  switch (manager.status) {
  | Done =>
    <div>
      <p>
        <em className="title is-warning">
          {React.string("Graph explorer is still under construction")}
        </em>
      </p>
      <Cytoscape
        cy={cy =>
          Cytoscape.Instance.onNodes(
            Cytoscape.Instance.nodes(cy),
            "tap",
            event => {
              ReactEvent.Mouse.stopPropagation(event);

              let node =
                Cytoscape.Instance.id(ReactEvent.Mouse.target(event));
              updateManager(manager =>
                {
                  ...manager,
                  graph: Graph.expandNode(node, manager.graph),
                }
              );
            },
          )
        }
        elements={
          manager.graph
          |> Graph.cytoscapeElements
          |> Cytoscape.Encode.elements
          |> Cytoscape.normalizeElements
        }
        style
        layout
      />
    </div>;
  | _ => React.null
  };
};
