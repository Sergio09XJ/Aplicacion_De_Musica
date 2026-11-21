#ifndef MINERO_HPP
#define MINERO_HPP

#include "MetadatosMP3.hpp"
#include "CompiladorBusquedasFacade.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <pqxx/pqxx>

/**
 * @class  LectorEtiquetasMP3; 
 * @brief  Esta clase nos permite acceder así como leer las etiquetas de los archivos mp3. 
 */
class LectorEtiquetasMP3 {
private:
    static void aplicarValoresPorDefecto(const std::filesystem::path& ruta, MetadatosMP3& meta);

public:
    MetadatosMP3 leer(const std::filesystem::path& ruta) const;
};

/**
 * @class  Minero; 
 * @brief  Esta clase sirve para acceder a la rutas de los archivos. 
 */
class Minero {
private:
    CompiladorBusquedasFacade& buscador;
    LectorEtiquetasMP3 lector;
    std::unique_ptr<pqxx::connection> conn;

    int nextIdPerformer = 2;
    int nextIdAlbum = 1;
    int nextIdRola = 1;

    static constexpr int ID_TIPO_DEFAULT = 1;
    static constexpr int ID_PERFORMER_DEFAULT = 1;
    static constexpr int ID_PERSON_DEFAULT = 1;

    std::unordered_map<std::string, int> cachePerformers;
    std::unordered_map<std::string, int> cacheAlbums;
    
    /**
    * @brief Método para saber si es un archivo mp3. 
    * @param ruta; la ruta hasta el archivo 
    * @return un booleano para saber su tipo de dato. 
    */
    static bool esArchivoMP3(const std::filesystem::path& ruta);

     /**
    * @brief obtenerMaxId; El Id mas reciente o alto en la tabla 
    * @param string  string, la tabla, string la columna 
    * @return int, devuelve el registro
    */
    int obtenerMaxId(const std::string& entidad, const std::string& columnaPk);

     /**
    * @brief obteneinicializarContadores; inicializa las variables con el id mas reciente
    * @param nada 
    * @return nada 
    */
    void inicializarContadores();

    /**
    * @brief asegurarRegistrosBase; crea registros "comodin" para poder acceder a las tablas. 
    * @param nada
    * @return nada
    */
    void asegurarRegistrosBase();

    /**
    * @brief vincularPerformerAAlbum; conecta al artista con el album 
    * @param string int, id del album, id del artista
    * @return nada
    */
    void vincularPerformerAAlbum(int idAlbum, int idPerformer);

     /**
    * @brief obtenerOCrearPerformer; permite acceder o crear un album artista
    * @param string nombreArtista
    * @return retorno int
    */
    int obtenerOCrearPerformer(const std::string& nombreArtist);

    /**
    * @brief obtenerOCrearAlbum; permite acceder o crear un album nuevo
    * @param string nombreAlbum, string rutaCarpeta, int año, int idPerformer
    * @return retorno int
    */
    int obtenerOCrearAlbum(const std::string& nombreAlbum,   const std::string& rutaCarpeta,  int year,  int idPerformer);

public:

    /**
    * @brief Constructor del Minero; es explicit para que no se pueda crear de forma implicita una instancia de minero a partir de CompBusqueda; esto para evitar que se cree otra conexión la base de datos. 
    * @param el buscador, la base de datos 
    * @return 
    */
    explicit Minero(
        CompiladorBusquedasFacade& buscador,
        const std::string& conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432");

    /**
    * @brief Destructor del minero. 
    * @param nada 
    * @return nada
    */
    ~Minero();

    /**
    * @brief minarDirectorio; Es quien se encarga de minar los datos de las rutas y agregarlos a la base de datos. 
    * @param string las rutas 
    * @return instancia del struct resumen minado. 
    */
    ResumenMinado minarDirectorio(const std::string& rutaRaiz);
};

#endif // MINERO_HPP
