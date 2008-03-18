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
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings() {
  // Get the previously saved data from Data Base
  $settings = variable_get('charts_settings', array());

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
  $form['chart'] = array(
    '#value'          => charts_chart($data)
  );

  $options = module_invoke_all('chartsinfo', 'list');
  $form['plugin'] = array(
    '#default_value'  => empty($settings['#plugin']) ? '' : $settings['#plugin'],
    '#options'        => $options,
    '#type'           => 'select',
    '#title'          => t('Chart plugin'),
  );
  $options = _charts_module_invoke_all('chartsinfo', 'charttypes');
  asort($options);
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

  // Color for the first 4 series.
  $form[0]['#tree'] = TRUE;
  $form[0]['color'] = array(
    '#default_value'  => empty($settings[0]['#color']) ? 'ff0000' : $settings[0]['#color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Series 1 Color'),
  );
  $form[1]['#tree'] = TRUE;
  $form[1]['color'] = array(
    '#default_value'  => empty($settings[1]['#color']) ? '00ff00' : $settings[1]['#color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Series 2 Color'),
  );
  $form[2]['#tree'] = TRUE;
  $form[2]['color'] = array(
    '#default_value'  => empty($settings[2]['#color']) ? '0000ff' : $settings[2]['#color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Series 3 Color'),
  );
  $form[3]['#tree'] = TRUE;
  $form[3]['color'] = array(
    '#default_value'  => empty($settings[3]['#color']) ? '000000' : $settings[3]['#color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Series 4 Color'),
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
  unset($settings['op']);

  // Add a '#' in all field names
  foreach ($settings as $index => $value) {
    if (!is_numeric($index)) {
      $settings["#$index"] = $value;
      unset($settings[$index]);
    }
    else {
      foreach ($value as $index2 => $value2) {
        $settings[$index]["#$index2"] = $value2;
        unset($settings[$index][$index2]);
      }
    }
  }

  // Save the data into database
  variable_set('charts_settings', $settings);

  // Print a 'OK' message
  drupal_set_message('Settings saved');
}
