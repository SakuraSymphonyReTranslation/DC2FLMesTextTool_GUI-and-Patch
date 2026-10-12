# DC2FL Patch — Instalasi dan Build

**Bahasa Indonesia** | [English](Readme_en.md)

Patch runtime dan custom launcher ini dikhususkan untuk **D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ** (Da Capo II Fall in Love). Source utama implementasi patch: `src/dc2flpatch.cpp` dan launcher: `src/launcher.cpp`.

## Struktur Direktori

```text
DC2FLPatch a/
├── CMakeLists.txt        # Konfigurasi CMake build
├── build.bat             # Script build otomatis (NMake / MSVC)
├── icon.ico              # Ikon game DC2FL (IconGroup 105)
├── src/
│   ├── dc2flpatch.cpp    # Kode utama hook DLL patch (injected version)
│   ├── launcher.cpp      # Kode peluncur DC2FLLauncher (injeksi DLL + Locale Emulator)
│   ├── launcher.rc       # Resource peluncur (ikon & metadata)
│   ├── d3dx9_proxy.cpp   # Proxy D3DX9 opsional
│   └── dsound_proxy.cpp  # Proxy DSound opsional
├── Detours/              # Microsoft Detours library
├── Locale-Emulator-2.5.0.1/ # Library Locale Emulator
└── build/
    └── Released/         # Paket siap pakai (DC2FLLauncher.exe, DC2FLPatch.dll, dll)
```

## Instalasi

Unduh paket `Released_patch.rar` dari [GitHub Releases](https://github.com/SakuraSymphonyReTranslation/DC2FLMesTextTool_GUI-and-Patch/releases/latest), atau gunakan berkas di `build/Released`.

`<GAME_DIR>` berarti **folder utama instalasi game pilihan pemain**, yaitu folder yang memuat `DC2FL.EXE` dan `AdvData`. Ganti placeholder ini dengan lokasi game di komputer Anda; tidak ada drive yang diwajibkan.

1. Tutup game dan buat cadangan file lama yang akan diganti.
2. Ekstrak isi `Released_patch.rar` langsung ke `<GAME_DIR>`, sehingga file `DC2FLLauncher.exe`, `DC2FLPatch.dll`, dan `DC2FLPatch.ini` berada di sebelah `DC2FL.EXE`.
3. Jalankan **`<GAME_DIR>\DC2FLLauncher.exe`** dengan working directory `<GAME_DIR>`. Jika menggunakan shortcut, setel **Start in** ke `<GAME_DIR>`. Launcher akan memuat DLL patch dan mengaktifkan locale Jepang secara otomatis.
4. Letakkan naskah terjemahan Indonesia di `<GAME_DIR>\id_Data\MES` atau bahasa Inggris di `<GAME_DIR>\eng_data\MES`.

Executable referensi yang diuji memiliki SHA256:

```text
aacb2192b8ab89d77ebc08addf10d993a1bd2401fc11e93389c2f917a0d079eb  DC2FL.EXE
```

## Fitur Patch & Konfigurasi

- **Font Rendering & Advanced Scaling**:
  Diatur melalui `DC2FLPatch.ini`:
  - `DialogueRenderScale=74`
  - `DialogueAdvanceScale=62`
  - `DialogueSpaceScale=24`
  Menjaga keterbacaan teks alfabet Latin/Indonesia pada dialog tanpa tumpang-tindih.
- **Backlog & Popups**:
  `ShowBacklogIcon=1` memelihara ikon pembicara di backlog, serta memusatkan tampilan dialog konfirmasi sistem.
- **File Redirection**:
  `EnableFileRedirection=1` mengarahkan pembacaan file MES ke folder `id_Data\MES` atau `eng_data\MES`.
- **Locale Emulator**:
  Terintegrasi langsung di launcher untuk Windows native serta kompatibel dengan Wine / Proton pada Linux / Steam Deck.

## Cara Build (Visual Studio 2022, x86 Native Tools)

Dari folder `DC2FLPatch a`:

```powershell
.\build.bat
```

Atau manual menggunakan CMake:

```powershell
cmake -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Output kompilasi:
- `build\DC2FLLauncher.exe`
- `build\DC2FLPatch.dll`
