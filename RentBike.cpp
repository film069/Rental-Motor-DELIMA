#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

// Class Motor
class Motor {
public:
    int idMotor;
    string namaMotor;
    int hargaSewa;
    string status;

    Motor(int id, string nama, int harga, string stat = "Tersedia")
        : idMotor(id), namaMotor(nama), hargaSewa(harga), status(stat) {}

    string toString() const {
        return to_string(idMotor) + "," + namaMotor + "," + to_string(hargaSewa) + "," + status;
    }
};

// Class Penyewa
class Penyewa {
public:
    int idPenyewa;
    string namaPenyewa;
    string kontak;

    Penyewa(int id, string nama, string kontak) : idPenyewa(id), namaPenyewa(nama), kontak(kontak) {}

    string toString() const {
        return to_string(idPenyewa) + "," + namaPenyewa + "," + kontak;
    }
};

// Class Transaksi
    class Transaksi {
    public:
        int idTransaksi;
        int idMotor;
        int idPenyewa;
        string tanggalSewa;
        string tanggalKembali;

    Transaksi(int id, int motorId, int penyewaId, string tglSewa, string tglKembali = "")
        : idTransaksi(id), idMotor(motorId), idPenyewa(penyewaId), tanggalSewa(tglSewa), tanggalKembali(tglKembali) {}

    string toString() const {
        return to_string(idTransaksi) + "," + to_string(idMotor) + "," + to_string(idPenyewa) + "," +
               tanggalSewa + "," + (tanggalKembali.empty() ? "Belum" : tanggalKembali);
    }
};

// Class RentalMotor
class RentalMotor {
private:
    vector<Motor> motorList;
    vector<Penyewa> penyewaList;
    vector<Transaksi> transaksiList;

    void simpanMotor() {
        ofstream file("motor.txt");
        for (const auto& motor : motorList) {
            file << motor.toString() << endl;
        }
        file.close();
    }

    void simpanPenyewa() {
        ofstream file("penyewa.txt");
        for (const auto& penyewa : penyewaList) {
            file << penyewa.toString() << endl;
        }
        file.close();
    }

    void simpanTransaksi() {
        ofstream file("transaksi.txt");
        for (const auto& transaksi : transaksiList) {
            file << transaksi.toString() << endl;
        }
        file.close();
    }

    vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(str);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    void muatData() {
        // Muat data motor
        ifstream fileMotor("motor.txt");
        string line;
        while (getline(fileMotor, line)) {
            auto data = split(line, ',');
            motorList.emplace_back(stoi(data[0]), data[1], stoi(data[2]), data[3]);
        }
        fileMotor.close();

        // Muat data penyewa
        ifstream filePenyewa("penyewa.txt");
        while (getline(filePenyewa, line)) {
            auto data = split(line, ',');
            penyewaList.emplace_back(stoi(data[0]), data[1], data[2]);
        }
        filePenyewa.close();

        // Muat data transaksi
        ifstream fileTransaksi("transaksi.txt");
        while (getline(fileTransaksi, line)) {
            auto data = split(line, ',');
            transaksiList.emplace_back(stoi(data[0]), stoi(data[1]), stoi(data[2]), data[3], data[4]);
        }
        fileTransaksi.close();
    }

    void tambahMotor(int id, string nama, int harga) {
        motorList.emplace_back(id, nama, harga);
        simpanMotor();
        cout << "Motor " << nama << " berhasil ditambahkan.\n";
    }

    void tambahPenyewa(int id, string nama, string kontak) {
        penyewaList.emplace_back(id, nama, kontak);
        simpanPenyewa();
        cout << "Penyewa " << nama << " berhasil ditambahkan.\n";
    }

    void sewaMotor(int idMotor, int idPenyewa, string tanggalSewa) {
        auto motorIt = find_if(motorList.begin(), motorList.end(),
            [idMotor](const Motor& m) { return m.idMotor == idMotor && m.status == "Tersedia"; });

        auto penyewaIt = find_if(penyewaList.begin(), penyewaList.end(),
            [idPenyewa](const Penyewa& p) { return p.idPenyewa == idPenyewa; });

        if (motorIt != motorList.end() && penyewaIt != penyewaList.end()) {
            motorIt->status = "Disewa";
            int idTransaksi = transaksiList.size() + 1;
            transaksiList.emplace_back(idTransaksi, idMotor, idPenyewa, tanggalSewa);
            simpanMotor();
            simpanTransaksi();
            cout << "Motor " << motorIt->namaMotor << " berhasil disewa oleh " << penyewaIt->namaPenyewa << ".\n";
        } else {
            cout << "Motor tidak tersedia atau penyewa tidak valid.\n";
        }
    }

    void kembalikanMotor(int idTransaksi, string tanggalKembali) {
        auto transaksiIt = find_if(transaksiList.begin(), transaksiList.end(),
            [idTransaksi](const Transaksi& t) { return t.idTransaksi == idTransaksi && t.tanggalKembali.empty(); });

        if (transaksiIt != transaksiList.end()) {
            transaksiIt->tanggalKembali = tanggalKembali;

            auto motorIt = find_if(motorList.begin(), motorList.end(),
                [transaksiIt](const Motor& m) { return m.idMotor == transaksiIt->idMotor; });

            if (motorIt != motorList.end()) {
                motorIt->status = "Tersedia";
                simpanMotor();
                simpanTransaksi();
                cout << "Motor " << motorIt->namaMotor << " berhasil dikembalikan.\n";
            }
        } else {
            cout << "Transaksi tidak valid atau motor sudah dikembalikan.\n";
        }
    }

    void lihatMotor() {
        cout << "Daftar Motor:\n";
        for (const auto& motor : motorList) {
            cout << motor.idMotor << ": " << motor.namaMotor << " - " << motor.hargaSewa
                 << " - " << motor.status << "\n";
        }
    }

    void lihatPenyewa() {
        cout << "Daftar Penyewa:\n";
        for (const auto& penyewa : penyewaList) {
            cout << penyewa.idPenyewa << ": " << penyewa.namaPenyewa << " - " << penyewa.kontak << "\n";
        }
    }

    void lihatTransaksi() {
        cout << "Riwayat Transaksi:\n";
        for (const auto& transaksi : transaksiList) {
            cout << transaksi.idTransaksi << ": Motor " << transaksi.idMotor << " oleh " << transaksi.idPenyewa
                 << " tanggal " << transaksi.tanggalSewa << " - Kembali: "
                 << (transaksi.tanggalKembali.empty() ? "Belum" : transaksi.tanggalKembali) << "\n";
        }
    }
};

int main() {
    RentalMotor rental;
    rental.muatData();

    int pilihan;
    do {
        cout << "\n=== Menu Rental Motor ===\n";
        cout << "1. Tambah Motor\n";
        cout << "2. Tambah Penyewa\n";
        cout << "3. Sewa Motor\n";
        cout << "4. Kembalikan Motor\n";
        cout << "5. Lihat Daftar Motor\n";
        cout << "6. Lihat Daftar Penyewa\n";
        cout << "7. Lihat Riwayat Transaksi\n";
        cout << "0. Keluar\n==========================\n";
        cout << "Pilih opsi: ";      
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                int id, harga;
                string nama;
                cout << "Masukkan ID Motor: ";
                cin >> id;
                cin.ignore();
                cout << "Masukkan Nama Motor: ";
                getline(cin, nama);
                cout << "Masukkan Harga Sewa: ";
                cin >> harga;
                rental.tambahMotor(id, nama, harga);
                break;
            }
            case 2: {
                int id;
                string nama, kontak;
                cout << "Masukkan ID Penyewa: ";
                cin >> id;
                cin.ignore();
                cout << "Masukkan Nama Penyewa: ";
                getline(cin, nama);
                cout << "Masukkan Kontak Penyewa: ";
                getline(cin, kontak);
                rental.tambahPenyewa(id, nama, kontak);
                break;
            }
            case 3: {
                int idMotor, idPenyewa;
                string tanggalSewa;
                cout << "Masukkan ID Motor: ";
                cin >> idMotor;
                cout << "Masukkan ID Penyewa: ";
                cin >> idPenyewa;
                cout << "Masukkan Tanggal Sewa (YYYY-MM-DD): ";
                cin >> tanggalSewa;
                rental.sewaMotor(idMotor, idPenyewa, tanggalSewa);
                break;
            }
            case 4: {
                int idTransaksi;
                string tanggalKembali;
                cout << "Masukkan ID Transaksi: ";
                cin >> idTransaksi;
                cout << "Masukkan Tanggal Kembali (YYYY-MM-DD): ";
                cin >> tanggalKembali;
                rental.kembalikanMotor(idTransaksi, tanggalKembali);
                break;
            }
            case 5:
                rental.lihatMotor();
                break;
            case 6:
                rental.lihatPenyewa();
                break;
            case 7:
                rental.lihatTransaksi();
                break;
            case 0:
                cout << "Keluar dari program...\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}
