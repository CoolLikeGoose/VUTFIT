# Exportér SNMP Gauge metrik do OpenTelemetry (OTEL)

Projekt ISA: exportér SNMP v2c Gauge metrik do OpenTelemetry přes OTLP/HTTP (JSON).

## Zadání

`snmp2otel` periodicky dotazuje SNMP agenta (UDP, výchozí port 161) na zadané OID a naměřené hodnoty odesílá na OTEL endpoint (HTTP, typicky Collector na `/v1/metrics`).

- podporován pouze SNMP v2c `Get`
- OID ve skalárním tvaru zakončeném `.0`
- exportované metriky pouze typu **Gauge**

## Spuštění

```bash
snmp2otel -t target [-C community] -o oids_file -e endpoint [-i interval] [-r retries] [-T timeout] [-p port] [-v]
```

### Parametry

- `-t target` – IP/DNS SNMP agenta
- `-C community` – community string (default `public`)
- `-o oids_file` – soubor s OID
- `-e endpoint` – OTLP/HTTP JSON endpoint (např. `http://localhost:4318/v1/metrics`)
- `-i interval` – perioda dotazování v sekundách (default `10`)
- `-r retries` – počet retransmisí (default `2`)
- `-T timeout` – timeout v ms (default `1000`)
- `-p port` – UDP port SNMP agenta (default `161`)
- `-v` – verbose výpis

## Formát OID souboru

ASCII text, jedno numerické OID na řádek. Prázdné řádky a řádky začínající `#` se ignorují.

```txt
# System uptime
1.3.6.1.2.1.1.3.0
```

## Volitelný mapping soubor

```json
{
	"1.3.6.1.2.1.1.3.0": { "name": "snmp.sysUpTime", "unit": "ms", "type": "gauge" }
}
```

`type` může být pouze `gauge`. Pokud mapování chybí, název metriky se odvodí z OID.

## Chování programu

- v běžném režimu bez výpisu
- v `-v` režimu ladicí informace (dotazy, odpovědi, export)
- chyby se vypisují na `stderr`
- při timeoutu/retry nebo chybě exportu program pokračuje dál

## Build a test

Projekt obsahuje Makefile s cíli:

- `make all`
- `make run`
- `make test`
- `make clean`
