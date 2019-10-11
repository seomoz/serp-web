const request = require('request-promise-native');

const HOST = 'http://stage.roger.dal.moz.com';
const BASE_URL = '/feedstore-prod/related';
const LOCALE = 'en-US';

const TYPES = {
  QUESTIONS: 'related_questions',
  SEARCHES: 'related_searches',
  TOPICS: 'related_topics',
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
 *  related_searchs: [
 *    counts: {},
 *    topic: string,
 *    related_searches: []
 *  ]
 * }
 *
 * Related Topics: {
 *  related_topics: [
 *    counts: {},
 *    topic: string,
 *    related_topics: []
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
  return request(options({ keyword, type }));
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

module.exports = {
  TYPES,
  fetch,
  fetchTypes,
}
