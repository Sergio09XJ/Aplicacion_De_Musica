#include <gtest/gtest.h>
#include "Groups.hpp"

TEST(GroupsTest, CicloCRUD) {
    const int testId = 9993;

    // 1. Crear (limpieza previa por idempotencia)
    Groups g(testId, "Grupo Test", "2010-01-01", "2020-01-01");
    g.Eliminar_Borrar();
    g.Crear_Insertar();

    // 2. Leer
    json res = g.Leer_Consultar(testId);
    ASSERT_FALSE(res.empty());
    EXPECT_EQ(res["name"], "Grupo Test");
    EXPECT_EQ(res["start_date"], "2010-01-01");

    // 3. Actualizar (case 1 = name)
    Groups gMod(testId, "Grupo Editado", "2010-01-01", "2020-01-01");
    gMod.Actualizar_Modificar(1);
    json resMod = gMod.Leer_Consultar(testId);
    EXPECT_EQ(resMod["name"], "Grupo Editado");

    // 4. Eliminar
    gMod.Eliminar_Borrar();
    EXPECT_TRUE(gMod.Leer_Consultar(testId).empty());
}
