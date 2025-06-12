cmake_minimum_required(VERSION 3.16)
project(LayoutOverlapDetector LANGUAGES CXX)

# 1) C++17 + auto Qt moc/uic/rcc
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 2) Qt6
find_package(Qt6 COMPONENTS Core Quick REQUIRED)

# 3) nlohmann-json (header-only, but we still need the include path)
find_package(nlohmann_json CONFIG REQUIRED)

# 4) moc for Q_OBJECT headers
set(MOC_HEADERS
    ${CMAKE_SOURCE_DIR}/include/LayoutModel.h
    ${CMAKE_SOURCE_DIR}/include/WorkerThread.h
)
qt6_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

# 5) resources
qt6_add_resources(RESOURCES
    resources/resources.qrc
)

# 6) executable
add_executable(LayoutOverlapDetector
    src/main.cpp
    src/Rectangle.cpp
    src/OverlapEngine.cpp
    src/LayoutModel.cpp
    src/WorkerThread.cpp
    ${MOC_SOURCES}
    ${RESOURCES}
)

target_include_directories(LayoutOverlapDetector PRIVATE include)

# 7) link everything
target_link_libraries(LayoutOverlapDetector
    PRIVATE
        Qt6::Core
        Qt6::Quick
        nlohmann_json::nlohmann_json   # <-- this fixes the header-path error
)
