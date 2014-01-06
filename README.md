# EtherCurve

`EtherCurve` is a tool for visualizing packets on your network. The
packets are drawn using [a space-filling Hilbert
curve](http://en.wikipedia.org/wiki/Hilbert_curve). Packets are scaled
by their sizes and coloured using a qualitative colour map that can
easily be customized.

## Requirements

* `cmake` >= 2.8.9 (for building the program)
* `Qt` >= 5.2 (for rendering)
* `libpcap` >= 1.3.0 (older versions should work as well because I am
  not doing anything special here)

## Build instructions

After extracting the source files to a directory, the following should
be sufficient to build `EtherCurve`:

    $ mkdir build
    $ cd build
    $ cmake ../
    $ make

## Usage

`EtherCurve` provides you with a prompt for choosing a network
interface. Provided you do not wish to change any options, a simple
`./ethercurve` suffices to start the tool. See `./ethercurve -h` for
more options.

## Changing the colour scheme

`EtherCurve` can read a text file containing RGB colours. These colours
will be used for packet rendering. The file needs to consist of triples
of integers in the range from 0&ndash;255. In addition, the file needs
to have at least 4 lines, as `EtherCurve` currently knows three types of
packets, i.e. TCP, UDP, ICMP, and the rest. The following is a valid
file for `EtherCurve`:

    255,0,0
    0,255,0
    0,0,255
    255,255,255

## Licence

`EtherCurve` is released under the MIT licence. See the file `LICENCE`
in the source directory for details.
