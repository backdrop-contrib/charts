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
  $data = $data + _charts_settings();

  if (empty($data['#plugin']) and empty($default['#plugin'])) {
    return '';
  }

  // Split the color palette data into inidividual values
  $color_palette = explode(',', ereg_replace('#', '', $default['#color_palette']));

  // Check if the Chart will use the color palette for individual values
  // instead for series, like Pie Charts
  $options = array('pie2D' => TRUE, 'pie3D' => TRUE);
  if ((!empty($data['#type']) and !empty($options[$data['#type']]) ) or
      (!empty($default['#type']) and !empty($options[$default['#type']]) ) ) {
    $individual_color_palette = TRUE;
  }

  // Merge all series option to the main data array,
  foreach (element_children($data) as $series) {
    if (!empty($default[$series])) {
      $data[$series] = array_merge($default[$series], $data[$series]);
    }
    unset($default[$series]);

    // Apply the Color Palette: normally, apply one color to each series.
    // But for some types of charts, is one color to each value into the series
    if (empty($individual_color_palette) and empty($data[$series]['#color'])) {
      $data[$series]['#color'] = $color_palette[$series];
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
  static $default = array();

  if (empty($default)) {
    $default = variable_get('charts_settings', array());

    // Plugin
    $charts_info = module_invoke_all('charts_info', 'list');
    foreach ($charts_info as $chart_code => $chart) {
      $default['#plugins'][$chart_code] = $chart['name'];
    }
    asort($default['#plugins']);
    $default['#plugin'] = empty($default['#plugin']) ? current(array_keys($default['#plugins'])) : $default['#plugin'];

    // Type
    $default['#types'] = module_invoke_all('chart_types');
    $ctypes_allowed = array_fill_keys($charts_info[$default['#plugin']]['types'], TRUE);
    foreach (array_keys($default['#types']) as $ctype_code) {
      if (empty($ctypes_allowed[$ctype_code])) {
        unset($default['#types'][$ctype_code]);
      }
    }
    $default['#type']  = empty($default['#type']) ? current(array_keys($default['#types'])) : $default['#type'];

    // Width and Height
    $default['#width']  = empty($default['#width'])  ? 400 : $default['#width'];
    $default['#height'] = empty($default['#height']) ? 200 : $default['#height'];

    // Color Palette
    $default['#color_palettes'] = _charts_settings_color_palette();
    if (empty($default['#color_palette'])) {
      $default['#color_palette'] = current(array_keys($default['#color_palettes']));
    }

    // Color
    $default['#color'] = explode( ',', $default['#color_palette']);
    $default['#color']['background'] = array_shift($default['#color']);
    $default['#color']['text']       = array_shift($default['#color']);
  }

  return $default;
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
