cmake_minimum_required(VERSION 3.28)

project(AplicacionMusica VERSION 0.1 LANGUAGES CXX)

if(APPLE)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Configuración estándar de Qt (6.4 funciona en Ubuntu 24.04 Docker y en macOS)
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.4)

# 2. Rutas específicas para macOS (Homebrew) antes de buscar paquetes
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/libpq" "/opt/homebrew")
    list(APPEND CMAKE_PROGRAM_PATH "/opt/homebrew/opt/libpq/bin")
    set(PostgreSQL_ROOT "/opt/homebrew/opt/libpq")
    set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/opt/libpq/lib/pkgconfig:/opt/homebrew/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()

find_package(Doxygen)
find_package(PkgConfig REQUIRED)

if(APPLE)
    pkg_check_modules(TAGLIB QUIET IMPORTED_TARGET taglib)
else()
    pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib)
endif()


find_package(libpqxx CONFIG QUIET)
if(libpqxx_FOUND)
    set(PQXX_TARGET libpqxx::pqxx)
else()
    pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)
    set(PQXX_TARGET PkgConfig::PQXX)
endif()

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

# 4. Biblioteca principal del proyecto
add_library(AplicacionMusica
    # Definición de los cpp para las clases de las tablas
    src/Types.cpp
    src/Performers.cpp
    src/Persons.cpp
    src/Groups.cpp
    src/In_Group.cpp
    src/Albums.cpp
    src/Rolas.cpp
    #Definición de la implementación para el compilador.
    src/CatalogoEsquema.cpp
    src/NodoCriterio.cpp
    src/ConsultaBuilder.cpp
    src/GeneradorSQL.cpp
    src/CompiladorBusquedasFacade.cpp
    #Definición de la implementación para el minero
    src/Minero.cpp
    # Definición de los hpp para las clases de las tablas
    include/InterFacadeTablas.hpp
    include/Types.hpp
    include/Performers.hpp
    include/Persons.hpp
    include/Groups.hpp
    include/In_Group.hpp
    include/Albums.hpp
    include/Rolas.hpp
    #Definicion de clases para el compilador.
    include/CatalogoEsquema.hpp
    include/NodoCriterio.hpp
    include/ConsultaBuilder.hpp
    include/GeneradorSQL.hpp
    include/CompiladorBusquedasFacade.hpp
    include/TiposCompilador.hpp
    include/ConsultaAST.hpp
    #Definicion de las clases y structs para el minero
    include/MetaDatosMP3.hpp
    include/Minero.hpp
)

target_include_directories(AplicacionMusica
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(APPLE)
    target_include_directories(AplicacionMusica
        SYSTEM PUBLIC
        "/opt/homebrew/include"
        "/opt/homebrew/opt/libpq/include"
    )
    target_compile_features(AplicacionMusica PUBLIC cxx_std_20)
else()
    target_compile_features(AplicacionMusica PUBLIC cxx_std_17)
endif()

target_link_libraries(AplicacionMusica
    PUBLIC
    $<$<BOOL:${TAGLIB_FOUND}>:PkgConfig::TAGLIB>
    ${PQXX_TARGET}
    nlohmann_json::nlohmann_json
)


target_compile_options(AplicacionMusica
    PUBLIC
    -Wall -Wextra
)

# 5. Ejecutable de la aplicación de usuario (QML)
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

# 6. Pruebas unitarias
add_executable(AplicacionMusicaTest
    tests/TypesTest.cpp
    tests/PerformersTest.cpp
    tests/In_GroupTest.cpp
    tests/AlbumsTest.cpp
    tests/GroupsTest.cpp
    tests/PersonsTest.cpp
    tests/RolasTest.cpp
)

target_link_libraries(AplicacionMusicaTest
    PRIVATE
    AplicacionMusica
    GTest::gtest_main
)

enable_testing()
gtest_discover_tests(AplicacionMusicaTest)

# 7. Instalación y documentación
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




