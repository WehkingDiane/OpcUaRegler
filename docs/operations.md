# Betrieb

## Build

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Build mit open62541 OPC-UA-Server

Voraussetzung: `open62541` ist mit CMake-Package-Dateien installiert.

```sh
cmake -S . -B build -DOPCUAREGLER_WITH_OPEN62541=ON
cmake --build build
```

Danach in der Konfiguration setzen:

```toml
[service]
simulation = false
```

Der Dienst startet dann einen OPC-UA-Server auf dem konfigurierten Port. Im Simulationsmodus wird kein echter OPC-UA-Server geoeffnet.

Fuer lokale Tests mit UAExpert kann die vorbereitete Serverkonfiguration genutzt werden:

```sh
./build/opcuaregler --config config/regler.open62541.toml
```

Unter WSL ist der Server fuer UAExpert in Windows typischerweise ueber `opc.tcp://localhost:4840` oder ueber die WSL-IP erreichbar, zum Beispiel `opc.tcp://172.x.x.x:4840`.

### UAExpert-Test in WSL/Windows

Fuer manuelle Tests in UAExpert:

1. Mit `opc.tcp://localhost:4840` oder der WSL-IP verbinden.
2. Unter `Objects/Regler` die Bereiche `Parameters`, `Process`, `Commands`, `Alarms` und `Simulation` pruefen.
3. Ohne Simulation koennen `Process/ActualValue`, `Process/QualityGood`, `Parameters/Setpoint` und die Werte unter `Commands` geschrieben werden.
4. Fuer die interne Simulation `Simulation/Enabled` auf `true` setzen. Danach `Simulation/ActualValue`, `Simulation/Disturbance`, `Simulation/TimeConstantSeconds` und `Simulation/Reset` bedienen.
5. `Simulation/Reset` setzt den simulierten Istwert auf den aktuellen Sollwert und wird vom Server wieder auf `false` zurueckgesetzt.

## Start im Simulationsmodus

```sh
./build/opcuaregler --config config/regler.example.toml
```

Unter Windows mit CMake/Visual Studio liegt die EXE je nach Generator unter `build/Debug/opcuaregler.exe` oder `build/Release/opcuaregler.exe`.

## Linux systemd Beispiel

```ini
[Unit]
Description=OPC UA Working Point Controller
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=opcuaregler
Group=opcuaregler
WorkingDirectory=/opt/opcuaregler
ExecStart=/opt/opcuaregler/bin/opcuaregler --config /etc/opcuaregler/regler.toml
Restart=on-failure
RestartSec=3
WatchdogSec=30

[Install]
WantedBy=multi-user.target
```

## Produktionshinweise

- OPC-UA-Security mit Zertifikaten aktivieren, sobald das open62541-Backend produktiv ist.
- Maschinen-NodeIds niemals hart im Code hinterlegen, sondern in Konfigurationsdateien.
- Sollwert- und Parameterzugriffe mit Benutzerrechten begrenzen.
- Zykluszeit passend zur Maschine waehlen und auf dem Industrie-PC messen.
