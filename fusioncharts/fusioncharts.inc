<?php
// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file fusioncharts.module
 * Use FusionCharts on your site.
 */

/**
 * Create the values object using FusionCharts
 *
 * @param $type
 *   String. The data type. It can be 'category', 'set' or 'trendline'
 * @param $data
 *   Array. The data to be converted to XML
 * @param $options
 *   Array (optional). Data options
 * @return
 *   Array, the XML array
 */
function _fusioncharts_values($type, $data, $options = array()) {
  switch ($type) {
    case 'categories':
      $attr         = 'label';
      $global_type  = 'categories';
    break;
    case 'dataSet':
      $attr         = 'value';
      $global_type  = 'dataSet';
    break;
    case 'trendlines':
      $attr         = 'value';
      $global_type  = 'trendlines';
    break;
  }

  // Create each value.
  $values = array();
  while (list(, $value) = each($data)) {
    if (is_array($value)) {
      $values[] = array('key' => $type, 'attributes' => $value);
    }
    else {
      $values[] = array('key' => $type, 'attributes' => array($attr => $value));
    }
  }

  // Create the XML array
  $data_complete = array(
    'key'         => $global_type,
    'value'       => $values,
    'attributes'  => $options
  );

  return $data_complete;
}

/**
 * Convert all Chart-level data.
 *
 * @param &$chart
 *   Array. The array that will be converted into a string for FusionCharts
 * @param &$data
 *   Array. The raw data.
 * @return
 *   String. The string presentation of series data
 */
function _fusioncharts_chart(&$chart, &$data) {
}

/**
 * Convert all Series-level data.
 *
 * @param &$chart
 *   Array. The array that will be converted into a string for FusionCharts
 * @param &$data
 *   Array. The raw data.
 * @return
 *   String. The string presentation of series data
 */
function _fusioncharts_series(&$chart, &$data) {
  // Convert the chat DATA into the FusionCharts way.
  // Since its a requirement to build the chart on FusionCharts, if the value
  // was not found, return nothing and stop the execution.
  foreach (element_children($data) as $series) {
    $series_data = _charts_series_values($data[$series]);

    $chart['chart'][] = _fusioncharts_values('dataSet', $series_data);

    foreach (element_children($data[$series]) as $values) {
      $value_labels_temp[] = empty($data[$series][$value]['#label']) ? NULL : $data[$series][$value]['#label'];
    }

    // X labels
    $value_labels = array();
    $toogle = FALSE;
    foreach (array_keys($series_data) as $value) {
      if (empty($data[$series][$value]['#label'])) {
        $value_labels[] = '';
      }
      else {
        $value_labels[] = $data[$series][$value]['#label'];
        $toogle = TRUE;
      }
    }
    if (!empty($toogle)) {
      $chart['chart'][] = _fusioncharts_values('categories', $value_labels);
    }
  }
}
