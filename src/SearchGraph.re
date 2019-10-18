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

[@react.component]
let make = (~search) => {
  let (status, setStatus) = React.useState(() => NotStarted);
  let (graph, setSearchGraph) = React.useState(() => Graph.empty);

  React.useEffect1(
    () => {
      setStatus(_ => NotStarted);
      setSearchGraph(_ => Graph.empty);

      None;
    },
    [|search|],
  );

  // TODO DRY up with SearchResults
  switch (search, status) {
  | (None, _) => ()
  | (Some(search), NotStarted) =>
    setSearchGraph(graph => Graph.setSearch(search, graph));
    setStatus(_ => InFlight);
    Js.Promise.(
      Fetch.fetch("/api/search?keyword=" ++ encodeURIComponent(search))
      |> then_(Fetch.Response.text)
      |> then_(results => {
           setStatus(_status => Returned(results));
           resolve();
         })
    )
    |> ignore;
  | (Some(search), Returned(results)) =>
    setStatus(_ => Done);
    let data = results |> Json.parseOrRaise |> Response.Decode.response;
    Js.log("updating graph");
    setSearchGraph(graph => Graph.addResult(search, data, graph));
  | (_, _) => ()
  };

  switch (status) {
  | Done =>
    <div>
      <p>
        <em className="title is-warning">
          {React.string("Graph explorer is still under construction")}
        </em>
      </p>
      <Cytoscape
        elements={
          graph
          |> Graph.cytoscapeElements
          |> Cytoscape.Encode.elements
          |> Cytoscape.normalizeElements
        }
        style
        layout
      />
    </div>
  | _ => React.null
  };
};
