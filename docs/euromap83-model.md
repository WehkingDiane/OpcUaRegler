# EUROMAP 83 orientiertes Modell

## Referenz

EUROMAP 83 entspricht OPC 40083 und definiert allgemeine OPC-UA-Typen fuer Kunststoff- und Gummimaschinen. Die General-Types-URI ist:

```text
http://opcfoundation.org/UA/PlasticsRubber/GeneralTypes/
```

Dieses Projekt verwendet einen eigenen Namespace:

```text
urn:opcuaregler:euromap83-working-point-controller
```

## Modellgrenzen

Das Projekt bildet keinen offiziellen EUROMAP-Server vollstaendig nach. Es erstellt ein eigenes Reglerobjekt, das in einer Anlage neben EUROMAP-konformen Maschinen- oder Peripherie-Servern laufen kann.

## Objektstruktur

```text
Regler
|-- Identification
|   |-- ApplicationUri
|   `-- SemanticBase
|-- Parameters
|   |-- Setpoint
|   |-- ToleranceWarning
|   |-- ToleranceError
|   |-- Kp
|   |-- Ki
|   |-- OutputMin
|   `-- OutputMax
|-- Process
|   |-- ActualValue
|   |-- Deviation
|   |-- OutputValue
|   |-- State
|   `-- QualityGood
|-- Alarms
|   |-- WarningActive
|   |-- ErrorActive
|   |-- ErrorCode
|   `-- ErrorText
`-- Commands
    |-- Enable
    |-- Acknowledge
    |-- ManualMode
    `-- ManualOutput
```

Der open62541-Server ergaenzt fuer lokale Tests zusaetzlich:

```text
Regler
`-- Simulation
    |-- Enabled
    |-- ActualValue
    |-- Disturbance
    |-- TimeConstantSeconds
    `-- Reset
```

## Schreibbare Werte

- `Parameters/Setpoint`
- `Parameters/ToleranceWarning`
- `Parameters/ToleranceError`
- `Parameters/Kp`
- `Parameters/Ki`
- `Parameters/OutputMin`
- `Parameters/OutputMax`
- `Process/ActualValue`
- `Process/QualityGood`
- `Commands/Enable`
- `Commands/Acknowledge`
- `Commands/ManualMode`
- `Commands/ManualOutput`
- `Simulation/Enabled`
- `Simulation/ActualValue`
- `Simulation/Disturbance`
- `Simulation/TimeConstantSeconds`
- `Simulation/Reset`

Schreibbare Prozesswerte sind bewusst fuer Tests, Simulation und einfache Adapter vorgesehen. In einer produktiven Anlage sollte die Schreibbarkeit ueber Benutzerrechte und Maschinenmapping eingeschraenkt werden.

## Node-Liste

| Pfad | Datentyp | Schreibbar | Default | Bedeutung |
| --- | --- | --- | --- | --- |
| `Identification/ApplicationUri` | String | Nein | `urn:opcuaregler:working-point-controller` | OPC-UA-Application-URI des Dienstes |
| `Identification/SemanticBase` | String | Nein | EUROMAP-83-General-Types-URI | Semantische Referenz auf OPC 40083 / EUROMAP 83 |
| `Parameters/Setpoint` | Double | Ja | `100.0` | Sollwert des Arbeitspunktes |
| `Parameters/ToleranceWarning` | Double | Ja | `2.0` | Warnschwelle fuer Soll/Ist-Abweichung |
| `Parameters/ToleranceError` | Double | Ja | `5.0` | Fehlerschwelle fuer Soll/Ist-Abweichung |
| `Parameters/Kp` | Double | Ja | `1.0` | Proportionalanteil des PI-Reglers |
| `Parameters/Ki` | Double | Ja | `0.0` | Integralanteil des PI-Reglers |
| `Parameters/OutputMin` | Double | Ja | `0.0` | Untere Stellwertbegrenzung |
| `Parameters/OutputMax` | Double | Ja | `100.0` | Obere Stellwertbegrenzung |
| `Process/ActualValue` | Double | Ja | `100.0` | Istwert; ohne Simulation direkt beschreibbar |
| `Process/Deviation` | Double | Nein | `0.0` | Sollwert minus Istwert |
| `Process/OutputValue` | Double | Nein | `0.0` | Aktueller Stellwert |
| `Process/State` | String | Nein | `INIT` | Reglerzustand |
| `Process/QualityGood` | Boolean | Ja | `true` | Qualitaet des Eingangswerts |
| `Alarms/WarningActive` | Boolean | Nein | `false` | Mindestens eine Warnung aktiv |
| `Alarms/ErrorActive` | Boolean | Nein | `false` | Mindestens ein Fehler aktiv |
| `Alarms/ErrorCode` | Int32 | Nein | `0` | Code des aktiven Fehlers |
| `Alarms/ErrorText` | String | Nein | leer | Text des aktiven Fehlers |
| `Commands/Enable` | Boolean | Ja | `true` | Regler aktivieren oder deaktivieren |
| `Commands/Acknowledge` | Boolean | Ja | `false` | Aktive Alarme quittieren; wird zurueckgesetzt |
| `Commands/ManualMode` | Boolean | Ja | `false` | Handbetrieb aktivieren |
| `Commands/ManualOutput` | Double | Ja | `0.0` | Stellwert im Handbetrieb |
| `Simulation/Enabled` | Boolean | Ja | `false` | Interne Prozesssimulation aktivieren |
| `Simulation/ActualValue` | Double | Ja | `100.0` | Simulierter Istwert |
| `Simulation/Disturbance` | Double | Ja | `0.0` | Additive Stoergroesse |
| `Simulation/TimeConstantSeconds` | Double | Ja | `5.0` | Zeitkonstante der simulierten Strecke |
| `Simulation/Reset` | Boolean | Ja | `false` | Simulierten Istwert auf Sollwert setzen; wird zurueckgesetzt |

## Simulation

Der Bereich `Simulation` stellt eine einfache interne Prozesssimulation fuer lokale WSL-/UAExpert-Tests bereit. Standardmaessig ist `Simulation/Enabled` aus, damit `Process/ActualValue` direkt von UAExpert oder einem externen Adapter geschrieben werden kann.

Wenn `Simulation/Enabled` aktiv ist, berechnet der Server `Simulation/ActualValue` als einfache Strecke erster Ordnung aus `Process/OutputValue`, `Simulation/Disturbance` und `Simulation/TimeConstantSeconds`. Der simulierte Wert wird nach `Process/ActualValue` uebernommen und vom Regler im naechsten Zyklus gelesen. Mit `Simulation/Reset` wird der simulierte Istwert auf den aktuellen Sollwert gesetzt.

## Mapping-Regeln fuer reale Anlagen

- Istwerte sollten aus dem passenden Maschinen- oder Peripherie-OPC-UA-Server gelesen werden.
- Schreibende Zugriffe auf Maschinenstellwerte muessen pro Maschine explizit gemappt und getestet werden.
- EUROMAP 83 liefert allgemeine Typdefinitionen. Geraetespezifische Standards wie EUROMAP 77 / OPC 40077 oder EUROMAP 82.x muessen je Anlage separat betrachtet werden.
- Eigene Regler-Nodes bleiben im Projekt-Namespace. Referenzen auf EUROMAP-Typen werden erst im echten OPC-UA-Backend umgesetzt.
