# SWUpdateNoticed — Qt/QML Progress Monitor for SWUpdate

SWUpdateNoticed is a Qt/QML application for monitoring SWUpdate firmware update progress on embedded Linux devices. It receives progress notifications through SWUpdate's progress IPC interface, shows a progress window when an update starts, and hides it when the update succeeds or fails.

It is intended for devices with a graphical display that need a simple visual indication while SWUpdate installs firmware. SWUpdate performs the update; SWUpdateNoticed displays its progress.

## Features

- Connects to SWUpdate using the `libswupdate` progress IPC API.
- Displays a frameless window sized to the primary screen during updates.
- Shows the percentage reported for the current update step in a progress bar.
- Hides the window on success or failure and keeps running to listen for subsequent updates.

The backend also receives fields such as the current image, step count, and update information. The current QML interface displays only the progress bar, without detailed status text or a persistent success/failure message.

## Requirements

- Linux with a graphical session and a suitable Qt platform plugin
- Qt 5.15 or a compatible Qt 6 installation; the build configuration supports both Qt major versions
- Qt development components: Core, Gui, Quick, Qml, and LinguistTools
- QML runtime modules: QtQuick, QtQuick.Window, and QtQuick.Controls (the QML files use version 2.15 imports)
- CMake 3.14 or later
- SWUpdate development headers, including `progress_ipc.h`, and the `libswupdate` library
- C++11-compatible compiler

At runtime, SWUpdate must be running with its progress IPC interface available to the application. Dependency package names vary by distribution or embedded Linux build system.

## Building

```bash
# Configure (out-of-source)
cmake -B build -S .

# Build
cmake --build build

# Install (optional)
cmake --install build
```

## Usage

Start SWUpdate using your system's update configuration, then launch SWUpdateNoticed from a graphical session:

```bash
# Run from the build directory
./build/src/SWUpdateNoticed

# Or run the installed executable when its install directory is on PATH
SWUpdateNoticed
```

The application initially waits with its window hidden and tries to connect to SWUpdate's progress IPC interface. When it receives a START or RUN notification, it shows the window and updates the progress bar as step progress arrives. A SUCCESS or FAILURE notification hides the window; the application remains running to monitor subsequent updates.

The progress bar represents the current step's percentage, rather than a calculated percentage for the entire firmware update.

### Startup integration

SWUpdateNoticed is a long-running GUI application. This repository does not install a boot service or configure automatic startup. To launch it automatically, configure your device's graphical session or startup mechanism with access to the display and SWUpdate progress IPC interface.

### Troubleshooting

- **The application cannot connect to SWUpdate:** check that SWUpdate is running, its progress IPC interface is available, and the application's user can access it. Connection attempts are logged to the console.
- **The application reports Qt platform or QML module errors:** check that the required platform plugin and QML runtime modules are installed and that the process has access to the graphical session.
- **No window is visible while idle:** this is expected. The window appears when an update starts and is hidden when the update succeeds or fails.

## Related documentation

- [SWUpdate project](https://github.com/sbabic/swupdate)
- [SWUpdate documentation](https://sbabic.github.io/swupdate/)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Report problems through [GitHub issues](https://github.com/hyyoxhk/SWUpdateNoticed/issues) or submit changes through [pull requests](https://github.com/hyyoxhk/SWUpdateNoticed/pulls). For build or runtime issues, include your Qt and SWUpdate versions, platform, and relevant console output.
