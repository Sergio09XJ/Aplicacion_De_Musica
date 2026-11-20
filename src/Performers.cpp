#include "Persons.hpp"

Persons::Persons(int id_persons, std::string stage_name, std::string real_name,
                 std::string birth_date, std::string death_date, int performers_id)
    : id_persons(id_persons),
    stage_name(std::move(stage_name)),
    real_name(std::move(real_name)),
    birth_date(std::move(birth_date)),
    death_date(std::move(death_date)),
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

Persons::~Persons() {
    conn.reset();
}

void Persons::Crear_Insertar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    txn.exec_params(
        "INSERT INTO Persons (id_persons, stage_name, real_name, birth_date, death_date, performers_id) "
        "VALUES ($1, $2, $3, $4, $5, $6);",
        this->id_persons,
        this->stage_name,
        this->real_name,
        this->birth_date,
        this->death_date,
        this->performers_id
        );

    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}

json Persons::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_persons, stage_name, real_name, birth_date, death_date, performers_id "
        "FROM Persons WHERE id_persons = $1;",
        id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_persons"].as<int>(0)) {
                this->id_persons = row["id_persons"].as<int>(0);
                this->stage_name = row["stage_name"].as<std::string>("");
                this->real_name = row["real_name"].as<std::string>("");
                this->birth_date = row["birth_date"].as<std::string>("");
                this->death_date = row["death_date"].as<std::string>("");
                this->performers_id = row["performers_id"].as<int>(0);

                return json{
                    {"id_persons", this->id_persons},
                    {"stage_name", this->stage_name},
                    {"real_name", this->real_name},
                    {"birth_date", this->birth_date},
                    {"death_date", this->death_date},
                    {"performers_id", this->performers_id}
                };
            }
        }
    }
    return json{};
}

void Persons::Actualizar_Modificar(int elemento) {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE Persons SET stage_name = $1 WHERE id_persons = $2;", this->stage_name, this->id_persons);
        break;
    case 2:
        txn.exec_params("UPDATE Persons SET real_name = $1 WHERE id_persons = $2;", this->real_name, this->id_persons);
        break;
    case 3:
        txn.exec_params("UPDATE Persons SET birth_date = $1 WHERE id_persons = $2;", this->birth_date, this->id_persons);
        break;
    case 4:
        txn.exec_params("UPDATE Persons SET death_date = $1 WHERE id_persons = $2;", this->death_date, this->id_persons);
        break;
    case 5:
        txn.exec_params("UPDATE Persons SET performers_id = $1 WHERE id_persons = $2;", this->performers_id, this->id_persons);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }

    txn.commit();
    std::cout << "Se actualizo tu parametro con exito." << std::endl;
}

void Persons::Eliminar_Borrar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params("DELETE FROM Persons WHERE id_persons = $1;", this->id_persons);
    txn.commit();
    std::cout << "Se elimino la fila con exito." << std::endl;
}
