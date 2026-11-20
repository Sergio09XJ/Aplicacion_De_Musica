#include "In_Group.hpp"

In_Group::In_Group(int id_person, int id_group)
    : id_person(id_person), id_group(id_group) {

    std::string conn_str = "dbname=musica user=postgres password=music_dev_password host=postgres port=5432";

    try {
        this->conn = std::make_unique<pqxx::connection>(conn_str);
        if (conn->is_open()) {
            std::cout << "La conexión con la base de datos fue Exitosa." << std::endl;
        } else {
            std::cout << "La conexión con la base de datos No fue Exitosa." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error crítico en la base de datos: " << e.what() << std::endl;
    }
}

In_Group::~In_Group() {
    conn.reset();
}

void In_Group::Crear_Insertar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    txn.exec_params(
        "INSERT INTO In_Group (id_person, id_group) VALUES ($1, $2);",
        this->id_person,
        this->id_group
        );

    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}

json In_Group::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_person, id_group FROM In_Group WHERE id_person = $1;",
        id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_person"].as<int>(0)) {
                this->id_person = row["id_person"].as<int>(0);
                this->id_group = row["id_group"].as<int>(0);

                return json{
                    {"id_person", this->id_person},
                    {"id_group", this->id_group}
                };
            }
        }
    }
    return json{};
}

void In_Group::Actualizar_Modificar(int elemento) {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE In_Group SET id_group = $1 WHERE id_person = $2;", this->id_group, this->id_person);
        break;
    case 2:
        txn.exec_params("UPDATE In_Group SET id_person = $1 WHERE id_group = $2;", this->id_person, this->id_group);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }

    txn.commit();
    std::cout << "Se actualizo tu parametro con exito." << std::endl;
}

void In_Group::Eliminar_Borrar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params(
        "DELETE FROM In_Group WHERE id_person = $1 AND id_group = $2;",
        this->id_person,
        this->id_group
        );
    txn.commit();
    std::cout << "Se elimino la fila con exito." << std::endl;
}
