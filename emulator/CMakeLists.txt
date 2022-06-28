add_executable(server_emulator emulator.cpp stdafx.h handler.h handler.cpp)
target_link_libraries(server_emulator PRIVATE cpprestsdk::cpprest)