/**
 * @file
 * JavaScript integration between Google Charts and Drupal.
 */
(function ($) {

Drupal.behaviors.chartsGoogle = {};
Drupal.behaviors.chartsGoogle.attach = function(context, settings) {
  google.setOnLoadCallback(renderCharts);
  function renderCharts() {
    for (var elementId in settings['chartsGoogle']) {
      if (settings['chartsGoogle'].hasOwnProperty(elementId)) {
        var config = settings['chartsGoogle'][elementId];
        var wrap = new google.visualization.ChartWrapper();
        wrap.setChartType(config.visualization);
        wrap.setDataTable(config.data);
        wrap.setOptions(config.options);
        wrap.setContainerId(elementId);

        // Apply data formatters. This only affects tooltips. The same format is
        // already applied via the hAxis/vAxis.format option.
        var dataTable = wrap.getDataTable();
        if (config.options.series) {
          for (var n = 0; n < config.options.series.length; n++) {
            if (config.options.series[n]['_format']) {
              var format = config.options.series[n]['_format'];
              if (format['dateFormat']) {
                var formatter = new google.visualization.DateFormat({ pattern: format['dateFormat'] });
              }
              else {
                var formatter = new google.visualization.NumberFormat({ pattern: format['format'] });
              }
              formatter.format(dataTable, n + 1);
            }
          }
        }

        // Apply individual point properties, by adding additional "role"
        // columns to the data table. So far this only applies "tooltip"
        // properties to individual cells. Ideally, this would support "color"
        // also. Feature request:
        // https://code.google.com/p/google-visualization-api-issues/issues/detail?id=1267
        var columnsToAdd = {};
        var rowCount = dataTable.getNumberOfRows();
        var columnCount = dataTable.getNumberOfColumns();
        for (var rowIndex in config._data) {
          if (config._data.hasOwnProperty(rowIndex)) {
            for (var columnIndex in config._data[rowIndex]) {
              if (config._data[rowIndex].hasOwnProperty(columnIndex)) {
                for (var role in config._data[rowIndex][columnIndex]) {
                  if (config._data[rowIndex][columnIndex].hasOwnProperty(role)) {
                    if (!columnsToAdd[columnIndex]) {
                      columnsToAdd[columnIndex] = {};
                    }
                    if (!columnsToAdd[columnIndex][role]) {
                      columnsToAdd[columnIndex][role] = new Array(rowCount);
                    }
                    columnsToAdd[columnIndex][role][rowIndex] = config._data[rowIndex][columnIndex][role];
                  }
                }
              }
            }
          }
        }
        // Add columns from the right-most position.
        for (var columnIndex = columnCount; columnIndex >= 0; columnIndex--) {
          if (columnsToAdd[columnIndex]) {
            for (var role in columnsToAdd[columnIndex]) {
              if (columnsToAdd[columnIndex].hasOwnProperty(role)) {
                dataTable.insertColumn(columnIndex + 1, {
                  type: 'string',
                  role: role,
                });
                for (var rowIndex in columnsToAdd[columnIndex][role]) {
                  dataTable.setCell(parseInt(rowIndex) - 1, columnIndex + 1, columnsToAdd[columnIndex][role][rowIndex]);
                }
              }
            }
          }
        }

        wrap.draw();
      }
    }
  }
};

})(jQuery);
