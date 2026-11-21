import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

ToolBar {
    id: barra
    property string tituloActual: "Ninguna pista seleccionada"
    property string artistaActual: ""
    property string rutaActual: ""
    property bool reproduciendo: reproductor.playbackState === MediaPlayer.PlayingState

    AudioOutput {
        id: salidaAudio
        volume: 1.0
    }

    MediaPlayer {
        id: reproductor
        audioOutput: salidaAudio
        source: rutaActual !== "" ? encodeURI("file://" + rutaActual) : ""
        onErrorOccurred: (error, errorString) => console.log("Error de audio:", errorString)
    }

    function cargarPista(titulo, artista, ruta) {
        tituloActual = titulo
        artistaActual = artista
        rutaActual = ruta
        reproductor.play()
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        ColumnLayout {
            Layout.fillWidth: true
            Label { text: tituloActual + (artistaActual ? " — " + artistaActual : ""); font.bold: true }
            Label { text: rutaActual; font.pixelSize: 11; opacity: 0.7; elide: Text.ElideMiddle; Layout.fillWidth: true }
        }
        Button {
            text: reproduciendo ? "Pausar" : "Reproducir"
            enabled: rutaActual !== ""
            onClicked: reproduciendo ? reproductor.pause() : reproductor.play()
        }
        Button {
            text: "Detener"
            enabled: rutaActual !== ""
            onClicked: {
                reproductor.stop()
                tituloActual = "Detenido"
                artistaActual = ""
                rutaActual = ""
            }
        }
    }
}
