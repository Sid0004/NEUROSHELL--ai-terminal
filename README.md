NEUROSHELL

A minimal green-on-black Qt GUI that behaves like a terminal. No AI yet; basic commands only.

Tech used
- C++17
- Qt 6 (Widgets)
- CMake
- MinGW or MSVC toolchain

Project layout
- `main.cpp`: Qt app entry point
- `TerminalWindow.h/.cpp`: Terminal-like UI and command handler
- `CMakeLists.txt`: Build configuration (Qt6 Widgets)

Build and run (Windows, MinGW)
1. Open the "Qt 6.x.x for MinGW 64-bit" command prompt, or run:
   ```bat
   "C:\\Qt\\6.9.2\\mingw_64\\bin\\qtenv2.bat"
   ```
2. Configure and build:
   ```bat
   cd C:\\Users\\siddh\\OneDrive\\Documents\\Desktop\\NEUROSHELL
   cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\\Qt\\6.9.2\\mingw_64"
   cmake --build build --config Release
   ```
3. Run:
   ```bat
   build\\NEUROSHELL.exe
   ```

Build and run (Windows, MSVC)
1. Open "Qt 6.x.x for MSVC 64-bit" command prompt, or:
   ```bat
   "C:\\Qt\\6.9.2\\msvc2022_64\\bin\\qtenv2.bat"
   ```
2. Configure and build (Ninja or NMake):
   ```bat
   cmake -S . -B build -G "Ninja" -DCMAKE_PREFIX_PATH="C:\\Qt\\6.9.2\\msvc2022_64"
   cmake --build build --config Release
   ```

UI / Behavior
- Green text on black background, monospace font
- Prompt label: `cli >>` next to the input
- Command history navigation with Up/Down arrows
- Output area above, read-only

Commands
- `help`: Show available commands
- `clear` / `cls`: Clear the screen
- `echo TEXT`: Print TEXT
- `date`: Current date (ISO)
- `time`: Current time (HH:mm:ss)
- `whoami`: Current user
- `version`: App + Qt version, OS pretty name
- `history`: Show recent commands
- `aboutme`: Project info
- `exit`: Close the window

Next steps (ideas)
- Add real shell command execution (carefully sandboxed)
- Add tabs/sessions
- Add copy/paste and selection tweaks
- Add settings for colors/fonts
- Integrate AI later


cd C:\Users\siddh\OneDrive\Documents\Desktop\NEUROSHELL
cmake --build build --config Release
build\NEUROSHELL.exe