[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

type status =
  | NotStarted
  | InFlight
  | Error(string)
  | Done(string);

type resultTypes =
  | Keywords
  | Questions
  | Searches;

type response = {
  keywords: list(entry),
  questions: list(entry),
  searches: list(entry),
}
and entry = {
  topic: string,
  related: list(string),
};

module Decode {
  let entry = (type_, json) => {
    let prop = switch type_ {
      | Keywords => "related_keywords"
      | Questions => "related_questions"
      | Searches => "related_searches"
    };
    Json.Decode.{
      topic: json |> field("topic", string),
      related: json |> field(prop, list(string)),
    }
  };

  let response = json =>
    Json.Decode.{
      keywords: json |> field("related_keywords", list(entry(Keywords))),
      questions: json |> field("related_questions", list(entry(Questions))),
      searches: json |> field("related_searches", list(entry(Searches))),
    };
}

let renderTab = (resultType, results) => {
  let entries = switch resultType {
    | Keywords => results.keywords
    | Questions => results.questions
    | Searches => results.searches
  };

  let rows = entries
    |> List.map(row => {
      <li>
        {React.string(row.topic)}
        <ul>
          {
            row.related
              |> List.map(related => <li>{React.string(related)}</li>)
              |> Array.of_list
              |> React.array
          }
        </ul>
      </li>
    })
    |> Array.of_list
    |> React.array;

  <div className="content">
    <ul>{rows}</ul>
  </div>
};

[@react.component]
let make = (~search) => {
  let (status, setStatus) = React.useState(() => NotStarted);
  let (activeTab, setActiveTab) = React.useState(() => Keywords);

  React.useEffect1(
    () => {
      setStatus(_ => NotStarted);
      None
    },
    [| search |]
  );

  switch (search, status) {
    | (None, _) => ReasonReact.null
    | (Some(keyword), NotStarted) => {
        setStatus(_ => InFlight)
        Js.Promise.(
          Fetch.fetch("/api/search?keyword=" ++ encodeURIComponent(keyword))
          |> then_(Fetch.Response.text)
          |> then_(results => {
            Js.log(results);
            setStatus((_status) => Done(results));
            resolve()
          })
        ) |> ignore;
        ReasonReact.null
      }
    | (Some(_), InFlight) => {
        <p>{ReasonReact.string("Loading...")}</p>
      }
    | (Some(_), Error(error)) => <p>{ReasonReact.string(error)}</p>
    | (Some(_), Done(results)) => {
      let data = results |> Json.parseOrRaise |> Decode.response;
      <>
        <div className="tabs">
          <ul>
            <li className={(activeTab == Keywords) ? "is-active" : ""}>
              <a onClick={_ => setActiveTab(_ => Keywords)}>
                {React.string("Keywords")}
              </a>
            </li>
            <li className={(activeTab == Questions) ? "is-active" : ""}>
              <a onClick={_ => setActiveTab(_ => Questions)}>
                {React.string("Questions")}
              </a>
            </li>
            <li className={(activeTab == Searches) ? "is-active" : ""}>
              <a onClick={_ => setActiveTab(_ => Searches)}>
                {React.string("Searches")}
              </a>
            </li>
          </ul>
        </div>
        <div>
          {renderTab(activeTab, data)}
        </div>
      </>
    }
  }
};
