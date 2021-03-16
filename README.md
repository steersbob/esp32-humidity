# ESP32 Humidity test project

Reads a DHT22 sensor, and publishes MQTT events to Brewblox.

## Notes

ESP installed through the VSCode Espressif IDF plugin setup.

Additional notes from https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html

Added the ESP export script to terminal shell args in VSCode settings.

Added user to `dialout` group.

~~My board (ESP-WROOM-32 dev) requires the 'boot' button to be pressed during the flash,~~
~~when it prints `connecting ....____....`.~~
~~Once it is busy transferring, you can release the button.~~
~~Circumventing this issue would require [soldering a capacitator to the board](https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/).~~

The above does not seem to be the case anymore. Unclear whether this was changed in a ESP update.

DHT22 code copied from https://github.com/gosouth/DHT22.

The .vscode/settings.json file includes paths to python executable.
This will need updating when run on machines not configured to use pyenv.

## To use

In a new shell, run `source $HOME/esp/esp-idf/export.sh`. (This is done automatically in VSCode terminals)

Due to it including Wifi config info, sdkconfig is included in .gitignore.
To generate it, run:

```
idf.py menuconfig
```
Edit the settings for Humidity.

To flash your ESP32, run:

```
idf.py flash monitor
```

~~Press the 'boot' button when it prints `connecting......____....`~~

Press Ctrl+] to exit monitor.
