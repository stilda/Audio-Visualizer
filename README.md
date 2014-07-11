Audio-Visualizer
================

Real-time visualization of audio signal in a number of ways.

The goal of this project is to easily show different signal characterisctics on a different plots in real-time.

The core of this process is a directed graph of signal converters. 
Each node (converter) takes inputs, perform calculations on them and generate output that is broadcast to all child nodes.
Initial signal goes into the root of the graph and passes down through it with convertions on its way.

There is a bunch of atomic converters. 
For example it can be filter, smoother, derivator, histogram in moving window, etc.
An incoming sequence of floats can be converted to sequence of tripples representing colors.
Then on lower level of the graph sequence of colors can be converted to sequence of matrices of colored pixels.

There is a bunch of atomic visualizers, plots that can draw different types of sequences - 1D signals, 2D curves, 3D surfaces.
It is possible to bind visualizers to any node of graph to see the signal there.


