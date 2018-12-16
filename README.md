# SanLa-classic
Sanomalaite M/90 homage

## Repo structure

| Dir/File | description |
| --- | --- |
include | Put project header files here
lib | Put here project specific (private) libraries
src | Put project source files here
platformio.ini | Project Configuration File

## PlatformIO commands

Process/build project from the current directory

`pio run`

Upload firmware to a target

`pio run --target upload` or `pio run -t upload`

Clean project (remove compiled files)

`pio run --target clean`

Additional information

`pio run --help`

(LINUX)NOTE: be sure to have udev rules in place [rules](https://raw.githubusercontent.com/platformio/platformio-core/develop/scripts/99-platformio-udev.rules). [More info](https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules)