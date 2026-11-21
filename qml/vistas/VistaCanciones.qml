import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../componentes"


Item {
    id: raiz
    property var canciones: []
    signal reproducirRola(string titulo, string artista, string ruta)

    function cargar() { canciones = biblioteca.obtenerCanciones() }
    Component.onCompleted: cargar()

    DialogoEditarCancion {
        id: dialogoEditar
        onGuardado: raiz.cargar()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12

        RowLayout {
            Label { text: "Canciones (" + canciones.length + ")"; font.bold: true; font.pixelSize: 16; Layout.fillWidth: true }
            Button { text: "Actualizar"; onClicked: raiz.cargar() }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: canciones
            spacing: 4

            delegate: Frame {
                width: ListView.view.width
                RowLayout {
                    anchors.fill: parent
                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: (modelData.track ? modelData.track + ". " : "") + modelData.title; font.bold: true }
                        Label { text: (modelData.artista || "Desconocido") + " — " + (modelData.album || "Sin álbum") + " (" + modelData.year + ")"; opacity: 0.75 }
                    }
                    Button {
                        text: "Play"
                        onClicked: raiz.reproducirRola(modelData.title, modelData.artista || "", modelData.path)
                    }
                    Button {
                        text: "Editar"
                        onClicked: dialogoEditar.abrir(modelData)
                    }
                    Button {
                        text: "Borrar"
                        onClicked: {
                            if (biblioteca.eliminarCancion(modelData.id_rola)) raiz.cargar()
                        }
                    }
                }
            }
        }
    }
}
