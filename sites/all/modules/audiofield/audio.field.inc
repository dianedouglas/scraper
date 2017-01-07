<?php
/**
 * @file
 * Implement an audio field, based on the file module's file field.
 */

/**
 * Implements hook_field_prepare_view().
 */
function audiofield_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  //TODO: Check this:
  //  Remove files specified to not be displayed.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (!file_field_displayed($item, $field)) {
        unset($items[$id][$delta]);
      }
      // Ensure consecutive deltas.
      $items[$id] = array_values($items[$id]);
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function audiofield_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}

/**
 * Implements hook_field_widget_info().
 */
function audiofield_field_widget_info() {
  return array(
    'audiofield_widget' => array(
      'label' => t('Audio Upload'),
      'field types' => array('file'),
      'settings' => array(
        'progress_indicator' => 'throbber',
        //'file_extensions' => 'mp3',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function audiofield_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form['progress_indicator'] = array(
    '#type' => 'radios',
    '#title' => t('Progress indicator'),
    '#options' => array(
      'throbber' => t('Throbber'),
      'bar' => t('Bar with progress meter'),
    ),
    '#default_value' => $settings['progress_indicator'],
    '#description' => t('The throbber display does not show the status of uploads but takes up less space. The progress bar is helpful for monitoring progress on large uploads.'),
    '#weight' => 16,
    '#access' => file_progress_implementation(),
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function audiofield_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Add display_field setting to field because file_field_widget_form() assumes it is set.
  $field['settings']['display_field'] = 0;

  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  $settings = $instance['settings'];

  foreach (element_children($elements) as $delta) {
    $elements[$delta]['#process'] = array_merge(element_info_property('managed_file', '#process', array()), array('audiofield_field_widget_process'));
  }
  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array(
        'description' => $instance['description'],
        'upload_validators' => $elements[0]['#upload_validators']
      ));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }

  return $elements;
}

/**
 * An element #process callback for the audiofield_widget field type.
 *
 * Display audio player in node edit mode
 */
function audiofield_field_widget_process($element, &$form_state, $form) {
  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;
  
  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];
  $field = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);
   // Add the display field if enabled.
  if (!empty($field['settings']['display_field']) && $item['fid']) {
    $element['display'] = array(
      '#type' => empty($item['fid']) ? 'hidden' : 'checkbox',
      '#title' => t('Include file in display'),
      '#value' => isset($item['display']) ? $item['display'] : $field['settings']['display_default'],
      '#attributes' => array('class' => array('file-display')),
    );
  }
  else {
    $element['display'] = array(
      '#type' => 'hidden',
      '#value' => '1',
    );
  }
  if ($fid && $element['#file']) {
    $audiofile = file_create_url($element['#file']->uri);
    $info = pathinfo($element['#file']->uri);
    $op = $info['extension'];
    $element['filename'] = array(
      '#type' => 'markup',
      '#markup' => audiofield_get_player($audiofile, $op),
      '#weight' => -10,
    );
  }
  
  // Add the description field if enabled.
  if (!empty($instance['settings']['description_field']) && $item['fid']) {
    $element['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#value' => isset($item['description']) ? $item['description'] : '',
      '#type' => variable_get('file_description_type', 'textfield'),
      '#maxlength' => variable_get('file_description_length', 128),
      '#description' => t('The description may be used as the label of the link to the file.'),
    );
  }
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function audiofield_field_formatter_info() {
  $formatters = array(
    'audiofield_embedded' => array(
      'label' => t('Audio player with download'),
      'field types' => array('file'),
      'description' => t('Displays an audio player and optional download link.'),
    ),
    'audiofield_nodownload' => array(
      'label' => t('Audio player only'),
      'field types' => array('file'),
      'description' => t('Displays only an audio player.'),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_view().
TODO: Can implement playlists to group audios hold in multiple valued fields
 */
function audiofield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $elements = array();

  switch ($display['type']) {
    case 'audiofield_embedded' :
    case 'audiofield_nodownload':
      global $user;
      foreach ($items as $delta => $item) {
        $audiofile = file_create_url($item['uri']);
        $info = pathinfo($item['uri']);
        $op = $info['extension'];
        $options = array(
          'entity_type' => $entity_type,
          'entity' => $entity,
          'field' => $field,
          'instance' => $instance,
          'langcode' => $langcode,
          'items' => $items,
          'display' => $display,
        );

        $download_link = '';
        if($display['type'] == 'audiofield_embedded') {
          if ($user->uid == $item['uid'] && user_access('download own audio files')) {
            $download_link .= '<div class="audio-download"><b>' . t('Download') . ':</b>' . theme('file_link', array('file' => (object) $item)) . '</div>';
          }
          elseif (user_access('download all audio files')) {
            //If user has permission to download all audio files, then display download link
            $download_link .= '<div class="audio-download"><b>' . t('Download') . ':</b>' . theme('file_link', array('file' => (object) $item)) . '</div>';
          }
        }
        $elements[] = array('#markup' => audiofield_get_player($audiofile, $op, $options).$download_link);
      }
      break;
  }

  return $elements;
}
