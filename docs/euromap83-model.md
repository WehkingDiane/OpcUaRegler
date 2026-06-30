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

## Mapping-Regeln fuer reale Anlagen

- Istwerte sollten aus dem passenden Maschinen- oder Peripherie-OPC-UA-Server gelesen werden.
- Schreibende Zugriffe auf Maschinenstellwerte muessen pro Maschine explizit gemappt und getestet werden.
- EUROMAP 83 liefert allgemeine Typdefinitionen. Geraetespezifische Standards wie EUROMAP 77 / OPC 40077 oder EUROMAP 82.x muessen je Anlage separat betrachtet werden.
- Eigene Regler-Nodes bleiben im Projekt-Namespace. Referenzen auf EUROMAP-Typen werden erst im echten OPC-UA-Backend umgesetzt.
