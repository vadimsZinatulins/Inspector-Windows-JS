'use strict';

const iw = require('./build/Release/inspectorWindows');

module.exports = {
    /**
    * Retrieves information about Windows Application content at specific screen coordinates.
    * @param {Number} x Screen Coordinate X
    * @param {Number} y Screen Coordinate Y
    * @returns {Object} Informatio about Windows Application content
    */
    getInfoFromPoint(x, y) {
        return iw.getInfoFromPoint(x, y);
    }
}