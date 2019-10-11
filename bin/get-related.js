#! /usr/bin/env node

const { TYPES, fetch } = require('../lib/clients/feedstore');

const [ keyword, type ] = process.argv.slice(2);

(async () => {
  const reqs = type ?
    [ fetch({ keyword, type }) ] :
    Object.values(TYPES).map(type => fetch({ keyword, type }));

  const resp = (await Promise.all(reqs)).reduce((memo, resp) => ({
    ...memo,
    [resp.type]: resp[resp.type],
  }), {});
  console.dir(resp);
})();
