# STM32 Layer-First Architecture 🚀

![STM32](https://img.shields.io/badge/Platform-STM32F0-blue.svg)
![Language](https://img.shields.io/badge/Language-C-green.svg)
![Architecture](https://img.shields.io/badge/Architecture-Layer--First-orange.svg)

Implementasi arsitektur **Layer-First** pada mikrokontroler STM32F030F4PX. Proyek ini bertujuan untuk mendemonstrasikan pemisahan kode antara hardware-specific (HAL) dan logic-specific menggunakan struktur folder yang terisolasi di dalam `/Locker`.


## 🏗️ Arsitektur Proyek
Repositori ini menggunakan folder `/Locker` sebagai jantung dari logika aplikasi agar kode tetap modular dan mudah di-porting ke MCU lain:

1. **Locker/Applications**: Berisi *Main Application Logic* (misal: state machine, proses bisnis utama).
2. **Locker/Services**: Berisi *Middleware* atau *Helper* yang bersifat hardware-agnostic (misal: buffer management, protocol parser).
3. **Locker/Drivers**: Berisi driver spesifik untuk komponen eksternal (misal: sensor BME280 atau PN532) yang diabstraksikan dari HAL.

## 📁 Struktur Direktori
```text
.
├── Core/               # Entry point & STM32 Hardware Init (Auto-generated)
├── Drivers/            # CMSIS & Standard HAL Library
├── Locker/             # 🛠️ Independent Logic Layer (Core of the project)
│   ├── Applications/   # App-level logic
│   ├── Drivers/        # External peripheral drivers
│   └── Services/       # Middleware & reusable services
├── Docs/               # Technical documentation
└── stm32f030f4px.ioc   # STM32CubeMX configuration file
```

## 🛠️ Tech Stack & Tools
* Hardware: STM32F030F4PX
* IDE: VSCode dengan ekstensi STM32 & AntiGravity
* AI Pair Programming: GitHub Copilot (Low-level) & Gemini (High-level logic)
* Framework: Clean Code for Embedded C/C++

## 🚀 Cara Penggunaan
1. Clone: ```git clone https://github.com/octarudin/stm32-layer-first.git```
2. Setup: Buka melalui STM32CubeIDE atau VSCode.
3. Build: Pastikan folder Locker sudah masuk ke dalam Include Paths di pengaturan compiler.

Developed by Octarudin Mahendra - Embedded System Engineer
