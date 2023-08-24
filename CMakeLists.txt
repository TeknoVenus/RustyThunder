project(RustyPlugin)

cmake_minimum_required(VERSION 3.3)

include(FetchContent)

find_package(WPEFramework)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)

# Corrosion provides nice Cargo <-> CMake integration
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Corrosion)

# Install CXX code generator (if not already)
find_program(CXXBRIDGE cxxbridge PATHS "$ENV{HOME}/.cargo/bin/")

if(CXXBRIDGE STREQUAL "CXXBRIDGE-NOTFOUND")
    message("Could not find cxxbridge, trying to install with `cargo install cxxbridge-cmd'")
    find_program(CARGO cargo PATHS "$ENV{HOME}/.cargo/bin/")

    if(CARGO STREQUAL "CARGO-NOTFOUND")
        message(FATAL_ERROR "Requires cargo available in path, install via rustup https://rustup.rs/")
    endif()

    execute_process(COMMAND ${CARGO} install cxxbridge-cmd)
    find_program(CXXBRIDGE cxxbridge PATHS "$ENV{HOME}/.cargo/bin/")
endif()

# Trigger CXX code generation
corrosion_import_crate(MANIFEST_PATH Rust/Cargo.toml)
corrosion_add_cxxbridge(rust_beer_bridge CRATE rust_beer MANIFEST_PATH Rust FILES lib.rs)

add_library(${MODULE_NAME} SHARED
    RustyPlugin.cpp
    RustProxy.cpp
    Module.cpp
)

set_target_properties(${MODULE_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

target_link_libraries(${MODULE_NAME}
    PRIVATE
    CompileSettingsDebug::CompileSettingsDebug
    ${NAMESPACE}Plugins::${NAMESPACE}Plugins

    # Rust library
    rust_beer_bridge
)

install(TARGETS ${MODULE_NAME}
    DESTINATION lib/${STORAGE_DIRECTORY}/plugins)

write_config()