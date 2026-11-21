#include <gtest/gtest.h>
#include "Types.hpp"

TEST(TypesTest, CicloCRUD) {
    const int testId = 9999;

    // 1. Crear (limpiando primero por si quedó basura de una corrida previa)
    Types t(testId, "Tipo Test");
    t.Eliminar_Borrar();
    t.Crear_Insertar();

    // 2. Leer
    json res = t.Leer_Consultar(testId);
    ASSERT_FALSE(res.empty());
    EXPECT_EQ(res["description"], "Tipo Test");

    // 3. Actualizar
    Types tMod(testId, "Tipo Editado");
    tMod.Actualizar_Modificar(1);
    json resMod = tMod.Leer_Consultar(testId);
    EXPECT_EQ(resMod["description"], "Tipo Editado");

    // 4. Eliminar
    tMod.Eliminar_Borrar();
    EXPECT_TRUE(tMod.Leer_Consultar(testId).empty());
}
