type id = string;

type node = {data: nodeData}
and nodeData = {
  id,
  label: string,
};

let makeNode = id => {
  data: {
    id,
    label: id,
  },
};

type edge = {data: edgeData}
and edgeData = {
  source: id,
  target: id,
  label: string,
};

let makeEdge = (source, target) => {
  data: {
    source,
    target,
    label: target,
  },
};

type elements = {
  nodes: list(node),
  edges: list(edge),
};

// TODO variant type with supported layouts
type layout = string;

[@bs.val] [@bs.module "react-cytoscapejs"]
external normalizeElements: Js.Json.t => Js.t({..}) = "normalizeElements";

module Encode = {
  let node = (node: node) => {
    Json.Encode.(
      object_([
        (
          "data",
          object_([
            ("id", string(node.data.id)),
            ("label", string(node.data.label)),
          ]),
        ),
      ])
    );
  };

  let edge = (edge: edge) => {
    Json.Encode.(
      object_([
        (
          "data",
          object_([
            ("source", string(edge.data.source)),
            ("target", string(edge.data.target)),
            ("label", string(edge.data.label)),
          ]),
        ),
      ])
    );
  };

  let elements = (elements: elements) => {
    Json.Encode.(
      object_([
        (
          "nodes",
          List.map(node, elements.nodes)
          |> Array.of_list
          |> Json.Encode.jsonArray,
        ),
        (
          "edges",
          List.map(edge, elements.edges)
          |> Array.of_list
          |> Json.Encode.jsonArray,
        ),
      ])
    );
  };

  let layout = (layout: layout) => {
    Json.Encode.(object_([("name", string(layout))]));
  };
};

[@bs.module "react-cytoscapejs"] [@react.component]
external make:
  (~elements: Js.t({..}), ~style: ReactDOMRe.style, ~layout: Js.Json.t) =>
  React.element =
  "default";
