import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../componentes"


Item {
    id: raiz
    property var performers: []
    property var personas: []
    property var grupos: []

    function cargarTodo() {
        performers = biblioteca.obtenerPerformers()
        personas = biblioteca.obtenerPersonas()
        grupos = biblioteca.obtenerGrupos()
    }
    Component.onCompleted: cargarTodo()

    DialogoPersonaGrupo {
        id: dialogoPG
        onGuardado: raiz.cargarTodo()
    }

    DialogoIntegrantes {
        id: dialogoIntegrantes
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12

        TabBar {
            id: barraTabs
            Layout.fillWidth: true
            TabButton { text: "Intérpretes" }
            TabButton { text: "Personas" }
            TabButton { text: "Grupos" }
        }

        StackLayout {
            currentIndex: barraTabs.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Tab 0: Intérpretes
            ColumnLayout {
                RowLayout {
                    TextField { id: campoNuevoPerf; placeholderText: "Nombre del intérprete"; Layout.fillWidth: true }
                    Button {
                        text: "Agregar Intérprete"
                        onClicked: {
                            if (campoNuevoPerf.text.trim() !== "" && biblioteca.registrarPerformer(campoNuevoPerf.text, 1)) {
                                campoNuevoPerf.text = ""
                                raiz.cargarTodo()
                            }
                        }
                    }
                }
                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true
                    model: performers
                    delegate: Frame {
                        width: ListView.view.width
                        Label { text: "[" + modelData.id_performer + "] " + modelData.name + " (" + (modelData.tipo || "") + ")" }
                    }
                }
            }

            // Tab 1: Personas
            ColumnLayout {
                Button { text: "+ Nueva Persona"; onClicked: dialogoPG.abrirPersona(null) }
                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true
                    model: personas
                    delegate: Frame {
                        width: ListView.view.width
                        RowLayout {
                            anchors.fill: parent
                            Label {
                                text: modelData.stage_name + " (" + modelData.real_name + ") — Nac: " + modelData.birth_date
                                Layout.fillWidth: true
                            }
                            Button { text: "Editar"; onClicked: dialogoPG.abrirPersona(modelData) }
                        }
                    }
                }
            }

            // Tab 2: Grupos
            ColumnLayout {
                Button { text: "+ Nuevo Grupo"; onClicked: dialogoPG.abrirGrupo(null) }
                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true
                    model: grupos
                    delegate: Frame {
                        width: ListView.view.width
                        RowLayout {
                            anchors.fill: parent
                            Label {
                                text: modelData.name + " (Inicio: " + modelData.start_date + ")"
                                Layout.fillWidth: true
                            }
                            Button { text: "Integrantes"; onClicked: dialogoIntegrantes.abrir(modelData.id_group, modelData.name) }
                            Button { text: "Editar"; onClicked: dialogoPG.abrirGrupo(modelData) }
                        }
                    }
                }
            }
        }
    }
}
