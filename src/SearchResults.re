[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

type status =
  | NotStarted
  | InFlight
  | Error(string)
  | Done(string);

[@react.component]
let make = (~search) => {
  let (status, setStatus) = React.useState(() => NotStarted);
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
    | (Some(_), Done(results)) => <pre>{ReasonReact.string(results)}</pre>
  }
};
