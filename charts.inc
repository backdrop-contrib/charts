<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file charts.inc
 * Transform DATA into INFORMATION using beautiful CHARTS.
 */

/**
 * Color module integration with Charts settings.
 *
 * @ingroup from
 */
function _charts_color_form(&$form, $form_state) {
  // The Color module integration starts with a field set
  $form['color'] = array(
    '#attributes'     => array('id' => 'charts_color', 'class'=> ' clear-block'),
    '#collapsible'    => TRUE,
    '#type'           => 'fieldset',
    '#title'          => t('Color'),
  );

  // Load some color palettes
  $schemes = _charts_color_palette();
  $form['color']['scheme'] = array(
    '#type'           => 'select',
    '#title'          => t('Color set'),
    '#options'        => $schemes,
    '#default_value'  => '',
  );
  $form['color']['schemes'] = array(
    '#type'           => 'value',
    '#value'          => $schemes
  );

  // Add palette fields. Since all
  $names = array(
    'color1'    => t('Color %number', array('%number' => 1)),
    'color2'    => t('Color %number', array('%number' => 2)),
    'color3'    => t('Color %number', array('%number' => 3)),
    'color4'    => t('Color %number', array('%number' => 4)),
    'color5'    => t('Color %number', array('%number' => 5)),
    'color65'   => t('Color %number', array('%number' => 6)),
    'color7'    => t('Color %number', array('%number' => 7)),
    'color8'    => t('Color %number', array('%number' => 8)),
  );
  $reference_colors = array(
    'color1'    => '#ffffff',
    'color2'    => '#ffffff',
    'color3'    => '#ffffff',
    'color4'    => '#ffffff',
    'color5'    => '#ffffff',
    'color65'   => '#ffffff',
    'color7'    => '#ffffff',
    'color8'    => '#ffffff',
  );
  $form['color']['palette'] = array(
    '#prefix'           => '<div id="preview"></div><div id="farbtastic"></div><div id="palette" class="clear-block">',
    '#suffix'           => '</div>',
    '#tree'             => true,
  );
  foreach ($reference_colors as $name => $value) {
    $form['color']['palette'][$name] = array(
      '#type'           => 'textfield',
      '#title'          => $names[$name],
      '#default_value'  => $value,
      '#size'           => 8,
    );
  }
  $form['color']['farbtastic'] = array(
    '#prefix'          => '<div id="farbtastic">',
    '#sufix'           => '</div>',
  );

  // Add the necessary JS data into the page
  drupal_add_js(array('charts_color' => array('reference' => $reference_colors)), 'setting');

  return $form;
}

/**
 * List all preset color palette
 */
function _charts_color_palette() {
  return array(
    '' => t('Custom'),
    '#FF0000,#00CC00,#0066B3,#FF8000,#FFCC00,#330099,#990099,#CCFF00' => t('Primary'),
    '#FF6600,#009999,#1919B3,#FFB200,#FFFF00,#660099,#E60066,#33FF00' => t('Secondary'),
  );
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
function _charts_settings($form_state) {
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

//   $form['color'] = array(
//     '#default_value'  => empty($settings['#color']) ? 'FFFFFF' : $settings['#color'],
//     '#description'    => t('Use the hexadecimal RGB value'),
//     '#type'           => 'textfield',
//     '#title'          => t('Background Color'),
//   );

  _charts_color_form($form, $form_state);
  // Color palette for the series and values.
//   $colors = empty($settings['#color_palette']) ?
//     array(
//       'FF8000',
//       'FFC080',
//       'FFDFBF',
//       'FFC080',
//       'FFCC00',
//       'FFE500',
//       'FFF9BF',
//       '78C0E9',
//       '179CE8',
//       '30769E',
//       'C8E9FC',
//       'ECF8FF',
//       '00CCFF',
//       '4086AA',
//       '91C3DC',
//       '87907D',
//       'AAB6A2',
//       '555555',
//       '666666',
//       '21B6A8',
//       '177F75',
//       'B6212D',
//       '7F171F',
//       'B67721',
//       '7F5417',
//     ) :
//     $settings['#color_palette'];
//   $form['color_palette'] = array(
//     '#default_value'  => implode(', ', $colors),
//     '#description'    => t('Used to differentiate series or pie-chart pieces. Use the hexadecimal RGB value'),
//     '#type'           => 'textarea',
//     '#title'          => t('Color Palette'),
//   );
//   $colors_example = '';
//   foreach ($colors as $color) {
//     $colors_example .= "<span style='color:#$color;'>########### $color ###########</span><br />";
//   }
//   $form['color_palette_example'] = array(
//     '#value'  => $colors_example,
//   );

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

/**
 * Theme color form.
 *
 * @ingroup @themeable
 */
function theme__charts_settings($form) {
  // Preview
  $output .= '<div id="preview"></div>';

  // The rest of the form
  $output .= drupal_render($form);

  // Add Farbtastic color picker JS
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'charts') .'/charts_color.js');

  // Add the necessary CSS
  drupal_add_css(drupal_get_path('module', 'charts') .'/charts_color.css');

  // Put everything inside given DIV. Its necessary for JS code
  return $output;
}
