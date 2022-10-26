# `getwpk`

A Linux kernel module for getting the Windows Product Key in the Microsoft Data Management table in
the Advanced Configuration and Power Interface.

## Make

### Building

To build `getwpk`, run `make` or `make build`.

### Cleaning

To clean the current working directory of each generated file, run `make clean`.

### Installing

To build and install `getwpk`, run `make install` as root.

### Uninstalling

To clean the current working directory of each generated file and uninstall `getwpk`, run
`make uninstall` as root.

## Usage

`getwpk` will write the WPK to a file called `wpk` in `/sys/devices/platform/getwpk/wpk`. Thus, to
use `getwpk` and therefore get the WPK, all you must do is run
`cat /sys/devices/platform/getwpk/wpk` as root.