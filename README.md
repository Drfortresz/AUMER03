<div align="center">
<p align="center">
  <img src="https://raw.githubusercontent.com/Drfortresz/AUMER03/main/redcore.png" 
       alt="Redcore Logo" width="512" height="512" />
</p>

# ✨redcores/WRO future engineer 2026


**Kuwait National Qualifier · May 31, 2026**

![Arduino](https://img.shields.io/badge/Arduino-Uno-1abc9c?style=for-the-badge&logo=arduino&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-C%2FC%2B%2B-e74c3c?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Status](https://img.shields.io/badge/Status-Competition%20Ready-9b59b6?style=for-the-badge)
![WRO](https://img.shields.io/badge/WRO-Future%20Engineers-f39c12?style=for-the-badge)

</div>

---

## 📋 Table of Contents

- [🧩 Project Overview](#-project-overview)
- [📁 Repository Structure](#-repository-structure)
- [🔩 Hardware & Components](#-hardware--components)
- [⚡ Power & Electrical System](#-power--electrical-system)
- [💻 Software Description](#-software-description)
- [🚗 Mobility, Power & Sensing System](#-mobility-power--sensing-system)
- [👁️ Obstacle Management & Vision System](#️-obstacle-management--vision-system)
- [🧱 3D Model](#-3d-model)
- [📸 Robot Photos](#-robot-photos)
- [🧪 Testing & Calibration](#-testing--calibration)
- [📈 Results & Performance](#-results--performance)
- [🎬 Video Demonstrations](#-video-demonstrations)
- [👨‍💻 Team Members](#-team-members)

---

## 🧩 Project Overview

The redcore initiative showcases a self-driving vehicle engineered to complete three autonomous laps on a closed course marked with randomized traffic signs. The design blends mechanical engineering, embedded electronics, and computer vision to deliver smooth driving, adaptive decision-making, and intelligent obstacle handling.

At its core, the robot is organized into four interconnected subsystems:

**Motion System**  
A sturdy chassis powered by a DC motor for propulsion and a servo motor for steering. Real-time sensor feedback ensures precise control through curves and speed changes.

**Energy System**  
A 7.4V Li-Po battery provides reliable power to both motors and control electronics. The Xmotion board regulates distribution, reducing electrical noise and maintaining consistent performance.

**Detection System**  
Three ultrasonic sensors continuously measure distances to walls and obstacles, keeping the robot centered. A PixyCam vision sensor identifies red and green traffic markers to guide directional choices.

**Smart Obstacle Handling**  
By fusing ultrasonic data with PixyCam vision, the robot interprets traffic signs, maintains lane balance, avoids collisions, and completes its run by autonomously parking in the designated zone.

Together, these subsystems demonstrate a holistic approach to autonomous mobility, combining dependable sensing, efficient power management, and intelligent control strategies to operate safely in dynamic environments.

---

## 📁 Repository Structure

```
WRO-Future-Engineers-2026/
├── README.md
├── src/
│   ├── Open_Challenge/
│   │   └── Open_Challenge.ino       ← PD wall-following controller
│   └── Obstacle_Challenge/
│       └── Obstacle_Challenge.ino   ← PD + Pixy2 obstacle avoidance
├── Vehicle_Photos/                   ← 6-angle robot photography
├── Models/                           ← 3D design files (.3mf)
├── Schemes/                          ← Electrical schematic & wiring
├── videos/                           ← Demo video links
└── docs/                             ← Team logo & assets
```

---

## 🔩 Hardware & Components

| Component | Model / Spec | Purpose |
|-----------|-------------|---------|
| Microcontroller | Arduino Uno (ATmega328P, 16MHz) | Main logic, sensor reading & control |
| Motor Driver | Cytron MD13S | DC motor speed & direction control |
| Drive Motor | Brushed DC 7.4V | Rear-wheel propulsion (RWD) |
| Steering Servo | Standard servo motor | Front Ackermann steering |
| Distance Sensors | 2× HC-SR04 ultrasonic | Left/right wall distance measurement |
| Vision Sensor | Pixy2 Camera (SPI) | Red/green pillar color detection |
| Battery | 7.4V LiPo 2S | Main power supply for all systems |
| Chassis | WLtoys 284010 (1:28 RC scale) | Compact, robust, competition-ready platform |

**Physical Specs:**  
📐 Dimensions: ~17 × 9 × 7 cm — within WRO 30×20×30 cm limit ✅  
⚖️ Mass: ~0.5 kg (without electronics) — within 1.5 kg limit ✅

<div align="center">
<img width="512" src="https://github.com/user-attachments/assets/b178bf55-6c87-4521-a2f3-9c944bf2b491" alt="Robot Components"/>
<br/><sub><i>Robot components layout</i></sub>
</div>

---

## ⚡ Power & Electrical System

```
LiPo 7.4V ──→ Cytron MD13S  (motor power)
          └──→ Arduino Vin   (logic power)
                  └──→ 5V pin ──→ Servo + HC-SR04 (×2) + Pixy2
```

**Wiring Summary:**

| Component | Pin Connection |
|-----------|---------------|
| HC-SR04 Left | TRIG → D4 · ECHO → D5 · VCC → 5V |
| HC-SR04 Right | TRIG → D2 · ECHO → D9 · VCC → 5V |
| Servo | SIG → A0 · VCC → 5V |
| Cytron MD13S | PWM → D3 · DIR → D8 |
| Pixy2 (SPI) | CS → D10 · MOSI → D11 · MISO → D12 · SCK → D13 |

<div align="center">
<img width="512" src="https://github.com/user-attachments/assets/b750eb5a-1c5d-4c75-9160-fdc2dd91ea35" alt="Lab Tools"/>
<br/><sub><i>Tools and equipment used in development</i></sub>
</div>

---

## 💻 Software Description

The Arduino Uno is programmed using **Arduino IDE** with code written in C/C++. The code architecture is divided into two main modules:

- **`Open_Challenge.ino`** — PD wall-following controller with anti-zigzag techniques
- **`Obstacle_Challenge.ino`** — PD wall-following + Pixy2 vision mode switching

Both sketches share the same core PD logic and tuned parameters, with the Obstacle Challenge adding a hysteresis-based mode manager for pillar detection.

### Control Flow Diagram

```
[HC-SR04 L/R] ──→ [3× Average + LPF] ──→ ┐
                                           ├──→ [Mode Manager] ──→ [PD / Pixy2] ──→ [Servo A0]
[Pixy2 Camera] ──→ [Area + X Filter]  ──→ ┘                                              │
                                                                                           ↓
                                                                                  [Cytron MD13S]
                                                                                           │
                                                                                      [DC Motor]
```

### Tuned Parameters

| Parameter | Value | Role |
|-----------|-------|------|
| `KP` | 0.10 | Proportional gain |
| `KD` | 0.09 | Derivative gain |
| `MOTOR_SPEED` | 55 | Normal cruising PWM |
| `TURN_SPEED` | 45 | Reduced PWM on sharp turns |
| `CENTER_ANGLE` | 90° | Servo straight-ahead |
| `ERROR_DEADBAND` | 0.8 cm | Noise suppression threshold |
| `DERIV_ALPHA` | 0.45 | Derivative low-pass filter |
| `SERVO_ALPHA` | 0.55 | Servo output smoother |
| `MAX_VALID_CM` | 75 cm | Corner spike clamp value |
| `HARD_TURN_DEG` | 28° | Speed-reduction trigger |

---

## 🚗 Mobility, Power & Sensing System

The robot's mobility relies on a single DC motor controlled through the **Cytron MD13S** driver, providing smooth forward motion. A **7.4V LiPo battery** powers all components efficiently through the Arduino's onboard regulator.

**Wall-following PD Controller:**

```
error      = leftDist − rightDist
derivative = smoothed(error − lastError)
output     = KP × error + KD × derivative
servoAngle = CENTER_ANGLE + output
```
