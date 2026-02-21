zephyr_include_directories(BTN LED scenes)

add_subdirectory(BTN)
add_subdirectory(LED)
add_subdirectory(scenes)
add_subdirectory_ifdef(CONFIG_DISPLAY LCD)
