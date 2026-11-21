#include <gtest/gtest.h>
#include "Rolas.hpp"
#include "Types.hpp"
#include "Performers.hpp"
#include "Albums.hpp"
#include "Persons.hpp"

TEST(RolasTest, CicloCRUD) {
    const int testId = 9991;

    // Entidades padre para satisfacer todas las llaves foráneas de Rolas
    Types t(testId, "Tipo Test");
    Performers perf(testId, testId, "Performer Test", "/img/perf.png");
    Albums alb(testId, "/music/album", "Album Test", 2024, "/img/cover.png");
    Persons per(testId, "Compositor Test", "Nombre Real", "1985-01-01", "2020-01-01");

    Rolas r(testId, testId, testId, testId, testId,
            "/music/album/01.mp3", "Rola Test", "01", 1, 2024, "Rock", "Comentario test");

    // Limpieza previa (de hijos a padres)
    r.Eliminar_Borrar();
    alb.Eliminar_Borrar();
    per.Eliminar_Borrar();
    perf.Eliminar_Borrar();
    t.Eliminar_Borrar();

    // 1. Crear (de padres a hijos)
    t.Crear_Insertar();
    perf.Crear_Insertar();
    alb.Crear_Insertar();
    per.Crear_Insertar();
    r.Crear_Insertar();

    // 2. Leer
    json res = r.Leer_Consultar(testId);
    ASSERT_FALSE(res.empty());
    EXPECT_EQ(res["title"], "Rola Test");
    EXPECT_EQ(res["track"], "01");
    EXPECT_EQ(res["disk_number"], 1);

    // 3. Actualizar (case 6 = title)
    Rolas rMod(testId, testId, testId, testId, testId,
               "/music/album/01.mp3", "Rola Editada", "01", 1, 2024, "Rock", "Comentario test");
    rMod.Actualizar_Modificar(6);
    json resMod = rMod.Leer_Consultar(testId);
    EXPECT_EQ(resMod["title"], "Rola Editada");

    // 4. Eliminar todo en orden inverso
    rMod.Eliminar_Borrar();
    EXPECT_TRUE(rMod.Leer_Consultar(testId).empty());

    alb.Eliminar_Borrar();
    per.Eliminar_Borrar();
    perf.Eliminar_Borrar();
    t.Eliminar_Borrar();
}
