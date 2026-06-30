# Architektur

## Ziel

`opcuaregler` ist ein eigenstaendiger Microservice fuer einen industriellen Arbeitspunkt-Regler. Der Dienst laeuft auf einem Linux Industrie-PC, liest Prozesswerte ueber OPC UA, berechnet einen Stellwert und publiziert Zustand, Stellwert und Alarme wieder ueber OPC UA.

## Schichten

```text
Linux systemd service
        |
        v
Microservice main loop
        |
        +--> OpcUaBackend interface
        |       +--> MemoryOpcUaBackend fuer Simulation und Tests
        |       +--> Open62541 backend geplant
        |
        +--> WorkingPointController
        |       +--> PI-Regler
        |       +--> Toleranzbewertung
        |       +--> Stellwertbegrenzung
        |       +--> Anti-Windup
        |
        +--> AlarmManager
        |
        +--> Euromap83WorkingPointModel
```

## Regelzyklus

1. OPC-UA-Backend liest `ActualValue`, `Enable` und optional `Acknowledge`.
2. Regler aktualisiert Prozessabbild.
3. Regler berechnet `Deviation = Setpoint - ActualValue`.
4. Toleranzbewertung setzt `RUNNING`, `WARNING` oder `ERROR`.
5. PI-Regler berechnet `OutputValue` mit Begrenzung und Ausgaberampe.
6. OPC-UA-Backend publiziert Prozesswerte und Alarme.
7. Dienst wartet bis zum naechsten Zyklus.

## Fehlerstrategie

- Schlechte oder veraltete Istwertqualitaet fuehrt zu `ERROR`.
- Ueberschreitung der Warnschwelle fuehrt zu `WARNING`, Regelung laeuft weiter.
- Ueberschreitung der Fehlerschwelle fuehrt zu `ERROR`. Der aktuelle Code publiziert den Zustand; anlagenspezifische Sperrlogik soll im Backend oder in einer Safety-Schicht ergaenzt werden.
- Stellwertbegrenzung erzeugt eine Warnung und verhindert Integrator-Windup.

## EUROMAP-83-Einordnung

Der Microservice kopiert kein EUROMAP NodeSet. Er stellt einen eigenen Namespace bereit und dokumentiert die semantische Beziehung zu EUROMAP 83 / OPC 40083 General Types. Das vermeidet Lizenz- und Versionsprobleme und erlaubt spaeter ein echtes NodeSet-Mapping pro Maschine.

