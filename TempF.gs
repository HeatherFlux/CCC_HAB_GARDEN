

/* Using spreadsheet API */

function doGet(e) { 
  Logger.log( JSON.stringify(e) );  // view parameters

  var result = 'Ok'; // assume success

  if (e.parameter == undefined) {
    result = 'No Parameters';
  }
  else {
    var id = '16IQP2zjOqVH1qWQJ1IGiygshhNX7mFOuPMxMCBTza-c'; 
    var sheet = SpreadsheetApp.openByUrl('https://docs.google.com/spreadsheets/d/16IQP2zjOqVH1qWQJ1IGiygshhNX7mFOuPMxMCBTza-c/edit#gid=530749950').getActiveSheet();
    var newRow = sheet.getLastRow();
    var rowData = [];
    //var waktu = new Date();
    for (var param in e.parameter) {
      Logger.log('In for loop, param='+param);
      var value = stripQuotes(e.parameter[param]);
      //Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        case 'tempfData': //Parameter
          rowData[0] = value; //Value in column B
          break;
      //case 'column_C':
        //rowData[2] = value;
        //break;
        default:
          result = "unsupported parameter";
      }
    }
    Logger.log(JSON.stringify(rowData));

    // Write new row below
    var newRange = sheet.getRange(newRow, 5, 1, rowData.length);
    newRange.setValues([rowData]);
  }

  // Return result of operation
  return ContentService.createTextOutput(result);
}

/**
* Remove leading and trailing single or double quotes
*/
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
