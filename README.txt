Charts
======

Charts module provides a unified format to build any kind of chart with any
chart provider.

Each chart solution found on internet, like Google Charts and Highcharts,
have a specific data scheme. Its very hard and even impossible to build a unique
chart data that would be used in more that one chart provider. Or users get
binded to a solution forever or they have to rewrite all exported data again.

Thats why Charts is so great. It uses a standard data scheme do describe charts
data and thru filters, it converts automatically to each solution. You might
change to another solution at anytime.

The Chart schema is very similiar to Drupal's Form API schema.

Chart Providers
---------------

Out of the Box, you will be able to use 2 chart solutions. Which one of them
has particular advantages and disadvantages.

* Google Charts: This library does not require any external downloads. It
  generates interactive charts using SVG and VML.

* Highcharts: This library is one of the premier solutions for generating
  charts. Although it is very powerful and aesthetically pleasing with smooth
  animations, it requires a commercial license. It's free for non-commercial
  use. See http://www.highcharts.com

Installing Highcharts
---------------------

If you decide to use the Highcharts library, you'll need to first install the
Libraries module for Drupal: https://drupal.org/project/libraries

Then you'll need to download Highcharts and place it in sites/all/libraries
directory. The highcharts.js file should be located at:
sites/all/libraries/highcharts/highcharts.js. You will also need to remove a
directory called "exporting-server" within the Highcharts download. This
directory contains sample code for exporting charts that may compromise the
security of your site. The module will throw an error if this directory is left
in place.

Creating Charts in the UI
-------------------------

This module provides a configuration page at admin/config/content/charts. You
may set site-wide defaults on this page (for example set the default color
scheme).

In order to actuall create a chart through the UI, you'll need to use Views
module.

- Create a new view:
  Visit admin/structure/views/add and select the display format of "Chart" for
  your new page or block.

- Enable aggregation on the view:
  In order to display data, most of the time you'll need enable aggregation on
  the view. Under the right column of settings, there is a section for
  "Use aggregation". Enable this setting.

- Add a label field:
  Under the "Fields" section, add a field you would like to be used as labels
  along one axis of the chart (or slices of the pie). In the "Aggregation type"
  setting, set it to "Group results together".

- Add data fields:
  Now a second field which will be used to determine the data values. Usually
  this will be an ID field, such as NID or CID. The label you give this field
  will be used in the chart's legend to represent this series. After adding the
  field, set the "Aggregation type" for this field to "Count". Do this again for
  each different quantity you would like to chart. Note that some charts
  (e.g. Pie) only support a single data column.

- Configure the chart display:
  Click on the "Settings" link in the Format section to configure the chart.
  Select your chart type. Some options may not be available to all chart types
  and will be adjusted based on the type selected. Because no live preview is
  available in Charts, you may need to open a saved version of the view in a
  different window to see the impact of your changes.

- Save your view.

Tip: You may find it easier to start with a "Table" display and convert it to a
chart display after setting up the data. It can be easier to visualize what
the result of the chart will be if it's been laid out in a table first.

Create Charts through the API
-----------------------------

Charts module includes an extensive API for creating charts through code. See
the charts.api.php file included with this module for documentation.

Support
-------

For bug reports and feature requests please use the Drupal.org issue tracker:
http://drupal.org/project/issues/charts.
