type resultTypes =
  | Graph
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

module Decode = {
  let entry = (type_, json) => {
    let prop =
      switch (type_) {
      | Graph => ""
      | Keywords => "related_keywords"
      | Questions => "related_questions"
      | Searches => "related_searches"
      };
    Json.Decode.{
      topic: json |> field("topic", string),
      related: json |> field(prop, list(string)),
    };
  };

  let response = json =>
    Json.Decode.{
      keywords: json |> field("related_keywords", list(entry(Keywords))),
      questions: json |> field("related_questions", list(entry(Questions))),
      searches: json |> field("related_searches", list(entry(Searches))),
    };
};
