cmake_minimum_required (VERSION 3.8)

project(wnd_service_test)

add_executable (wnd_service_test WIN32
    "wnd_service_test.cpp"
    "../WndService.cpp"
)

target_compile_definitions(wnd_service_test PRIVATE UNICODE _UNICODE)
