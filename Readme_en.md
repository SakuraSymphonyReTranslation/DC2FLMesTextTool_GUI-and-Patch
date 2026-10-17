# DC2FL MES Text Tool — GUI and CLI

[Bahasa Indonesia](README.md) | **English**

Scenario script importer/exporter and runtime patch for **D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ** (Da Capo II Fall in Love).

This distribution exclusively supports **Da Capo 2 Fall in Love** (`dc2fl`, MES script version `0x9C69`). The GUI, CLI, C++ reference tables, patch DLL, and launcher are tailored specifically for this game. The primary patch source code is located at `DC2FLPatch a/src/dc2flpatch.cpp`.

## Downloads and Installation Folders

Download both packages from the latest [GitHub Releases](https://github.com/SakuraSymphonyReTranslation/DC2FLMesTextTool_GUI-and-Patch/releases/latest):

- **DC2FLMesTextTool_GUI.rar**: MES ↔ JSON conversion tool (GUI and CLI), including Indonesian and English documentation.
- **Released_patch.rar**: Custom launcher with IconGroup105 icon, patch DLL, INI configuration, reference executable, and Locale Emulator libraries.

Both packages are available under release **v1.0.0**. The updated packages include the layout fix for choices beginning with `○` and refreshed documentation.

`<GAME_DIR>` denotes the **main game installation folder chosen by the player**, specifically the folder containing `DC2FL.EXE` and `AdvData`. This is a documentation placeholder: replace it with your actual game folder path (such as `C:\Games\DC2FL` or wherever your game is installed), without `<` or `>`. There is no mandatory installation drive or fixed folder.

### Patch Installation Steps

1. Close the game and create a backup of original files that will be replaced, especially the executable and any existing INI file.
2. Extract the contents of `Released_patch.rar` directly into `<GAME_DIR>`, so that `DC2FLLauncher.exe`, `DC2FLPatch.dll`, and `DC2FLPatch.ini` are placed alongside `DC2FL.EXE`.
3. Launch **`<GAME_DIR>\DC2FLLauncher.exe`** with `<GAME_DIR>` as the working directory. If creating a shortcut, ensure the **Start in** field is set to `<GAME_DIR>`. The launcher initializes Japanese locale (CP932) via Locale Emulator and injects `DC2FLPatch.dll` into the game.
4. Place translated Indonesian MES script files under `<GAME_DIR>\id_Data\MES`. If using English translation scripts, place them under `<GAME_DIR>\eng_data\MES`. Ensure the `Language` setting in `DC2FLPatch.ini` matches your desired translation (`1` for Indonesian, `2` for English).

```text
<GAME_DIR>/
├── DC2FL.EXE
├── DC2FLLauncher.exe
├── DC2FLPatch.dll
├── DC2FLPatch.ini
├── IconGroup105.ico
├── LoaderDll.dll
├── LocaleEmulator.dll
├── AdvData/
├── id_Data/
│   └── MES/
└── eng_data/
    └── MES/
```

Both RAR packages supply the tools and patch runtime; neither package contains the full game translated script text. The source repository folder may be located anywhere on your system. A ready-to-use patch bundle is also available in the repository at `DC2FLPatch a/build/Released`.

## Launching the Tool

Extract `DC2FLMesTextTool_GUI.rar` to a folder of your choice and execute `DC2FLMesTextTool_GUI.exe`. If building from source, run `bin\Release\net8.0-windows\DC2FLMesTextTool_GUI.exe`.
The ready-to-distribute package is located in the same folder: `DC2FLMesTextTool_GUI.rar`.
Requires the [.NET 8 Desktop Runtime](https://dotnet.microsoft.com/download/dotnet/8.0).

## Single File

### MES → JSON (Exporter)

1. Select the original `.mes` script file in **Input MES/JSON**.
2. Specify **Output JSON/MES**, or leave it blank to automatically output a `.json` file in the same folder.
3. Click **Export (MES -> JSON)**.

The default output pairs character names and dialogue according to the DC2FL script structure:

```json
[
  { "message": "Narration text." },
  { "name": "Yoshiyuki", "message": "「Character dialogue.」" }
]
```

Exports from both Single File and Batch tabs consistently generate the combined `{name, message}` format shown above. Dialogue text, Japanese/translated names, and newlines are preserved accurately.

### JSON → MES (Importer)

1. Select the translated `.json` file in **Input MES/JSON**.
2. Ensure the original base `.mes` file with the same filename is located in the same folder (or specify the template file).
3. Set the **Output JSON/MES** path to a destination different from the original base file.
4. Set the **Word Wrap Width**: default is **70** across Single File, Batch, and CLI. If text appears too long or tightly clipped in the game's dialogue box, adjust this value accordingly.
5. Click **Import (JSON -> MES)**.

The importer handles combined JSON, legacy flat format, and mixed formats. Script tokens and label structures are strictly validated against the original template to ensure scenario flow stability.

## Batch

- **Batch Export:** Specify **MES Folder** and **Output Folder**, then click **Batch Export (MES -> JSON)** to convert all `.mes` files in the folder to `.json`.
- **Batch Import:** Specify **JSON Folder**, original **MES Folder** (original template), and **Output Folder**, then click **Batch Import (JSON -> MES)**.
- Automatic filename matching: for example, `scene01.json` automatically pairs with `scene01.mes`.
- Failed files are logged in detail while the batch run continues with remaining files.

## Layout & Runtime Patch Settings

In-game text rendering and layout are handled dynamically at runtime by `DC2FLPatch.dll`. Configuration options in `DC2FLPatch.ini`:

```ini
[Fonts]
BacklogFont=MS PGothic
BacklogSize=-15
DialogueFont=MS PGothic
DialogueSize=-15
BacklogNameFont=MS PGothic
BacklogNameSize=-11
BacklogXOffset=0
BacklogLineSpacing=0
BacklogYOffset=12
BacklogNameXOffset=13
BacklogNameYOffset=0
BacklogNameSpacing=0
BacklogDialogSpacing=0
DialogueRenderScale=74
DialogueAdvanceScale=62
DialogueSpaceScale=24
AdvancedSettings=1

[Settings]
ShowBacklogIcon=1
EnableFileRedirection=1
Language=1
DisableBacklogFont=0
DisableBacklogSpacing=0
DisableBacklogTranslation=0
```

- **Font & Render Scaling**: `DialogueRenderScale=74` and `DialogueAdvanceScale=62` adjust character spacing so Latin text renders cleanly without character overlap.
- **Redirection**: `EnableFileRedirection=1` reroutes script reads to `id_Data\MES` or `eng_data\MES` without modifying original game archives.
- **Backlog Icons**: `ShowBacklogIcon=1` maintains and aligns character avatar icons in the game backlog window.

### Choice Layout `○`

- Translated choices beginning with `○` use proportional text with normal word spacing. Extra spaces and legacy `~` separators are normalized while rendering.
- The circle follows the original button bounds, with a **16-pixel** left inset. Text starts at the same position across choices; the font shrinks when a label is too long.
- A **480-pixel** button provides **448 pixels** for the circle and text. Button backgrounds, hit areas, colors, and shadows retain their native behavior.
- The fix is provided by `DC2FLPatch.dll`. If translated MES files are already installed, replace the DLL while the game is closed; **no importer/exporter update or JSON → MES reimport is required**.
- The `○` marker may be followed directly by text or by one space, for example `○First choice` and `○ Second choice`. Do not add padding spaces to position labels. Confirmation popup and dialogue box size settings remain unchanged.

Technical details and C++ patch building instructions are provided in the [Patch README](DC2FLPatch%20a/Readme_en.md).

## Validation and Encoding

- Circus script text is encoded using **CP932 (Shift-JIS)**.
- Extended Latin characters outside basic Shift-JIS are safely normalized (such as em dashes `— → ―`, accented letters `û → u`, `ë → e`, `é → e`, `à → a`). Unsupported characters are cleanly rejected with informative log diagnostics rather than silently corrupted.
- All file writes use temporary buffering and validation prior to replacement, guarding against incomplete writes.

## CLI

From the executable folder (`bin\Release\net8.0-windows` or the extracted RAR folder):

```powershell
.\DC2FLMesTextTool_GUI.exe export "scene.mes" "scene.json"
.\DC2FLMesTextTool_GUI.exe import "scene.json" "original\scene.mes" "new\scene.mes" -w 70
.\DC2FLMesTextTool_GUI.exe batch-export "original_mes" "json_output"
.\DC2FLMesTextTool_GUI.exe batch-import "json_folder" "original_mes" "mes_output" -w 70
```

- Parameter `-w <width>`: Configures word wrap boundary (default: `70`).
- Supports drag-and-drop: drag `.mes` files or folders directly onto the `.exe` to trigger automatic export.

## Verification Results

- Script export and import formats were validated against original DC2FL scenarios.
- An audit of 386 translated JSON files found **54 choice entries** across 23 files; per-file choice counts match the installed MES files.
- **108 render fixtures** (54 JSON + 54 MES) and 9 exclusion cases passed. All choice labels fit within 448 pixels, with a maximum bitmap width of 445 pixels.
- Runtime logs confirm the new position in two sample menus (5 labels). Coverage of all 54 choices comes from render tests; visual checks of every choice and hover state are not yet complete.
- The tested reference game executable has SHA-256 hash:

```text
aacb2192b8ab89d77ebc08addf10d993a1bd2401fc11e93389c2f917a0d079eb  DC2FL.EXE
```

## Building from Source

From the repository root:

```powershell
# Build C# GUI & CLI (.NET 8 SDK required)
dotnet build -c Release
```

For the C++ patch (Visual Studio 2022 x86 Native Tools):

```powershell
cd "DC2FLPatch a"
build.bat
```

For details on finding memory offsets and hooking functions, refer to [FINDING_ADDRESSES.md](DC2FLPatch%20a/FINDING_ADDRESSES.md).
