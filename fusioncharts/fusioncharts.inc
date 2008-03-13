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
    case 'category':
      $attr         = 'label';
      $global_type  = 'categories';
    break;
    case 'set':
      $attr         = 'value';
      $global_type  = 'dataSet';
    break;
    case 'trendline':
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
 * Format an attribute string to insert in a tag, but
 * instead envolve data into double quotes, like
 * drupal_attributes() function, it put data into
 * single quotes.
 *
 * @param $attributes
 *   An associative array of HTML attributes.
 * @return
 *   An HTML string ready for insertion in a tag.
 * @see drupal_attributes()
 */
function _fusioncharts_xml_attributes($attributes = array()) {
  if (is_array($attributes)) {
    $t = '';
    foreach ($attributes as $key => $value) {
      $t .= " $key=".'\''. check_plain($value) .'\'';
    }
    return $t;
  }
}

/**
 * Format XML elements.
 *
 * @param $array
 *   An array where each item represent an element and is either a:
 *   - (key => value) pair (<key>value</key>)
 *   - Associative array with fields:
 *     - 'key': element name
 *     - 'value': element contents
 *     - 'attributes': associative array of element attributes
 *
 * In both cases, 'value' can be a simple string, or it can be another array
 * with the same format as $array itself for nesting.
 */
function _fusioncharts_xml_elements($array) {
  foreach ($array as $key => $value) {
    if (is_numeric($key)) {
      if ($value['key']) {
        $output .= ' <'. $value['key'];
        if (isset($value['attributes']) && is_array($value['attributes'])) {
          $output .= _fusioncharts_xml_attributes($value['attributes']);
        }

        if ($value['value'] != '') {
          $output .= '>'. (is_array($value['value']) ? _fusioncharts_xml_elements($value['value']) : check_plain($value['value'])) .'</'. $value['key'] .">\n";
        }
        else {
          $output .= " />";
        }
      }
    }
    else {
      $output .= " <$key>". (is_array($value) ? _fusioncharts_xml_elements($value) : check_plain($value)) ."</$key>";
    }
  }
  return $output;
}