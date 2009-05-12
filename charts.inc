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
  $settings = variable_get('charts_settings', array());

  if (empty($data['#plugin']) and empty($settings['#plugin'])) {
    return '';
  }

  // Split the color palette data into inidividual values
  $color_palette = explode(',', ereg_replace('#', '', $settings['#color_palette']));

  // Check if the Chart will use the color palette for individual values
  // instead for series, like Pie Charts
  $options = array('pie2D' => TRUE, 'pie3D' => TRUE);
  if ((!empty($data['#type']) and !empty($options[$data['#type']]) ) or
      (!empty($settings['#type']) and !empty($options[$settings['#type']]) ) ) {
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

  // Now, merge all the rest of the options saved by default
  $data = array_merge($settings, $data);

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
