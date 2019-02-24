# pd_helios
Pure data external for the Helios Laser DAC

* https://github.com/Grix/helios_dac
* https://github.com/pure-data/pure-data

## Version

0.1

## Use

pd_helios currently builds on MacOS, tested on Mojave (OSX v10.14)

## To build

Clone this repo and the source code to pure data (see above)

```
cd pd_helios

make PDINCLUDEDIR=../pure-data/src/
```

(or wherever you cloned pure data)

To run the example patch:

```
pd help-helios.pd
```

(from wherever you have installed Pd. For the Purr-data distribution of Pd, I used this:)

```
/Applications/Pd-l2ork.app/Contents/MacOS/nwjs help-helios.pd 
```

## Copyright

Except as otherwise noted, all files in the this distribution are

###Copyright (c) 2019 Tim Redfern

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see LICENSE.txt included in this distribution.
(Note that Helios SDK and libusb are copyrighted separately).

## NOTE

Lasers are dangerous. Improper use of this software can damage your laser, or yourself. This software comes wit NO WARRANTY, see above.

## Acknowledgements

Thanks to NULL + VOID