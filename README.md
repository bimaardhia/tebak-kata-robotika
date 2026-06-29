# 🤖 Tebak Kata - Robotika Edition

![C](https://img.shields.io/badge/language-C-blue.svg)
![Status](https://img.shields.io/badge/status-completed-brightgreen.svg)
![Course](https://img.shields.io/badge/course-Pemrograman_Dasar-orange.svg)

**Proyek Akhir Praktikum Dasar Pemrograman**

Aplikasi Command Line Interface <CLI> interaktif untuk permainan Hangman dengan tema terminologi Robotika dan Kecerdasan Buatan. Proyek ini dibangun dengan menerapkan prinsip _Software Engineering_ yang mencakup manajemen memori, _pointer arithmetic_, _binary search_, dan _insertion sort_.

---

## 🛠️ Cara Kompilasi dan Menjalankan

Program ini ditulis dalam bahasa C murni dan dapat dikompilasi menggunakan `gcc`.

**1. Kompilasi Program**
Buka terminal dan jalankan perintah berikut:

```bash
gcc -Wall -o tebak_kata tebak_kata.c
```

_Catatan: Flag `-Wall` digunakan untuk memastikan tidak ada warning pada saat kompilasi._

**2. Menjalankan Program**

- **Linux / macOS:**
  ```bash
  ./tebak_kata
  ```
- **Windows:**
  ```cmd
  tebak_kata.exe
  ```

---

## ✨ Daftar Fitur

1.  **Main Game Baru:** Permainan tebak kata klasik. Pemain menebak huruf demi huruf dari kata rahasia yang dipilih secara acak.
2.  **Sistem Skor Dinamis:** Perhitungan skor memperhitungkan sisa nyawa dan panjang kata rahasia <Rumus: Sisa Nyawa x 100 + Panjang Kata x 20>.
3.  **Papan Peringkat <Hall of Fame>:** Menyimpan 10 pemain terbaik menggunakan algoritma _Bubble Sort_ secara menurun _<descending>_. Terintegrasi dengan `<time.h>` untuk pencatatan tanggal otomatis.
4.  **Tambah Kata ke Bank:** Pemain dapat menambah perbendaharaan kata. Program otomatis mengecek duplikasi menggunakan algoritma _Binary Search_ O<log n>, lalu menyisipkannya sesuai urutan alfabet menggunakan _Insertion Sort_.
5.  **Robust Input Handling:** Memiliki sistem _input buffer clearing_ untuk mencegah _infinite loop_ saat pengguna memasukkan karakter spasi atau input berlebih.

---

## ⚠️ Keterbatasan yang Diketahui <Known Limitations>

- **Penyimpanan Volatil:** Data bank kata baru dan Hall of Fame hanya tersimpan di dalam memori <RAM> selama program berjalan. Data akan _reset_ jika program ditutup karena belum menggunakan operasi _File I/O_.
- **Kapasitas Tetap:** Bank kata dibatasi maksimal 50 entri `MAX_WORDS` dan Hall of Fame dibatasi 10 entri `HOF_SIZE`.
- **Karakter Alfabet Saja:** Permainan saat ini belum menangani input angka atau simbol secara spesifik dalam kata rahasia, meskipun input dikonversi otomatis ke huruf kapital.

---

## 📊 Flowchart Sistem Utama

Berikut adalah alur logika program yang digambarkan menggunakan Mermaid diagram. Sesuai standar, tanda kurung siku siku atau angle brackets digunakan untuk parameter penjelas.

### A. Alur Menu Utama

```mermaid
flowchart TD
    Start[Mulai Program] --> Init[Inisialisasi Random Seed]
    Init --> Show[Tampilkan Menu Utama]
    Show --> Input[Baca Pilihan Input]
    Input --> Switch{Evaluasi Pilihan}

    Switch -->|1| Play[Jalankan play_game]
    Switch -->|2| HOF[Jalankan show_hall_of_fame]
    Switch -->|3| Add[Jalankan add_word]
    Switch -->|Lainnya| Invalid[Cetak Pesan Pilihan Tidak Valid]
    Switch -->|0| Exit[Cetak Pesan Terima Kasih]

    Play --> CheckLoop
    HOF --> CheckLoop
    Add --> CheckLoop
    Invalid --> CheckLoop

    CheckLoop{Pilihan != 0 ?}
    CheckLoop -->|Ya| Show
    CheckLoop -->|Tidak| Selesai[Program Selesai Keluar]
    Exit --> Selesai
```

### B. Alur Sesi Permainan Keseluruhan <play_game>

```mermaid
flowchart TD
    Start[Mulai play_game] --> InputName[Input Nama Pemain]
    InputName --> Init[Inisialisasi Variabel Game & Pilih Kata Acak]
    Init --> LoopCond{wrong < 6 DAN Belum Menang?}

    LoopCond -->|Ya| Draw[Gambar Hangman & Cetak Info]
    Draw --> InputGuess[Input Tebakan Huruf]
    InputGuess --> Process[Jalankan process_guess]
    Process --> LoopCond

    LoopCond -->|Tidak| EndDraw[Gambar Hangman Terakhir]
    EndDraw --> CheckWin{Menang?}

    CheckWin -->|Ya| CalcScore[Hitung Skor]
    CalcScore --> AskHOF{Simpan ke HOF?}
    AskHOF -->|Ya| Save[Jalankan save_score] --> Selesai[Selesai play_game]
    AskHOF -->|Tidak| Selesai

    CheckWin -->|Tidak| ShowWord[Tampilkan Kata yang Benar] --> Selesai
```

### C. Proses Satu Tebakan <process_guess>

```mermaid
flowchart TD
    Start[Mulai process_guess] --> Search[Cari Huruf di Array Guessed <Linear Search>]
    Search --> CheckExist{Huruf Sudah Ada?}

    CheckExist -->|Ya| Ret1[Kembalikan Status 1 <Sudah Ditebak>]
    CheckExist -->|Tidak| AddGuessed[Catat Huruf ke Array Guessed]

    AddGuessed --> Traversal[Telusuri Kata Rahasia Menggunakan Pointer]
    Traversal --> CheckChar{Huruf Cocok?}

    CheckChar -->|Ya| Buka[Buka Huruf di Display Mask] --> LanjutTelusur[Lanjut Karakter Berikutnya]
    CheckChar -->|Tidak| LanjutTelusur
    LanjutTelusur --> CheckEnd{Akhir String?}
    CheckEnd -->|Belum| CheckChar
    CheckEnd -->|Sudah| EvalMatch{Ada Huruf yang Cocok?}

    EvalMatch -->|Ya| Ret2[Kembalikan Status 2 <Benar>]
    EvalMatch -->|Tidak| IncWrong[Tambah Jumlah Salah <wrong++>]
    IncWrong --> Ret0[Kembalikan Status 0 <Salah>]

    Ret1 --> Selesai[Selesai process_guess]
    Ret2 --> Selesai
    Ret0 --> Selesai
```

### D. Proses Penambahan Kata <add_word & insert_word_sorted>

```mermaid
flowchart TD
    Start[Mulai add_word] --> CheckLimit{Bank Kata Penuh?}
    CheckLimit -->|Ya| PrintFull[Tampilkan Pesan Penuh] --> Selesai[Selesai add_word]
    CheckLimit -->|Tidak| InputWord[Baca Kata Baru dari Input]

    InputWord --> Upper[Ubah ke Huruf Kapital]
    Upper --> BinSearch[Jalankan binary_search_word]

    BinSearch --> CheckFound{Hasil != -1 ?}
    CheckFound -->|Ya| PrintExist[Tampilkan Pesan Kata Sudah Ada] --> Selesai
    CheckFound -->|Tidak| InsertSort[Jalankan insert_word_sorted]

    InsertSort --> Alloc[Alokasi Memori Dinamis <malloc>]
    Alloc --> LoopShift[Geser Elemen Array Lebih Besar ke Kanan]
    LoopShift --> Assign[Masukkan Alamat Memori Kata ke Array]
    Assign --> IncCount[Naikkan word_count++]

    IncCount --> PrintSukses[Tampilkan Pesan Berhasil Ditambahkan] --> Selesai
```

### E. Alur Menyimpan Skor dan Pengurutan <save_score & bubble_sort_hof>

```mermaid
flowchart TD
    Start[Mulai save_score] --> CheckQualify{Papan Penuh & Skor Lebih Rendah?}
    CheckQualify -->|Ya| Reject[Tampilkan Pesan Tidak Layak Masuk HOF] --> Selesai[Selesai save_score]

    CheckQualify -->|Tidak| GetTime[Ambil Waktu Sistem <localtime>]
    GetTime --> InsertData[Tulis Nama, Skor, Tanggal ke Array HOF Terbawah]
    InsertData --> CheckCount{hof_count < HOF_SIZE?}
    CheckCount -->|Ya| IncCount[hof_count++] --> Sort[Jalankan bubble_sort_hof]
    CheckCount -->|Tidak| Sort

    Sort --> BubbleOut[Looping Array HOF <Luar>]
    BubbleOut --> BubbleIn[Looping Array HOF <Dalam>]
    BubbleIn --> CheckPoints{Poin Saat Ini < Poin Berikutnya?}
    CheckPoints -->|Ya| Swap[Tukar Posisi Struct Data] --> BubbleInNext[Lanjut Looping Dalam]
    CheckPoints -->|Tidak| BubbleInNext
    BubbleInNext --> BubbleOutNext[Lanjut Looping Luar]
    BubbleOutNext --> PrintSuccess[Tampilkan Pesan Tanggal & Skor Tersimpan] --> Selesai
```

---

_Dokumentasi ini disusun untuk memenuhi standar submisi Proyek Akhir Praktikum Pemrograman Dasar._
