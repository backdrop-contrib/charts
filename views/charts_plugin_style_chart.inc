<?php
/**
 * @file
 * Contains the chart style plugin.
 */

/**
 * Style plugin to render view as a chart.
 *
 * @ingroup views_style_plugins
 */
class charts_plugin_style_chart extends views_plugin_style {
  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    // Get the default chart values
    module_load_include('inc', 'charts', 'includes/charts.pages');
    $defaults = variable_get('charts_default_settings', charts_default_settings());
    foreach ($defaults as $default_key => $default_value) {
      $options[$default_key]['default'] = $default_value;
    }

    // Views-specific chart options.
    $options['label_field'] = array(
      'default' => NULL,
    );

    return $options;
  }

  /**
   * Generate a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one field before you can configure your table settings') . '</div>',
      );
      return;
    }

    // Merge in the global chart settings form.
    module_load_include('inc', 'charts', 'includes/charts.pages');
    $field_options = $this->display->handler->get_field_labels();
    $form = charts_settings_form($form, $this->options, $field_options, array('style_options'));
  }

  /**
   * Generate a form for setting options.
   */
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
  }

  /**
   * Make sure the display and all associated handlers are valid.
   *
   * @return
   *   Empty array if the display is valid; an array of error strings if it is not.
   */
  function validate() {
    $errors = array();
    $field_handlers = $this->display->handler->get_handlers('field');

    if (count($field_handlers)) {
      $data_field_key = !empty($this->options['data_fields']) ? current($this->options['data_fields']) : NULL;
      if (empty($data_field_key)) {
        $errors[] = t('At least one data field must be selected in the chart configuration before this chart may be shown');
      }
      else {
        $data_field = isset($field_handlers[$data_field_key]) ? $field_handlers[$data_field_key] : NULL;
        if (!isset($data_field)) {
          $errors[] = t('A field you have specified as a data field in your chart settings no longer exists. Edit the chart settings and select at least one data field.');
        }
      }
    }

    return $errors;
  }

  /**
   * Define and display a chart from the grouped values.
   */
  function render() {
    // Calculate the labels field alias.
    $field_handlers = $this->display->handler->get_handlers('field');
    $label_field = FALSE;
    if ($this->options['label_field'] && array_key_exists($this->options['label_field'], $field_handlers)) {
      $label_field = $field_handlers[$this->options['label_field']];
    }
    $data_field_options = array_filter($this->options['data_fields']);
    $data_fields = array();
    foreach ($data_field_options as $field_key) {
      if (isset($field_handlers[$field_key])) {
        $data_fields[$field_key] = $field_handlers[$field_key];
      }
    }

    $chart_id = $this->view->name . '__' . $this->view->current_display;
    $chart = array(
      '#type' => 'chart',
      '#chart_type' => $this->options['type'],
      '#chart_library' => $this->options['library'],
      '#chart_id' => $chart_id,
      '#id' => drupal_clean_css_identifier('chart_' . $chart_id),
      '#title' => $this->options['title_position'] ? $this->options['title'] : FALSE,
      '#title_position' => $this->options['title_position'],
      '#colors' => $this->options['colors'],
      '#background' => $this->options['background'] ? $this->options['background'] : 'transparent',
      '#legend' => $this->options['legend_position'] ? TRUE : FALSE,
      '#legend_position' => $this->options['legend_position'] ? $this->options['legend_position'] : NULL,
      '#width' => $this->options['width'],
      '#height' => $this->options['height'],
    );
    $chart_type_info = chart_get_type($this->options['type']);

    if ($chart_type_info['axis'] === CHARTS_SINGLE_AXIS) {
      $data_field = reset($data_fields);

      $data = array();
      foreach ($this->view->result as $row) {
        $data_row = array();
        if ($label_field) {
          $data_row[] = $label_field->render($row);
        }
        $data_row[] = (int) $row->{$data_field->field_alias};
        $data[] = $data_row;
      }

      if ($label_field) {
        $chart['#legend_title'] = $label_field->options['label'];
      }

      $chart['series'] = array(
        '#type' => 'chart_data',
        '#data' => $data,
        '#title' => $data_field->options['label'],
      );
    }
    else {
      $chart['xaxis'] = array(
        '#type' => 'chart_xaxis',
        '#title' => $this->options['xaxis_title'] ? $this->options['xaxis_title'] : FALSE,
        '#labels_rotation' => $this->options['xaxis_labels_rotation'],
      );
      $chart['yaxis'] = array(
        '#type' => 'chart_yaxis',
        '#title' => $this->options['yaxis_title'] ? $this->options['yaxis_title'] : FALSE,
        '#labels_rotation' => $this->options['yaxis_labels_rotation'],
        '#max' => $this->options['yaxis_max'],
        '#min' => $this->options['yaxis_min'],
      );
      foreach ($data_fields as $field_key => $field_handler) {
        $chart[$field_handler->field_alias] = array(
          '#type' => 'chart_data',
          '#data' => array(),
          '#color' => isset($this->options['field_colors'][$field_key]) ? $this->options['field_colors'][$field_key] : NULL,
          '#title' => $field_handler->options['label'],
          '#prefix' => $this->options['yaxis_prefix'] ? $this->options['yaxis_prefix'] : NULL,
          '#suffix' => $this->options['yaxis_suffix'] ? $this->options['yaxis_suffix'] : NULL,
          '#decimal_count' => $this->options['yaxis_decimal_count'] ? $this->options['yaxis_decimal_count'] : NULL,
        );
      }

      foreach ($this->view->result as $row) {
        if ($label_field) {
          $chart['xaxis']['#labels'][] = $label_field->render($row);
          unset($row->{$label_field->field_alias});
        }
        $row = (array) $row;
        foreach ($row as $field_alias => $data_item) {
          $chart[$field_alias]['#data'][] = (int) $data_item;
        }
      }
    }

    // Print the chart.
    return drupal_render($chart);
  }

}
