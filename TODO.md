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
- [x] Lokale open62541-Serverkonfiguration fuer UAExpert-Test ergaenzt.

## In Arbeit

- [ ] open62541-Server-Implementierung auf Zielsystem bauen und mit OPC-UA-Client testen.
- [ ] Schreibrechte der OPC-UA-Variablen mit UAExpert pruefen und korrigieren, damit freigegebene Werte bearbeitet werden koennen.

## Spaeter

- [ ] OPC-UA-Benutzer-/Passwort-Authentifizierung implementieren und Konfigurationsoptionen dokumentieren.
- [ ] OPC-UA-Zertifikatsbetrieb implementieren und mit UAExpert testen.
- [ ] Bedienbare Prozesssimulation hinzufuegen, damit Istwert und Reglerverhalten ohne echte Maschine getestet werden koennen.
- [ ] Vollstaendige open62541-Server-Implementierung mit Zertifikaten testen.
- [ ] systemd-Unit und Installationspaket erstellen.
- [ ] Mapping gegen konkrete Maschinen-NodeIds validieren.
- [ ] Persistente Historie fuer Alarme und Prozesswerte ergaenzen.
