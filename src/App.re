// Looks a little funky, but this is to trick Reason into generating an import
// directive for a sass file. This is pretty much just for Webpack
[@bs.module "./styles/app.scss"] external _unused: string = "_";

[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";
[@bs.val] external decodeURIComponent: string => string = "decodeURIComponent";

type appMode =
  | Search
  | Explorer;

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let params =
    Js.String.split("&", url.search)
    |> Array.to_list
    |> List.map(Js.String.split("="));

  // TODO get a real query-string parser and move that into state
  let search =
    params
    |> List.filter(
         fun
         | [|"search", _|] => true
         | _ => false,
       )
    |> (
      fun
      | [[|_key, ""|]] => None
      | [[|_key, search|]] => Some(decodeURIComponent(search))
      | _ => None
    );

  let (mode, setMode) = React.useState(() => Search);
  React.useEffect(() =>
    params
    |> List.filter(
         fun
         | [|"mode", _|] => true
         | _ => false,
       )
    |> (
      fun
      | [[|_key, "search"|]] => {
          setMode(_ => Search);
          None;
        }
      | [[|_key, "explorer"|]] => {
          setMode(_ => Explorer);
          None;
        }
      | _ => None
    )
  );

  let (keyword, onKeywordChange) = React.useState(() => "");
  React.useEffect1(
    () =>
      switch (search) {
      | Some(search) =>
        onKeywordChange(_ => search);
        None;
      | None => None
      },
    [|search|],
  );

  let handleSearch = event => {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReactRouter.push(
      "/?search="
      ++ encodeURIComponent(keyword)
      ++ "&mode="
      ++ (
        switch (mode) {
        | Search => "search"
        | Explorer => "explorer"
        }
      ),
    );
  };

  let handleModeClick = (mode, event) => {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReactRouter.push(
      "/?search="
      ++ encodeURIComponent(keyword)
      ++ "&mode="
      ++ (
        switch (mode) {
        | Search => "search"
        | Explorer => "explorer"
        }
      ),
    );
  };

  <>
    <nav
      className="navbar is-primary"
      role="navigation"
      ariaLabel="main navigation">
      <div className="navbar-brand">
        <h1 className="title is-4"> {React.string("SERP Web")} </h1>
      </div>
      <div className="navbar-menu">
        <div className="navbar-start">
          <a
            className={"navbar-item" ++ (mode == Search ? " is-active" : "")}
            onClick={handleModeClick(Search)}>
            {React.string("Search")}
          </a>
          <a
            className={"navbar-item" ++ (mode == Explorer ? " is-active" : "")}
            onClick={handleModeClick(Explorer)}>
            {React.string("Explorer")}
          </a>
        </div>
      </div>
    </nav>
    <section className="section">
      <form className="container">
        <div className="field has-addons">
          <p className="control">
            <input
              className="input"
              type_="text"
              name="keyword"
              onChange={event =>
                onKeywordChange(ReactEvent.Form.target(event)##value)
              }
              value=keyword
            />
          </p>
          <input
            className="button is-info"
            type_="submit"
            value="Search"
            onClick=handleSearch
          />
        </div>
      </form>
    </section>
    {switch (mode) {
     | Search =>
       <section className="section">
         <div className="container"> <SearchResults search /> </div>
       </section>

     | Explorer => <div className="container"> <SearchGraph search /> </div>
     }}
  </>;
};
