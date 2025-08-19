cmake_minimum_required(VERSION 3.13)
project(interbuf VERSION 0.1.0)

add_subdirectory("interbuf")
add_subdirectory("example")

# Generate the version file for the config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/interbufConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Create config file
configure_package_config_file(
    cmake/interbufConfig.cmake.in interbufConfig.cmake
    INSTALL_DESTINATION lib/cmake/interbuf
)

# Install config files
install(
    FILES
       ${CMAKE_CURRENT_BINARY_DIR}/interbufConfig.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/interbufConfigVersion.cmake
    DESTINATION
        lib/cmake/${PROJECT_NAME}
)

# Exporting Targets from the Build Tree
install(EXPORT interbufTargets
    FILE interbufTargets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)
