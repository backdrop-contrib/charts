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
function _charts_color_form_complete(&$form, $default) {
  // Use ColorPicker module if possible
  $field_type = module_exists('colorpicker') ? 'colorpicker_textfield' : 'textfield';

  $form['color'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Color'),
    '#tree'           => TRUE
  );
  $form['color']['color_palettes'] = array(
    '#type'           => 'select',
    '#title'          => t('Color palettes'),
    '#options'        => $default['#color_palettes'],
    '#default_value'  => $default['#color_palette'],
  );
  $form['color']['background'] = array(
    '#type'           => $field_type,
    '#title'          => t('Background'),
    '#default_value'  => $default['#color']['background'],
//     '#colorpicker'    => 'colorpicker',
  );
  $form['color']['text'] = array(
    '#type'           => $field_type,
    '#title'          => t('Text'),
    '#default_value'  => $default['#color']['text'],
  );
  for ($series = 0; $series < 8; $series++) {
    $form['color'][$series] = array(
      '#type'           => $field_type,
      '#title'          => t('Series %serie', array('%serie' => $series + 1)),
      '#default_value'  => $default['#color'][$series],
    );
  }

  drupal_add_js(drupal_get_path('module', 'charts') .'/charts_color.js');
  drupal_add_js(array('chartsColorCustom' => t('Custom')), 'setting');
}

/**
 * Subform that use simple color inputs.
 *
 * @ingroup from
 */
function _charts_color_form_simple(&$form, $settings) {
  // Load some color palettes
  $form['color_palettes'] = array(
    '#type'           => 'select',
    '#title'          => t('Color palettes'),
    '#options'        => $settings['#color_palettes'],
    '#default_value'  => $settings['#color_palette'],
  );
}

/**
 * Generate a generic chart example
 *
 * @param $data
 *   Array (optional). Chart data, following the Charts Schema
 * @return
 *   HTML. The generic Chart example
 */
function _charts_example() {
//   if (empty($data)) {
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
//   }
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
function _charts_settings_form(&$form, $default = array(), $options = array()) {
  module_load_include('inc', 'charts');
  $default = $default + _charts_settings();

  $options = array_fill_keys($options, TRUE);

  if (!empty($options['example'])) {
    $form['chart_example'] = array(
      '#value'          => _charts_example(),
    );
  }

  asort($default['#plugins']);
  $form['plugin'] = array(
    '#default_value'  => $default['#plugin'],
    '#options'        => $default['#plugins'],
    '#type'           => 'select',
    '#title'          => t('Chart plugin'),
  );

  asort($default['#types']);
  $form['type'] = array(
    '#default_value'  => $default['#type'],
    '#options'        => $default['#types'],
    '#type'           => 'radios',
    '#title'          => t('Chart type'),
  );

  $form['width'] = array(
    '#default_value'  => $default['#width'],
    '#description'    => t('The chart width, in pixels'),
    '#size'           => 8,
    '#type'           => 'textfield',
    '#title'          => t('Width'),
  );
  $form['height'] = array(
    '#default_value'  => $default['#height'],
    '#description'    => t('The chart height, in pixels'),
    '#size'           => 8,
    '#type'           => 'textfield',
    '#title'          => t('Height'),
  );

  if (empty($options['color_complete'])) {
    _charts_color_form_simple($form, $default);
  }
  else {
    _charts_color_form_complete($form, $default);
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
  _charts_settings_form($form, array(), array('example', 'color_complete'));

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
  $settings['color_palette'] = implode(',', $settings['color']);

  // Include a # sign in all attributes, because it will make the
  // merge between the chart data and the defaults easier on every
  // chart display.
  foreach ($settings as $index => $value) {
    $default['#'. $index] = $value;
  }

  // Save the data into database
  variable_set('charts_settings', $default);
}
