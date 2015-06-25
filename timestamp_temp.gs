//-----------------------------------------------
//Originally published by Mogsdad@Stackoverflow
//Modified for jarkomdityaz.appspot.com
//Modified by github.com/HeatherFlux for CCC NASA HAB project
//-----------------------------------------------
/*

GET request query:
https://script.google.com/macros/s/<gscript id>/exec?tempData=data_here

*/


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
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    //var waktu = new Date();
    rowData[0] = new Date(); // Timestamp in column A
    for (var param in e.parameter) {
      Logger.log('In for loop, param='+param);
      var value = stripQuotes(e.parameter[param]);
      //Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        case 'tempData': //Parameter
          rowData[1] = value; //Value in column B
          break;
     // case 'column_C':
       // rowData[2] = value;
       // break;
        default:
          result = "unsupported parameter";
      }
    }
    Logger.log(JSON.stringify(rowData));

    // Write new row below
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
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
