import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialogo
    title: "Editar Canción"
    standardButtons: Dialog.Save | Dialog.Cancel
    anchors.centerIn: Overlay.overlay
    width: 420

    property int idRola: 0
    property var listaPerformers: []
    signal guardado()

    function abrir(cancion) {
        idRola = cancion.id_rola
        campoTitle.text = cancion.title || ""
        campoTrack.text = cancion.track || ""
        campoDisc.text = cancion.disc_number || 1
        campoYear.text = cancion.year || 0
        campoGenre.text = cancion.genre || ""
        campoComment.text = cancion.comment || ""

        listaPerformers = biblioteca.obtenerPerformers()
        var idx = 0
        for (var i = 0; i < listaPerformers.length; i++) {
            if (listaPerformers[i].id_performer === cancion.id_performer) { idx = i; break }
        }
        comboPerf.currentIndex = idx
        open()
    }

    onAccepted: {
        var idPerf = listaPerformers.length > 0 ? listaPerformers[comboPerf.currentIndex].id_performer : 1
        if (biblioteca.editarCancion(idRola, campoTitle.text, campoTrack.text,
                                     parseInt(campoDisc.text) || 1, parseInt(campoYear.text) || 0,
                                     campoGenre.text, campoComment.text, idPerf)) {
            guardado()
        }
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        Label { text: "Título:" }       TextField { id: campoTitle; Layout.fillWidth: true }
        Label { text: "Intérprete:" }   ComboBox  { id: comboPerf; model: listaPerformers.map(p => p.name); Layout.fillWidth: true }
        Label { text: "Pista:" }        TextField { id: campoTrack; Layout.fillWidth: true }
        Label { text: "Disco:" }        TextField { id: campoDisc; Layout.fillWidth: true }
        Label { text: "Año:" }          TextField { id: campoYear; Layout.fillWidth: true }
        Label { text: "Género:" }       TextField { id: campoGenre; Layout.fillWidth: true }
        Label { text: "Comentario:" }   TextField { id: campoComment; Layout.fillWidth: true }
    }
}
