# ESP32 Humidity test project

Reads a DHT22 sensor, and publishes MQTT events to Brewblox.

## Notes

ESP installed through the VSCode Espressif IDF plugin setup.

Additional notes from https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html

Convenience aliases added to `~/.bash_aliases`:
```
alias dotdf='. $HOME/esp-idf/export.sh'
alias idf='idf.py'
```

Added user to `dialout` group.

My board (ESP-WROOM-32 dev) requires the 'boot' button to be pressed during the flash,
when it prints `connecting ....____....`.
Once it is busy transferring, you can release the button.
This does not appear to be the case for all models.

