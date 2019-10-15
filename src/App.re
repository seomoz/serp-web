[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";
[@bs.val] external decodeURIComponent: string => string = "decodeURIComponent";

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let search =
    Js.String.split("&", url.search)
    |> Array.to_list
    |> List.map(Js.String.split("="))
    |> List.filter(
         fun
         | [|"search", _|] => true
         | _ => false,
       )
    |> (
      fun
      | [[|_key, search|]] => Some(decodeURIComponent(search))
      | _ => None
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
      "/?search=" ++ encodeURIComponent(keyword),
    );
  };

  <>
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
    <section className="section">
      <div className="container"> <SearchResults search /> </div>
    </section>
  </>;
};
