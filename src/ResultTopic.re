[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

type tabs =
  | Keywords
  | Questions
  | Searches;

[@react.component]
let make = (~result: Response.entry) => {
  let (activeTab, setActiveTab) = React.useState(() => Keywords);

  let handleTabClick = (tab, event) => {
    ReactEvent.Mouse.preventDefault(event);
    setActiveTab(_ => tab);
  };

  let handleEntryClick = (search, event) => {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReactRouter.push("/?search=" ++ encodeURIComponent(search));
  };

  let entries =
    switch (activeTab) {
    | Keywords => result.keywords
    | Questions => result.questions
    | Searches => result.searches
    };

  <div className="container">
    <h2 className="title is-4"> {React.string(result.topic)} </h2>
    <div>
      <div className="tabs">
        <ul>
          <li className={activeTab == Keywords ? "is-active" : ""}>
            <a href="#" onClick={handleTabClick(Keywords)}>
              {React.string("Moz Suggested Keywords")}
            </a>
          </li>
          <li className={activeTab == Questions ? "is-active" : ""}>
            <a href="#" onClick={handleTabClick(Questions)}>
              {React.string("Related Questions")}
            </a>
          </li>
          <li className={activeTab == Searches ? "is-active" : ""}>
            <a href="#" onClick={handleTabClick(Searches)}>
              {React.string("Related Searches")}
            </a>
          </li>
        </ul>
      </div>
      <div className="content">
        <ul>
          {entries
           |> List.map(entry =>
                <li key={"related-entry" ++ entry}>
                  <a href="#" onClick={handleEntryClick(entry)}>
                    {React.string(entry)}
                  </a>
                </li>
              )
           |> Array.of_list
           |> React.array}
        </ul>
      </div>
    </div>
  </div>;
};
