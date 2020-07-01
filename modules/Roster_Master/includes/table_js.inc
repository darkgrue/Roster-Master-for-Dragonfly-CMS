<?php 
/***********************************************************************
  Roster Master for Dragonfly(TM) CMS
  **********************************************************************
  Copyright (C) 2005-2020 by Dark Grue

  JavaScript scroll- and resize-able table module.

  License:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA
***********************************************************************/
if (!defined('CPG_NUKE')) { exit; }


echo '<script type="text/javascript">
<!-- Hide script from old browsers.'."\n";

echo '// ************* VSCROLL TABLE **************
function mid'.$module_id.'_createElement(element) {
  if (typeof document.createElementNS != "undefined") {
    return document.createElementNS("http://www.w3.org/1999/xhtml", element);
  }
  if (typeof document.createElement != "undefined") {
    return document.createElement(element);
  }

  return false;
}

function mid'.$module_id.'_fixRowWidth(tbl) {
  var rowLenInd = tbl.tHead.rows[0].cells.length;
  var newRow = mid'.$module_id.'_createElement("tr");

  for (var i = 0; i < rowLenInd; i++) {
  var cell = tbl.tHead.rows[0].cells[i];

    cell.width = cell.offsetWidth - ((cell.currentStyle) ? parseInt(cell.currentStyle["paddingLeft"]) + parseInt(cell.currentStyle["paddingRight"]): 0);
    cell.nowrap = "nowrap";

    // Create spacer image.
    var newSpacer = mid'.$module_id.'_createElement("img");
    newSpacer.src = "modules/'.$mod_dirname.'/images/blank.png";
    newSpacer.alt = "";
    newSpacer.border = 0;
    newSpacer.width = cell.width;
    newSpacer.height = 0;
    newSpacer.style.margin = 0;
    newSpacer.style.padding = 0;
    // Move spacer into heading cell.
    if (i != 0) cell.appendChild(mid'.$module_id.'_createElement("br"));
    cell.appendChild(newSpacer);

    // Clone td node and spacer image into a new row.
    var newCell = mid'.$module_id.'_createElement("td");
    newCell.width = cell.width;
    newCell.nowrap = "nowrap";
//  newCell.height = 0;
    newCell.style.height = "0px";
    newCell.style.paddingTop = newCell.style.paddingBottom = 0;
    newCell.style.marginTop = newCell.style.marginBottom = 0;
    newCell.appendChild(newSpacer.cloneNode(true));
    newRow.appendChild(newCell);
  }

  // Insert new spacer row into table before row2.
  tbl.tBodies[0].insertBefore(newRow, tbl.tBodies[0].rows[0]);

  tbl.style.tableLayout = "fixed";
}

function mid'.$module_id.'_getScrollbarWidth(element) {
  var tDiv = mid'.$module_id.'_createElement("div");
  var tTextarea = mid'.$module_id.'_createElement("textarea");

  tTextarea.rows = 2;
  tDiv.appendChild(tTextarea);

  element.appendChild(tDiv);

  tTextarea.wrap = "off";
  scrollBarW = tTextarea.offsetHeight;
  tTextarea.wrap = "soft";
  scrollBarW -= tTextarea.offsetHeight;

  element.removeChild(tDiv);

  //alert("Your scrollbars are " + scrollBarW + " pixels wide.");
  return (scrollBarW > 0) ? scrollBarW : 25;
}

var mid'.$module_id.'_oldOnMouseUp;
var mid'.$module_id.'_tblHeight;
var mid'.$module_id.'_pDiv;

function mid'.$module_id.'_makeScrollableTable(tblID) {
  var newTable, newBody, hdr, newCell, newRow, newSpacer;

  // Get TABLE node.
  if (!document.getElementById) return;
  var tbl = document.getElementById(tblID);

  // Get parent DIV node.
  mid'.$module_id.'_pDiv = tbl.parentNode;
  // Get parent TD node.
  var ptd = tbl.parentNode.parentNode;

  // Fix column widths in table header and body.
  mid'.$module_id.'_fixRowWidth(tbl);

  // Get current table dimensions.
  var tblWidth = tbl.offsetWidth;
  var tblHeight = tbl.offsetHeight;

  // Clone the original table node.
  newTable = tbl.cloneNode(false);
  newTable.id += ":Header";
  newTable.width = tblWidth;
  newTable.style.tableLayout = "fixed";
  // Move original THEAD into new table node.
  newTable.appendChild(tbl.tHead);
  // Move into beginning of DIV node.
  ptd.insertBefore(newTable, mid'.$module_id.'_pDiv);

  tbl.width = tblWidth;
  tbl.style.tableLayout = "fixed";

  // Wrap the onmouseup method to keep compatibility with other scripts
  // that may be running on the page.
  mid'.$module_id.'_oldOnMouseUp = (typeof document.onmouseup == "function") ? document.onmouseup : function() {};
  document.onmouseup = function(evt) { mid'.$module_id.'_oldOnMouseUp(evt); mid'.$module_id.'_setNewPos(evt); };
}

// ************** TABLE RESIZE **************
// Global variables for saving state.
var mid'.$module_id.'_dragEnabled = false;
var mid'.$module_id.'_dragStartPos;

function mid'.$module_id.'_getPosition(evt) {
  evt = (evt) ? evt : ((window.event) ? window.event : "");
  if (evt) {
    if (!document.all) {
      mid'.$module_id.'_dragStartPos = evt.screenY;
    } else {
      mid'.$module_id.'_dragStartPos = evt.clientY;
    }
  }

  document.body.style.cursor = "move";
  mid'.$module_id.'_dragEnabled = true;

  return false;
}

function mid'.$module_id.'_setNewPos(evt) {
  evt = (evt) ? evt : ((window.event) ? window.event : "");
  if (evt && mid'.$module_id.'_dragEnabled) {
    if (!document.all) {
      endPos = evt.screenY;
    } else {
      endPos = evt.clientY;
    }
    diffpos = mid'.$module_id.'_dragStartPos - endPos;

    var newHeight = parseInt(mid'.$module_id.'_pDiv.style.height) - diffpos;
    mid'.$module_id.'_pDiv.style.height = ((newHeight > 10) ? ((newHeight > mid'.$module_id.'_tblHeight) ? mid'.$module_id.'_tblHeight : newHeight) : 10) + "px";

    document.body.style.cursor = "";
    mid'.$module_id.'_dragEnabled = false;
  }
}'."\n\n";

echo '// ************* ROW HIGHLIGHT **************
// Global variables for saving state.
var mid'.$module_id.'_savedStates = new Array();
var mid'.$module_id.'_rowLocked = false;

// This function takes an element as a parameter and returns an object
// which contain the saved state of the element\'s background color.
function mid'.$module_id.'_saveBackgroundStyle(myElement) {
  var saved = new Object();

  saved.element = myElement;
  saved.className = myElement.className;
  saved.backgroundColor = myElement.style["backgroundColor"];

  return saved;
}

// This function restores the restores the saved state of all elements\' background color.
function mid'.$module_id.'_restoreBackgroundStyle() {
  var state = new Object();

  while(state = mid'.$module_id.'_savedStates.pop()) {
    state.element.style["backgroundColor"] = state.backgroundColor;
    if (state.className) {
      state.element.className = state.className;
    }
  }
}

// This function is used by mid'.$module_id.'_highlightTableRow() to
// find table cells (TD) nodes.
function mid'.$module_id.'_findNode(startingNode, tagName) {
  myElement = startingNode;

  // The browser DOM may not have the TD node as the firstChild node of the TR node.
  var i = 0;
  while(myElement && (!myElement.tagName || (myElement.tagName && myElement.tagName != tagName))) {
    myElement = startingNode.childNodes[i++];
  }
  if (myElement && myElement.tagName && (myElement.tagName == tagName)) {
    return myElement;
  }
  else if (startingNode.firstChild) {
    return mid'.$module_id.'_findNode(startingNode.firstChild, tagName);
  }

  return false;
}

// Highlight table row.
function mid'.$module_id.'_highlightTableRow(myElement, myTable, highlightColor, click) {
  if (mid'.$module_id.'_rowLocked && !click) {
    // Table highlighting is locked, bail out.
    return;
  }

  // Restore color of the previously highlighted row.
  mid'.$module_id.'_restoreBackgroundStyle();

  // Traverse the parent nodes upwards to find a row (TR) element that is contained within the calling table.
  // Netscape has a weird node (if the mouse is over a text object, then there\'s no tagName attribute).
  while(myElement && ((myElement.tagName && myElement.tagName != "TR") || (myElement.parentNode && (myTable != myElement.parentNode.parentNode)) || !myElement.tagName)) {
    myElement = myElement.parentNode;
  }

  // Do not highlight table header or footer.
  if (!myElement || (myElement.parentNode && (myElement.parentNode.tagName == "THEAD") || (myElement.parentNode.tagName == "TFOOT"))) {
    return;
  }

  // Highlight every cell in the row.
  if (myElement) {
    var tableRow = myElement;

    // Save the backgroundColor style OR the style class of the row (if
    // defined).
    if (tableRow) {
      mid'.$module_id.'_savedStates.push(mid'.$module_id.'_saveBackgroundStyle(tableRow));
    }

    // myElement is a TR, find the first TD.
    var tableCell = mid'.$module_id.'_findNode(myElement, "TD");

    // Loop through every sibling (a sibling of a cell should be a cell) and
    // set the highlight.
    var i = 0;
    while(tableCell) {
      // Make sure it is actually a cell (a TD).
      if (tableCell.tagName == "TD") {
        // If no style has been assigned, assign it, otherwise Netscape will behave weird.
        if (!tableCell.style) {
          tableCell.style = {};
        } else {
          mid'.$module_id.'_savedStates.push(mid'.$module_id.'_saveBackgroundStyle(tableCell));
        }
        // Assign the highlight color.
        tableCell.style["backgroundColor"] = highlightColor;

        i++;
      }

      // Go to the next cell in the row.
      tableCell = tableCell.nextSibling;
    }
  }
}

// If the element that fired the event is within a table row, this
// function will highlight the row.
// This function is to be assigned to a TABLE mouse event handler.
function mid'.$module_id.'_trackTableHighlight(mEvent, myTable, highlightColor, click) {
  if (click) {
    if (mid'.$module_id.'_rowLocked) {
      mid'.$module_id.'_rowLocked = false;
    } else {
      highlightColor = "'.$config['rowhighlight_color'].'";
      mid'.$module_id.'_rowLocked = true;
    }
  } else if (mid'.$module_id.'_rowLocked) {
    // Table highlighting is locked, bail out.
    return;
  }

  if (!mEvent) {
    mEvent = window.event;
  }

  // Internet Explorer
  if (mEvent.srcElement) {
    mid'.$module_id.'_highlightTableRow(mEvent.srcElement, myTable, highlightColor, click);
  }
  // Netscape and Firefox
  else if (mEvent.target) {
    mid'.$module_id.'_highlightTableRow(mEvent.target, myTable, highlightColor, click);
  }
}'."\n";

echo '// End -->
</script>'."\n";