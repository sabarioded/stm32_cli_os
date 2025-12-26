# STM32 Custom CLI & RTOS

A lightweight, preemptive Real-Time Operating System (RTOS) built from scratch for the STM32L476RG (Cortex-M4). This project demonstrates the implementation of a kernel, a thread-safe memory allocator, and an interactive Command Line Interface (CLI) without relying on the standard STM32 HAL or external RTOS libraries.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32L476-green.svg)
![Architecture](https://img.shields.io/badge/arch-ARM%20Cortex--M4-orange.svg)

## Key Features

### 1. Preemptive Kernel
* **Round-Robin Scheduling:** Implements true context switching using `PendSV` and assembly (PSP/MSP separation).
* **Task Management:** Supports dynamic task creation, deletion, and sleeping (`task_sleep_ticks`).
* **Context Safety:** Full register context saving (R4-R11) and FPU safety.
* **Idle Task:** Automatic garbage collection and power saving (`WFI`) when no tasks are ready.

### 2. Custom Memory Management
* **Heap Allocator:** A `malloc`/`free` implementation using a "Best-Fit" strategy with block coalescing to reduce fragmentation.
* **Thread Safety:** A wrapper (`stm32_alloc.c`) protects the heap using `BASEPRI` masking, preventing corruption from interrupts.
* **Diagnostics:** Built-in commands to visualize heap map and fragmentation.

### 3. Interactive CLI
* **UART Driver:** Interrupt-driven (non-blocking) UART with Ring Buffers for RX/TX.
* **Command Parser:** Token-based argument parsing (argc/argv style).
* **Extensible:** Easy API to register custom commands via `cli_register_command`.

### 4. Bare-Metal Drivers
* **Zero-HAL:** Custom register definitions (`device_registers.h`) instead of vendor HAL bloat.
* **System Clock:** Dynamic configuration of PLL, MSI, and HSI16 oscillators.
* **Peripherals:** Drivers for GPIO, SysTick, and USART2.

---
