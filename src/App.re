[@react.component]
let make = () => {
  let (keyword, onKeywordChange) = React.useState(() => "");
  let (search, setSearch) = React.useState(() => None);

  let handleSearch = event => {
    ReactEvent.Mouse.preventDefault(event);
    setSearch(_ => Some(keyword));
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
              value={keyword}
            />
          </p>
          <input
            className="button is-info"
            type_="submit"
            value="Search"
            onClick={handleSearch}
          />
        </div>
      </form>
    </section>
    <section className="section">
      <div className="container">
        <SearchResults search />
      </div>
    </section>
  </>
};
