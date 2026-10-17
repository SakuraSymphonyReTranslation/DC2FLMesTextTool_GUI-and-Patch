# DC2FL MES Text Tool — GUI dan CLI

**Bahasa Indonesia** | [English](Readme_en.md)

Importer/exporter naskah skenario dan patch runtime untuk **D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ** (Da Capo II Fall in Love).

Distribusi ini hanya mendukung **Da Capo 2 Fall in Love** (`dc2fl`, versi MES `0x9C69`). GUI, CLI, tabel referensi C++, patch, dan launcher dikhususkan untuk game ini. Source utama patch berada di `DC2FLPatch a/src/dc2flpatch.cpp`.

## Unduhan dan folder instalasi

Unduh kedua paket terbaru dari [GitHub Releases](https://github.com/SakuraSymphonyReTranslation/DC2FLMesTextTool_GUI-and-Patch/releases/latest):

- **DC2FLMesTextTool_GUI.rar**: Importer/exporter MES ↔ JSON (GUI dan CLI), termasuk dokumentasi bahasa Indonesia dan Inggris.
- **Released_patch.rar**: Launcher kustom berikon IconGroup105, DLL patch, konfigurasi INI, reference executable, dan library Locale Emulator.

Kedua paket tersedia pada release **v1.0.0**. Paket terbaru menyertakan perbaikan layout pilihan berawalan `○` dan dokumentasi yang diperbarui.

`<GAME_DIR>` berarti **folder utama instalasi game pilihan pemain**, yaitu folder yang berisi `DC2FL.EXE` dan `AdvData`. Ini placeholder dokumentasi: ganti dengan lokasi instalasi game Anda di komputer (misalnya `C:\Games\DC2FL` atau folder pilihan lainnya), tanpa tanda `<` dan `>`. Tidak ada drive atau lokasi instalasi wajib.

### Langkah Instalasi Patch

1. Tutup game dan buat cadangan file asli yang akan diganti, terutama executable dan file INI jika sebelumnya sudah ada.
2. Ekstrak isi `Released_patch.rar` langsung ke `<GAME_DIR>`, sehingga `DC2FLLauncher.exe`, `DC2FLPatch.dll`, dan `DC2FLPatch.ini` berada satu level di sebelah `DC2FL.EXE`.
3. Jalankan **`<GAME_DIR>\DC2FLLauncher.exe`** dengan working directory `<GAME_DIR>`. Jika Anda membuat shortcut di desktop, pastikan kolom **Start in** / **Working directory** mengarah ke `<GAME_DIR>`. Launcher akan menginisialisasi locale Jepang (CP932) via Locale Emulator dan menginjeksi `DC2FLPatch.dll` ke dalam game.
4. Letakkan file skenario MES terjemahan bahasa Indonesia di dalam `<GAME_DIR>\id_Data\MES`. Jika menggunakan terjemahan bahasa Inggris, letakkan di `<GAME_DIR>\eng_data\MES`. Pastikan opsi `Language` pada `DC2FLPatch.ini` sesuai (`1` untuk Indonesia, `2` untuk Inggris).

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

Kedua arsip RAR menyediakan tool dan runtime patch; keduanya tidak berisi seluruh naskah terjemahan game. Repositori source dapat ditempatkan di folder mana saja. Paket patch siap pakai juga tersedia di dalam repositori pada folder `DC2FLPatch a/build/Released`.

## Buka aplikasi

Ekstrak `DC2FLMesTextTool_GUI.rar` ke folder pilihan Anda dan jalankan `DC2FLMesTextTool_GUI.exe`. Jika memakai source repositori, jalankan `bin\Release\net8.0-windows\DC2FLMesTextTool_GUI.exe`.
Paket siap pakai berada di folder yang sama: `DC2FLMesTextTool_GUI.rar`.
Aplikasi membutuhkan [.NET 8 Desktop Runtime](https://dotnet.microsoft.com/download/dotnet/8.0).

## Single File

### MES → JSON (Exporter)

1. Isi **Input MES/JSON** dengan file `.mes` asli.
2. Isi **Output JSON/MES**, atau kosongkan untuk otomatis membuat file `.json` di sebelah file input.
3. Klik **Export (MES -> JSON)**.

Hasil default menggabungkan nama karakter dan dialog berdasarkan struktur opcode skrip DC2FL:

```json
[
  { "message": "Narasi cerita." },
  { "name": "Yoshiyuki", "message": "「Dialog karakter.」" }
]
```

Ekspor dari tab Single File dan Batch selalu menggunakan format gabungan nama/dialog di atas. Teks, nama Jepang/Indonesia, dan baris baru dipertahankan secara akurat saat ekspor.

### JSON → MES (Importer)

1. Isi **Input MES/JSON** dengan file `.json` terjemahan.
2. Pastikan file template `.mes` asli dengan nama yang sama berada di folder yang sama (atau tentukan file MES asli sebagai basis).
3. Tentukan file **Output JSON/MES** yang berbeda dari file MES asli.
4. Atur **Word Wrap Width**: default **70** pada Single File, Batch, dan CLI. Jika teks tampak terlalu panjang atau sempit pada dialog box di game, nilai ini dapat disesuaikan.
5. Klik **Import (JSON -> MES)**.

Importer menerima JSON gabungan, format legacy, serta campuran keduanya. Jumlah token teks dan struktur label dicocokkan dengan MES asli untuk menjamin integritas alur permainan.

## Batch

- **Batch Export:** Isi **MES Folder** dan **Output Folder**, lalu klik **Batch Export (MES -> JSON)**. Semua file `.mes` di folder tersebut akan diekspor menjadi `.json`.
- **Batch Import:** Isi **JSON Folder**, **Original MES Folder** (template asli), dan **Output Folder** (tempat file `.mes` baru dihasilkan), lalu klik **Batch Import (JSON -> MES)**.
- Pencocokan nama file otomatis: misalnya `scene01.json` akan menggunakan template `scene01.mes`.
- File yang gagal diproses akan dicatat di panel log, dan proses batch tetap berlanjut ke file berikutnya.

## Pengaturan Layout & Patch Runtime

Rendering teks di layar diatur secara otomatis saat game berjalan oleh `DC2FLPatch.dll`. Pengaturan konfigurasi tersedia di `DC2FLPatch.ini`:

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

- **Font & Render Scaling**: `DialogueRenderScale=74` dan `DialogueAdvanceScale=62` mengatur proporsi lebar font Latin agar proporsional dan tidak saling menumpuk.
- **Redirection**: `EnableFileRedirection=1` mengalihkan pembacaan naskah ke folder terjemahan `id_Data\MES` atau `eng_data\MES` tanpa menimpa arsip asli game.
- **Backlog Icons**: `ShowBacklogIcon=1` mempertahankan dan menyesuaikan rendering ikon pembicara di backlog game.

### Layout Pilihan `○`

- Pilihan terjemahan berawalan `○` menggunakan huruf proporsional dengan jarak kata normal. Spasi berlebih dan pemisah `~` dari impor lama dirapikan saat dirender.
- Posisi lingkaran mengikuti batas tombol asli, dengan inset kiri **16 piksel**. Awal teks sejajar antar pilihan; ukuran font menyesuaikan jika kalimat terlalu panjang.
- Pada tombol selebar **480 piksel**, tersedia **448 piksel** untuk lingkaran dan teks. Latar tombol, area klik, warna, dan bayangan tetap mengikuti game.
- Perbaikan berlaku melalui `DC2FLPatch.dll`. Jika MES terjemahan sudah terpasang, cukup perbarui DLL saat game ditutup; **tidak perlu memperbarui importer/exporter atau mengimpor ulang JSON → MES**.
- Tanda `○` dapat langsung diikuti teks atau satu spasi, misalnya `○Pilihan pertama` dan `○ Pilihan kedua`. Tidak perlu menambahkan spasi untuk mengatur posisi. Pengaturan ukuran popup konfirmasi dan kotak dialog tetap sama.

Petunjuk teknis patch dan build C++ tersedia di [README Patch](DC2FLPatch%20a/README.md).

## Validasi dan Encoding

- Format teks skrip Circus dikodekan dalam **CP932 (Shift-JIS)**.
- Karakter Latin di luar tabel dasar Shift-JIS dipetakan ke padanan yang aman (misalnya tanda hubung panjang `— → ―`, huruf beraksen `û → u`, `ë → e`, `é → e`, `à → a`). Karakter yang tidak valid akan ditolak secara aman dengan pesan error yang jelas pada log.
- Penulisan berkas baru selalu dilakukan melalui file temporer dan divalidasi sebelum disimpan, untuk mencegah kerusakan file jika proses terhenti.

## CLI

Dari folder executable (`bin\Release\net8.0-windows` atau folder hasil ekstrak RAR):

```powershell
.\DC2FLMesTextTool_GUI.exe export "scene.mes" "scene.json"
.\DC2FLMesTextTool_GUI.exe import "scene.json" "original\scene.mes" "new\scene.mes" -w 70
.\DC2FLMesTextTool_GUI.exe batch-export "original_mes" "json_output"
.\DC2FLMesTextTool_GUI.exe batch-import "json_folder" "original_mes" "mes_output" -w 70
```

- Parameter `-w <lebar>`: Mengatur batas wrap kata (default: `70`).
- Mendukung opsi drag-and-drop: Tarik file `.mes` atau folder ke file `.exe` untuk mengekspor otomatis.

## Hasil Verifikasi

- Format ekspor dan impor naskah telah diverifikasi terhadap skenario asli DC2FL.
- Audit 386 JSON terjemahan menemukan **54 entri pilihan** dalam 23 file; jumlah pilihan per file cocok dengan MES terpasang.
- **108 uji render** (54 JSON + 54 MES) dan 9 kasus pengecualian lulus. Semua teks pilihan muat dalam 448 piksel, dengan lebar bitmap maksimum 445 piksel.
- Log runtime mengonfirmasi posisi baru pada dua menu contoh (5 label). Cakupan 54 pilihan berasal dari uji render; pengujian visual setiap pilihan dan keadaan hover belum lengkap.
- Executable referensi game yang diuji memiliki hash SHA-256:

```text
aacb2192b8ab89d77ebc08addf10d993a1bd2401fc11e93389c2f917a0d079eb  DC2FL.EXE
```

## Build dari Source

Dari root folder repositori:

```powershell
# Build C# GUI & CLI (.NET 8 SDK diperlukan)
dotnet build -c Release
```

Untuk build patch C++ (Visual Studio 2022 x86 Native Tools):

```powershell
cd "DC2FLPatch a"
build.bat
```

Panduan lengkap mengenai pencarian alamat fungsi / offset memori dapat dibaca di [FINDING_ADDRESSES.md](DC2FLPatch%20a/FINDING_ADDRESSES.md).
