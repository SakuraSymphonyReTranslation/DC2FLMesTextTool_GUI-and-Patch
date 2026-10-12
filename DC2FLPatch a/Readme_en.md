# DC2FL Patch — Installation and Build

[Bahasa Indonesia](README.md) | **English**

This runtime patch and custom launcher exclusively support **D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ** (Da Capo II Fall in Love). Core patch implementation: `src/dc2flpatch.cpp`; launcher implementation: `src/launcher.cpp`.

## Directory Structure

```text
DC2FLPatch a/
├── CMakeLists.txt        # CMake build configuration
├── build.bat             # Automated build script (NMake / MSVC)
├── icon.ico              # DC2FL game icon (IconGroup 105)
├── src/
│   ├── dc2flpatch.cpp    # Main patch hook DLL code (injected version)
│   ├── launcher.cpp      # DC2FLLauncher source code (DLL injection + Locale Emulator)
│   ├── launcher.rc       # Launcher resources (icon & metadata)
│   ├── d3dx9_proxy.cpp   # Optional D3DX9 proxy
│   └── dsound_proxy.cpp  # Optional DSound proxy
├── Detours/              # Microsoft Detours library
├── Locale-Emulator-2.5.0.1/ # Locale Emulator library
└── build/
    └── Released/         # Ready-to-use distribution package
```

## Installation

Download `Released_patch.rar` from [GitHub Releases](https://github.com/SakuraSymphonyReTranslation/DC2FLMesTextTool_GUI-and-Patch/releases/latest), or use the files under `build/Released`.

`<GAME_DIR>` denotes the **main game installation folder chosen by the player**, specifically the folder containing `DC2FL.EXE` and `AdvData`. Replace this placeholder with your actual game folder path; no specific drive is required.

1. Close the game and back up any files that will be replaced.
2. Extract the contents of `Released_patch.rar` directly into `<GAME_DIR>`, placing `DC2FLLauncher.exe`, `DC2FLPatch.dll`, and `DC2FLPatch.ini` alongside `DC2FL.EXE`.
3. Launch **`<GAME_DIR>\DC2FLLauncher.exe`** with `<GAME_DIR>` as the working directory. If creating a shortcut, set **Start in** to `<GAME_DIR>`. The launcher injects the patch DLL and sets the Japanese locale automatically.
4. Place Indonesian translation scripts under `<GAME_DIR>\id_Data\MES` or English scripts under `<GAME_DIR>\eng_data\MES`.

The tested reference executable has SHA256:

```text
aacb2192b8ab89d77ebc08addf10d993a1bd2401fc11e93389c2f917a0d079eb  DC2FL.EXE
```

## Patch Features & Configuration

- **Font Rendering & Advanced Scaling**:
  Configured in `DC2FLPatch.ini`:
  - `DialogueRenderScale=74`
  - `DialogueAdvanceScale=62`
  - `DialogueSpaceScale=24`
  Ensures clean proportional Latin/Indonesian font rendering without character crowding or overlap.
- **Backlog & Popups**:
  `ShowBacklogIcon=1` maintains speaker portrait icons in the backlog and centers system confirmation popups.
- **File Redirection**:
  `EnableFileRedirection=1` routes MES script reads to `id_Data\MES` or `eng_data\MES`.
- **Locale Emulator**:
  Integrated directly into the launcher for Windows native execution and fully compatible with Wine / Proton on Linux / Steam Deck.

## Building (Visual Studio 2022, x86 Native Tools)

From the `DC2FLPatch a` folder:

```powershell
.\build.bat
```

Or manually using CMake:

```powershell
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Build outputs:
- `build\DC2FLLauncher.exe`
- `build\DC2FLPatch.dll`
