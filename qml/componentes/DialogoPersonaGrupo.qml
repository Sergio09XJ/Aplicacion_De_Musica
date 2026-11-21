import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialogo
    standardButtons: Dialog.Save | Dialog.Cancel
    anchors.centerIn: Overlay.overlay
    width: 380

    property bool esPersona: true
    property int idRegistro: 0
    signal guardado()

    function abrirPersona(p) {
        esPersona = true
        title = p ? "Editar Persona" : "Nueva Persona"
        idRegistro = p ? p.id_person : 0
        c1.text = p ? (p.stage_name || "") : ""
        c2.text = p ? (p.real_name || "") : ""
        c3.text = p ? (p.birth_date || "") : ""
        c4.text = p ? (p.death_date || "") : ""
        open()
    }

    function abrirGrupo(g) {
        esPersona = false
        title = g ? "Editar Grupo" : "Nuevo Grupo"
        idRegistro = g ? g.id_group : 0
        c1.text = g ? (g.name || "") : ""
        c2.text = g ? (g.start_date || "") : ""
        c3.text = g ? (g.end_date || "") : ""
        c4.text = ""
        open()
    }

    onAccepted: {
        var ok = esPersona
            ? biblioteca.guardarPersona(idRegistro, c1.text, c2.text, c3.text, c4.text)
            : biblioteca.guardarGrupo(idRegistro, c1.text, c2.text, c3.text)
        if (ok) guardado()
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        Label { text: esPersona ? "Nombre Artístico:" : "Nombre Grupo:" } ; TextField { id: c1; Layout.fillWidth: true }
        Label { text: esPersona ? "Nombre Real:" : "Fecha Inicio:" }      ; TextField { id: c2; Layout.fillWidth: true }
        Label { text: esPersona ? "Nacimiento:" : "Fecha Fin:" }          ; TextField { id: c3; Layout.fillWidth: true }
        Label { text: "Fallecimiento:"; visible: esPersona }              ; TextField { id: c4; visible: esPersona; Layout.fillWidth: true }
    }
}
