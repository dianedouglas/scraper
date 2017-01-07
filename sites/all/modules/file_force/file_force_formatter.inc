<?php
// $Id$

/**
 * @file
 * This file contains the specialized field formatters.
 */

/**
 * Returns HTML for a link to a file.
 *
 * @param $variables
 *   An associative array containing:
 *   - file: A file object to which the link will be created.
 *   - icon_directory: (optional) A path to a directory of icons to be used for
 *     files. Defaults to the value of the "file_icon_directory" variable.
 *
 * @ingroup themeable
 */
function theme_file_force_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = file_force_create_url($file->uri, FALSE);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  $options['query']['download'] = '1';

  return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
}

/**
 * Returns HTML for a file attachments table.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An array of file attachments.
 *
 * @ingroup themeable
 */
function theme_file_force_file_formatter_table($variables) {
  $header = array(t('Attachment'), t('Size'));
  $rows = array();
  foreach ($variables['items'] as $delta => $item) {
    $rows[] = array(
      theme('file_force_file_link', array('file' => (object) $item)),
      format_size($item['filesize']),
    );
  }

  return empty($rows) ? '' : theme('table', array('header' => $header, 'rows' => $rows));
}
