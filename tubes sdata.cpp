#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Struct untuk menyimpan data KTP
struct KTP {
    string nik;
    string nama;
    string alamat;
};

// Struct untuk menyimpan item barang
struct Item {
    string kode;
    string tanggal;
};

// Queue untuk antrian pembeli
struct Pembeli {
    string nik;
    string waktu_kedatangan;
    string kode_item;
};

// Fungsi untuk menampilkan menu utama
void menu() {
    cout << "\nMenu Utama:\n";
    cout << "1. Admin Sign In\n";
    cout << "2. Entri Item Barang\n";
    cout << "3. Catat Pembeli\n";
    cout << "4. Jual Item Barang (LIFO atau FIFO)\n";
    cout << "5. Cetak Daftar Barang\n";
    cout << "6. Cetak Data KTP dan Pembeli\n";
    cout << "0. Keluar\n";
    cout << "Pilihan Anda: ";
}

// Fungsi untuk entri data KTP dan menyimpannya ke file
void entriKTP() {
    ofstream file("data_ktp.txt", ios::app);
    if (!file) {
        cerr << "Gagal membuka file untuk menulis data KTP!" << endl;
        return;
    }

    KTP data;
    cout << "Masukkan NIK: "; cin >> data.nik;
    cout << "Masukkan Nama: "; cin.ignore(); getline(cin, data.nama);
    cout << "Masukkan Alamat: "; getline(cin, data.alamat);

    file << data.nik << "," << data.nama << "," << data.alamat << endl;
    file.close();
    cout << "Data KTP berhasil disimpan!\n";
}

// Fungsi untuk membaca dan mencetak semua data KTP dan pembeli dari file
void cetakKTP(const queue<Pembeli>& antrian) {
    ifstream file("data_ktp.txt");
    if (!file) {
        cerr << "Gagal membuka file untuk membaca data KTP!" << endl;
        return;
    }

    cout << "\nDaftar Data KTP:\n";
    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(",");
        size_t pos2 = line.find_last_of(",");

        string nik = line.substr(0, pos1);
        string nama = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string alamat = line.substr(pos2 + 1);

        cout << "\nNIK    : " << nik << endl;
        cout << "Nama   : " << nama << endl;
        cout << "Alamat : " << alamat << endl;
    }

    file.close();

    if (!antrian.empty()) {
        cout << "\nDaftar Pembeli:\n";
        queue<Pembeli> temp = antrian;
        while (!temp.empty()) {
            Pembeli pembeli = temp.front();
            temp.pop();

            cout << "\nNIK            : " << pembeli.nik << endl;
            cout << "Kode Barang    : " << pembeli.kode_item << endl;
            cout << "Waktu Kedatangan : " << pembeli.waktu_kedatangan << endl;
        }
    }
}

// Fungsi untuk entri item barang (stack untuk LIFO)
void entriItem(stack<Item>& gudang) {
    Item item;
    cout << "Masukkan Kode Item: "; cin >> item.kode;
    cout << "Masukkan Tanggal (YYYY-MM-DD): "; cin >> item.tanggal;
    
    gudang.push(item);
    cout << "Item berhasil ditambahkan ke gudang!\n";
}

// Fungsi untuk mencatat pembeli (queue untuk FIFO)
void catatPembeli(queue<Pembeli>& antrian) {
    Pembeli pembeli;
    cout << "Masukkan NIK Pembeli: "; cin >> pembeli.nik;
    cout << "Masukkan Waktu Kedatangan (HH:MM): "; cin >> pembeli.waktu_kedatangan;
    cout << "Masukkan Kode Item yang Dibeli: "; cin >> pembeli.kode_item;
    
    antrian.push(pembeli);
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

    Pembeli pembeli = antrian.front();
    Item item = gudang.top();

    if (pembeli.kode_item == item.kode) {
        cout << "Barang dengan kode " << item.kode << " dijual ke pembeli dengan NIK " << pembeli.nik << "\n";
        gudang.pop();
        antrian.pop();
    } else {
        cout << "Kode barang tidak sesuai dengan permintaan pembeli!\n";
    }
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
        Item item = gudang.top();
        cout << left << setw(10) << item.kode << setw(15) << item.tanggal << endl;
        gudang.pop();
    }
}

int main() {
    stack<Item> gudang;
    queue<Pembeli> antrian;

    int pilihan;
    do {
        menu();
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                entriKTP();
                break;
            case 2:
                entriItem(gudang);
                break;
            case 3:
                catatPembeli(antrian);
                break;
            case 4:
                jualItem(gudang, antrian);
                break;
            case 5:
                cetakDaftarBarang(gudang);
                break;
            case 6:
                cetakKTP(antrian);
                break;
            case 0:
                cout << "Keluar dari program. Terima kasih!\n";
                break;
            default:
                cout << "Pilihan tidak valid, coba lagi.\n";
        }
    } while (pilihan != 0);

    return 0;
}

