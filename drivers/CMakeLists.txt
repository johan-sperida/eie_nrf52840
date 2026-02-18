zephyr_include_directories(BTN LED customDisplay)

add_subdirectory(BTN)
add_subdirectory(LED)
add_subdirectory(customDisplay)
add_subdirectory_ifdef(CONFIG_DISPLAY LCD)
