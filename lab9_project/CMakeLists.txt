add_executable(lab9.elf main.c gameControl.c snake.c snakeMap.c)
target_link_libraries(lab9.elf ${330_LIBS} touchscreen intervalTimer interrupts buttons_switches)
set_target_properties(lab9.elf PROPERTIES LINKER_LANGUAGE CXX)