<?php

/**
 * Implementation of hook_feeds_importer_default().
 */
function example_web_scraper_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'stanford_event';
  $feeds_importer->config = array(
    'name' => 'stanford event',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserHTML',
      'config' => array(
        'sources' => array(
          'xpathparser:0' => 'h1',
          'xpathparser:1' => 'dl/dd[1]',
          'xpathparser:2' => 'dl/dd[2]',
          'xpathparser:3' => 'p',
        ),
        'rawXML' => array(
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:3' => 0,
        ),
        'context' => '//div[@id=\'record\']',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:3' => 0,
          ),
          'tidy_encoding' => 'UTF8',
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsSelfNodeProcessor',
      'config' => array(
        'content_type' => 'event',
        'expire' => -1,
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:0',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:1',
            'target' => 'field_when',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_where',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:3',
            'target' => 'body',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => 2,
        'input_format' => NULL,
      ),
    ),
    'content_type' => 'event',
    'update' => 0,
    'import_period' => '0',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['stanford_event'] = $feeds_importer;

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'stanford_schedule_page';
  $feeds_importer->config = array(
    'name' => 'stanford schedule page',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsCrawler',
      'config' => array(
        'auto_detect_feeds' => 0,
        'use_pubsubhubbub' => 0,
        'designated_hub' => '',
        'crawler' => array(
          'num_pages' => '5',
          'delay' => '1',
          'first_run' => '0',
          'auto' => '0',
          'xpath' => '',
          'url' => array(
            'url_pattern' => 'http://events.stanford.edu/2011/January/$index/',
            'initial' => '2',
            'increment' => '1',
          ),
        ),
        'crawled' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserHTML',
      'config' => array(
        'sources' => array(
          'xpathparser:1' => 'a',
          'xpathparser:2' => 'a/@href',
        ),
        'rawXML' => array(
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
        ),
        'context' => '//div[@class=\'eventItemText\']',
        'exp' => array(
          'errors' => 1,
          'debug' => array(
            'context' => 'context',
            'xpathparser:1' => 'xpathparser:1',
            'xpathparser:2' => 'xpathparser:2',
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'event',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:1',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'xpathparser:2',
            'target' => 'feeds_source',
            'unique' => 1,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => 'stanford_schedule_page',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['stanford_schedule_page'] = $feeds_importer;

  return $export;
}
