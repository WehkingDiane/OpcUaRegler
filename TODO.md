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
- [x] OPC-UA-Schreibrechte im open62541-Backend technisch korrigiert: `AccessLevel` und `UserAccessLevel` werden fuer schreibbare Nodes gesetzt.
- [x] Bedienbare Basis-Prozesssimulation im OPC-UA-Adressraum unter `Regler/Simulation` ergaenzt.
- [x] OPC-UA-Testnodes fuer Simulation definiert: simulierter Istwert, Stoergroesse, Streckenzeitkonstante und Reset.
- [x] Prozesssimulation in automatisierten Tests abgedeckt: Normalbewegung, Stoergroesse und negative Zeitdifferenz.
- [x] Bedienablauf fuer UAExpert dokumentiert: Verbinden, Nodes finden, Werte schreiben, Alarme quittieren.
- [x] Node-Liste mit Datentypen, Schreibrechten, Defaultwerten und Bedeutung dokumentiert.

## In Arbeit

- [ ] open62541-Server-Implementierung in WSL/Linux weiterentwickeln und mit UAExpert unter Windows testen.
- [ ] Schreibrechte der OPC-UA-Variablen in WSL/Linux mit UAExpert manuell bestaetigen. [MANUELL: UAExpert unter Windows]

## Naechste Schritte: Bedienbarkeit und Simulation

- [ ] Schreibbare Nodes in UAExpert gegen den WSL-Server manuell pruefen: `ActualValue`, `QualityGood`, `Setpoint`, `Enable`, `ManualMode`, `ManualOutput`, `Acknowledge`. [MANUELL: UAExpert unter Windows]
- [ ] Prozesssimulation fachlich erweitern und testen: Sollwertsprung, Warnschwelle, Fehlerschwelle, Sensorfehler.
- [ ] Bedienablauf und Node-Liste mit UAExpert manuell gegenpruefen. [MANUELL: UAExpert-Screens/Bedienung pruefen]

## Reglerfunktion fuer Produktion

- [ ] Betriebsartenmodell ergaenzen: `Off`, `Manual`, `Auto`, `Hold`, `Fault`.
- [ ] Bumpless Transfer zwischen `Manual` und `Auto` implementieren.
- [ ] Plausibilitaetspruefung fuer Istwerte implementieren, inklusive Min/Max, Sprungbegrenzung und veralteter Werte.
- [ ] Watchdog fuer Kommunikationsverlust und veraltete Eingangsdaten ergaenzen.
- [ ] Definierten Stellwert-Failsafe bei Fehlern implementieren.
- [ ] Hysterese oder Totband fuer Warn- und Fehlergrenzen bewerten und implementieren.
- [ ] Sollwertrampe getrennt von Stellwertrampe ergaenzen.
- [ ] Integrator-Reset und Anti-Windup-Verhalten fuer Betriebsartenwechsel testen.
- [ ] Fehlerquittierung nur erlauben, wenn der ausloesende Zustand behoben ist.

## OPC-UA Security und Rechte

- [ ] OPC-UA-Benutzer-/Passwort-Authentifizierung implementieren und Konfigurationsoptionen dokumentieren.
- [ ] Rollen/Rechte modellieren: Lesen, Bedienen, Parametrieren, Service.
- [ ] Anonymous-Zugriff konfigurierbar abschaltbar machen.
- [ ] OPC-UA-Zertifikatsbetrieb implementieren: Serverzertifikat, Private Key, Trustlist und Rejected-Ordner.
- [ ] Security Policies aktivieren und dokumentieren, mindestens `Basic256Sha256`, wenn von open62541-Build unterstuetzt.
- [ ] Zertifikats- und Benutzerlogin mit UAExpert manuell testen. [MANUELL: UAExpert Zertifikate/Trustlist]
- [ ] Audit-Log fuer Sollwertaenderungen, Parameterzugriffe, Login-Versuche und Quittierungen ergaenzen.

## OPC-UA Modell und Maschinenintegration

- [ ] Maschinen- oder kundenspezifische NodeIds aus Konfiguration laden statt im Code festzulegen.
- [ ] Statuscodes und Quality-Werte fuer alle relevanten Nodes sauber setzen.
- [ ] EUROMAP-83-/OPC-40083-Bezug weiter nur semantisch referenzieren; keine fremden NodeSets kopieren.

## Betrieb auf Linux Industrie-PC

- [ ] systemd-Unit und Installationspaket erstellen.
- [ ] Healthcheck oder Watchdog-Konzept fuer systemd ergaenzen.
- [ ] Logging nach journald oder Datei inklusive Logrotation definieren.
- [ ] Version, Build-Info und Git-Commit im OPC-UA-Adressraum anzeigen.
- [ ] Ressourcenlimits fuer CPU, Speicher und Dateizugriffe definieren.
- [ ] Alle betrieblichen Funktionen zunaechst in WSL/Linux vorbereiten; echter IPC-Test erfolgt erst am Projektende.

## Persistenz und Diagnose

- [ ] Persistente Historie fuer Alarme und Prozesswerte ergaenzen.
- [ ] Diagnose-Nodes fuer Zykluszeit, Kommunikationsstatus, letzte Fehler und Backendstatus ergaenzen.
- [ ] Export oder Logformat fuer Alarmhistorie festlegen.

## Safety und Abgrenzung

- [ ] Dokumentieren, dass der Regler keine Sicherheitssteuerung ersetzt.
- [ ] Safety-relevante Signale und Abschaltungen als externe SPS-/Safety-Funktion abgrenzen.
- [ ] Verhalten bei Not-Aus, Maschinenfreigabeverlust und Kommunikationsverlust dokumentieren. [MANUELL: Anlagenkonzept pruefen]

## Abschluss-Gate: Industrie-PC und reale Anlage

- [ ] open62541-Server-Implementierung auf dem Industrie-PC bauen und starten. [MANUELL: IPC am Projektende]
- [ ] Zykluszeit und Jitter auf Zielhardware messen. [MANUELL: IPC am Projektende]
- [ ] Neustartverhalten bei Fehlern und Stromausfall auf Zielhardware testen. [MANUELL: IPC am Projektende]
- [ ] Mapping gegen konkrete Maschinen-NodeIds validieren. [MANUELL: Zielmaschine/SPS am Projektende]
- [ ] Mapping mit einer realen Maschine oder SPS testen. [MANUELL: Zielmaschine/SPS am Projektende]
- [ ] Vollstaendige open62541-Server-Implementierung mit Zertifikaten auf Zielumgebung testen. [MANUELL: IPC/UAExpert am Projektende]

## Dokumentation und Release

- [ ] Konfigurationsreferenz fuer alle Optionen erstellen.
- [ ] Installationsanleitung fuer WSL-Test, Linux-IPC und open62541-Abhaengigkeit erstellen.
- [ ] Security-Hardening-Anleitung erstellen.
- [ ] Release-Checkliste erstellen: Build, Tests, UAExpert-Test, Zielhardware-Test, Security-Konfiguration.
