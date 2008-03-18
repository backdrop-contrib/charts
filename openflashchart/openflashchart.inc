<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file openflashchart.inc
 * Use Open Flash Chart on your site.
 */

/**
 * Convert all Chart-level data.
 *
 * @param &$chart
 *   Object. The Open Flash Chart object
 * @param &$data
 *   Array. The raw data.
 */
function _openflashchart_chart(&$chart, &$data) {
  $chart->set_title($data['#title'], '{font-size: 20px;}' );
  $chart->set_width($data['#width']);
  $chart->set_height($data['#height']);

  $chart->bar(10, '#9933CC', '#8010A0', 'Page views', 10);

  $chart->set_bg_colour($data['#color']);
}

/**
 * Convert all Series-level data.
 *
 * @param &$chart
 *   Object. The Open Flash Chart object
 * @param &$data
 *   Array. The raw data.
 */
function _openflashchart_series(&$chart, &$data) {
  foreach (element_children($data) as $series) {
    // Get only the numeric values from the series
    $series_data = _charts_series_values($data[$series]);

    // Get the highest value on the series, to be a reference point
    $max = $max < max($series_data) ? max($series_data) : $max;

    $chart->set_data($series_data);
  }

  $chart->set_y_max( $max );

//   $chart->set_x_labels();
}
