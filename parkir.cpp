#include <iostream>
#include <string>
#include <cctype>  // Untuk validasi digit (isdigit)

using namespace std;

// --- DEKLARASI STRUCT ---
// Struct untuk menyimpan seluruh informasi tiket parkir
struct DataParkir {
    string jenis;   // "Mobil" atau "Motor"
    string masuk;   // Format "HH:MM"
    string keluar;  // Format "HH:MM"
};

// --- FUNGSI VALIDASI FORMAT WAKTU ---
// Memastikan string waktu mematuhi format "HH:MM" dan logika jam (00-23) serta menit (00-59).
bool cek_format_waktu(string waktu) {
    // 1. Validasi Panjang & Separator: Harus 5 karakter dan karakter ke-3 adalah ':'
    if (waktu.length() != 5) return false;
    if (waktu[2] != ':') return false;

    // 2. Validasi Digit: Memastikan jam dan menit hanya berisi angka
    if (!isdigit(waktu[0]) || !isdigit(waktu[1]) || 
        !isdigit(waktu[3]) || !isdigit(waktu[4])) {
        return false;
    }

    // 3. Validasi Logika Waktu: Konversi ke int untuk cek apakah waktu yang diberikan masuk akal 
    // penggunaan stoi() dan substr(): untuk string waktu = "12:59"
    // waktu.substr(0, 2) -> Ambil 2 karakter mulai index 0 ("12")
    // stoi(...)          -> Ubah string "12" menjadi angka 12
    int jam = stoi(waktu.substr(0, 2));
    int menit = stoi(waktu.substr(3, 2));
    if (jam < 0 || jam > 23) return false;   // Jam hanya bisa 0-23
    if (menit < 0 || menit > 59) return false; // Menit hanya bisa 0-59

    return true;
}

// --- FUNGSI INPUT ---
// Mengambil input user dengan mekanisme "Do-While" agar user berada dalam loop sampai memasukkan data yang valid.
DataParkir ambil_input() {
    DataParkir d;
    bool valid = false; // tidak valid by default

    cout << "=== PROGRAM PARKIR ===" << endl;

    // Loop input Jenis Kendaraan (Case Sensitive: Mobil/Motor)
    do {
        cout << "Input Jenis (Mobil/Motor) : ";
        cin >> d.jenis;
        if (d.jenis == "Mobil" || d.jenis == "Motor") {
            valid = true;
        } else {
            cout << "  [!] Salah! Masukkan 'Mobil' atau 'Motor' (Perhatikan huruf besar)." << endl;
            valid = false;
        }
    } while (!valid);

    // Loop input Waktu Masuk (Format HH:MM)
    do {
        cout << "Waktu Masuk (HH:MM)       : ";
        cin >> d.masuk;
        if (cek_format_waktu(d.masuk)) {
            valid = true;
        } else {
            cout << "  [!] Format salah/Waktu tidak masuk akal! Gunakan format HH:MM (00:00 - 23:59)." << endl;
            valid = false;
        }
    } while (!valid);

    // Loop input Waktu Keluar (Format HH:MM)
    do {
        cout << "Waktu Keluar (HH:MM)      : ";
        cin >> d.keluar;
        if (cek_format_waktu(d.keluar)) {
            valid = true;
        } else {
            cout << "  [!] Format salah/Waktu tidak masuk akal! Gunakan format HH:MM (00:00 - 23:59)." << endl;
            valid = false;
        }
    } while (!valid);

    return d; // Mengembalikan struct yang sudah terisi data valid
}

// --- FUNGSI KALKULASI DURASI ---
// Mengonversi string waktu ke total menit dan menghitung selisih durasi.
// Menangani kasus lintas hari (misal masuk 23:00, keluar 01:00).
int hitung_lama_parkir(string masuk, string keluar) {
    // Parsing string ke integer
    int jam_masuk   = stoi(masuk.substr(0, 2));
    int menit_masuk = stoi(masuk.substr(3, 2));
    int jam_keluar  = stoi(keluar.substr(0, 2));
    int menit_keluar = stoi(keluar.substr(3, 2));

    // Konversi ke satuan terkecil (menit) dari 00:00
    int total_masuk = (jam_masuk * 60) + menit_masuk;
    int total_keluar = (jam_keluar * 60) + menit_keluar;

    int durasi = total_keluar - total_masuk;

    // Jika durasi negatif, berarti melewati tengah malam (+24 jam atau 1440 menit)
    if (durasi < 0) durasi += 1440; 

    return durasi;
}

// --- FUNGSI KALKULASI BIAYA ---
// Menghitung biaya berdasarkan jenis kendaraan, durasi, dan aturan pembulatan jam.
// Menerapkan batas maksimal tarif.
int hitung_biaya(string jenis, int lama_menit) {
    int biaya = 0; // anu
    int tarif_awal, tarif_berikutnya, tarif_maksimal;

    // Setup tarif dasar
    if (jenis == "Mobil") {
        tarif_awal = 4000; tarif_berikutnya = 2000; tarif_maksimal = 15000;
    } else { 
        tarif_awal = 2000; tarif_berikutnya = 1000; tarif_maksimal = 5000;
    } 

    // Logika perhitungan
    if (lama_menit <= 60) {
        biaya = tarif_awal;
    } else {
        // Pembulatan ke atas: 61 menit dianggap 2 jam (bener gk ya logikanya?)
        int jam_tagihan = lama_menit / 60;
        if (lama_menit % 60 > 0) jam_tagihan++;
        
        // Rumus: Tarif Jam 1 + (Sisa Jam * Tarif Berikutnya)
        biaya = tarif_awal + ((jam_tagihan - 1) * tarif_berikutnya);
    }
    
    // Capping biaya jika melebihi batas maksimal
    if (biaya > tarif_maksimal) biaya = tarif_maksimal;
    
    return biaya;
}

// --- FUNGSI OUTPUT ---
// Menampilkan nota rapi menggunakan setw untuk alignment tabel.
void print_nota(DataParkir d, int lama_menit, int biaya) {
    // Konversi menit kembali ke Jam & Menit untuk display
    int jam_display = lama_menit / 60;
    int menit_display = lama_menit % 60;

    cout << endl;
    cout << "---------------------------------------" << endl;
    cout << "           NOTA PEMBAYARAN             " << endl;
    cout << "---------------------------------------" << endl;
    cout << left << "Jenis Kendaraan  : " << d.jenis << endl;
    cout << left << "Lama Parkir      : " << jam_display << " Jam " << menit_display << " Menit" << endl;
    cout << "---------------------------------------" << endl;
    cout << left << "Total Biaya      : Rp" << biaya  << endl;
    cout << "---------------------------------------" << endl;
}

// --- MAIN PROGRAM ---
// Mengontrol alur utama: Input -> Proses -> Output
int main() {
    // 1. Ambil data (sudah tervalidasi)
    DataParkir tiket = ambil_input();
    
    // 2. Proses perhitungan logika
    int durasi = hitung_lama_parkir(tiket.masuk, tiket.keluar);
    int total_biaya = hitung_biaya(tiket.jenis, durasi);
    
    // 3. Tampilkan hasil akhir
    print_nota(tiket, durasi, total_biaya);
    
    return 0;
}