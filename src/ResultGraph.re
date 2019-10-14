let style = ReactDOMRe.Style.make(~width="800px", ~height="800px", ());

let makeNodes = (entry: Response.entry): list(Cytoscape.node) => {
  [
    ({
       data: {
         id: entry.topic,
         label: entry.topic,
       },
     }: Cytoscape.node),
    ...List.map(
         (related): Cytoscape.node => {
                                         data: {
                                           id: related,
                                           label: related,
                                         },
                                       },
         entry.related,
       ),
  ];
};
let makeEdge = (source: string, entry: Response.entry): Cytoscape.edge => {
  data: {
    source,
    target: entry.topic,
    label: entry.topic,
  },
};

let makeEdges = (source: string, entry: Response.entry): list(Cytoscape.edge) => {
  [
    (
      {
        data: {
          source,
          target: entry.topic,
          label: entry.topic,
        },
      }: Cytoscape.edge
    ),
    ...List.map(
         (related): Cytoscape.edge =>
           {
             data: {
               source: entry.topic,
               target: related,
               label: related,
             },
           },
         entry.related,
       ),
  ];
};

[@react.component]
let make = (~search: string, ~results: Response.response) => {
  let nodes =
    [
      [
        ({
           data: {
             id: search,
             label: search,
           },
         }: Cytoscape.node),
        ({
           data: {
             id: "keywords",
             label: "Keywords",
           },
         }: Cytoscape.node),
        ({
           data: {
             id: "questions",
             label: "Questions",
           },
         }: Cytoscape.node),
        ({
           data: {
             id: "searches",
             label: "Searches",
           },
         }: Cytoscape.node),
      ],
      List.flatten(List.map(makeNodes, results.keywords)),
      List.flatten(List.map(makeNodes, results.questions)),
      List.flatten(List.map(makeNodes, results.searches)),
    ]
    |> List.concat;
  let edges =
    [
      [
        (
          {
            data: {
              source: search,
              target: "keywords",
              label: "Keywords",
            },
          }: Cytoscape.edge
        ),
        (
          {
            data: {
              source: search,
              target: "questions",
              label: "Questions",
            },
          }: Cytoscape.edge
        ),
        (
          {
            data: {
              source: search,
              target: "searches",
              label: "Searches",
            },
          }: Cytoscape.edge
        ),
      ],
      List.flatten(List.map(makeEdges("keywords"), results.keywords)),
      List.flatten(List.map(makeEdges("questions"), results.questions)),
      List.flatten(List.map(makeEdges("searches"), results.searches)),
    ]
    |> List.concat;

  let data: Cytoscape.elements = {nodes, edges};
  let elements =
    data |> Cytoscape.Encode.elements |> Cytoscape.normalizeElements;

  let layout = Cytoscape.Encode.layout("breadthfirst");

  <Cytoscape elements style layout />;
};
