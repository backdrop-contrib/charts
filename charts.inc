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

  $form['color'] = array(
    '#default_value'  => empty($settings['#color']) ? 'FFFFFF' : $settings['#color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Background Color'),
  );
  // Color palette for the series and values.
  $colors = empty($settings['#color_palette']) ?
    array(
      'FF8000',
      'FFC080',
      'FFDFBF',
      'FFC080',
      'FFCC00',
      'FFE500',
      'FFF9BF',
      '78C0E9',
      '179CE8',
      '30769E',
      'C8E9FC',
      'ECF8FF',
      '00CCFF',
      '4086AA',
      '91C3DC',
      '87907D',
      'AAB6A2',
      '555555',
      '666666',
      '21B6A8',
      '177F75',
      'B6212D',
      '7F171F',
      'B67721',
      '7F5417',
    ) :
    $settings['#color_palette'];
  $form['color_palette'] = array(
    '#default_value'  => implode(', ', $colors),
    '#description'    => t('Used to differentiate series or pie-chart pieces. Use the hexadecimal RGB value'),
    '#type'           => 'textarea',
    '#title'          => t('Color Palette'),
  );
  $colors_example = '';
  foreach ($colors as $color) {
    $colors_example .= "<span style='color:#$color;'>########### $color ###########</span><br />";
  }
  $form['color_palette_example'] = array(
    '#value'  => $colors_example,
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
    $settings["#$index"] = $value;
    unset($settings[$index]);
  }

  $settings['#color_palette'] = explode(', ', $settings['#color_palette']);

  // Save the data into database
  variable_set('charts_settings', $settings);

  // Print a 'OK' message
  drupal_set_message('Settings saved');
}
