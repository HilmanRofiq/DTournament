#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Pengguna {
    char username[50];
    char password[50];
    char namaLengkap[50];
    char prestasi[200];
    int saldo;         
    int role;               
} userAktif;

struct Turnamen {
    int id;
    char namaTurnamen[100];
    char tipeGame[50];
    char status[20];        
    int prizePool;
    int biayaDaftar;
    int kuotaTim;
    int timTerdaftar;
    char tipeBracket[50];   
} dataTurnamen;

struct Tim {
    char namaTim[50];
    char idPemain[5][50];   
    int idTurnamen;         
    int isApproved;         
    int skor;
};

struct Komentar {
    char username[50];
    char isiKomentar[200];
    int idTurnamen;
};

FILE *fuser;
FILE *fturnamen;
FILE *ftim;
FILE *fkomentar;

int main() {
    MenuLogin();
    return 0;
}


void MenuLogin(){
    int pilihan;
    while (1) {
        system("cls");
        printf("\n=== SELAMAT DATANG DI DTournament ===\n");
        printf("1. Login Admin\n");
        printf("2. Login User\n");
        printf("3. Registrasi User\n");
        printf("4. Logout\n");
        printf("Silahkan input pilihan anda: ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1:
            LoginAdmin();
            break;

            case 2:
            LoginUser();
            break;

            case 3:
            RegistrasiUser();
            break;

            case 4:
            printf("Terimakasih Telah Menggunakan DTournament!\n");
            exit(0);
            break;
    
            default:
            printf("Pilihan tidak valid! Silahkan coba lagi.\n");
            system("pause");
        }    
    }    
};

void LoginAdmin(){
    char username[50], password[50];
    system("cls");
    int i = 0;

    printf("=== Login Admin DTournament===\n");
    while (i < 3) {
        printf("\nMasukan Username: ");
        scanf("%s", &username);
        printf("\nMasukan Password: ");
        scanf("%s", &password);
        if (strcmp(username, "Admin") == 0 && strcmp (password, "Admin123") == 0) {
            printf("Login Berhasil...\n");
            printf("Selamat Datang Admin\n");
            getchar();
            system("pause");
            MenuAdmin(1);
            break;
        } 
        else {
            printf("Username atau Password salah!\n");
            i++;
        }
        if (i < 3) {
            printf("Klik enter untuk ke halaman selanjutnya...\n");
            getchar();
        }
        else {
            printf("Mohon maaf, batas percobaan login telah habis, silahkan coba lagi nanti...");
            system("pause");
            MenuLogin();
        }   

    }
};

void LoginUser(){
    char username[50], password[50];
    system("cls");
    int i = 0;
    int found = 0;

    printf("=== Login User DTournament===\n");
    while (i < 3) {
        printf("\nMasukan Username: ");
        scanf("%s", &username);
        printf("\nMasukan Password: ");
        scanf("%s", &password);

        fuser = fopen("users.dat", "rb");
        if (fuser == NULL) {
            printf("File user tidak ditemukan!\n");
            return;
        }

        struct Pengguna tempUser;
        while (fread(&tempUser, sizeof(struct Pengguna), 1, fuser)) {
            if (strcmp(tempUser.username, username) == 0 && strcmp(tempUser.password, password) == 0) {
                found = 1;
                userAktif = tempUser; 
                break;
            }
        }
        fclose(fuser);

        if (found) {
            printf("Login Berhasil...\n");
            printf("Selamat Datang %s\n", userAktif.namaLengkap);
            getchar();
            system("pause");
            MenuUser();
            break;
        } 
        else {
            printf("Username atau Password salah!\n");
            i++;
        }
        if (i < 3) {
            printf("Klik enter untuk ke halaman selanjutnya...\n");
            getchar();
        }
        else {
            printf("Mohon maaf, batas percobaan login telah habis, silahkan coba lagi nanti...");
            system("pause");
            MenuLogin();
        }
    }
};

void RegistrasiUser(){
    struct Pengguna newUser;
    system("cls");

    printf("=== Registrasi User DTournament===\n");
    printf("Masukan Username: ");
    scanf("%s", newUser.username);
    printf("Masukan Password: ");
    scanf("%s", newUser.password);
    getchar();
    printf("Masukan Nama Lengkap: ");
    fgets(newUser.namaLengkap, sizeof(newUser.namaLengkap), stdin);
    newUser.namaLengkap[strcspn(newUser.namaLengkap, "\n")] = '\0';
    newUser.saldo = 0; 
    newUser.role = 0; 

    fuser = fopen("users.dat", "ab");
    if (fuser == NULL) {
        printf("Gagal membuka file user untuk penulisan!\n");
        return;
    }

    fwrite(&newUser, sizeof(struct Pengguna), 1, fuser);
    fclose(fuser);

    printf("Registrasi Berhasil! Silahkan login untuk melanjutkan.\n");
    system("pause");
    MenuLogin();
};


void MenuAdmin(){
    int pilihan;
    while (1) {
        system("cls");
        printf("\n=== MENU ADMIN DTournament ===\n");
        printf("1. Buat Turnamen\n");
        printf("2. Lihat List Turnamen\n");
        printf("3. Pengaturan Turnamen\n");
        printf("4. Manajemen Tim dan Skor\n");
        printf("5. Logout\n");
        printf("Silahkan input pilihan anda: ");
        scanf("%d", &pilihan);
        getchar();
        switch (pilihan) {
            case 1:
            BuatTurnamen();
            break;

            case 2:
            LihatListTurnamenAdmin();
            break;

            case 3:
            PengaturanTurnamen();
            break;

            case 4:
            ManajemenTimDanSkor();
            break;

            case 5:
            printf("Logout Berhasil!\n");
            system("pause");
            MenuLogin();
            break;
    
            default:
            printf("Pilihan tidak valid! Silahkan coba lagi.\n");
            system("pause");
        }
    }
};

void BuatTurnamen(){
    struct Turnamen newTurnamen;
    system("cls");

    printf("=== Buat Turnamen DTournament===\n");
    printf("Masukan Nama Turnamen: ");
    fgets(newTurnamen.namaTurnamen, sizeof(newTurnamen.namaTurnamen), stdin);
    newTurnamen.namaTurnamen[strcspn(newTurnamen.namaTurnamen, "\n")] = '\0';
    
    printf("Masukan Tipe Game: ");
    fgets(newTurnamen.tipeGame, sizeof(newTurnamen.tipeGame), stdin);
    newTurnamen.tipeGame[strcspn(newTurnamen.tipeGame, "\n")] = '\0';
    strcpy(newTurnamen.status, "Pending"); 

    printf("Masukan Prize Pool: ");
    scanf("%d", &newTurnamen.prizePool);

    printf("Masukan Biaya Daftar per Tim: ");
    scanf("%d", &newTurnamen.biayaDaftar);

    printf("Masukan Kuota Tim: ");
    scanf("%d", &newTurnamen.kuotaTim);
    newTurnamen.timTerdaftar = 0;
    getchar();

    printf("Masukan Tipe Bracket (Single Elimination/Double Elimination): ");
    fgets(newTurnamen.tipeBracket, sizeof(newTurnamen.tipeBracket), stdin);
    newTurnamen.tipeBracket[strcspn(newTurnamen.tipeBracket, "\n")] = '\0';
    fturnamen = fopen("turnamen.dat", "ab");
    if (fturnamen == NULL) {
        printf("Gagal membuka file turnamen untuk penulisan!\n");
        return;
    }
    
    fwrite(&newTurnamen, sizeof(struct Turnamen), 1, fturnamen);
    fclose(fturnamen);
    
    printf("Turnamen berhasil dibuat!\n");
    system("pause");
    MenuAdmin();
};

void LihatListTurnamenAdmin(){
    struct Turnamen tempTurnamen;
    system("cls");

    printf("=== List Turnamen DTournament===\n");
    fturnamen = fopen("turnamen.dat", "rb");
    if (fturnamen == NULL) {
        printf("File turnamen tidak ditemukan!\n");
        return;
    }

    int count = 0;
    while (fread(&tempTurnamen, sizeof(struct Turnamen), 1, fturnamen)) {
        count++;
        printf("\nTurnamen #%d\n", count);
        printf("Nama Turnamen: %s\n", tempTurnamen.namaTurnamen);
        printf("Tipe Game: %s\n", tempTurnamen.tipeGame);
        printf("Status: %s\n", tempTurnamen.status);
        printf("Prize Pool: %d\n", tempTurnamen.prizePool);
        printf("Biaya Daftar per Tim: %d\n", tempTurnamen.biayaDaftar);
        printf("Kuota Tim: %d\n", tempTurnamen.kuotaTim);
        printf("Tim Terdaftar: %d\n", tempTurnamen.timTerdaftar);
        printf("Tipe Bracket: %s\n", tempTurnamen.tipeBracket);
        printf("-------------------------\n");
        printf("Klik enter untuk ke halaman selanjutnya...\n");
        getchar();
    }
    fclose(fturnamen);
    system("pause");
}; 

void SortirPrizePool(){

};   

void PengaturanTurnamen(){

};    

void ManajemenTimDanSkor(){};  
void MencariListTim(){};
void MenyetujuiTim(){};
void MengeditScore(){};



void MenuUser(){};

void LihatProfil(){}; 

void TopUpSaldo(){};    

void DaftarTurnamen(){}; 

void MelihatListTim(){};

void LihatProgresBracket(){};  

void TambahKomentar(){};         

void SearchingTim(){};
