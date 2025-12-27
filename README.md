<!-- # Smart-Street-Poles-IoT-ATmega2560 -->

# Smart Street Light Poles – IoT Edge-Based System
IoT-based smart street pole system for pedestrian walkways, enabling energy-efficient lighting, environmental monitoring, and public safety. Uses ATmega2560 edge nodes with ZigBee communication to a Raspberry Pi 3 gateway and MQTT-based CCC integration. Fully simulated using Proteus with a future roadmap toward real-world.

## Overview
This project presents an **IoT-based smart lighting system** designed for **pedestrian walkways**, aiming to:
- Reduce energy consumption
- Improve public safety
- Monitor environmental conditions
- Provide scalable data collection for smart city infrastructure

The system is built around a **distributed edge-computing architecture**, where each light pole operates autonomously while reporting data to a centralized control system.

---

## System Architecture

The system consists of three main layers:

### 1. Edge Layer (Smart Light Poles)
Each light pole contains an **Edge Controller** responsible for:
- Local decision-making
- Light control
- Safety handling
- Environmental monitoring

**Controller Used:**  
`ATmega2560`

**Why ATmega2560?**
- Powerful enough to handle multiple sensors and real-time decisions
- No unnecessary features (Wi-Fi / Bluetooth)
- Lower cost and power consumption compared to ESP32
- Suitable for deterministic, local control tasks

Each pole operates **independently**, without relying on the gateway for real-time decisions.

---

### 2. Gateway Layer
- One **Gateway** serves approximately **60 smart poles**
- Implemented using **Raspberry Pi 3**
- Communicates with edge nodes via **XBee (ZigBee protocol)**
- Forwards collected data to the Central Control Center (CCC)

The gateway acts as a **bridge**, not a decision-maker.

---

### 3. Central Control Center (CCC)
- Receives data from all gateways
- Responsible for:
  - Data storage
  - Analytics
  - Alerts
  - APIs for city authorities or service providers

---

## System Diagrams

### Block Diagram
<img src="Smart Poles Docs/Smart Poles Block Diagram/Smart Poles Block Diagram.svg" width="650" />

### Schematic Diagram
<img src="Smart Poles Docs/Smart Poles Schematic/Smart Poles Schematic.svg" width="650" />

> PDF versions are available in the `Smart Poles Docs/Smart Poles Schematic/Smart Poles Schematic.pdf` directory.

---

## Light Control Logic

### Day Mode
- Determined using an **LDR sensor**
- All lighting systems are turned OFF
- PIR sensor power is cut via MOSFET to save energy

### Night Mode
- LEDs operate at **20% brightness (PWM)** by default  
  This ensures:
  - Basic visibility
  - Psychological safety (users can see the entire path)

### Motion Detection
- If motion is detected using **PIR**:
  - Current pole LED → **100% brightness**
  - Adjacent pole LED → **100% brightness**
  - Other poles remain at 20%

### Motion + Noise Detection
- If motion and sound are detected simultaneously:
  - LED remains at 100%
  - A **12V auxiliary lamp** is activated
  - Lamp stays ON for 5 seconds
  - Timer resets if sound continues

This avoids frequent on/off behavior and improves user comfort.

---

## Environmental Monitoring

Each edge node monitors and reports:
- Temperature
- Humidity
- Noise level
- Air quality (particles)

Collected data is sent to the gateway via **UART → XBee → ZigBee**.

---

## Safety Features

### Fire Detection
- Flame sensor triggers:
  - Local fire alarm (buzzer)
  - Immediate alert to CCC

### Gas Leakage
- MQ-2 gas sensor detects leakage
- Data sent to CCC
- CCC can notify emergency or gas service providers

---

## Sensors Used

| Sensor | Purpose |
|------|--------|
| LDR | Day/Night detection |
| PIR | Motion detection |
| Noise Sensor | Detect sound activity |
| Flame Sensor | Fire detection |
| MQ-2 | Gas leakage detection |
| LM35 | Temperature monitoring |
| HIH Sensor | Humidity monitoring |
| PM Sensor | Air particle (dust) measurement |

---

## Actuators Used

| Actuator | Function |
|-------|---------|
| LED | Main lighting (PWM controlled) |
| MOSFET #1 | PIR power control |
| MOSFET #2 | 12V lamp control |
| MOSFET #3 | Fire alarm (buzzer) |

---
## External Components

The following components are used in this project.  
All components should be obtained from their official sources.

- **[LDR Module](https://electronicstree.com/ldr-module-proteus8-library/)**

- **[PIR Sensor](https://electronicstree.com/new-pir-sensor-proteus-library/)**

- **[Sound Sensor FC-04](https://electronicstree.com/sound-sensor-with-arduino-proteus-library/)**

- **[PM2.5 Air Quality Sensor](https://electronicstree.com/new-air-quality-sensor-proteus-library/)**

- **[Flame Sensor Module](https://electronicstree.com/new-flame-sensor-module-proteus8-library/)**

- **[MQ2 Gas Sensor](https://electronicstree.com/mq2-gas-sensor-in-proteus/)**

- **[XBee Module](https://www.theengineeringprojects.com/2016/01/xbee-library-proteus.html)**

---
## Communication Protocols

### Edge ↔ Gateway
- **ZigBee (XBee modules)**
- Chosen for:
  - Low power consumption
  - High scalability
  - Secure mesh networking
  - Suitable data rate for sensor data

### Gateway ↔ CCC
- **MQTT** (planned)
- Standardized and device-agnostic
- Suitable for large-scale IoT deployments

---

## Security Design

- **Node Isolation**  
  Each pole communicates only with the gateway (no peer-to-peer communication)

- **Authentication**
  - Each node sends its MAC address
  - Gateway validates against a MAC whitelist
  - Unauthorized nodes are blocked

This provides basic but effective security for constrained IoT devices.

---

## Simulation Environment

- Entire edge system is simulated using **Proteus**
- All sensors and actuators are fully functional
- XBee modules are logically represented (UART transmission implemented)

### Known Simulation Limitations
- `millis()` timing is inaccurate in Proteus
- Clock instability affects UART baud rates
- Serial communication requires:
  - Arduino code baud rate: `9600`
  - Proteus virtual terminal baud rate: `1200`
- DHT22 sensor could not operate reliably due to clock issues

⚠️ These issues are simulation-related and do not represent real hardware behavior.

---

## Project Structure

<pre>
Smart Street Poles IoT ATmega2560/
├── Smart Poles Code/
│   └── Smart_Poles_Code/
|       └── Smart_Poles_Code.ino
├── Smart Poles Docs/              
│   ├── Smart Poles Block Diagram/
|   |   ├── Smart Poles Block Diagram.png
|   |   └── Smart Poles Block Diagram.svg
|   └── Smart Poles Schematic/
|       ├── Smart Poles Schematic.pdf
|       ├── Smart Poles Schematic.png
|       └── Smart Poles Schematic.svg
├── Smart Poles Simulation/
|   └── Smart_Poles.pdsprj 
├── .gitignore
├── LICENSE
└── README.md
</pre>
External component libraries are intentionally excluded.
Users should download sensors and libraries from official sources.

## Future Work

- Build a real hardware prototype using Arduino and XBee
- Multi-stage prototyping on breadboard
- Design custom PCB
- Optimize sensor placement per pole
- Open-source physical pole design
- Develop CCC backend and dashboards

## Motivation

This project demonstrates how edge computing can:
- Reduce latency
- Save energy
- Improve safety
- Scale efficiently in smart city environments

The design prioritizes **practicality, reliability, and cost-efficiency**.

## Author
**Ahmed Waheed**

ICT – Software Development Track

test