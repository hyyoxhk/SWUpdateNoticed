# SWUpdateNoticed

A Qt-based user interface for monitoring SWUpdate progress. This application provides a visual interface to track system updates through the SWUpdate daemon.

## Requirements

- Qt 5.15 or later
- CMake 3.14 or later
- SWUpdate library/headers (for `progress_ipc.h`)
- C++17-compatible compiler

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

1. The application starts automatically as a daemon when the system boots
2. It continuously monitors for system updates through the SWUpdate daemon
3. When an update is detected, it creates a display window showing:
   - A progress bar indicating the current update status
   - Detailed update information (steps, percentage, etc.)
4. The window automatically closes when the update completes
5. The daemon continues running to monitor for future updates

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

If you want to contribute changes, you can send Github pull requests at https://github.com/hyyoxhk/SWUpdateNoticed/pulls
