find_package(peff REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_library(interbuf SHARED)
target_compile_definitions(interbuf PRIVATE IS_INTERBUF_BASE_BUILDING=1)
set_target_properties(interbuf PROPERTIES CXX_STANDARD 17)
target_sources(interbuf PRIVATE ${HEADERS} ${SRC})
target_include_directories(interbuf PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/> $<INSTALL_INTERFACE:include/>)
target_link_libraries(interbuf PUBLIC peff_base peff_utils peff_containers)
set_target_properties(interbuf PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    ARCHIVE FALSE
)

add_library(interbuf_static STATIC)
target_compile_definitions(interbuf_static PRIVATE IS_INTERBUF_BASE_BUILDING=1)
target_compile_definitions(interbuf_static PUBLIC INTERBUF_STATIC_LINK=1 IS_INTERBUF_BASE_BUILDING=1)
set_target_properties(interbuf_static PROPERTIES CXX_STANDARD 17)
target_sources(interbuf_static PRIVATE ${HEADERS} ${SRC})
target_link_libraries(interbuf_static PUBLIC peff_base_static peff_utils_static peff_containers_static)
target_include_directories(interbuf_static PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/> $<INSTALL_INTERFACE:include/>)
set_target_properties(interbuf_static PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
    VERSION ${PROJECT_VERSION}
)

install(TARGETS interbuf interbuf_static
    EXPORT interbufTargets
    LIBRARY DESTINATION lib/
    ARCHIVE DESTINATION lib/
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/peff/base/
)
