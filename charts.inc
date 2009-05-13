<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * The real caller for building charts.
 */

/**
 * The main Chart API function, that calls any chart provider
 * to print the given data.
 *
 * @param &$data
 *   Array. The chart data, described on chart_api.txt
 * @return
 *   String. The HTML with the propper chart (might include Flash or
 *   JavaScript external files)
 */
function _charts_chart(&$data) {
  // Get the previously saved data from database
  $settings = _charts_settings();

  if (empty($data['#plugin']) and empty($settings['plugin'])) {
    return '';
  }

  // Split the color palette data into inidividual values
  $color_palette = explode(',', ereg_replace('#', '', $settings['color_palette']));

  // Check if the Chart will use the color palette for individual values
  // instead for series, like Pie Charts
  $options = array('pie2D' => TRUE, 'pie3D' => TRUE);
  if ((!empty($data['#type']) and !empty($options[$data['#type']]) ) or
      (!empty($settings['type']) and !empty($options[$settings['type']]) ) ) {
    $individual_color_palette = TRUE;
  }

  // Merge all series option to the main data array,
  foreach (element_children($data) as $series) {
    if (!empty($settings[$series])) {
      $data[$series] = array_merge($settings[$series], $data[$series]);
    }
    unset($settings[$series]);

    // Apply the Color Palette: normally, apply one color to each series.
    // But for some types of charts, is one color to each value into the series
    if (empty($individual_color_palette) and empty($data[$series]['#color'])) {
      $data[$series]['color'] = $color_palette[$series];
    }
    elseif (!empty($individual_color_palette)) {
      foreach (element_children($data[$series]) as $values) {
        if (!is_array($data[$series][$values])) {
          $data[$series][$values] = array(
            '#value' => $data[$series][$values],
            '#color' => $color_palette[$values],
          );
        }
        elseif (empty($data[$series][$values]['#color'])) {
          $data[$series][$values]['#color'] = $color_palette[$values];
        }
      }
    }
  }

  // Get the information about chart modules
  $chart_provider = module_invoke_all('charts_info');

  if (isset($chart_provider[$data['#plugin']]['file'])
      and is_file($chart_provider[$data['#plugin']]['file'])
      and $func = $chart_provider[$data['#plugin']]['render']) {

    // Include the file that has the rendering function
    include_once $chart_provider[$data['#plugin']]['file'];

    // Using the filter's rendering function, print the chart
    return $func($data);
  }
  return '';
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings() {
  // Get the previously saved data from Data Base
  static $settings, $filtered = array();

  if (empty($settings)) {
    $settings = variable_get('charts_settings', array());

    // Plugin
    $charts_info = module_invoke_all('charts_info', 'list');
    foreach ($charts_info as $chart_code => $chart) {
      $settings['plugins'][$chart_code] = $chart['name'];
    }
    asort($settings['plugins']);
    $settings['plugin'] = empty($settings['plugin']) ? current(array_keys($settings['plugins'])) : $settings['plugin'];

    // Type
    $settings['types'] = module_invoke_all('chart_types');
    $ctypes_allowed = array_fill_keys($charts_info[$settings['plugin']]['types'], TRUE);
    foreach (array_keys($settings['types']) as $ctype_code) {
      if (empty($ctypes_allowed[$ctype_code])) {
        unset($settings['types'][$ctype_code]);
      }
    }
    $settings['type']  = empty($settings['type']) ? current(array_keys($settings['types'])) : $settings['type'];

    // Width and Height
    $settings['width']  = empty($settings['width'])  ? 400 : $settings['width'];
    $settings['height'] = empty($settings['height']) ? 200 : $settings['height'];

    // Color Palette
    $settings['color_palettes'] = _charts_settings_color_palette();
    if (empty($settings['color_palette'])) {
      $settings['color_palette'] = current(array_keys($settings['color_palettes']));
    }

    // Color
    $settings['color'] = explode( ',', $settings['color_palette']);
    $settings['color']['background'] = array_shift($settings['color']);
    $settings['color']['text']       = array_shift($settings['color']);
  }

  return $settings;
}

/**
 * List all preset color palette
 */
function _charts_settings_color_palette() {
  return array(
    '#ffffff,#000000,#ff0000,#00cc00,#0066b3,#ff8000,#ffcc00,#330099,#990099,#ccff00' => t('Primary'),
    '#ffffff,#000000,#ff6600,#009999,#1919b3,#ffb200,#ffff00,#660099,#e60066,#33ff00' => t('Secondary'),
    '' => t('Custom'),
  );
}
