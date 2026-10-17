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
│   ├── choice_layout.h   # Normalisasi dan render proporsional pilihan ○
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

Versi release tetap **v1.0.0**. Untuk memasang perbaikan pilihan pada instalasi patch yang sudah ada, tutup game lalu ganti `DC2FLPatch.dll` dengan versi terbaru. INI dan MES terjemahan yang sudah terpasang dapat tetap digunakan; impor ulang JSON → MES tidak diperlukan.

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
- **Pilihan `○`**:
  Lingkaran mengikuti tepi tombol dengan inset 16 piksel. Teks memakai huruf proporsional, jarak kata normal, dan ukuran font 28 hingga 18 piksel sesuai ruang yang tersedia. Pada tombol 480 piksel, lingkaran dan teks dibatasi 448 piksel. Popup konfirmasi dan ukuran kotak dialog tidak diubah.
- **File Redirection**:
  `EnableFileRedirection=1` mengarahkan pembacaan file MES ke folder `id_Data\MES` atau `eng_data\MES`.
- **Locale Emulator**:
  Terintegrasi langsung di launcher untuk Windows native serta kompatibel dengan Wine / Proton pada Linux / Steam Deck.

## Verifikasi Layout Pilihan

- 54 pilihan dari 386 JSON terjemahan diuji bersama 54 label dalam MES terpasang: **108 uji render dan 9 kasus pengecualian lulus**.
- Lebar bitmap maksimum 445/448 piksel; ukuran font yang digunakan fixture adalah 22–28 piksel.
- Log game mengonfirmasi dua menu contoh dengan 5 label. Seluruh 54 pilihan belum diuji secara visual satu per satu; pengecekan hover belum lengkap.

`src/choice_layout.h` menangani normalisasi CP932, spasi, dan rasterisasi. `src/dc2flpatch.cpp` mencocokkan label dengan kotak tombol asli. Batas clip label bawaan bergantung pada panjang teks, sehingga clip diperluas hanya selama menggambar pilihan lalu dipulihkan. Pengaturan popup dan dialog tidak berubah.

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
