# APS6404L_STM32_DRIVER

This repository demonstrates how to interface the **AP Memory APS6404L (8MB PSRAM)** with an **STM32G474** using **Quad-SPI (QSPI) with DMA**, achieving high-speed memory transfers. The STM32G474 has **only 128 KB of internal RAM**, but this **external PSRAM significantly expands memory capacity**, making it suitable for applications like **continuous data acquisition**.

## 🛠 Hardware Details
- **Microcontroller:** STM32G474 (128 KB SRAM)
- **PSRAM:** APS6404L (8 MB, Quad-SPI)
- **Board:** Custom PCB (signal integrity issues above 25 MHz)
- **QSPI Clock:** Limited to **~24.3 MHz (Prescaler 6)**
- **Mode:** **Quad-SPI (QPI) in wrap mode (32-byte bursts)**
- **Transfer Type:** Blocking & DMA-based QSPI transfers

## 🚀 Achieved Speeds

| Transfer Mode  | Write Speed | Read Speed |
|---------------|------------|------------|
| **Blocking (CPU)**  | **1.14 MB/s** | **1.33 MB/s** |
| **DMA (Peripheral)** | **10.00 MB/s** | **10.00 MB/s** |

⚡ **Using DMA for both read & write boosted speeds to 10 MB/s!**  
⚠ **Above 25 MHz, signal integrity degrades on the custom board, limiting further improvements.**

## 🛠 Features
- **QSPI initialization, wrap mode setup (0x5F)**
- **Fast read/write (0xEB / 0x38)**
- **DMA-based QSPI transfers**
- **Speed tests & data integrity checks**
- **Minimal setup required—just connect APS6404L to STM32G4's QSPI pins!**

## 📂 Code Overview
- **`psram_aps6404l.c/h`** → QSPI driver for APS6404L
- **`main.c`** → Initialization, speed tests, DMA callbacks
- **`README.md`** → This file 😎

## 🛠 Setup & Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/STM32G4-PSRAM-DMA-QSPI.git
