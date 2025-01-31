#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// ==============================================
// STRUCT DEFINITIONS
// ==============================================
// Struct untuk menyimpan data KTP (Nomor Induk Kependudukan)
struct KTP {
    string nik;    // Nomor Induk Kependudukan
    string nama;   // Nama lengkap pemilik KTP
    string alamat; // Alamat pemilik KTP
};

// Struct untuk menyimpan data item barang
struct Item {
    string kode;     // Kode unik untuk item barang
    string tanggal;  // Tanggal masuknya item ke gudang (format: YYYY-MM-DD)
};

// Struct untuk menyimpan data pembeli dalam antrian
struct Pembeli {
    string nik;               // NIK pembeli (harus sesuai dengan data KTP)
    string waktu_kedatangan;  // Waktu kedatangan pembeli (format: HH:MM)
    string kode_item;         // Kode item yang dibeli oleh pembeli
};

// ==============================================
// MENU FUNCTION
// ==============================================
// Fungsi untuk menampilkan menu utama program
void menu() {
    cout << "\nMenu Utama:\n";
    cout << "1. Admin Sign In\n";                   // Opsi untuk masuk sebagai admin
    cout << "2. Entri Item Barang\n";              // Opsi untuk menambahkan item barang ke gudang
    cout << "3. Catat Pembeli\n";                  // Opsi untuk mencatat pembeli dalam antrian
    cout << "4. Jual Item Barang (LIFO atau FIFO)\n"; // Opsi untuk menjual item barang
    cout << "5. Cetak Daftar Barang\n";            // Opsi untuk mencetak daftar barang di gudang
    cout << "6. Cetak Data KTP dan Pembeli\n";     // Opsi untuk mencetak data KTP dan pembeli
    cout << "0. Keluar\n";                         // Opsi untuk keluar dari program
    cout << "Pilihan Anda: ";
}

// ==============================================
// KTP FUNCTIONS
// ==============================================
// Fungsi untuk entri data KTP dan menyimpannya ke file
void entriKTP() {
    ofstream file("data_ktp.txt", ios::app); // Membuka file untuk menambahkan data KTP
    if (!file) {
        cerr << "Gagal membuka file untuk menulis data KTP!" << endl;
        return;
    }

    KTP data;
    cout << "Masukkan NIK: "; cin >> data.nik;
    cout << "Masukkan Nama: "; cin.ignore(); getline(cin, data.nama);
    cout << "Masukkan Alamat: "; getline(cin, data.alamat);

    file << data.nik << "," << data.nama << "," << data.alamat << endl; // Menulis data ke file
    file.close();
    cout << "Data KTP berhasil disimpan!\n";
}

// Fungsi untuk membaca dan mencetak semua data KTP dan pembeli dari file
void cetakKTP(const queue<Pembeli>& antrian) {
    ifstream file("data_ktp.txt"); // Membuka file untuk membaca data KTP
    if (!file) {
        cerr << "Gagal membuka file untuk membaca data KTP!" << endl;
        return;
    }

    cout << "\nDaftar Data KTP:\n";
    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(","); // Mencari posisi koma pertama
        size_t pos2 = line.find_last_of(","); // Mencari posisi koma terakhir
        string nik = line.substr(0, pos1); // Ekstrak NIK
        string nama = line.substr(pos1 + 1, pos2 - pos1 - 1); // Ekstrak Nama
        string alamat = line.substr(pos2 + 1); // Ekstrak Alamat

        cout << "\nNIK    : " << nik << endl;
        cout << "Nama   : " << nama << endl;
        cout << "Alamat : " << alamat << endl;
    }

    file.close();

    // Mencetak data pembeli jika antrian tidak kosong
    if (!antrian.empty()) {
        cout << "\nDaftar Pembeli:\n";
        queue<Pembeli> temp = antrian; // Salin antrian untuk iterasi
        while (!temp.empty()) {
            Pembeli pembeli = temp.front();
            temp.pop();

            cout << "\nNIK            : " << pembeli.nik << endl;
            cout << "Kode Barang    : " << pembeli.kode_item << endl;
            cout << "Waktu Kedatangan : " << pembeli.waktu_kedatangan << endl;
        }
    }
}

// ==============================================
// ITEM FUNCTIONS
// ==============================================
// Fungsi untuk menambahkan item barang ke gudang (menggunakan stack untuk LIFO)
void entriItem(stack<Item>& gudang) {
    Item item;
    cout << "Masukkan Kode Item: "; cin >> item.kode;
    cout << "Masukkan Tanggal (YYYY-MM-DD): "; cin >> item.tanggal;
    
    gudang.push(item); // Menambahkan item ke stack gudang
    cout << "Item berhasil ditambahkan ke gudang!\n";
}

// Fungsi untuk mencetak daftar barang di gudang
void cetakDaftarBarang(stack<Item> gudang) {
    if (gudang.empty()) {
        cout << "Gudang kosong, tidak ada barang yang tercatat!\n";
        return;
    }

    cout << "\nDaftar Barang di Gudang:\n";
    cout << left << setw(10) << "Kode" << setw(15) << "Tanggal" << endl;
    while (!gudang.empty()) {
        Item item = gudang.top(); // Mengambil item teratas di stack
        cout << left << setw(10) << item.kode << setw(15) << item.tanggal << endl;
        gudang.pop(); // Menghapus item teratas dari stack
    }
}

// ==============================================
// PEMBELI FUNCTIONS
// ==============================================
// Fungsi untuk mencatat pembeli dalam antrian (menggunakan queue untuk FIFO)
void catatPembeli(queue<Pembeli>& antrian) {
    Pembeli pembeli;
    cout << "Masukkan NIK Pembeli: "; cin >> pembeli.nik;
    cout << "Masukkan Waktu Kedatangan (HH:MM): "; cin >> pembeli.waktu_kedatangan;
    cout << "Masukkan Kode Item yang Dibeli: "; cin >> pembeli.kode_item;
    
    antrian.push(pembeli); // Menambahkan pembeli ke antrian
    cout << "Pembeli berhasil dicatat dalam antrian!\n";
}

// Fungsi untuk menjual item barang (LIFO atau FIFO)
void jualItem(stack<Item>& gudang, queue<Pembeli>& antrian) {
    if (gudang.empty()) {
        cout << "Gudang kosong, tidak ada barang yang dapat dijual!\n";
        return;
    }

    if (antrian.empty()) {
        cout << "Tidak ada pembeli dalam antrian!\n";
        return;
    }

    Pembeli pembeli = antrian.front(); // Mengambil pembeli pertama dalam antrian
    Item item = gudang.top(); // Mengambil item teratas di gudang

    // Memeriksa apakah kode barang yang diminta pembeli sesuai dengan item teratas di gudang
    if (pembeli.kode_item == item.kode) {
        cout << "Barang dengan kode " << item.kode << " dijual ke pembeli dengan NIK " << pembeli.nik << "\n";
        gudang.pop(); // Menghapus item dari gudang
        antrian.pop(); // Menghapus pembeli dari antrian
    } else {
        cout << "Kode barang tidak sesuai dengan permintaan pembeli!\n";
    }
}

// ==============================================
// MAIN FUNCTION
// ==============================================
// Fungsi utama program yang mengatur alur eksekusi
int main() {
    stack<Item> gudang; // Stack untuk menyimpan item barang (LIFO)
    queue<Pembeli> antrian; // Queue untuk menyimpan antrian pembeli (FIFO)

    int pilihan;
    do {
        menu(); // Menampilkan menu utama
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                entriKTP(); // Memanggil fungsi entri KTP
                break;
            case 2:
                entriItem(gudang); // Memanggil fungsi entri item barang
                break;
            case 3:
                catatPembeli(antrian); // Memanggil fungsi catat pembeli
                break;
            case 4:
                jualItem(gudang, antrian); // Memanggil fungsi jual item barang
                break;
            case 5:
                cetakDaftarBarang(gudang); // Memanggil fungsi cetak daftar barang
                break;
            case 6:
                cetakKTP(antrian); // Memanggil fungsi cetak data KTP dan pembeli
                break;
            case 0:
                cout << "Keluar dari program. Terima kasih!\n"; // Keluar dari program
                break;
            default:
                cout << "Pilihan tidak valid, coba lagi.\n"; // Pesan kesalahan untuk pilihan tidak valid
        }
    } while (pilihan != 0);

    return 0;
}
