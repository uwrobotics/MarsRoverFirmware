# Mars Rover 2020 Firmware Repository
![CI](https://github.com/uwrobotics/MarsRover2020-firmware/workflows/CI/badge.svg)

## Platform: [STM32F446xE](https://www.st.com/resource/en/datasheet/stm32f446re.pdf) / [NUCLEO-F446RE](https://os.mbed.com/platforms/ST-Nucleo-F446RE/)

This repository contains:
- Arm MBED OS 6 SDK source [[mbed-os](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/mbed-os)]
- Custom and external libaries [[lib](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/lib)]
- Applications for running on each control board [[apps](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/apps)]
- Test applications for testing code components [[apps/test_xxxx](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/apps)]
- Configuration files for each target hardware [[targets](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/targets)]
- Miscellaneous items [[misc](https://github.com/uwrobotics/MarsRover2020-firmware/tree/alex/add-readme/misc)]
- Github Actions configurations for automatic build testing [[.github](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/.github)]
- Makefile [[makefile](https://github.com/uwrobotics/MarsRover2020-firmware/tree/master/makefile)]

## Best Contribution Practices and Tips

- Create a branch in the format `yourName/#<issue-number>/featureName` for every feature you are working on
- Rebase onto master and test on hardware before merging into master
- Add a Github Actions build target for your application if it is not a test application
- Create a pull request to merge any branch into master and select everyone else working on the feature as reviewers
  - Name the pull request `Closes #<issue-number>: FeatureTitle`
- When merging a pull request that fixes an issue title the commit `Fixes #issueNumber: FeatureTitle`
- Clean binaries between making changes to the makefile
- There seems to be an annoying mix of CamelCase and snake_case in MBED but just try to be consistent with whatever code is nearby
- Squash when merging pull requests

## UWRT Firmware Development Instructions
1. Download the development toolchain (gcc and cmake) and serial interface software
   
    For Ubuntu (18.04 preferred)
    - `sudo apt update`
    - `sudo apt install make` for build
    - `sudo apt install screen can-utils` for serial and CAN interfacing
    - Install/update ARM GCC toolchain:
        ```
        sudo apt autoremove gcc-arm-none-eabi
        wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2
        sudo tar -xvf gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 -C /opt/
        echo "PATH=\$PATH:/opt/gcc-arm-none-eabi-9-2020-q2-update/bin" >> ~/.bashrc
        export PATH=$PATH:/opt/gcc-arm-none-eabi-9-2020-q2-update/bin 
        ```
        **Note:** If you are not using Ubuntu 18.04 and/or bash you may need to modify this script's paths/files.
    - Install CMake:
        * Follow [kitware instructions](https://apt.kitware.com/) to add Latest CMake apt repository
        * `sudo apt install cmake`
    
    For Windows
    - Install [Windows Subsystem for Linux (WSL)](https://linuxconfig.org/how-to-install-ubuntu-18-04-on-windows-10) with Ubuntu 18.04
    - Follow Ubuntu setup instructions (optionally instead of `screen` you can use [PuTTy](https://www.chiark.greenend.org.uk/~sgtatham/putty/), a GUI Windows app)
    
    For Mac (Not Recommended)
    - Open Command Line
    - Install Homebrew if not installed
        ```
        /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
        ```
    - Download auto-run script, which will auto install with latest version:
        ```
        brew tap ARMmbed/homebrew-formulae
        ```
    - Install ARM GCC toolchain via HomeBrew:
        ```
        brew install arm-none-eabi-gcc
        ```
    - Install [ZOC](https://www.emtec.com/zoc/index.html) for serial interfacing

2. Verify the the toolchains were installed correctly

    Open a new Command Prompt / Terminal window and run the following commands:
    ```shell script
    make --version                    # Should be v3.8.x or newer
    arm-none-eabi-gcc --version       # Should be v9.3.x or newer
    cmake --version                   # Should be v3.15.7 or newer
    ```

3. Download source code

    ```shell script
    git clone --recurse-submodules https://github.com/uwrobotics/MarsRover2020-firmware.git
    cd MarsRover2020-firmware
    ```

    **Note:** The repository has a submodule. To update the submodule, use `git submodule update`

4. Run make with the target application and board

    Ex. Compile the science application for the science board:  
    `make APP=science TARGET=science`

    Ex. Compile the arm application for the arm board:  
    `make APP=arm TARGET=arm`
    
    After compiling an application you should see a message similar to the following:  
    ```shell script
    [100%] Linking CXX executable arm.arm-board.elf
    Generating bin from elf
    make[4]: Leaving directory '/home/wmmc88/MarsRover2020-firmware/build-arm-board'
    [100%] Built target arm.arm-board.elf
    make[3]: Leaving directory '/home/wmmc88/MarsRover2020-firmware/build-arm-board'
    make[2]: Leaving directory '/home/wmmc88/MarsRover2020-firmware/build-arm-board'
    make[1]: Leaving directory '/home/wmmc88/MarsRover2020-firmware/build-arm-board'
    ```
    
    **Note:** Our makefile automatically detects the number of available execution threads and uses them all to 
    significantly speed up compile time. You can choose to use fewer threads during build with the following command:
    ```
    UWRT_FIRMWARE_MAX_JOBS=<max number of threads> make APP=<app-name> TARGET=<target-name>
    ```
    You can add the `UWRT_FIRMWARE_MAX_JOBS` to your `.bashrc` to have this max thread limit be persistent.
    ```
    echo "export UWRT_FIRMWARE_MAX_JOBS=<max number of threads>" >> ~/.bashrc
    ``` 
   
   **Tip:** You can choose to build all the supported app/target configs at once using `make all`
   

5. Deploy onto board (see below for how to connect to a rover control board)

    **Note:** The following instructions only apply to Nucleo and Rev 1 boards. For Rev 2 boards and beyond, see [Using the ST-Link to Program Rover Boards (Rev 2 +)](#using-the-st-link-to-program-rover-boards-rev-2-)

    Find the application .bin file, located in the build-<TARGET>-board/apps/<APP> directory.

	For Ubuntu
		
    - Install libusb `sudo apt install libusb-1.0-0-dev`
    - Drag and Drop .bin file into NODE_F446RC device folder
	
	For Windows
    
    - Drag and Drop .bin file into NODE_F446RC device folder OR if this does not work or debugging is required:
    - Download [st-link utility](http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stsw-link004.html). Scroll down to Get Software
    - Connect USB to nucleo board and open st-link utility
    - Load code by going to Target->Program and browse for .bin file
	
	For Mac
    - Drag and Drop .bin file into NODE_F446RC disk

    After deploying, the Nucleo will begin to flash red and green. Once the LED stays green, power-cycle the board by unplugging and replugging the 5V connector on the Nucleo.

- To clean the project workspace of app and library build files, run `make clean`
- To clean compiled MBED SDK files, run `make clean-mbed`

## Using the ST-Link to Program Rover Boards (Rev 2 +)

Rev 2 PCBs come with an ARM 10-pin SWD header and can be programmed via ST-Link. A 20-pin to 10-pin adapter is needed to hook the ST-Link 20-pin header to the rover board 10-pin header.

### ST-Link Software Installation

### Ubuntu
1.  Download and build the ST-Link utilities:
    ```
    git clone https://github.com/stlink-org/stlink
    cd stlink
    cmake .
    make
    ```

2. Copy the built binaries into their place:
    ```
    cd bin
    sudo cp st-* /usr/local/bin
    cd ../lib
    sudo cp *.so* /lib32
    ```

3. Copy the udev rules to their place (this lets you run the st-link commands without using `sudo`):
    ```
    cd ../config/udev/rules.d
    sudo cp 49-stlinkv* /etc/udev/rules.d/
    ```

### Windows

1. Download [st-link utility](http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stsw-link004.html)

### Steps to Flashing a Rover Board

#### Ubuntu

1. Cd into the folder containing the binary of the program to flash (eg. `MarsRover2020-firmware/build-<target name>/apps/<app name>`)
2. Ensure that the ST-Link is connected to your computer and to the rover board.
    - **Tip:** You can run `st-info --probe` to check if the ST-Link connection is detected
3. Flash the program binary: `st-flash write <app name>.bin 0x8000000` (may need to run this command twice to get a successful flash)
    - If the flashing was successful, you should see a blinking LED pattern on the board and the following message: `Flash written and verified! jolly good!`
4. Reset the board: `st-flash reset`
    - This is required to run the program that was flashed

- Alternatively, instead of doing srun `st-flash --reset write <app name>.bin 0x8000000` to flash the board and trigger a reset both before and after flashing
- See [github.com/stlink-org/stlink](https://github.com/stlink-org/stlink) for further documentation

#### Windows

1. Connect the ST-Link to the rover board and to your computer
2. Open ST-Link Utility
3. Click "Connect to the target"
4. Click "Open file" and select the `.bin` file of the program to flash
5. Click "Program & verify:
6. Click "Start"

## Using the Nucleo Dev Board to Program the Rover Boards (Rev 1)

In order to use the Nucleo development board as a programmer, the two jumpers (black caps) labelled NUCLEO - ST-LINK will need to be removed. This will sever the ST-LINK debugger portion of the Nucleo from the MCU side, allowing it to be used as a general debugger.

The ST-LINK debugger can then be connected via header CN4 (pins 1-5 with 1 nearest to the SWD label) to a rover board debug header (pins should be labelled) to program it according to the following table:

```
+-----------------------+-----------------------------------+
| Nucleo CN4 Pin Number | Rover Board Debug Header Pin Name |
+-----------------------+-----------------------------------+
| 1 (VREF)              | VCC                               |
| 2 (SWCLK)             | CLK                               |
| 3 (GND)               | GND                               |
| 4 (SWDIO)             | IO                                |
| 5 (NRST)              | RST                               |
| 6 (SWO)               | Not Connected                     |
+-----------------------+-----------------------------------+
```
![](https://github.com/uwrobotics/MarsRover2020-firmware/blob/cindy/SWO-logging/misc/NucleoSWDLabels.png)

After deploying the binary to the board, the Nucleo's `LD1` LED will flash red and green. Programming is complete when the LED stays green, so don't powercycle the board before this.

## Serial Wire Output (SWO) (Rev 2 +)

The 10-pin Serial Wire Debug (SWD) programming interface does not come with UART lines for standard printf usage. Instead, we will use Serial Wire Output (SWO), a single wire interface to transmit trace messages to an external debugger.

See `apps/test-logger` for an example of using the SWO-supported logger utility.

### Ubuntu

**TODO**

### Windows

1. Ensure that the rover board is running and that the ST-Link is connected
2. In the ST-Link Utility software, click "Print via SWO viewer"
3. Set the system clock rate to 180000000Hz and set the stimulus port to 0
4. Click "Start". The SWO print statements should appear in the Serial Wire Viewer console.

## Serial Communication (Rev 1)

The boards can be communicated with through the serial interface exposed through the debug pins. You can use the USB-serial interface built into the Nucleo dev boards to communicate with the control boards by connecting the TX pin to the board's RX pin and the RX pin to the board's TX pin (transmit to recieve and vice versa). Ensure the program running on the nucleo is not printing too.

On Ubuntu
- Run `screen /dev/serial/by-id/usb-STM* 115200` from the terminal. You may need to prepend this with `sudo`.

On Windows
- Device manager, go to Ports (COM & LPT) and find the name of the Nucleo port (ie COM4)
- Open PuTTy, select the Serial radio button, enter the COM port name and the baud rate (default 115200) and click open

## CAN Communication

The boards can also be communicated with over the CAN bus interfaces. You can use a CANable serial USB-CAN dongle to communicate with them from your development computer. Connect the CAN_H, CAN_L, and GND pins of the CANable to the corresponding pins on the board, and the dongle to your computer.

On Ubuntu
- Run `sudo slcand -o -c -s6 /dev/serial/by-id/*CAN*-if00 can0` to set up the CAN interface
    - The flag `-s6` sets the bus speed to 500 kbps
    - The flag `-s8` sets the bus speed to 1 Mbps
- Run `sudo ip link set can0 up` to enable the interface
- Run `cansend can0 999#DEADBEEF` to send a frame to ID 0x999 with payload 0xDEADBEEF
- Run `candump can0` to show all traffic received by can0

See the [CANable Getting Started guide](https://canable.io/getting-started.html) for more information including Windows support.

## Clang-Format
This repository follows the formatting rules outlines in the [.clang-format](.clang-format) file. You can make your code conformant by 
using the CLI ([documentation here](https://clang.llvm.org/docs/ClangFormat.html)) or by installing a clang-format plugin/extension in 
your IDE of choice. For example, CLion has [built-in Clang-Format support](https://www.jetbrains.com/help/clion/clangformat-as-alternative-formatter.html) 
and VS Code has a [decent extension](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format).

You may have to edit the settings of the plugin to match the version of clang-format that we are using. The current version can be seen [here](.github/workflows/clang-format.yaml) under `clangFormatVersion`.

To download the matching version of LLVM(contains clang-format):
```shell script
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh <version number>
```

For VS Code you will need to specify the path to the clang-format executable. To do this:
1. In shell script copy the output of the following command: ` which clang-format-11`
2. Open the User Settings in JSON format
3. Add the two following settings to the file
"editor.formatOnSave": true,
"clang-format.executable": "{path copied from step 1}"

## Writing Test Apps
For every feature that gets added, a test app that tests the feature in isolation should be written to verify that the feature actually functions properly in our hardware. The author of the feature should ensure that the test app works on all the board targets that make sense. Typically this means that the test app should work on at least the nucleo board and the board that the feature was designed for. For example, the `test-can` app should be able to be compiled and run successfully for all of our boards, including a nucleo dev board.

## Add New Apps, Libraries, and Board Targets
You can take a look at the apps, libraries, and board targets that are already added in existing CMakeLists to see how you declare new CMake targets. If more clarification is needed, you can refer to the [official CMake documentation](https://cmake.org/documentation/).

Tips:
* target_set_mbed_dependency(_my-library-target-name_) is a custom CMake function that should be used on all library targets. It is what links the library to mbed (and the board target) and also provides precompiled headers.
* target_set_firmware_properties(_my-app-name_.${TARGET}-board.elf) is a custom CMake function that should be used on all app elf targets. It links against mbed, applies the proper linker script and generates the .bin target.
* target_compile_options(_my-cmake-target-name_ PRIVATE -w) can be used to disable all warnings from within this target. This should **only** be used to disable warnings coming from mbed in board targets.
* target_compile_options(_my-board-target-name_ PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>) should be used on all board targets to make sure that mbed is compiled with correct settings. 

**Important Note:** Whenever an app or board target is added, make sure to also add the relevant configurations to the `supported_build_configurations.yaml`. This list is used to keep track of what app-target tuples are "supported" and should always build properly. For example, if you add a new board target, add the board target to each app that should be able to compile for your new board. As another example, if you add a new app, you should add the app to the `supported_build_configurations.yaml` and list out all the boards it should be able to be built on. 


## CMake: Adding mbed-os features
This repository uses our own custom CMake files to configure and compile our applications, including mbed-os. Only certain parts of mbed are currently compiled into the mbed-os library target. If other mbed features are required, add the required sources to the `target_sources` property and the required include paths to the `target_include_directories`of the `mbed-os` target. 

**Tip:** These commands may be useful in determining what files you need to include:
```
// Outputs a alphabetically-sorted list of the paths to all source files within a directory and all subdirectories 
find <path to desired feature folder> -name "*.cpp" -or -name "*.cxx" -or -name "*.c" -or -name "*.S" -or -name "*.s" | sort -n

// Outputs a alphabetically-sorted list of the paths to all header files within a directory and all subdirectories 
find <path to desired feature folder> -name "*.hpp" -or -name "*.h"| sort -n
```
## Updating Files from Upstream
Most of this repositories upstream code is contained within the `mbed-os` git submodule and can be updated by just changing the tag that the submodule is pinned to; however, there are some exceptions. These exceptions must be manually updated:
* `test-blinky/main.cpp`: This file is a mirror of the main.cpp from the https://github.com/ARMmbed/mbed-os-example-blinky repository
* `targets/nucleo/include/PeripheralNames.h`: This file is from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/nucleo/include/PinNames.h`: This file is a mirror of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/nucleo/include/stm32f4xx_hal_conf.h`: This file is a mirror of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/nucleo/src/PeripheralPins.h`: This file is a mirror of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/nucleo/src/system_clock.c` : This file is a mirror of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/<non-nucleo target>/include/PeripheralNames.h`: This file is a customized version of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/<non-nucleo target>/include/PinNames.h`: This file is a customized version of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/<non-nucleo target>/include/stm32f4xx_hal_conf.h`: This file is a customized version of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/<non-nucleo target>/src/PeripheralPins.h`: This file is a customized version of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
* `targets/<non-nucleo target>/src/system_clock.c` : This file is a customized version of the file from the `targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TARGET_NUCLEO_F446RE` within the mbed-os git submodule.
