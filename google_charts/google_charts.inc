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
 * @param $values
 *   Array. A series of numeric data values
 * @param $max
 *   Number. The biggest number to be the reference point
 */
function _google_charts_codingsimple($values, $max) {
  // Set the list of characters and the size of the list
  $simple_encoding = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  $strlen = strlen($simple_encoding) - 1;

  // The final output is going to be build
  $chart_data = 's:';

  // For each chart value, encode it
  // Note: Underscore represents a missing value
  foreach ($values as $current_value) {
    if ($current_value >= 0) {
      $chart_data .= $simple_encoding[(round($strlen * $current_value / $max))];
    }
    else {
      $chart_data .= '_';
    }
  }

  return $chart_data;
}

function _google_charts_codingextended() {
}
