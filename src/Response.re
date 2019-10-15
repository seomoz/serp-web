type resultTypes =
  | Graph
  | Keywords
  | Questions
  | Searches;

type response = list(entry)
and entry = {
  topic: string,
  keywords: list(string),
  questions: list(string),
  searches: list(string),
};

module Decode = {
  let entry = (json) => {
    Json.Decode.{
      topic: json |> field("topic", string),
      keywords: json |> field("keywords", list(string)),
      questions: json |> field("questions", list(string)),
      searches: json |> field("searches", list(string)),
    };
  };

  let response = json : response => {
    json |> Json.Decode.list(entry)
  }
};
