# serp-web

SERP Web is an experimental proof-of-concept UI for the Feedstore related
entities API.

To use it, simply enter a search to explore all the related entities Feedstore
brings back a set of "topics" representing common themes from the SERP and
related searches.

Each topic has:

- Keywords: suggested related keywords from Moz corpus
- Questions: common search questions related to the topic extracted from SERP
  data
- Searches: common related searches related to the topic extracted from SERP
  data

The app has two views:

- A tab-and-links view on all the relationships returned from Feedstore.
  Clicking a related item starts a new search from that term
- An experimental graph view that visualizes the relationships and progressively
  reveals the data as you interact

The front-end is built with
[ReasonReact](https://reasonml.github.io/reason-react/en/). This means you'll
have to learn [ReasonML](https://reasonml.github.io/), but it's worth it.

The back-end serves as a thin HTTP API wrapper and an asset server -- it is
about as unfancy as anything you've seen.

# Setting up

Reason's compiler generates JavaScript from `*.re` files. From there we need
help stitching everything together into a bundle for the browser. Think of how
Babel and Webpack work together, and you've got the right idea.

First get your dependencies with `yarn install` (or `npm install` if that's how
you roll).

From there, open 3 CLI tabs to get up and running:

- `./bin/server.js [PORT]` to start the web server. `PORT` is optional and
  defaults to `4000`
- `yarn app:watch` to run the Reason compiler. You'll get lots of help here from
  the compiler figuring out bugs and refactoring code
- `yarn app:webpack` to run Webpack in watch mode

Open your browser to `localhost:4000` -- or whatever port you chose -- to get
into the app.

## Development and Editor Settings

The [Reason Documentation](https://reasonml.github.io/docs/en/editor-plugins)
includes instructions for setting up your editor. Focus on "BuckleScript
Development" and ignore "Native Development" for this project.

Please use `refmt` on your files. For Vim, these bindings will give you nice
integration to format your code and provide type hints and documentation if you
install [vim-reason-plus](https://github.com/reasonml-editor/vim-reason-plus):

```
nnoremap <silent> K :call LanguageClient_textDocument_hover()<CR>
nnoremap <silent> gd : call LanguageClient_textDocument_definition()<CR>
nnoremap <silent> <F2> :call LanguageClient_textDocument_rename()<CR>
nnoremap <silent> gf :call LanguageClient_textDocument_formatting()<cr>
```

## Build for Production

```sh
yarn run app:build:production
```
