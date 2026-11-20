#include "Groups.hpp"

Groups::Groups(int id_group, std::string name, std::string start_date, std::string end_date, int performers_id)
    : id_group(id_group),
    name(std::move(name)),
    start_date(std::move(start_date)),
    end_date(std::move(end_date)),
    performers_id(performers_id) {

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

Groups::~Groups() {
    conn.reset();
}

void Groups::Crear_Insertar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    txn.exec_params(
        "INSERT INTO Groups (id_group, name, start_date, end_date, performers_id) "
        "VALUES ($1, $2, $3, $4, $5);",
        this->id_group,
        this->name,
        this->start_date,
        this->end_date,
        this->performers_id
        );

    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}

json Groups::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_group, name, start_date, end_date, performers_id "
        "FROM Groups WHERE id_group = $1;",
        id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_group"].as<int>(0)) {
                this->id_group = row["id_group"].as<int>(0);
                this->name = row["name"].as<std::string>("");
                this->start_date = row["start_date"].as<std::string>("");
                this->end_date = row["end_date"].as<std::string>("");
                this->performers_id = row["performers_id"].as<int>(0);

                return json{
                    {"id_group", this->id_group},
                    {"name", this->name},
                    {"start_date", this->start_date},
                    {"end_date", this->end_date},
                    {"performers_id", this->performers_id}
                };
            }
        }
    }
    return json{};
}

void Groups::Actualizar_Modificar(int elemento) {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE Groups SET name = $1 WHERE id_group = $2;", this->name, this->id_group);
        break;
    case 2:
        txn.exec_params("UPDATE Groups SET start_date = $1 WHERE id_group = $2;", this->start_date, this->id_group);
        break;
    case 3:
        txn.exec_params("UPDATE Groups SET end_date = $1 WHERE id_group = $2;", this->end_date, this->id_group);
        break;
    case 4:
        txn.exec_params("UPDATE Groups SET performers_id = $1 WHERE id_group = $2;", this->performers_id, this->id_group);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }

    txn.commit();
    std::cout << "Se actualizo tu parametro con exito." << std::endl;
}

void Groups::Eliminar_Borrar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params("DELETE FROM Groups WHERE id_group = $1;", this->id_group);
    txn.commit();
    std::cout << "Se elimino la fila con exito." << std::endl;
}
