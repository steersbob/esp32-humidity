idf_component_register(
  SRCS
    wifi.cpp
    mqtt.cpp
    sensor.cpp
    main.cpp
  INCLUDE_DIRS
    .
)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
target_compile_options(${COMPONENT_LIB} PRIVATE "-fno-exceptions")
