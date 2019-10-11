const fs = require('fs');
const http = require('http');
const path = require('path');
const { URLSearchParams } = require('url');

const feedstore = require('./clients/feedstore');

const fsroot = `${__dirname}/..`;
console.log({ fsroot });

const server = http.createServer(async (req, res) => {
  const { url, method } = req;
  console.log({ url, method });

  try {
    switch (true) {
      case url === "index.html" || url === "/": {
        return fs.createReadStream(path.join(fsroot, 'build/index.html')).pipe(res);
      }

      case /^\/public/.test(url): {
        if (/\.js$/.test(url)) { res.setHeader('Content-Type', 'application/json'); }
        return fs.createReadStream(
          url.replace('\/public\/', path.join(fsroot, 'build/'))
        ).
          pipe(res).
          on('error', (err) => {
            res.statusCode = 404;
            res.write(JSON.stringify({ message: err.message, detail: err }, null, 2));
            res.end();
          });
      }

      case /^\/api\/search/.test(url): {
        try {
          const params = new URLSearchParams(url.split('?')[1]);
          const keyword = params.get('keyword');
          const type = params.get('type');

          if (!keyword) {
            res.statusCode = 400;
            res.setHeader('Content-Type', 'application/json');
            res.write(JSON.stringify({ message: 'keyword required' }));
            res.end();
          }

          const result = await feedstore.fetchTypes({ keyword, type });
          res.setHeader('Content-Type', 'application/json');
          res.write(JSON.stringify(result, null, 2));
        } catch (err) {
          console.log({ log: 'search error', err });
        } finally {
          return res.end();
        }
      }

      default: {
        res.statusCode = 404;
        res.write('Not Found');
        res.end();
      }
    }
  } catch (err) {
    res.statusCode = 500;
    res.write(JSON.stringify({ message: err.message, detail: err }, null, 2));
    res.end();
  }
});

server.on('clientError', (err, socket) => {
  socket.end('HTTP/1.1 400 Bad Request\r\n\r\n');
});

module.exports.start = (port) => {
  server.listen(port);
  console.log(`listening on :${port}`);
};
