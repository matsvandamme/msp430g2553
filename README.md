```markdown
# MSP430G2553 Project

---

## 🚀 About This Project

This repository contains a simple project for the Texas Instruments MSP430G2553 microcontroller. It's set up with a clean structure, including custom drivers and a Makefile for easy building and flashing. This project is a great starting point for anyone looking to get their hands dirty with embedded C programming on the MSP430 platform.

---

## 📂 Repository Structure

```

.
├── Makefile
├── datasheets
│   ├── ... (various datasheets)
├── drivers
│   ├── gpio.c
│   ├── gpio.h
│   ├── led.c
│   └── led.h
└── main.c

````

---

## 🛠️ Getting Started

### Prerequisites

* [MSP430-GCC Toolchain](http://www.ti.com/tool/msp430-gcc-opensource)
* [MSPDebug](http://mspdebug.sourceforge.net/)

### Building the Project

To build the project, simply run the `make` command in the root directory:

```sh
make
````

This will compile the source files and create the output file in the `build/bin` directory.

### Flashing the Code

To flash the compiled code to the MSP430G2553, run the following command:

```sh
make flash
```

### Cleaning Up

To remove the build files, run:

```sh
make clean
```

-----

## 💡 Code Overview

### `main.c`

The main application entry point. It initializes the GPIOs for the red and green LEDs and then enters an infinite loop.

### `drivers/`

This directory contains custom drivers for the MSP430G2553.

  * **`gpio.h` / `gpio.c`**: A GPIO driver for configuring and controlling GPIO pins.
  * **`led.h` / `led.c`**: A simple LED driver.

-----

```
```