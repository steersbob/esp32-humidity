# ESP32 Humidity test project

Reads a DHT22 sensor, and publishes MQTT events to Brewblox.

DHT22 code based on https://github.com/gosouth/DHT22.

## Config

Wifi SSID/password and MQTT broker URL are configured through sdkconfig, with the generated file being ignored in git.

To generate the sdkconfig file with your settings, run the "SDK Config" task.
The custom settings can be found under "Humidity configuration".

## Development: devcontainer

The development environment is based on the [development container](https://code.visualstudio.com/docs/remote/containers) feature in the [Visual Studio Code](https://code.visualstudio.com/) editor.
A [Dockerfile](./.devcontainer/Dockerfile) is provided that includes the required dependencies for most build types.

When opening the repository in VSCode, you will be prompted to reload the editor in the container.
When this is done, intellisense, build tasks, and the VSCode terminal will run in the container.

If you wish to use a different editor, you will need to either manually build the Docker image, or reproduce the installation steps on your local machine.
