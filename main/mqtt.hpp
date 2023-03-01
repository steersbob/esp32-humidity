#pragma once

namespace humidity {

void mqtt_init();

void publish(const char* topic, const char* data);

}
