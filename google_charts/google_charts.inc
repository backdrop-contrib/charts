<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file google_charts.inc
 * Use Google Charts on your site.
 */

/**
 * Encode the Chart data into a Alphanumeric code, follwing the
 * Google Chart API instruction. Its needed because there is a
 * size limmit to URL strings. So we reduce the amount of characters.
 *
 * It basicly uses a scale of 61 levels to represent each chart
 * value. If a more precise scale is needed, see
 * _google_charts_codingextended(), which produces a 4000 levels,
 * but also a bigger URL string.
 *
 * @param $data
 *   Array. A series of numeric data values
 * @return
 *   String. The string presentation of series data
 */
function _google_charts_data_codingsimple($value, $max) {
  // Set the list of characters and the size of the list
  $simple = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  $simple_len = strlen($simple) - 1;

  if ($value >= 0) {
    return $simple[round($simple_len * $value / $max)];
  }
  else {
    return  '_';
  }
}

function _google_charts_chart(&$chart, &$data) {
  // Convert the chat TYPE into the Google Chart way.
  // Since its a requirement to build the chart on Google, if the value
  // was not found, return nothing and stop the execution.
  $options = array(
    'line2D'  => 'lc',
    'hbar2D'  => 'bhg',
    'vbar2D'  => 'bvg',
    'pie2D'   => 'p',
    'pie3D'   => 'p3',
    'venn'    => 'v',
    'scatter' => 's',
  );
  if (empty($options[$data['#type']])) {
    return FALSE;
  }
  $chart[] = 'cht='. $options[$data['#type']];

  // Convert the chat SIZE into the Google Chart way.
  // Since its a requirement to build the chart on Google, if the value
  // was not found, return nothing and stop the execution.
  if (empty($data['#width']) or empty($data['#height'])) {
    return FALSE;
  }
  $chart[] = 'chs='. $data['#width'] .'x'. $data['#height'];

  // Add Title and Description to the chart
  if (!empty($data['#title'])) {
    $chart[] = 'chtt='. $data['#title'];
  }

  // Chart background color. Since the default color
  // is white (#ffffff), only different colors are considered
  if (!empty($data['#color']) and $data['#color'] != 'ffffff') {
    $chart[] = 'chf=bg,s,'. $data['#color'];
  }

  return TRUE;
}

function _google_charts_series(&$chart, &$data) {
  // The final output is going to be build
  $chart_data = '';

  // For each chart value, encode it
  // Note: Underscore represents a missing value
  foreach (element_children($data) as $series) {
    // Include a series separator
    if (!empty($chart_data)) {
      $chart_data .= ',';
    }

    // Get only the numeric values from the series
    $series = _charts_series_values($data[$series]);

    // Get the highest value on the series, to be a reference point
    $max = max($series);

    // For each series of data, scan it
    foreach (element_children($series) as $value) {
        $chart_data .= _google_charts_data_codingsimple($series[$value], $max);
    }
  }

  // Return the series of data
  if (empty($chart_data)) {
    return FALSE;
  }
  $chart[] = 'chd=s:'. $chart_data;
  return TRUE;
}

function _google_charts_values() {
}
