cmake_minimum_required(VERSION 3.28)

project(AplicacionMusica VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configuración estándar de Qt
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.9)

# 2. Carga de dependencias externas y herramientas
find_package(Doxygen)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/libpq")
    list(APPEND CMAKE_PROGRAM_PATH "/opt/homebrew/opt/libpq/bin")
    set(PostgreSQL_ROOT "/opt/homebrew/opt/libpq")
    if(DEFINED ENV{PKG_CONFIG_PATH})
        set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/opt/libpq/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    else()
        set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/opt/libpq/lib/pkgconfig")
    endif()
endif()

# Detección de libpqxx (CMake Config en macOS / pkg-config en Ubuntu Docker)
find_package(libpqxx CONFIG QUIET)
if(libpqxx_FOUND)
    set(PQXX_TARGET libpqxx::pqxx)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)
    set(PQXX_TARGET PkgConfig::PQXX)
endif()

# Descarga de GoogleTest y nlohmann/json
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(googletest json)

include(GNUInstallDirs)
include(GoogleTest)

# 3. Biblioteca principal del proyecto
add_library(AplicacionMusica
    src/Types.cpp
    src/Performers.cpp
    src/Persons.cpp
    src/Groups.cpp
    src/In_Group.cpp
    src/Albums.cpp
    src/Rolas.cpp
    include/InterFacadeTablas.hpp
    include/Types.hpp
    include/Performers.hpp
    include/Persons.hpp
    include/Groups.hpp
    include/In_Group.hpp
    include/Albums.hpp
    include/Rolas.hpp
)

target_link_libraries(AplicacionMusica
    PUBLIC
    ${PQXX_TARGET}
    nlohmann_json::nlohmann_json
)

target_include_directories(AplicacionMusica
    PUBLIC include
)

if(APPLE)
    target_include_directories(AplicacionMusica
        SYSTEM PUBLIC
        "/opt/homebrew/include"
        "/opt/homebrew/opt/libpq/include"
    )
endif()

target_compile_features(AplicacionMusica
    PUBLIC
    cxx_std_17
)

target_compile_options(AplicacionMusica
    PUBLIC
    -Wall -Wextra
)

# 4. Ejecutable de la aplicación de usuario (QML)
qt_add_executable(appAplicacionMusica
    main.cpp
)

qt_add_qml_module(appAplicacionMusica
    URI AplicacionMusica
    VERSION 0.1
    QML_FILES
        Main.qml
)

set_target_properties(appAplicacionMusica PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAplicacionMusica
    PRIVATE
    Qt6::Quick
    AplicacionMusica
)

# 5. Pruebas unitarias
add_executable(AplicacionMusicaTest
    tests/TypesTest.cpp
    tests/PerformersTest.cpp
)

target_link_libraries(AplicacionMusicaTest
    PRIVATE
    AplicacionMusica
    GTest::gtest_main
)

enable_testing()
gtest_discover_tests(AplicacionMusicaTest)

# 6. Instalación y documentación
install(TARGETS appAplicacionMusica
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(DOXYGEN_FOUND)
    add_custom_target(doc
      COMMAND ${DOXYGEN_EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generando documentación con doxygen."
    )
endif()