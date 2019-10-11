#! /usr/bin/env node

const [ PORT ] = process.argv.slice(2);

const server = require('../lib/server');
server.start(PORT || 4000);
