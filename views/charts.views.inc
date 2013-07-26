<?php
/**
 * @file
 * Views integration with Charts.
 */

/**
 * Implementation of hook_views_plugins().
 *
 * Define charts style for Views.
 */
function charts_views_plugins() {
  $plugins = array();

  $plugins['style']['chart'] = array(
    'title' => t('Chart'),
    'help' => t('Displays the content in several Chart styles.'),
    'handler' => 'charts_plugin_style_chart',
    'uses row plugin' => FALSE,
    'uses row class' => FALSE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'uses grouping' => FALSE,
    'type' => 'normal',
  );

  return $plugins;
}
