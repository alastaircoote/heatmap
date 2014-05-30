var HeatmapNative = require('./build/Release/heatmap.node');
var fs = require('fs');

heatmap = function(width, height, radius) {
    if (!this._checkForWholeNumber(width))
        throw new Error("Width must be a whole number.");
    if (!this._checkForWholeNumber(height))
        throw new Error("Height must be a whole number.");
    if (!radius)
        radius = 4;
    if (!this._checkForWholeNumber(radius))
        throw new Error("Radius must be a whole number.");

    this._width = width;
    this._height = height;
    this._radius = radius;
    this._dataPoints = [];

}

heatmap.prototype = {
    _checkForWholeNumber: function(i) {
        return Math.round(i) == i;
    },
    addPoint: function(x,y) {
        if (!x || !y)
            throw new Error("Point must have x and y coordinates.");
        this._dataPoints.push([x,y]);
    },
    drawPng: function() {
        var hm = HeatmapNative.create(this._width, this._height,this._radius,this._dataPoints);
        return hm;
    }
}

module.exports = heatmap;
