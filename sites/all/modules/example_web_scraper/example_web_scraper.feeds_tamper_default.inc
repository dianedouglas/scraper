<?php

/**
 * Implementation of hook_feeds_tamper_default().
 */
function example_web_scraper_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass;
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'Add domain';
  $feeds_tamper->importer = 'stanford_schedule_page';
  $feeds_tamper->source = 'xpathparser:2';
  $feeds_tamper->plugin_id = 'rewrite';
  $feeds_tamper->settings = array(
    'text' => 'http://events.stanford.edu/[xpathparser:2]',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'add domain';
  $export['Add domain'] = $feeds_tamper;

  return $export;
}
