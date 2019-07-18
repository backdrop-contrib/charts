/**
 * @file
 * JavaScript integration between Highcharts and Backdrop.
 */
(function ($) {

Backdrop.behaviors.chartsHighcharts = {};
Backdrop.behaviors.chartsHighcharts.attach = function(context, settings) {
  $('.charts-highchart', context).once('charts-highchart', function() {
    if ($(this).attr('data-chart')) {
      var config = $.parseJSON($(this).attr('data-chart'));
      $(this).highcharts(config);
    }
  });
};

})(jQuery);
