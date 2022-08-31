#pragma once
#include <cstdint>

namespace humidity {

enum class SensorError : uint8_t {
    OK = 0,
    CHECKSUM_ERROR = 1,
    TIMEOUT_ERROR = 2,
};

SensorError read_sensor();
float get_humidity();
float get_temperature();

}
