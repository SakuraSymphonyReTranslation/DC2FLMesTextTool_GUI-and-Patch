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
│   ├── choice_layout.h   # Normalization and proportional rendering of ○ choices
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

The release version remains **v1.0.0**. To apply the choice fix to an existing patch installation, close the game and replace `DC2FLPatch.dll` with the latest version. Existing INI settings and translated MES files can be kept; no JSON → MES reimport is needed.

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
- **Choices `○`**:
  Circles follow the button edge with a 16-pixel inset. Labels use proportional text, normal word spacing, and font sizes from 28 down to 18 pixels as needed. A 480-pixel button provides 448 pixels for the circle and text. Confirmation popup and dialogue box sizes remain unchanged.
- **File Redirection**:
  `EnableFileRedirection=1` routes MES script reads to `id_Data\MES` or `eng_data\MES`.
- **Locale Emulator**:
  Integrated directly into the launcher for Windows native execution and fully compatible with Wine / Proton on Linux / Steam Deck.

## Choice Layout Verification

- 54 choices from 386 translated JSON files were tested alongside 54 labels from installed MES files: **108 render fixtures and 9 exclusion cases passed**.
- Maximum bitmap width was 445/448 pixels; fixtures used font sizes of 22–28 pixels.
- Game logs confirm two sample menus containing 5 labels. All 54 choices have not been individually checked in-game; hover verification is not yet complete.

`src/choice_layout.h` handles CP932 normalization, spaces, and rasterization. `src/dc2flpatch.cpp` matches each label to its native button rectangle. The original label clip depends on text length, so the clip is expanded only while drawing the choice and restored afterward. Popup and dialogue settings are unchanged.

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
