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
  $data = variable_get('charts_settings', array());

  // This will hold the example chart
  $form['chart'] = array(
    '#value'          => 'TODO Chart demonstration'
  );

  // Hold all layout settings
  $form['layout'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Layout'),
    '#collapsible'    => TRUE,
    '#collapsed'      => FALSE,
    '#tree'           => TRUE
  );
  $form['layout']['background_color'] = array(
    '#default_value'  => empty($data['layout']['background_color']) ? 'fff' : $data['layout']['background_color'],
    '#description'    => 'Use the hexadecimal RGB value',
    '#type'           => 'textfield',
    '#title'          => t('Layout'),
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
  $data['layout'] = $form_state['values']['layout'];

  // Save the data into database
  variable_set('charts_settings', $data);

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
  // This will hold the example chart
  $form['chart'] = array(
    '#value'          => 'TODO Chart demonstration'
  );

  $form['data'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Data'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
    '#tree'          => TRUE
  );
  return $form;
}
