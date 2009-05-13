// $Id$
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Set the color palette to Charts.
 */
Drupal.behaviors.chartsColor = function (context) {
  var chartsColor = function() {
    var colors = $("#edit-color-color-palettes").val().split(",");
    var series = ["background", "text", 0, 1, 2, 3, 4, 5, 6, 7];
    var disabled = ($("#edit-color-palettes").children("option:selected").text() == Drupal.settings.chartsColorCustom);
    for (var s = 0, len = series.length; s < len; ++s) {
      $("#edit-color-"+ s).val(colors[s]);
      if (disabled) {
        $("#edit-color-"+ s).attr("disabled", "");
      }
      else {
        $("#edit-color-"+ s).attr("disabled", "disabled");
      }
    }
    if (disabled) {
      $("#edit-colorpicker").attr("disabled", "");
    }
    else {
      $("#edit-colorpicker").attr("disabled", "disabled");
    }
  };

  $("#edit-color-color-palettes").change(chartsColor);
  chartsColor();
};
