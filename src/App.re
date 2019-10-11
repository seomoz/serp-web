[@react.component]
let make = () => {
  let (keyword, onKeywordChange) = React.useState(() => "");
  let (search, setSearch) = React.useState(() => None);

  let handleSearch = event => {
    ReactEvent.Mouse.preventDefault(event);
    setSearch(_ => Some(keyword));
  };

  <div>
    <h1>{ReasonReact.string("SERP Web")}</h1>
    <form action="">
      <input
        type_="text"
        name="keyword"
        onChange={event => onKeywordChange(ReactEvent.Form.target(event)##value)}
        value={keyword}
      />
      <input
        type_="submit"
        value="Search"
        onClick={handleSearch}
      />
    </form>
    <div><SearchResults search /></div>
  </div>
};
