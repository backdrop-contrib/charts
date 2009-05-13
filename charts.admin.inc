<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Settings function for Charts module.
 */

/**
 * Subform that integartes Color module with Charts settings. Its called
 * inside the Charts settings page, by _charts_settings().
 *
 * @ingroup from
 */
function _charts_color_form(&$form, $settings) {
  // The Color module integration starts with a field set
  $form['color'] = array(
    '#attributes'     => array('id' => 'charts_color', 'class' => ' clear-block'),
    '#collapsible'    => TRUE,
    '#type'           => 'fieldset',
    '#theme'          => 'charts_settings_color',
    '#title'          => t('Color'),
  );

  // Load some color palettes
  $form['color']['scheme'] = array(
    '#type'           => 'select',
    '#title'          => t('Color set'),
    '#options'        => $settings['#color_palettes'],
    '#default_value'  => $settings['#color_palette'],
  );

  // Add palette fields. Since all
  $color_palette = explode(',', $settings['#color_palette']);

  $names = array(
    t('Color %number', array('%number' => 1)),
    t('Color %number', array('%number' => 2)),
    t('Color %number', array('%number' => 3)),
    t('Color %number', array('%number' => 4)),
    t('Color %number', array('%number' => 5)),
    t('Color %number', array('%number' => 6)),
    t('Color %number', array('%number' => 7)),
    t('Color %number', array('%number' => 8)),
  );
  $form['color']['palette'] = array(
    '#prefix'           => '<div id="preview"></div><div id="farbtastic"></div><div id="palette" class="clear-block">',
    '#suffix'           => '</div>',
    '#tree'             => TRUE,
  );
  foreach ($names as $color => $value) {
    $form['color']['palette']['color'. $color] = array(
      '#type'           => 'textfield',
      '#title'          => $value,
      '#default_value'  => empty($color_palette[$color]) ? '336699' : $color_palette[$color],
      '#size'           => 8,
    );
    $color_palette['color'. $color] = $color_palette[$color];
    unset($color_palette[$color]);
  }
  $form['color']['farbtastic'] = array(
    '#prefix'          => '<div id="farbtastic">',
    '#sufix'           => '</div>',
  );

  // Add the necessary JS data into the page
  drupal_add_js(array('charts_color' => array('reference' => $color_palette)), 'setting');
}

/**
 * Generate a generic chart example
 *
 * @param $data
 *   Array (optional). Chart data, following the Charts Schema
 * @return
 *   HTML. The generic Chart example
 */
function _charts_example($data = array()) {
  if (empty($data)) {
    // This will hold the example chart
    // Since the chart is an example, we should provide
    // and example data.
    $data[] = array(
      '#legend'     => 'Profit',
      array('#value' => 35, '#label' => t('Jan')),
      array('#value' => 25, '#label' => t('Feb')),
      array('#value' => 75, '#label' => t('Mar')),
      array('#value' => 50, '#label' => t('Apr')),
      array('#value' => 23, '#label' => t('May')),
      array('#value' => 12, '#label' => t('Jun')),
    );
    $data[] = array(
      '#legend'     => 'Revenue',
      10, 20, 55, 72, 45, 50
    );
    $data['#title'] = 'Testing Chart';
  }
  return charts_chart($data);
}

/**
 * Invoke a hook in all enabled modules that implement it.
 *
 * Its copy of module_invoke_all()
 * @see module_invoke_all()
 */
function _charts_module_invoke_all() {
  $args = func_get_args();
  $hook = $args[0];
  unset($args[0]);
  $return = array();
  foreach (module_implements($hook) as $module) {
    $function = $module .'_'. $hook;
    $result = call_user_func_array($function, $args);
    if (isset($result) && is_array($result)) {
      $return += $result;
    }
    elseif (isset($result)) {
      $return[] = $result;
    }
  }

  return $return;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings_form(&$form, $default = array(), $example = FALSE, $color_full = FALSE, $use_default = FALSE) {
  module_load_include('inc', 'charts');
  $default = $default + _charts_settings();

  if ($example) {
    $form['chart_example'] = array(
      '#value'          => _charts_example(),
    );
  }

  asort($default['plugins']);
  $form['plugin'] = array(
    '#default_value'  => $default['plugin'],
    '#options'        => $default['plugins'],
    '#type'           => 'select',
    '#title'          => t('Chart plugin'),
  );

  asort($default['types']);
  $form['type'] = array(
    '#default_value'  => $default['type'],
    '#options'        => $default['types'],
    '#type'           => 'radios',
    '#title'          => t('Chart type'),
  );

  $form['width'] = array(
    '#default_value'  => $default['width'],
    '#description'    => t('The chart width, in pixels'),
    '#size'           => 8,
    '#type'           => 'textfield',
    '#title'          => t('Width'),
  );
  $form['height'] = array(
    '#default_value'  => $default['height'],
    '#description'    => t('The chart height, in pixels'),
    '#size'           => 8,
    '#type'           => 'textfield',
    '#title'          => t('Height'),
  );

  if ($color_full) {
    _charts_color_form($form, $default);
  }
  else {
    $form['color_palette'] = array(
      '#type'           => 'select',
      '#title'          => t('Color set'),
      '#options'        => $default['color_palettes'],
      '#default_value'  => $default['color_palette'],
    );
  }

  return $form;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings_page() {
  $form = array();
  _charts_settings_form($form, array(), TRUE, TRUE);

  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => t('Save settings'),
  );

  return $form;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings_page_submit(&$form, &$form_state) {
  $settings = $form_state['values'];

  // Discart all form values non related to charts
  unset($settings['submit']);
  unset($settings['form_id']);
  unset($settings['form_build_id']);
  unset($settings['form_token']);
  unset($settings['op']);

  // Save the Color palette. Firts, filter the data, cluster the results
  // into a single string. Finally, unset all field values, since they
  // are useless
  $settings['color_palette'] = implode(',', $settings['palette']);
  unset($settings['palette']);
  unset($settings['scheme']);

  // Save the data into database
  variable_set('charts_settings', $settings);
}

/**
 * Theme color form.
 *
 * @ingroup @themeable
 */
function theme_charts_settings_color($form) {
  // Add Farbtastic color picker JS
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'charts') .'/charts_color.js');

  // Add the necessary CSS
  drupal_add_css(drupal_get_path('module', 'charts') .'/charts_color.css');

  // Preview and the rest of the form
  return '<div id="preview"></div>'. drupal_render($form);
}
