heatmap-native
=======

Node bindings for [heatmap](https://github.com/lucasb-eyer/heatmap), a high performance C-based heatmap image
generator. Unlike ```node-heatmap``` it does not have any external dependencies like
```node-canvas``` - internally, it uses the ```lodepng``` library to create the final image buffer.

Note that this is barely tested (and I do not know C well) and is not under very active development.
There are many options in ```heatmap``` that are not mapped - this has very basic functionality.

```npm install heatmap-native```

## Methods

### constructor - new Heatmap(width, height, radius)

Takes three arguments:

- width - width of final image
- height - height of final image
- radius - the radius of each individual heatmap point. Optional, defaults to 4.

### addPoint(x,y)

Adds a heatmap point.

### drawPng()

Renders the heatmap given the arguments and data previously provided. Returns a buffer
with the PNG file contained within.

All must be whole numbers.

Example usage:
    
    Heatmap = require('heatmap-native');
    h = new Heatmap(100,100,10);

    h.addPoint(1,1);
    h.addPoint(10,10);
    h.addPoint(50,50);

    png = h.drawPng();

    fs.writeFileSync('./png.png', png.toString('binary'), 'binary');