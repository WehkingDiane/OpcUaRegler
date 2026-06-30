# TODO

## Erledigt

- [x] Projektarbeitsregeln in `AGENTS.md` initialisiert.
- [x] `AGENTS.md` um Commit-Regeln, Arbeitsablauf, Verifikation und OPC-UA/EUROMAP-Leitplanken erweitert.
- [x] Fortschrittslog in `PROGRESS.md` angelegt.
- [x] Grundstruktur fuer C++ Microservice geplant.
- [x] CMake-Projektstruktur erstellt.
- [x] Reglerkern fuer Soll/Ist-Arbeitspunkt mit zwei Toleranzschwellen implementiert.
- [x] EUROMAP-83-orientiertes internes Informationsmodell dokumentiert.
- [x] OPC-UA-Abstraktion und Simulationsbackend vorbereitet.
- [x] Beispielkonfiguration und Betriebsdokumentation erstellt.
- [x] Tests fuer Reglerlogik und Toleranzbewertung erstellt.
- [x] Build auf einem System mit CMake und C++20 Compiler ausgefuehrt.
- [x] open62541 v1.4.17 lokal installiert und Backend-Build mit `OPCUAREGLER_WITH_OPEN62541=ON` erfolgreich ausgefuehrt.

## In Arbeit

- [ ] open62541-Server-Implementierung auf Zielsystem bauen und mit OPC-UA-Client testen.

## Spaeter

- [ ] Vollstaendige open62541-Server-Implementierung mit Zertifikaten testen.
- [ ] systemd-Unit und Installationspaket erstellen.
- [ ] Mapping gegen konkrete Maschinen-NodeIds validieren.
- [ ] Persistente Historie fuer Alarme und Prozesswerte ergaenzen.
