<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file charts.inc
 * Transform DATA into INFORMATION using beautiful CHARTS.
 */

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
    else if (isset($result)) {
      $return[] = $result;
    }
  }

  return $return;
}

/**
 * Even if the series have values with attributes,
 * return only the numeric values of them.
 *
 * @param
 *   Array. A given data series with or without attributes.
 * @return
 *   Array. A data series, but only with the values, without
 *   the attributes.
 */
function _charts_series_values($series) {
  $data = array();

  foreach ($series as $index => $value) {
    if (!is_numeric($index)) {
      continue;
    }

    if (is_array($value)) {
      $data[] = $value['#value'];
    }
    else {
      $data[] = $value;
    }
  }

  return $data;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings() {
  // Get the previously saved data from Data Base
  $settings = variable_get('charts_settings', array());

  // This will hold the example chart
  if (!empty($settings)) {
    // Since the chart is an example, we should provide
    // and example data.
    $settings[] = array(
      10, 20, 55, 72, 45, 50
    );
    $settings[] = array(
      '#label'      => 'Profit',
      '#color'      => '#ff0000',
      array('#value' => 35, '#label' => t('Jan')),
      array('#value' => 25, '#label' => t('Feb')),
      array('#value' => 75, '#label' => t('Mar')),
      array('#value' => 50, '#label' => t('Apr')),
      array('#value' => 23, '#label' => t('May')),
      array('#value' => 12, '#label' => t('Jun')),
    );

    // The testing chart needs a testing Title
    $settings['#title'] = 'Testing Chart';

    $form['chart'] = array(
      '#value'          => charts_chart($settings['#plugin'], $settings)
    );
  }

  $options = module_invoke_all('chartsinfo', 'list');
  $form['plugin'] = array(
    '#default_value'  => empty($settings['#plugin']) ? '' : $settings['#plugin'],
    '#options'        => $options,
    '#type'           => 'select',
    '#title'          => t('Chart plugin'),
  );
  $options = _charts_module_invoke_all('chartsinfo', 'charttypes');
  $form['type'] = array(
    '#default_value'  => empty($settings['#type']) ? '' : $settings['#type'],
    '#options'        => $options,
    '#type'           => 'select',
    '#title'          => t('Chart type'),
  );

  $form['color'] = array(
    '#default_value'  => empty($settings['#color']) ? 'ffffff' : $settings['#color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Background Color'),
  );
  $form['width'] = array(
    '#default_value'  => empty($settings['#width']) ? 400 : $settings['#width'],
    '#description'    => t('The chart width, in pixels'),
    '#type'           => 'textfield',
    '#title'          => t('Width'),
  );
  $form['height'] = array(
    '#default_value'  => empty($settings['#height']) ? 200 : $settings['#height'],
    '#description'    => t('The chart height, in pixels'),
    '#type'           => 'textfield',
    '#title'          => t('Height'),
  );

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
function _charts_settings_submit(&$form, &$form_state) {
  $settings = $form_state['values'];
  unset($settings['submit']);
  unset($settings['form_id']);
  unset($settings['form_build_id']);
  unset($settings['form_token']);

  // Add a '#' in all field names
  foreach ($settings as $index => $value) {
    $settings["#$index"] = $value;
    unset($settings[$index]);
  }

  // Save the data into database
  variable_set('charts_settings', $settings);

  // Print a 'OK' message
  drupal_set_message('Settings saved');
}

/**
 * Module testing page. Users will be able to test a given
 * data to see what its going to look like.
 *
 * @ingroup form
 */
function _charts_testing() {
  // Get the previously saved data from Data Base
  $settings = variable_get('charts_settings', array());

  // This will hold the example chart
  if (!empty($settings)) {
    $form['chart'] = array(
      '#value'          => charts_chart($settings['basic']['provider'], $data)
    );
  }

  $form['data'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Data'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
    '#tree'          => TRUE
  );
  return $form;
}

