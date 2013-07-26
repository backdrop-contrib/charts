/**
 * @file
 * JavaScript integration between Highcharts and Drupal.
 */
(function ($) {

Drupal.behaviors.chartsHighcharts = {};
Drupal.behaviors.chartsHighcharts.attach = function(context, settings) {
  for (var elementId in settings['chartsHighcharts']) {
    if (settings['chartsHighcharts'].hasOwnProperty(elementId)) {
      var config = settings['chartsHighcharts'][elementId];
      $('#' + elementId).highcharts(config);
    }
  }
};

})(jQuery);
