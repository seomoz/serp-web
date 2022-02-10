const request = require('request-promise-native');

const HOST = 'http://superfeedstore.data-platform-prod.aws.moz.com';
const BASE_URL = '/superfeedstore/related';
const LOCALE = 'en-US';

const TYPES = {
  QUESTIONS: 'related_questions',
  SEARCHES: 'related_searches',
  KEYWORDS: 'related_keywords',
};

/* Example Queries
 curl -vvv -d '{"keyword":"data science","locale":"en-US","cache":"true","type":"related_searches"}' -H "Content-Type: application/json" -X POST stage.roger.dal.moz.com/feedstore-prod/related | jq . > searches_data.json
 curl -vvv -d '{"keyword":"data science","locale":"en-US","cache":"true","type":"related_questions"}' -H "Content-Type: application/json" -X POST stage.roger.dal.moz.com/feedstore-prod/related | jq . > questions_data.json
 curl -vvv -d '{"keyword":"data science","locale":"en-US","cache":"true","type":"related_topics"}' -H "Content-Type: application/json" -X POST stage.roger.dal.moz.com/feedstore-prod/related | jq . > topics_data.json
 */

/*
 * Related Questions: {
 *  related_questions: [
 *    counts: {},
 *    topic: string,
 *    related_questions: []
 *  ]
 * }
 *
 * Related Searches: {
 *  related_searches: [
 *    counts: {},
 *    topic: string,
 *    related_searches: []
 *  ]
 * }
 *
 * Related Keywords: {
 *  related_keywords: [
 *    counts: {},
 *    topic: string,
 *    related_keywords: []
 *  ]
 * }
 *
 */

const options = ({ keyword, type }) => ({
  baseUrl: HOST,
  method: 'POST',
  url: BASE_URL,
  json: {
    keyword,
    type,
    locale: LOCALE,
    cache: true,
  }
});

const fetch = async ({ keyword, type }) => {
  try {
    return request(options({ keyword, type }));
  } catch (err) {
    console.log({ log: `Feedstore Request Error`, err, options: options({ keyword, type })})
    throw err;
  }
};

const fetchTypes = async ({ keyword, type }) => {
  const reqs = type ?
    [ fetch({ keyword, type }) ] :
    Object.values(TYPES).map(type => fetch({ keyword, type }));

    return (await Promise.all(reqs)).reduce((memo, resp) => ({
      ...memo,
      [resp.type]: resp[resp.type],
    }), {});
};

const fetchResultsByTopic = async ({ keyword }) => {
  const reqs = Object.values(TYPES).map(type => fetch({ keyword, type }));
  const resp = await Promise.all(reqs);

  const results = {};
  for (const r of resp) {
    let entries;
    switch (r.type) {
      case 'related_keywords': { entries = r.related_keywords; break; }
      case 'related_questions': { entries = r.related_questions; break; }
      case 'related_searches': { entries = r.related_searches; break; }
      default: entries = [];
    }

    for (const entry of entries) {
      const { topic } = entry;
      if (!(topic in results)) {
        results[topic] = { keywords: new Set(), questions: new Set(), searches: new Set() };
      }
      switch (r.type) {
        case 'related_keywords': {
          entry.related_keywords.forEach(entry => results[topic].keywords.add(entry));
          break;
        };
        case 'related_questions': {
          entry.related_questions.forEach(entry => results[topic].questions.add(entry));
          break;
        };
        case 'related_searches': {
          entry.related_searches.forEach(entry => results[topic].searches.add(entry));
          break;
        };
      }
    }
  }

  for (const topic in results) {
    results[topic].keywords = Array.from(results[topic].keywords);
    results[topic].questions = Array.from(results[topic].questions);
    results[topic].searches = Array.from(results[topic].searches);
  }
  return Object.keys(results).map(topic => ({
    topic,
    ...results[topic],
  }));
};

module.exports = {
  TYPES,
  fetch,
  fetchTypes,
  fetchResultsByTopic,
};
