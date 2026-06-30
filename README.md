# OpcUaRegler

C++ Microservice fuer einen industriellen Arbeitspunkt-Regler mit OPC-UA-Schnittstelle und EUROMAP-83-orientiertem Informationsmodell.

## Status

Das Projekt enthaelt aktuell:

- C++20 Reglerkern
- Soll/Ist-Abweichung
- zwei Toleranzschwellen fuer Warnung und Fehler
- PI-Regler mit Stellwertbegrenzung, Rampe und Anti-Windup
- Alarmmodell
- EUROMAP-83-orientierte Node-Struktur im eigenen Namespace
- Simulationsbackend ohne externe Abhaengigkeiten
- optionales `open62541`-Backend fuer einen echten OPC-UA-Server
- CMake-Build und einfache Tests

Das `open62541`-Backend ist optional, weil die Bibliothek auf Zielsystemen unterschiedlich installiert wird.

## Schnellstart

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
./build/opcuaregler --config config/regler.example.toml
```

## OPC-UA-Servermodus

```sh
cmake -S . -B build -DOPCUAREGLER_WITH_OPEN62541=ON
cmake --build build
```

In der Konfiguration `simulation = false` setzen. Dann stellt der Dienst die Regler-Nodes ueber OPC UA bereit.

## Wichtige Dateien

- `AGENTS.md`: Arbeitsregeln fuer weitere Codex-Sitzungen
- `TODO.md`: Aufgabenstand
- `PROGRESS.md`: persistenter Fortschritts- und Wissensstand
- `docs/architecture.md`: Architektur
- `docs/euromap83-model.md`: EUROMAP-83-orientiertes Modell
- `config/regler.example.toml`: Beispielkonfiguration
