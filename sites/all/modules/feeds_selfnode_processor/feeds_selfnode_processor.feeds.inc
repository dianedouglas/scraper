<?php

/**
 * @file
 * Feeds hooks.
 */

/**
 * Implements hook_feeds_plugins().
 */
function feeds_selfnode_processor_feeds_plugins() {
  $info = array();
  $info['FeedsSelfNodeProcessor'] = array(
    'name' => 'Self Node processor',
    'description' => 'Populate fields on a feed node.',
    'help' => 'Update the fields of a feed node from parsed content.',
    'handler' => array(
      'parent' => 'FeedsNodeProcessor',
      'class' => 'FeedsSelfNodeProcessor',
      'file' => 'FeedsSelfNodeProcessor.inc',
    ),
  );

  return $info;
}
