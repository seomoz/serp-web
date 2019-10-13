[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

type status =
  | NotStarted
  | InFlight
  | Error(string)
  | Done(string);

[@react.component]
let make = (~search) => {
  let (status, setStatus) = React.useState(() => NotStarted);
  let (activeTab, setActiveTab) = React.useState(() => Response.Keywords);

  React.useEffect1(
    () => {
      setStatus(_ => NotStarted);
      None;
    },
    [|search|],
  );

  switch (search, status) {
  | (None, _) => ReasonReact.null
  | (Some(keyword), NotStarted) =>
    setStatus(_ => InFlight);
    Js.Promise.(
      Fetch.fetch("/api/search?keyword=" ++ encodeURIComponent(keyword))
      |> then_(Fetch.Response.text)
      |> then_(results => {
           Js.log(results);
           setStatus(_status => Done(results));
           resolve();
         })
    )
    |> ignore;
    ReasonReact.null;
  | (Some(_), InFlight) => <p> {ReasonReact.string("Loading...")} </p>
  | (Some(_), Error(error)) => <p> {ReasonReact.string(error)} </p>
  | (Some(_), Done(results)) =>
    Js.log(activeTab);
    let data = results |> Json.parseOrRaise |> Response.Decode.response;
    <>
      <div className="tabs">
        <ul>
          <li className={activeTab == Keywords ? "is-active" : ""}>
            <a onClick={_ => setActiveTab(_ => Response.Keywords)}>
              {React.string("Keywords")}
            </a>
          </li>
          <li className={activeTab == Questions ? "is-active" : ""}>
            <a onClick={_ => setActiveTab(_ => Response.Questions)}>
              {React.string("Questions")}
            </a>
          </li>
          <li className={activeTab == Searches ? "is-active" : ""}>
            <a onClick={_ => setActiveTab(_ => Response.Searches)}>
              {React.string("Searches")}
            </a>
          </li>
        </ul>
      </div>
      <div>
        {switch (activeTab) {
         | Keywords => <ResultTab entries={data.keywords} />
         | Questions => <ResultTab entries={data.questions} />
         | Searches => <ResultTab entries={data.searches} />
         }}
      </div>
    </>;
  };
};
