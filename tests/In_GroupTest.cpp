#include <gtest/gtest.h>
#include "In_Group.hpp"
#include "Persons.hpp"
#include "Groups.hpp"

TEST(InGroupTest, CicloCRUD) {
    const int id1 = 9996;
    const int id2 = 9995;

    Persons per(id1, "Stage Test", "Real Test", "1990-01-01", "2020-01-01");
    Groups g1(id1, "Grupo 1", "2010-01-01", "2020-01-01");
    Groups g2(id2, "Grupo 2", "2015-01-01", "2022-01-01");

    In_Group ig(id1, id1);
    In_Group igMod(id1, id2);

    // Limpieza previa
    ig.Eliminar_Borrar();
    igMod.Eliminar_Borrar();
    per.Eliminar_Borrar();
    g1.Eliminar_Borrar();
    g2.Eliminar_Borrar();

    // 1. Crear
    per.Crear_Insertar();
    g1.Crear_Insertar();
    g2.Crear_Insertar();
    ig.Crear_Insertar();

    // 2. Leer
    json res = ig.Leer_Consultar(id1);
    ASSERT_FALSE(res.empty());
    EXPECT_EQ(res["id_group"], id1);

    // 3. Actualizar (cambiar id_group de id1 a id2)
    igMod.Actualizar_Modificar(1);
    json resMod = igMod.Leer_Consultar(id1);
    EXPECT_EQ(resMod["id_group"], id2);

    // 4. Eliminar
    igMod.Eliminar_Borrar();
    EXPECT_TRUE(igMod.Leer_Consultar(id1).empty());

    per.Eliminar_Borrar();
}