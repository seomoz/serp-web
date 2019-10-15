[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

type status =
  | NotStarted
  | InFlight
  | Error(string)
  | Done(string);

type dropdownState =
  | Closed
  | Open;

[@react.component]
let make = (~search) => {
  let (status, setStatus) = React.useState(() => NotStarted);
  let (selectedTopic, setSelectedTopic) = React.useState(() => None);
  let (topicDropdownState, setTopicDropdownState) =
    React.useState(() => Closed);
  React.useEffect1(
    () => {
      setStatus(_ => NotStarted);
      None;
    },
    [|search|],
  );

  let handleDropdownState = event => {
    ReactEvent.Mouse.preventDefault(event);
    setTopicDropdownState(
      fun
      | Closed => Open
      | Open => Closed,
    );
  };
  let handleTopicChange = (topic, event) => {
    ReactEvent.Mouse.preventDefault(event);
    setSelectedTopic(_ => Some(topic));
    setTopicDropdownState(_ => Closed);
  };

  let renderDropdown = (topics: list(string), selectedTopic) => {
    let dropdownClassName =
      switch (topicDropdownState) {
      | Closed => "dropdown"
      | Open => "dropdown is-active"
      };

    <div className="container">
      <div className=dropdownClassName>
        <div className="dropdown-trigger">
          <button
            ariaControls="topics-dropdown"
            className="button"
            onClick=handleDropdownState>
            <span> {React.string("Topics")} </span>
            <span className="icon is-small">
              <i className="fas fa-angle-down" ariaHidden=true />
            </span>
          </button>
        </div>
        <div className="dropdown-menu" id="topics-dropdown" role="menu">
          <div className="dropdown-content">
            {topics
             |> List.map(topic => {
                  let className =
                    switch (selectedTopic) {
                    | Some(selected) when selected == topic => "dropdown-item is-active"
                    | _ => "dropdown-item"
                    };
                  <a
                    className
                    href="#"
                    key={"topic-dropdown-" ++ topic}
                    onClick={handleTopicChange(topic)}>
                    {React.string(topic)}
                  </a>;
                })
             |> Array.of_list
             |> React.array}
          </div>
        </div>
      </div>
    </div>;
  };

  switch (search, status) {
  | (None, _) => ReasonReact.null
  | (Some(search), NotStarted) =>
    setStatus(_ => InFlight);
    Js.Promise.(
      Fetch.fetch("/api/search?keyword=" ++ encodeURIComponent(search))
      |> then_(Fetch.Response.text)
      |> then_(results => {
           setStatus(_status => Done(results));
           resolve();
         })
    )
    |> ignore;
    ReasonReact.null;
  | (Some(_), InFlight) => <p> {ReasonReact.string("Loading...")} </p>
  | (Some(_), Error(error)) => <p> {ReasonReact.string(error)} </p>
  | (Some(_keyword), Done(results)) =>
    let data = results |> Json.parseOrRaise |> Response.Decode.response;
    let topics = data |> List.map((entry: Response.entry) => entry.topic);
    let result =
      data
      |> List.filter(({topic}: Response.entry) =>
           switch (selectedTopic) {
           | Some(selected) when selected == topic => true
           | _ => false
           }
         )
      |> (
        fun
        | [result, ..._rest] => Some(result)
        | [] => None
      );
    <>
      {renderDropdown(topics, selectedTopic)}
      <hr />
      {switch (result) {
       | Some(result) => <ResultTopic result />
       | None => React.null
       }}
    </>;
  };
};
