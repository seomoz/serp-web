type drawerState =
  | Open
  | Closed;

module Map = Belt.Map.String;

[@react.component]
let make = (~entries) => {
  let (drawers, setDrawers) = React.useState(() => Map.empty);

  let setSearch = (search, event) => {
    ReactEvent.Mouse.preventDefault(event);
    // TODO Update nav location to start a new search
    Js.log(search);
  };

  let toggleDrawer = (topic, event) => {
    ReactEvent.Mouse.preventDefault(event);
    setDrawers(_ =>
      switch (Map.get(drawers, topic)) {
      | None => Map.set(drawers, topic, Open)
      | Some(Open) => Map.set(drawers, topic, Closed)
      | Some(Closed) => Map.set(drawers, topic, Open)
      }
    );
  };

  let drawerControl = (topic, status) => {
    switch (status) {
    | Some(Open) =>
      <a onClick={toggleDrawer(topic)}> {React.string("[-]")} </a>
    | Some(Closed) =>
      <a onClick={toggleDrawer(topic)}> {React.string("[+]")} </a>
    | None => <a onClick={toggleDrawer(topic)}> {React.string("[+]")} </a>
    };
  };

  let renderEntry = entry => {
    let {topic, related}: Response.entry = entry;
    let status = Map.get(drawers, topic);
    let topics =
      switch (status) {
      | None => React.null
      | Some(Closed) => React.null
      | Some(Open) =>
        <ul>
          {related
           |> List.map(related =>
                <li key={related}>
                  <a href="#" onClick={setSearch(related)}>
                    {React.string(related)}
                  </a>
                </li>
              )
           |> Array.of_list
           |> React.array}
        </ul>
      };

    <li key={topic}>
      <span>
        {drawerControl(topic, Map.get(drawers, topic))}
        {React.string(" " ++ topic)}
      </span>
      topics
    </li>;
  };

  let rows = entries |> List.map(renderEntry) |> Array.of_list |> React.array;

  <div className="content"> <ul> rows </ul> </div>;
};
