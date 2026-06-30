# AGENTS.md

## Projektziel

Dieses Repository entwickelt einen C++ Microservice fuer industrielle Arbeitspunkt-Regler. Der Dienst soll auf einem Linux Industrie-PC laufen, OPC UA als Ein-/Ausgangsschnittstelle nutzen und sich semantisch an EUROMAP 83 / OPC 40083 orientieren.

## Arbeitsregeln fuer Codex

- Fortschritt in `PROGRESS.md` dokumentieren, bevor groessere Arbeitsschritte abgeschlossen werden.
- Offene und erledigte Aufgaben in `TODO.md` pflegen.
- Architekturentscheidungen in `docs/architecture.md` oder spezifischen Dokumenten ablegen.
- Keine fremden oder unklaren EUROMAP-NodeSets kopieren. Nur eigene Namespaces modellieren und offizielle Standards referenzieren.
- C++ Code konservativ halten: klare Klassen, keine unnoetigen Frameworks, Standardbibliothek bevorzugen.
- Buildbarkeit regelmaessig mit CMake pruefen.
- OPC-UA-Backend austauschbar halten. Der Reglerkern darf nicht direkt von einer konkreten OPC-UA-Bibliothek abhaengen.
- Bestehende Nutzerarbeit schuetzen: keine unangeforderten Reverts, keine destruktiven Git-Kommandos, keine grossen Refactorings ohne technischen Grund.
- Aenderungen klein und nachvollziehbar halten. Neue Abstraktionen nur einfuehren, wenn sie konkrete Komplexitaet reduzieren oder die Backend-Austauschbarkeit schuetzen.

## Git- und Commit-Regeln

- Regelmaessig committen, sobald ein in sich geschlossener Arbeitsschritt fertig und dokumentiert ist.
- Vor jedem Commit `git status --short` pruefen und nur bewusst ausgewaehlte Dateien stagen.
- Commit-Nachrichten kurz, sachlich und im Imperativ formulieren, zum Beispiel `Dokumentiere Agent-Arbeitsregeln`.
- Keine generierten Build-Verzeichnisse, Binaerdateien oder lokalen IDE-Dateien committen.
- Wenn der Arbeitsbaum bereits fremde Aenderungen enthaelt, diese nicht ueberschreiben. Entweder sauber daneben arbeiten oder den Konflikt im Fortschrittslog festhalten.

## Arbeitsablauf fuer groessere Aenderungen

1. Relevante Dateien lesen und vorhandene Architektur verstehen.
2. Betroffene Aufgaben in `TODO.md` ergaenzen oder aktualisieren.
3. Implementierung eng am bestehenden Stil halten.
4. Dokumentation aktualisieren, wenn Verhalten, Konfiguration, Architektur oder Bedienung betroffen sind.
5. Build und passende Tests ausfuehren, sofern die Werkzeuge verfuegbar sind.
6. Ergebnis, Pruefungen und offene Risiken in `PROGRESS.md` dokumentieren.
7. Einen fokussierten Commit erstellen.

## Build und Verifikation

- Standardpruefung ohne OPC-UA-SDK:
  - `cmake -S . -B build`
  - `cmake --build build`
  - `ctest --test-dir build --output-on-failure`
- Optionaler Backend-Build mit open62541:
  - `cmake -S . -B build-open62541 -DOPCUAREGLER_WITH_OPEN62541=ON`
  - `cmake --build build-open62541`
- Wenn CMake, Compiler oder Bibliotheken fehlen, den genauen Grund in `PROGRESS.md` notieren und statische Pruefungen so weit wie moeglich durchfuehren.

## Dokumentationsregeln

- Nutzerrelevante Bedienung gehoert in `README.md` oder `docs/operations.md`.
- Architektur, Schichten und Schnittstellen gehoeren in `docs/architecture.md`.
- OPC-UA-Adressraum, Semantik und EUROMAP-Bezug gehoeren in `docs/euromap83-model.md`.
- Beispielwerte und erklaerte Konfigurationsoptionen muessen zu `config/regler.example.toml` passen.
- Entscheidungen mit spaeterem Risiko oder offenem Validierungsbedarf explizit als solche markieren.

## OPC-UA- und EUROMAP-Leitplanken

- Der Reglerkern kennt nur das abstrakte `OpcUaBackend` und keine konkrete OPC-UA-Bibliothek.
- Das eigene OPC-UA-Modell darf EUROMAP 83 / OPC 40083 semantisch referenzieren, aber keine fremden NodeSet-Dateien oder fremde Namespace-Strukturen kopieren.
- Maschinen- oder kundenspezifische NodeIds gehoeren in Konfiguration oder Adapterlogik, nicht fest in den Reglerkern.
- Safety-relevante Abschaltungen muessen explizit modelliert und dokumentiert werden. Der aktuelle Reglerstatus allein ersetzt keine Sicherheitsfunktion.

## Aktuelle technische Richtung

- Sprache: C++20
- Build: CMake
- OPC UA Backend: optional `open62541`; Kern laeuft auch ohne Backend im Simulationsmodus.
- Konfiguration: einfache TOML-aehnliche Datei `config/regler.example.toml`
- Zielplattform: Linux Industrie-PC, systemd-faehig
