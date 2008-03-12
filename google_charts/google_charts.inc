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
 */
function _google_charts_data_codingsimple($data) {
  // Set the list of characters and the size of the list
  $simple = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  $simple_len = strlen($simple) - 1;

  // The final output is going to be build
  $chart_data = '';

  // For each chart value, encode it
  // Note: Underscore represents a missing value
  foreach ($data as $series) {
    // Include a series separator
    if (!empty($chart_data)) {
      $chart_data .= '|';
    }

    // Get the highest value on the series, to be a reference point
    $max = max($series);

    // For each series of data, scan it
    foreach ($series as $value) {
      if ($value >= 0) {
        $chart_data .= $simple[round($simple_len * $value / $max)];
      }
      else {
        $chart_data .= '_';
      }
    }
  }

  // Return the series of data
  return 's:'. $chart_data;
}

function _google_charts_data_codingextended() {
}
