<?php

/**
 * @file
 * Contains FeedsSelfNodeProcessor.
 */

/**
 * Allows a feed node to populate fields on itself.
 */
class FeedsSelfNodeProcessor extends FeedsNodeProcessor {

  /**
   * Overrides parent::process().
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result) {
    // We need one item, no need for while loop.
    $item = $parser_result->shiftItem();

    // We know node exists because it is the source of this import.
    $nid = $source->feed_nid;
    $skip_existing = $this->config['update_existing'] == FEEDS_SKIP_EXISTING;

    // If we are not updating, bail.
    if ($skip_existing) {
      return;
    }

    $hash = $this->hash($item);

    // Do not proceed if the item has not changed, and we're not forcing
    // the update.
    if ($hash === $this->getHash($nid) && !$this->config['skip_hash_check']) {
      // The node has already been loaded, so this is free.
      $node = node_load($nid);
      drupal_set_message(t('%title has not changed.', array('%title' => $node->title)));
      return;
    }

    try {
      // Load entity.
      $node = $this->entityLoad($source, $nid);
      $this->newItemInfo($node, $node->nid, $hash);

      // Set property and field values.
      $this->map($source, $parser_result, $node);
      $this->entityValidate($node);

      // Allow modules to alter the entity before saving.
      module_invoke_all('feeds_presave', $source, $node, $item);
      if (module_exists('rules')) {
        rules_invoke_event('feeds_import_' . $source->importer()->id, $node);
      }

      // Enable modules to skip saving at all.
      if (!empty($node->feeds_item->skip)) {
        return;
      }

      // This will throw an exception on failure.
      $this->entitySaveAccess($node);

      // Remove Feeds tracking of this node.
      unset($node->feeds_item);

      $this->entitySave($node);

      $this->updateSelfNodeItemInfo($node, $hash);

      // Set message.
      drupal_set_message(t('Updated %title', array('%title' => $node->title)));
      $source->log('import', 'Updated %title', array('%title' => $node->title), WATCHDOG_INFO);
    }
    // Something bad happened, log it.
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'warning');
      $message = $this->createLogMessage($e, $node, $item);
      $source->log('import', $message, array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Updates {feeds_selfnode_processor_item}.
   *
   * @param object $node
   *   The node being updated.
   * @param string $hash
   *   The hash of the feed item.
   */
  protected function updateSelfNodeItemInfo(stdClass $node, $hash) {
    $insert = !db_query("SELECT 1 FROM {feeds_selfnode_processor_item} WHERE feed_nid = :nid", array(':nid' => $node->nid))->fetchField();

    $item = new stdClass();
    $item->feed_nid = $node->nid;
    $item->id = $this->id;
    $item->imported = REQUEST_TIME;
    $item->hash = $hash;
    if ($insert) {
      drupal_write_record('feeds_selfnode_processor_item', $item);
    }
    else {
      drupal_write_record('feeds_selfnode_processor_item', $item, 'feed_nid');
    }
  }

  /**
   * Overrides parent::getHash().
   */
  protected function getHash($nid) {
    return (string) db_query("SELECT hash FROM {feeds_selfnode_processor_item} WHERE feed_nid = :nid", array(':nid' => $nid))->fetchField();
  }

  /**
   * Overrides parent::hasConfigForm().
   */
  public function hasConfigForm() {
    return TRUE;
  }

  /**
   * Overrides parent::configDefaults().
   */
  public function configDefaults() {
    $defaults = parent::configDefaults();
    $defaults['update_existing'] = FEEDS_UPDATE_EXISTING;
    $defaults['expire'] = FEEDS_EXPIRE_NEVER;
    $defaults['import_on_create'] = FALSE;
    return $defaults;
  }

  /**
   * Overrides parent::getConfig().
   */
  public function getConfig() {
    $config = parent::getConfig();
    // Backwards compatible with older feeds versions.
    $config['content_type'] = $this->bundle();
    return $config;
  }

  /**
   * Overrides parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['update_non_existent']['#access'] = FALSE;
    $form['content_type']['#access'] = FALSE;
    $form['bundle']['#access'] = FALSE;

    $form['import_on_create'] = array(
      '#type' => 'checkbox',
      '#title' => t('Import on create'),
      '#description' => t('Check if import should be started as soon as the node is created.'),
      '#default_value' => $this->config['import_on_create'],
    );

    if (!$this->bundle()) {
      drupal_set_message(t('This importer must be attached to a <a href="@link">content type</a> for the Self Node Processor to work.', array('@link' => url('admin/structure/feeds/' . $this->id . '/settings'))), 'error', FALSE);
    }
    return $form;
  }

  /**
   * Overrides parent::bundle().
   */
  public function bundle() {
    return feeds_importer($this->id)->config['content_type'];
  }

  /**
   * Overrides parent::getMappingTargets().
   */
  public function getMappingTargets() {
    if (!$this->bundle()) {
      return array('' => array());
    }

    // Remove useless mappings.
    $mappings = parent::getMappingTargets();
    unset($mappings['url'], $mappings['guid'], $mappings['nid']);

    // No need for any uniqueness.
    foreach ($mappings as $key => $mapping) {
      unset($mappings[$key]['optional_unique']);
    }

    return $mappings;
  }

}
