<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file charts.inc
 * Transform DATA into INFORMATION using beautiful CHARTS.
 */

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
    $settings['data'] = array(10, 20, 55, 72, 35, 23);

    $form['chart'] = array(
      '#value'          => charts_chart($settings['basic']['provider'], $settings)
    );
  }

  // Basic info:
  //   - Provider: which is the Chart API that will be used, like Google,
  //               Open Flash Chart, FusionCharts...
  //   - Type:     there are dozen types of charts, like 'Line', 'Bar',
  //               'Scatter Plot', 'Pie'. And for each version, it can
  //               be in 3D or 2D, vertical or hotizontal.
  $form['basic'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Basic'),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
    '#tree'           => TRUE
  );
  $options = module_invoke_all('chartsinfo', 'list');
  $form['basic']['provider'] = array(
    '#default_value'  => empty($settings['basic']['provider']) ? '' : $settings['basic']['provider'],
    '#options'        => $options,
    '#type'           => 'select',
    '#title'          => t('Chart provider'),
  );
  $options = module_invoke_all('chartsinfo', 'charttypes');
  $form['basic']['charttype'] = array(
    '#default_value'  => empty($settings['basic']['charttype']) ? '' : $settings['basic']['charttype'],
    '#options'        => $options,
    '#type'           => 'select',
    '#title'          => t('Chart type'),
  );

  // Hold all layout settings, so independently from the Chart provider,
  // the Charts will always look almost the same:
  //   - Background Color: It can
  $form['layout'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Layout'),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
    '#tree'           => TRUE
  );
  $form['layout']['background_color'] = array(
    '#default_value'  => empty($settings['layout']['background_color']) ? 'ffffff' : $settings['layout']['background_color'],
    '#description'    => t('Use the hexadecimal RGB value'),
    '#type'           => 'textfield',
    '#title'          => t('Background Color'),
  );
  $form['layout']['width'] = array(
    '#default_value'  => empty($settings['layout']['width']) ? 400 : $settings['layout']['width'],
    '#description'    => t('The chart width, in pixels'),
    '#type'           => 'textfield',
    '#title'          => t('Width'),
  );
  $form['layout']['height'] = array(
    '#default_value'  => empty($settings['layout']['height']) ? 200 : $settings['layout']['height'],
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
  // Build the chart settings
  $settings['basic']  = $form_state['values']['basic'];
  $settings['layout'] = $form_state['values']['layout'];

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
