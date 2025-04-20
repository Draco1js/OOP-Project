cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

if(WIN32)
    add_executable(main WIN32 
        src/main.cpp
        src/Game.cpp
        src/Fighter.cpp
        src/Player.cpp
        src/HealthBar.cpp
        src/Animation.cpp
        src/Damage.cpp
        src/Attack.cpp
    )
    # When using WIN32, you need to modify your main function to use WinMain
    # Or link with the SFML main library which handles this for you
    target_link_libraries(main PRIVATE SFML::Graphics SFML::Main)
else()
    add_executable(main 
        src/main.cpp
        src/Game.cpp
        src/Fighter.cpp
        src/Player.cpp
        src/HealthBar.cpp
        src/Animation.cpp
        src/Damage.cpp
        src/Attack.cpp
    )
    target_link_libraries(main PRIVATE SFML::Graphics)
endif()
target_compile_features(main PRIVATE cxx_std_17)

if(APPLE)
    # Set the app icon
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    
    # Set the app bundle properties
    set_target_properties(main PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Street Fighter Clone"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.draco1js.streetfighterclone"
    )
    
    # Make sure the executable is built as a GUI application
    target_link_options(main PRIVATE -Wl,-sectcreate,__TEXT,__info_plist,${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()

add_executable(minimal_test src/minimal_test.cpp)
target_link_libraries(minimal_test PRIVATE SFML::Graphics)
target_compile_features(minimal_test PRIVATE cxx_std_17)
