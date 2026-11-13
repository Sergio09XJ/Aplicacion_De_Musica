
CREATE TABLE types (
  id_type       INTEGER PRIMARY KEY,
  description   TEXT
);

INSERT INTO types VALUES(0, 'Person');
INSERT INTO types VALUES(1, 'Group');
INSERT INTO types VALUES(2, 'Unknown');

CREATE TABLE performers (
  id_performer  INTEGER PRIMARY KEY,
  id_type       INTEGER,
  name          TEXT,
  image         BLOB, -- CAMBIO: Se agregó esta columna para la fotografía del artista (APIC)
  FOREIGN KEY   (id_type) REFERENCES types(id_type)
);

CREATE TABLE persons (
  id_person     INTEGER PRIMARY KEY,
  stage_name    TEXT,
  real_name     TEXT,
  birth_date    TEXT,
  death_date    TEXT,
  FOREIGN KEY   (id_person) REFERENCES performers(id_performer) -- CAMBIO: #Se agrego esta llave foránea para enlazarla con la tabla padre performers
);

CREATE TABLE groups (
  id_group      INTEGER PRIMARY KEY,
  name          TEXT,
  start_date    TEXT,
  end_date      TEXT,
  FOREIGN KEY   (id_group) REFERENCES performers(id_performer) -- CAMBIO: #Se agrego esta llave foránea para enlazarla con la tabla padre performers
);

CREATE TABLE in_group (
  id_person     INTEGER,
  id_group      INTEGER,
  PRIMARY KEY   (id_person, id_group),
  FOREIGN KEY   (id_person) REFERENCES persons(id_person),
  FOREIGN KEY   (id_group) REFERENCES groups(id_group)
);

CREATE TABLE albums (
  id_album      INTEGER PRIMARY KEY,
  id_performer  INTEGER, -- CAMBIO: Se agregó esta columna para enlazar con la banda/artista del álbum (TPE2)
  path          TEXT,
  name          TEXT,
  year          INTEGER,
  cover_image   BLOB,    -- CAMBIO: Se agregó para guardar la carátula del álbum (APIC)
  FOREIGN KEY   (id_performer) REFERENCES performers(id_performer) -- CAMBIO: Es la conexión con el artista del álbum
);

-- CAMBIO: Se actualizó el INSERT para que coincida con el nuevo número de columnas. 
INSERT INTO albums VALUES (0, NULL, '', 'Unknown', 0, NULL);

CREATE TABLE rolas (
  id_rola             INTEGER PRIMARY KEY,
  id_performer        INTEGER,
  id_album            INTEGER,
  id_composer         INTEGER, -- CAMBIO: Se agregó el compositor (TCOM)
  id_original_artist  INTEGER, -- CAMBIO: Se agregó el artista original para los covers (TOPE)
  path                TEXT,
  title               TEXT,
  track               INTEGER,   
  disc_number         INTEGER, -- CAMBIO: Se agregó el número de discos (TPOS)
  year                INTEGER,
  genre               TEXT,
  comment             TEXT,    -- CAMBIO: Se agregó el comentario (COMM)
  FOREIGN KEY   (id_performer) REFERENCES performers(id_performer),
  FOREIGN KEY   (id_album) REFERENCES albums(id_album),
  FOREIGN KEY   (id_composer) REFERENCES performers(id_performer),       -- CAMBIO: Referencia al compositor
  FOREIGN KEY   (id_original_artist) REFERENCES performers(id_performer) -- CAMBIO: Referencia al artista original
);


