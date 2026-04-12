# AMV VIRTUAL

# APRESENTAÇÃO
Um projeto de hardware de código aberto desenvolvido para promover uma operação ferroviária de forma segura e eficiente. O mesmo projeto faz parte do projeto final do Programa de capacitação em Sistemas Embarcados (EMBARCATECH) - segundo ciclo 2026.


# O AMV VIRTUAL

O AMV Virtual é uma ferramenta de hardware e software que simula o controle e monitoramento da mudança de vias ferreas através de um AMV (Aparelho de Mudança de Via). Ele integra o controle remoto físico que controla o AMV e uma representação simples do controle e monitoramento virtual, simulando o controle do movimento dentro do CCO (Centro de Controle Operacional).

# OBJETIVO DO AMV VIRTUAL

O objetivo do projeto é propor uma ideia de um equipamento para ser usado para mover uma via ferroviária de um lado para o outro de forma remota sem precisar, necessariamente, se deslocar a um pátio de manobras e movimentar de forma manual AMVs espalhados em diversos pontos, as vezes sob condições adversas (SALVO SE HOUVER PROBLEMAS DE QUEDA DE ENERGIA QUE, NESTE CASO EM ESPECÍFICO EXIGEM A MOVIMENTAÇÃO MANUAL). Este equipamento também pode ser utilizada de forma virtual e remotamente, diretamente no CCO. A proposta é criar uma forma de movimentação ferroviária sem precisar acionar equipes de manutenção de via permanente para acionar manualmente, além de criar camadas de segurança em vias ferroviarias sem expor o operador aos riscos de acidentes e revolucionar tecnologia ferroviária.

# INSTRUÇÕES DE INSTALAÇÃO E EXECUÇÃO

Todo o código-fonte do AMV VIRTUAL está 

 - Ativar BOOTSEL 

---
[![License: CERN-OHL-S v2.0](https://img.shields.io/badge/License-CERN--OHL--S%20v2.0-blue.svg)](https://cern.ch/cern-ohl 

## 🌟 What’s new in Version 7.0

✨ BitDogLab V7 improves upon the solid foundation of V6 with the following updates:

✅ Cost optimization: design adjustments to keep fabrication cost under US$ 25 (≈ R$ 250 in Brazil, including taxes).

✅ Support for Raspberry Pi Pico 2 (RP2350) with dual-core ARM and RISC-V architecture.

✅ Enhanced LED matrix: 5×5 WS2812B RGB array, now using smaller LEDs for better integration.

✅ Display connection updated: OLED now connected to I²C pins GPIO2 (SDA) and GPIO3 (SCL).

✅ New Battery Management System (BMS): integrated INA226 for easy monitoring and control.

✅ Flexible display options: support for OLED modules with 128×64 or 128×128 pixel resolution, expanding visualization capabilities.

✅ Additional input control: inclusion of a third button (A, B, and C), enabling more interactive applications.

✅ Modular microcontroller socket: a new socketed design for the MCU module, allowing future upgrades and simplified maintenance.

---

## 💻 How to Program BitDogLab

BitDogLab supports multiple programming environments, ranging from visual block-based tools for beginners to professional C/C++ development for advanced users.
Below is a step-by-step guide to start programming your board easily using Blockly.

### 🧩 Programming with Blockly (Visual Blocks)

Blockly is the easiest way to get started with BitDogLab — ideal for students and beginners learning programming logic and embedded systems.
You can access it directly online, without installing anything:

👉 Blockly for BitDogLab:
https://bitdoglab-blocos.github.io/BIPES-BITDOGLAB/ui/

Steps to use Blockly:

1) Connect your BitDogLab to your computer using a USB cable.

2) Open the link above in your browser (preferably Chrome or Edge).

3) Select the serial port corresponding to your board.

4) Drag and connect the programming blocks to create your program.

5) Click on “Run” or “Upload” to send the program to your BitDogLab.

⚠️ Important:
If your board already contains a file named main.py in its internal memory, delete it before using Blockly.
The presence of this file can prevent Blockly from communicating properly with the microcontroller.

Tip:

Blockly automatically generates MicroPython code, which can also be viewed and modified in the Thonny IDE if you prefer text-based programming.

###  Programming with MicroPython 

After experimenting with Blockly, you can easily migrate to MicroPython using Thonny IDE with the examples available in the /Firmware/examples/ folder of this repository.
https://gitlab.unicamp.br/fabiano/bitdoglab-v7/-/tree/main/Firmware/examples?ref_type=heads#bitdoglab-micropython-examples

#### 🧰 Tools & Environment
MicroPython with Thonny IDE

Steps:

Flash/update MicroPython firmware on Pico/Pico 2. OBS: Normaly you already received a BitDogLab with a the firmware flashed. 

Connect BitDogLab via USB, select the port under Tools → Interpreter.

Upload the generated script (main.py or similar).

If using Blockly, remove main.py first (it can block communication).


### Quick Guide — Programming BitDogLab with AI (using the BIH)

The simplest workflow to generate code (MicroPython or C/C++) with the help of Artificial Inteligence models using the Hardware Information Database (BIH) — a text file describing the BitDogLab hardware connection is explained bellow:


🔎 What is the BIH

The BIH [Hardware Bank Database (BIH)](https://docs.google.com/document/d/13-68OqiU7ISE8U2KPRUXT2ISeBl3WPhXjGDFH52eWlU/edit?usp=sharing)  is a plain text file containing the pinouts and peripherals of the board/project. It gives technical context so that an LLM (Large Language Model) can write correct code for your BitDogLab, avoiding pin/address mismatches.

#### AI Workflow (Step-by-Step)

1) Open the BIH from the link above.

2) Open your AI assistant (e.g., ChatGPT) and paste the BIH before requesting code.

3) Use a task prompt specifying: target language (MicroPython or C/C++), desired functionality, and constraints. Start with simple tasks.

4) Ask for complete, testable code (with main() or a full script and comments).

5) Iterate: send any compilation/runtime errors back to the AI, always including the BIH and the error log.


## 📂 Repository Structure
```
under construction ...
```


---


## 🛠️ Hardware Design Files (Open Hardware)

This repository holds **open-source design files** for BitDogLab V7, released under the **CERN Open Hardware Licence v2 – Strongly Reciprocal (CERN-OHL-S)**.  
All files can be freely **copied, manufactured, assembled, and improved** by the community, in alignment with open hardware principles.  

### 📂 Structure of hardware files
```
hardware/
├── kicad/
│ ├── bitdoglab/ → Schematics, PCB layout and gerbers (DIY)
│ ├── bitdoglabsmd/ → Schematics, PCB layout and gerbers (SMD)
│ └── libs/ → Symbols, footprints and 3D models
│
└── docs/
├── BOM/ → Bill of Materials (.xlsx and .csv)
└── fabrication/ → Panelization, pick-and-place, gerber and drill

```

### 🔑 Key Components – BitDogLab V7

- **MCU board:** Raspberry Pi Pico H/W or Pico 2 (RP2350)  
- **LEDs:** RGB LED (common cathode) + 5×5 WS2812B matrix  
- **Inputs:** Buttons A/B + Reset, analog joystick (VRx/VRy + SW)  
- **Audio:** Passive buzzers A and B (stereo capable)  
- **Display:** OLED 128×64 via I²C (GPIO2 = SDA, GPIO3 = SCL)  
- **Sensing:** Electret or MEMS microphone (analog)  
- **Power:** USB 5V, battery charger, **INA226** for monitoring (BMS)  
- **Expansion:** IDC header exposing GPIOs, I²C/SPI/UART, 3V3/5V/GND  



