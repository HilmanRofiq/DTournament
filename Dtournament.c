#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADMIN_USERNAME "Organizer"
#define ADMIN_PASSWORD "ORG123"

struct Pengguna {
    char username[50];
    char password[50];
    char namaLengkap[50];
    char prestasi[200]; // mungkin tidak usah? tapi gpp untuk disimpan saja
    int isKetuaTim; // nama diganti untuk merefleksikan 1 = ketua tim, 0 = player biasa
} userAktif;

struct Turnamen {
    int id;
    char namaTurnamen[100];
    char tipeGame[50];
    char status[20]; // upcoming, ongoing, completed (tapi hanya string saja)
    int prizePool;
    int kuotaTim;
    int timTerdaftar;
    char tipeBracket[50]; // single-elimination, group stage, double-elimination (string juga aja)
};

struct Tim {
    char namaTim[50];
    char idPemain[5][50];
    int idTurnamen;
    int isApproved;
    int skor;
};


FILE *fuser;
FILE *fturnamen;
FILE *ftim;

void MenuLogin();
void LoginAdmin();
void LoginUser();
void RegistrasiUser();
void MenuAdmin();
void BuatTurnamen();
void LihatListTurnamenAdmin(); // sama kayak list user
void SortirPrizePool();
void PengaturanTurnamen();
void ManajemenTimDanSkor();
void MencariListTim();
void MenyetujuiTim();
void MengeditScore();
void MenuUser();
void LihatProfil();
void TopUpSaldo();
void DaftarTurnamen();
void LihatProgresBracket();
void SearchingTim();
void LihatListTurnamenUser();
void clearScreen();
void pause();
void getStringInput(char *buffer, int size);

void clearScreen() {
    printf("\033[2J\033[1;1H"); // ANSI escape code https://apple.stackexchange.com/questions/72291/clear-output-screen-from-c-program
    fflush(stdout);
}

void pause() {
    printf("\nTekan ENTER untuk melanjutkan...");
    while(getchar() != '\n');
}

void getStringInput(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int main() {
    MenuLogin();
    return 0;
}


void MenuLogin() {
    int pilihan;

    while(1) {
        clearScreen();
        printf("====================================\n");
        printf("=   TOURNAMENT MANAGEMENT SYSTEM   =\n");
        printf("====================================\n\n");
        printf("1. Login Admin\n");
        printf("2. Login Player\n");
        printf("3. Registrasi Player\n");
        printf("4. Logout\n");
        printf("\nPilihan: ");
        scanf("%d", &pilihan); getchar();

        switch(pilihan) {
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
                printf("\nTerimakasih Telah Menggunakan DTournament!\n");
                exit(0);
            default:
                printf("\nPilihan tidak valid!\n");
                pause();
        }
    }
}

void LoginAdmin() {
    char username[50];
    char password[50];
    int kesempatan = 3;
    int found = 0;

    clearScreen();
    printf("====================================\n");
    printf("=           LOGIN ADMIN            =\n");
    printf("====================================\n\n");

    while(kesempatan > 0 && !found) {
        printf("Username: ");
        getStringInput(username, sizeof(username));
        
        printf("Password: ");
        getStringInput(password, sizeof(password));

        if(strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
            strcpy(userAktif.username, ADMIN_USERNAME);
            strcpy(userAktif.password, ADMIN_PASSWORD);
            strcpy(userAktif.namaLengkap, "Mister ORG");
            strcpy(userAktif.prestasi, "Organizer");
            userAktif.isKetuaTim = 1;
            found = 1;

            printf("\nLogin berhasil! Selamat datang, Organizer\n");
            pause();
            MenuAdmin();
        } else {
            kesempatan--;
            if(kesempatan > 0) {
                printf("\nUsername atau password salah. Kesempatan tersisa: %d\n", kesempatan);
                printf("Hint: Username='Organizer', Password='ORG123'\n");
            } else {
                printf("\nLogin gagal. Kembali ke menu utama.\n");
            }
        }
    }
    pause();
}

void LoginUser() {
    char username[50];
    char password[50];
    int kesempatan = 3;
    int found = 0;

    clearScreen();
    printf("====================================\n");
    printf("=           LOGIN PLAYER           =\n");
    printf("====================================\n\n");

    while(kesempatan > 0 && !found) {
        printf("Username: ");
        getStringInput(username, sizeof(username));
        
        printf("Password: ");
        getStringInput(password, sizeof(password));

        fuser = fopen("users.dat", "rb");
        if(fuser == NULL) {
            printf("\nGagal membuka file user untuk penulisan!\n");
            pause();
            return;
        }

        struct Pengguna temp;
        while(fread(&temp, sizeof(struct Pengguna), 1, fuser) == 1) {
            if(strcmp(temp.username, username) == 0 && 
               strcmp(temp.password, password) == 0) {
                userAktif = temp;
                found = 1;
                break;
            }
        }
        fclose(fuser);

        if(found) {
            printf("\nLogin berhasil! Selamat datang, %s\n", userAktif.namaLengkap);
            pause();
            MenuUser();
        } else {
            kesempatan--;
            if(kesempatan > 0) {
                printf("\nUsername atau password salah. Kesempatan tersisa: %d\n", kesempatan);
            } else {
                printf("\nLogin gagal. Kembali ke menu utama.\n");
            }
        }
    }
    pause();
}

void RegistrasiUser() {
    struct Pengguna userBaru;

    clearScreen();
    printf("====================================\n");
    printf("=      REGISTRASI PLAYER BARU      =\n");
    printf("====================================\n\n");

    printf("Username (tanpa spasi): ");
    getStringInput(userBaru.username, sizeof(userBaru.username));
    
    printf("Password (tanpa spasi): ");
    getStringInput(userBaru.password, sizeof(userBaru.password));
    
    printf("Nama Lengkap: ");
    getStringInput(userBaru.namaLengkap, sizeof(userBaru.namaLengkap));

    strcpy(userBaru.prestasi, "Belum ada prestasi");
    userBaru.isKetuaTim = 0;

    fuser = fopen("users.dat", "ab");
    if(fuser == NULL) {
        printf("\nGagal membuka file!\n");
        pause();
        return;
    }

    fwrite(&userBaru, sizeof(struct Pengguna), 1, fuser);
    fclose(fuser);

    printf("\nRegistrasi berhasil! Silakan login.\n");
    pause();
}

void MenuAdmin() {
    int pilihan;

    while(1) {
        clearScreen();
        printf("====================================\n");
        printf("=            MENU ADMIN            =\n");
        printf("====================================\n");
        printf("Hello, %s\n\n", userAktif.username);
        printf("1. Buat Tournament\n");
        printf("2. Lihat List Tournament\n");
        printf("3. Sortir Tournament\n");
        printf("4. Pengaturan Tournament\n");
        printf("5. Manajemen Tim dan Score\n");
        printf("6. Logout\n");
        printf("\nPilihan: ");
        scanf("%d", &pilihan); getchar();

        switch(pilihan) {
            case 1:
                BuatTurnamen();
                break;
            case 2:
                LihatListTurnamenAdmin();
                break;
            case 3:
                SortirPrizePool();
                break;
            case 4:
                PengaturanTurnamen();
                break;
            case 5:
                ManajemenTimDanSkor();
                break;
            case 6:
                return;
            default:
                printf("\nPilihan tidak valid!\n");
                pause();
        }
    }
}

void BuatTurnamen() {
    struct Turnamen tourBaru;
    int pilihanGame;

    clearScreen();
    printf("====================================\n");
    printf("=       BUAT TOURNAMENT BARU       =\n");
    printf("====================================\n\n");

    fturnamen = fopen("tournaments.dat", "rb");
    int maxId = 0;
    if(fturnamen != NULL) {
        struct Turnamen temp;
        while(fread(&temp, sizeof(struct Turnamen), 1, fturnamen) == 1) {
            if(temp.id > maxId) maxId = temp.id;
        }
        fclose(fturnamen);
    }
    tourBaru.id = maxId + 1;

    printf("Nama Tournament: ");
    getStringInput(tourBaru.namaTurnamen, sizeof(tourBaru.namaTurnamen));

    printf("\nPilih Tipe Game:\n");
    printf("1. Counter Strike 2\n");
    printf("2. Valorant\n");
    printf("3. Dota 2\n");
    printf("4. Mobile Legends\n");
    printf("5. Apex Legends\n");
    printf("Pilihan: ");
    scanf("%d", &pilihanGame); getchar();

    switch(pilihanGame) {
        case 1: strcpy(tourBaru.tipeGame, "Counter Strike 2"); break;
        case 2: strcpy(tourBaru.tipeGame, "Valorant"); break;
        case 3: strcpy(tourBaru.tipeGame, "Dota 2"); break;
        case 4: strcpy(tourBaru.tipeGame, "Mobile Legends"); break;
        case 5: strcpy(tourBaru.tipeGame, "Apex Legends"); break;
        default: strcpy(tourBaru.tipeGame, "Unknown"); break;
    }

    printf("Prize Pool (Rp): ");
    scanf("%d", &tourBaru.prizePool);
    getchar();

    strcpy(tourBaru.status, "Upcoming");
    tourBaru.kuotaTim = 0;
    tourBaru.timTerdaftar = 0;
    strcpy(tourBaru.tipeBracket, "Not Set");

    fturnamen = fopen("tournaments.dat", "ab");
    if(fturnamen == NULL) {
        printf("\nGagal membuka file!\n");
        pause();
        return;
    }

    fwrite(&tourBaru, sizeof(struct Turnamen), 1, fturnamen);
    fclose(fturnamen);

    printf("\nTournament berhasil dibuat dengan ID: %d\n", tourBaru.id);
    printf("  Nama       : %s\n", tourBaru.namaTurnamen);
    printf("  Game       : %s\n", tourBaru.tipeGame);
    printf("  Prize Pool : Rp %d\n", tourBaru.prizePool);
    pause();
}

void LihatListTurnamenAdmin() {
    int kategori;

    clearScreen();
    printf("====================================\n");
    printf("=       LIST TOURNAMENT            =\n");
    printf("====================================\n\n");
    printf("1. Upcoming\n");
    printf("2. Ongoing\n");
    printf("3. Completed\n");
    printf("4. Semua Tournament\n");
    printf("\nPilih kategori: ");
    scanf("%d", &kategori); getchar();

    char statusFilter[20];
    switch(kategori) {
        case 1: strcpy(statusFilter, "Upcoming"); break;
        case 2: strcpy(statusFilter, "Ongoing"); break;
        case 3: strcpy(statusFilter, "Completed"); break;
        case 4: strcpy(statusFilter, "All"); break;
        default:
            printf("\nPilihan tidak valid!\n");
            pause();
            return;
    }

    fturnamen = fopen("tournaments.dat", "rb");
    if(fturnamen == NULL) {
        printf("\nBelum ada tournament yang dibuat.\n");
        pause();
        return;
    }

    printf("\n%-5s %-30s %-20s %-12s %-15s\n", "ID", "Nama", "Game", "Status", "Prize Pool");
    printf("================================================================================\n");

    struct Turnamen temp;
    int count = 0;
    while(fread(&temp, sizeof(struct Turnamen), 1, fturnamen) == 1) {
        if(strcmp(statusFilter, "All") == 0 || strcmp(temp.status, statusFilter) == 0) {
            printf("%-5d %-30s %-20s %-12s Rp %-12d\n", 
                   temp.id, temp.namaTurnamen, temp.tipeGame, temp.status, temp.prizePool);
            count++;
        }
    }
    fclose(fturnamen);

    if(count == 0) {
        printf("\nTidak ada tournament dalam kategori ini.\n");
    } else {
        printf("\nTotal: %d tournament\n", count);
    }
    pause();
}

void SortirPrizePool() {
    int pilihan;

    clearScreen();
    printf("====================================\n");
    printf("=       SORTIR PRIZE POOL          =\n");
    printf("====================================\n\n");
    printf("1. Tertinggi ke Terendah\n");
    printf("2. Terendah ke Tertinggi\n");
    printf("\nPilihan: ");
    scanf("%d", &pilihan); getchar();

    fturnamen = fopen("tournaments.dat", "rb");
    if(fturnamen == NULL) {
        printf("\nBelum ada tournament.\n");
        pause();
        return;
    }

    struct Turnamen tournaments[100];
    int count = 0;

    while(fread(&tournaments[count], sizeof(struct Turnamen), 1, fturnamen) == 1) {
        count++;
    }
    fclose(fturnamen);

    for(int i = 1; i < count; i++) {
        struct Turnamen key = tournaments[i];
        int j = i - 1;
        
        if(pilihan == 1) {
            while(j >= 0 && tournaments[j].prizePool < key.prizePool) {
                tournaments[j + 1] = tournaments[j];
                j--;
            }
        } else {
            while(j >= 0 && tournaments[j].prizePool > key.prizePool) {
                tournaments[j + 1] = tournaments[j];
                j--;
            }
        }
        tournaments[j + 1] = key;
    }

    printf("\n%-5s %-30s %-15s %-15s\n", "ID", "Nama", "Status", "Prize Pool");
    printf("================================================================\n");

    for(int i = 0; i < count; i++) {
        printf("%-5d %-30s %-15s Rp %-12d\n", 
               tournaments[i].id, tournaments[i].namaTurnamen,
               tournaments[i].status, tournaments[i].prizePool);
    }
    printf("\nTotal: %d tournament\n", count);
    pause();
}

void PengaturanTurnamen() {
    int idTournament, pilihan;

    clearScreen();
    printf("====================================\n");
    printf("=     PENGATURAN TOURNAMENT        =\n");
    printf("====================================\n\n");
    printf("Masukkan ID Tournament: ");
    scanf("%d", &idTournament); getchar();

    fturnamen = fopen("tournaments.dat", "rb");
    if(fturnamen == NULL) {
        printf("\nFile tidak ditemukan!\n");
        pause();
        return;
    }

    struct Turnamen tournaments[100];
    int count = 0, found = -1;

    while(fread(&tournaments[count], sizeof(struct Turnamen), 1, fturnamen) == 1) {
        if(tournaments[count].id == idTournament) {
            found = count;
        }
        count++;
    }
    fclose(fturnamen);

    if(found == -1) {
        printf("\nTournament tidak ditemukan!\n");
        pause();
        return;
    }

    printf("\n=== Tournament: %s ===\n", tournaments[found].namaTurnamen);
    printf("Game           : %s\n", tournaments[found].tipeGame);
    printf("Status         : %s\n", tournaments[found].status);
    printf("Kuota Tim      : %d\n", tournaments[found].kuotaTim);
    printf("Tim Terdaftar  : %d\n", tournaments[found].timTerdaftar);
    printf("Bracket        : %s\n\n", tournaments[found].tipeBracket);

    printf("1. Atur Kuota Tim\n");
    printf("2. Pilih Tipe Bracket\n");
    printf("3. Ubah Status Tournament\n");
    printf("\nPilihan: ");
    scanf("%d", &pilihan); getchar();

    switch(pilihan) {
        case 1:
            printf("\nMasukkan kuota tim: ");
            scanf("%d", &tournaments[found].kuotaTim);
            getchar();
            printf("Kuota tim berhasil diatur menjadi %d!\n", tournaments[found].kuotaTim);
            break;
        case 2:
            printf("\nPilih Tipe Bracket:\n");
            printf("1. Single-Elimination\n");
            printf("2. Group Stage\n");
            printf("3. Double-Elimination\n");
            printf("Pilihan: ");
            int bracketChoice;
            scanf("%d", &bracketChoice);
            getchar();
            if(bracketChoice == 1) {
                strcpy(tournaments[found].tipeBracket, "Single-Elimination");
            } else if(bracketChoice == 2) {
                strcpy(tournaments[found].tipeBracket, "Group Stage");
            } else {
                strcpy(tournaments[found].tipeBracket, "Double-Elimination");
            }
            printf("Tipe bracket berhasil diatur menjadi %s!\n", tournaments[found].tipeBracket);
            break;
        case 3:
            printf("\nPilih Status:\n");
            printf("1. Upcoming\n");
            printf("2. Ongoing\n");
            printf("3. Completed\n");
            printf("Pilihan: ");
            int statusChoice;
            scanf("%d", &statusChoice);
            getchar();
            if(statusChoice == 1) {
                strcpy(tournaments[found].status, "Upcoming");
            } else if(statusChoice == 2) {
                strcpy(tournaments[found].status, "Ongoing");
            } else {
                strcpy(tournaments[found].status, "Completed");
            }
            printf("Status berhasil diubah menjadi %s!\n", tournaments[found].status);
            break;
        default:
            printf("\nPilihan tidak valid!\n");
            pause();
            return;
    }

    fturnamen = fopen("tournaments.dat", "wb");
    fwrite(tournaments, sizeof(struct Turnamen), count, fturnamen);
    fclose(fturnamen);
    pause();
}

void ManajemenTimDanSkor() {
    int pilihan;

    clearScreen();
    printf("====================================\n");
    printf("=     MANAJEMEN TIM & SCORE        =\n");
    printf("====================================\n\n");
    printf("1. Lihat dan Cari List Tim\n");
    printf("2. Setujui Tim yang Mendaftar\n");
    printf("3. Edit Score Pertandingan\n");
    printf("4. Kembali\n");
    printf("\nPilihan: ");
    scanf("%d", &pilihan); getchar();

    switch(pilihan) {
        case 1:
            MencariListTim();
            break;
        case 2:
            MenyetujuiTim();
            break;
        case 3:
            MengeditScore();
            break;
        case 4:
            return;
        default:
            printf("\nPilihan tidak valid!\n");
            pause();
    }
}

void MencariListTim() {
    char keyword[50];

    clearScreen();
    printf("====================================\n");
    printf("=           CARI TIM               =\n");
    printf("====================================\n\n");
    printf("Masukkan nama tim (atau ketik 'all' untuk semua): ");
    getStringInput(keyword, sizeof(keyword));

    ftim = fopen("teams.dat", "rb");
    if(ftim == NULL) {
        printf("\nBelum ada tim yang terdaftar.\n");
        pause();
        return;
    }

    printf("\n%-20s %-15s %-15s %-10s %-10s\n", 
           "Nama Tim", "Ketua", "ID Tournament", "Status", "Score");
    printf("========================================================================\n");

    struct Tim temp;
    int count = 0;
    while(fread(&temp, sizeof(struct Tim), 1, ftim) == 1) {
        if(strcmp(keyword, "all") == 0 || strstr(temp.namaTim, keyword) != NULL) {
            char status[20];
            if(temp.isApproved == 1) {
                strcpy(status, "Approved");
            } else {
                strcpy(status, "Pending");
            }
            printf("%-20s %-15s %-15d %-10s %-10d\n", 
                   temp.namaTim, temp.idPemain[0], temp.idTurnamen, status, temp.skor);
            count++;
        }
    }
    fclose(ftim);

    if(count == 0) {
        printf("\nTidak ada tim yang ditemukan.\n");
    } else {
        printf("\nTotal: %d tim ditemukan\n", count);
    }
    pause();
}

void MenyetujuiTim() {
    char namaTim[50];

    clearScreen();
    printf("====================================\n");
    printf("=         SETUJUI TIM              =\n");
    printf("====================================\n\n");

    ftim = fopen("teams.dat", "rb");
    if(ftim == NULL) {
        printf("\nBelum ada tim yang terdaftar.\n");
        pause();
        return;
    }

    printf("Tim yang menunggu persetujuan:\n");
    printf("%-20s %-15s %-20s\n", "Nama Tim", "ID Tournament", "Ketua");
    printf("========================================================\n");

    struct Tim teams[100];
    int count = 0;
    int pendingCount = 0;

    while(fread(&teams[count], sizeof(struct Tim), 1, ftim) == 1) {
        if(teams[count].isApproved == 0) {
            printf("%-20s %-15d %-20s\n", 
                   teams[count].namaTim, teams[count].idTurnamen, teams[count].idPemain[0]);
            pendingCount++;
        }
        count++;
    }
    fclose(ftim);

    if(pendingCount == 0) {
        printf("\nTidak ada tim yang menunggu persetujuan.\n");
        pause();
        return;
    }

    printf("\nMasukkan nama tim yang akan disetujui: ");
    getStringInput(namaTim, sizeof(namaTim));

    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(teams[i].namaTim, namaTim) == 0 && teams[i].isApproved == 0) {
            teams[i].isApproved = 1;
            found = 1;

            fturnamen = fopen("tournaments.dat", "rb");
            struct Turnamen tournaments[100];
            int tourCount = 0;

            while(fread(&tournaments[tourCount], sizeof(struct Turnamen), 1, fturnamen) == 1) {
                if(tournaments[tourCount].id == teams[i].idTurnamen) {
                    tournaments[tourCount].timTerdaftar++;
                }
                tourCount++;
            }
            fclose(fturnamen);

            fturnamen = fopen("tournaments.dat", "wb");
            fwrite(tournaments, sizeof(struct Turnamen), tourCount, fturnamen);
            fclose(fturnamen);
            break;
        }
    }

    if(found) {
        ftim = fopen("teams.dat", "wb");
        fwrite(teams, sizeof(struct Tim), count, ftim);
        fclose(ftim);
        printf("\nTim '%s' berhasil disetujui!\n", namaTim);
    } else {
        printf("\nTim tidak ditemukan atau sudah disetujui!\n");
    }
    pause();
}

void MengeditScore() {
    char namaTim[50];
    int newScore;

    clearScreen();
    printf("====================================\n");
    printf("=          EDIT SCORE              =\n");
    printf("====================================\n\n");

    ftim = fopen("teams.dat", "rb");
    if(ftim == NULL) {
        printf("\nFile tidak ditemukan!\n");
        pause();
        return;
    }

    printf("Tim yang dapat diedit scorenya:\n");
    printf("%-20s %-15s %-10s\n", "Nama Tim", "ID Tournament", "Score");
    printf("================================================\n");

    struct Tim teams[100];
    int count = 0;

    while(fread(&teams[count], sizeof(struct Tim), 1, ftim) == 1) {
        if(teams[count].isApproved == 1) {
            printf("%-20s %-15d %-10d\n", 
                   teams[count].namaTim, teams[count].idTurnamen, teams[count].skor);
        }
        count++;
    }
    fclose(ftim);

    printf("\nMasukkan nama tim: ");
    getStringInput(namaTim, sizeof(namaTim));

    printf("Masukkan score baru: ");
    scanf("%d", &newScore);
    getchar();

    int found = 0;
    for(int i = 0; i < count; i++) {
        if(strcmp(teams[i].namaTim, namaTim) == 0) {
            int oldScore = teams[i].skor;
            teams[i].skor = newScore;
            found = 1;
            printf("\nScore berhasil diupdate dari %d menjadi %d!\n", oldScore, newScore);
            break;
        }
    }

    if(found) {
        ftim = fopen("teams.dat", "wb");
        fwrite(teams, sizeof(struct Tim), count, ftim);
        fclose(ftim);
    } else {
        printf("\nTim tidak ditemukan!\n");
    }
    pause();
}

void MenuUser() {
    int pilihan;

    while(1) {
        clearScreen();
        printf("====================================\n");
        printf("=           MENU PLAYER            =\n");
        printf("====================================\n");
        printf("Selamat datang, %s\n", userAktif.namaLengkap);
        printf("1. Lihat Profil\n");
        printf("2. Daftar Tournament\n");
        printf("3. Lihat List Tournament\n");
        printf("4. Sortir Tournament\n");
        printf("5. Lihat Progres & Bracket\n");
        printf("6. Searching Tim\n");
        printf("7. Logout\n");
        printf("\nPilihan: ");
        scanf("%d", &pilihan); getchar();

        switch(pilihan) {
            case 1:
                LihatProfil();
                break;
            case 2:
                DaftarTurnamen();
                break;
            case 3:
                LihatListTurnamenUser();
                break;
            case 4:
                SortirPrizePool();
                break;
            case 5:
                LihatProgresBracket();
                break;
            case 6:
                SearchingTim();
                break;
            case 7:
                return;
            default:
                printf("\nPilihan tidak valid!\n");
                pause();
        }
    }
}

void LihatProfil() {
    clearScreen();
    printf("====================================\n");
    printf("=          PROFIL PLAYER           =\n");
    printf("====================================\n\n");
    printf("Username     : %s\n", userAktif.username);
    printf("Nama Lengkap : %s\n", userAktif.namaLengkap);
    printf("Prestasi     : %s\n", userAktif.prestasi);
    printf("Status       : %s\n", userAktif.isKetuaTim ? "Ketua Tim" : "Anggota");
    pause();
}

void DaftarTurnamen() {
    int idTournament;
    struct Tim timBaru;

    clearScreen();
    printf("====================================\n");
    printf("=       DAFTAR TOURNAMENT          =\n");
    printf("====================================\n\n");

    fturnamen = fopen("tournaments.dat", "rb");
    if(fturnamen == NULL) {
        printf("\nBelum ada tournament.\n");
        pause();
        return;
    }

    printf("Tournament yang tersedia:\n");
    printf("%-5s %-30s %-20s %-15s %-10s\n", 
           "ID", "Nama", "Game", "Prizepool", "Slot");
    printf("================================================================================\n");

    struct Turnamen temp;
    int availableCount = 0;
    while(fread(&temp, sizeof(struct Turnamen), 1, fturnamen) == 1) {
        if(strcmp(temp.status, "Upcoming") == 0) {
            printf("%-5d %-30s %-20s Rp %-12d %d/%d\n",
                   temp.id, temp.namaTurnamen, temp.tipeGame,
                   temp.prizePool, temp.timTerdaftar, temp.kuotaTim);
            availableCount++;
        }
    }
    fclose(fturnamen);

    if(availableCount == 0) {
        printf("\nTidak ada tournament yang tersedia saat ini.\n");
        pause();
        return;
    }

    printf("\nMasukkan ID Tournament: ");
    scanf("%d", &idTournament); getchar();

    fturnamen = fopen("tournaments.dat", "rb");
    int found = 0;
    while(fread(&temp, sizeof(struct Turnamen), 1, fturnamen) == 1) {
        if(temp.id == idTournament) {
            found = 1;
            if(temp.kuotaTim > 0 && temp.timTerdaftar >= temp.kuotaTim) {
                printf("\nKuota tournament sudah penuh!\n");
                fclose(fturnamen);
                pause();
                return;
            }
            break;
        }
    }
    fclose(fturnamen);

    if(!found) {
        printf("\nTournament tidak ditemukan!\n");
        pause();
        return;
    }

    printf("\nNama Tim: ");
    getStringInput(timBaru.namaTim, sizeof(timBaru.namaTim));

    printf("\nMasukkan ID 5 pemain (termasuk Anda):\n");
    for(int i = 0; i < 5; i++) {
        printf("Player %d: ", i+1);
        getStringInput(timBaru.idPemain[i], sizeof(timBaru.idPemain[i]));
    }

    timBaru.idTurnamen = idTournament;
    timBaru.isApproved = 0;
    timBaru.skor = 0;

    userAktif.isKetuaTim = 1;

    fuser = fopen("users.dat", "rb");
    struct Pengguna users[100];
    int userCount = 0;

    while(fread(&users[userCount], sizeof(struct Pengguna), 1, fuser) == 1) {
        if(strcmp(users[userCount].username, userAktif.username) == 0) {
            users[userCount].isKetuaTim = 1;
        }
        userCount++;
    }
    fclose(fuser);

    fuser = fopen("users.dat", "wb");
    fwrite(users, sizeof(struct Pengguna), userCount, fuser);
    fclose(fuser);

    ftim = fopen("teams.dat", "ab");
    fwrite(&timBaru, sizeof(struct Tim), 1, ftim);
    fclose(ftim);

    printf("\nPendaftaran berhasil! Menunggu persetujuan admin.\n");
    pause();
}

void LihatListTurnamenUser() {
    int kategori;

    clearScreen();
    printf("====================================\n");
    printf("=       LIST TOURNAMENT            =\n");
    printf("====================================\n\n");
    printf("1. Upcoming\n");
    printf("2. Ongoing\n");
    printf("3. Completed\n");
    printf("4. Semua Tournament\n");
    printf("\nPilih kategori: ");
    scanf("%d", &kategori); getchar();

    char statusFilter[20];
    switch(kategori) {
        case 1: strcpy(statusFilter, "Upcoming"); break;
        case 2: strcpy(statusFilter, "Ongoing"); break;
        case 3: strcpy(statusFilter, "Completed"); break;
        case 4: strcpy(statusFilter, "All"); break;
        default:
            printf("\nPilihan tidak valid!\n");
            pause();
            return;
    }

    fturnamen = fopen("tournaments.dat", "rb");
    if(fturnamen == NULL) {
        printf("\nBelum ada tournament.\n");
        pause();
        return;
    }

    printf("\n%-5s %-30s %-20s %-12s %-15s\n", "ID", "Nama", "Game", "Status", "Prize Pool");
    printf("================================================================================\n");

    struct Turnamen temp;
    int count = 0;
    while(fread(&temp, sizeof(struct Turnamen), 1, fturnamen) == 1) {
        if(strcmp(statusFilter, "All") == 0 || strcmp(temp.status, statusFilter) == 0) {
            printf("%-5d %-30s %-20s %-12s Rp %-12d\n", 
                   temp.id, temp.namaTurnamen, temp.tipeGame, temp.status, temp.prizePool);
            count++;
        }
    }
    fclose(fturnamen);

    if(count == 0) {
        printf("\nTidak ada tournament dalam kategori ini.\n");
    } else {
        printf("\nTotal: %d tournament\n", count);
    }
    pause();
}

void LihatProgresBracket() {
    int idTournament;

    clearScreen();
    printf("====================================\n");
    printf("=    PROGRES BRACKET / KLASEMEN    =\n");
    printf("====================================\n\n");
    printf("Masukkan ID Tournament: ");
    scanf("%d", &idTournament); getchar();

    ftim = fopen("teams.dat", "rb");
    if(ftim == NULL) {
        printf("\nData tournament/tim belum tersedia.\n");
        pause();
        return;
    }

    printf("\n%-20s %-10s %-15s\n", "Nama Tim", "Skor", "Status");
    printf("================================================\n");

    struct Tim temp;
    struct Tim approvedTeams[100];
    int approvedCount = 0;

    while(fread(&temp, sizeof(struct Tim), 1, ftim) == 1) {
        if(temp.idTurnamen == idTournament && temp.isApproved == 1) {
            approvedTeams[approvedCount++] = temp;
        }
    }
    fclose(ftim);

    for(int i = 1; i < approvedCount; i++) {
        struct Tim key = approvedTeams[i];
        int j = i - 1;
        
        while(j >= 0 && approvedTeams[j].skor < key.skor) {
            approvedTeams[j + 1] = approvedTeams[j];
            j--;
        }
        approvedTeams[j + 1] = key;
    }

    if(approvedCount > 0) {
        for(int i = 0; i < approvedCount; i++) {
            printf("%-20s %-10d %-15s\n",
                   approvedTeams[i].namaTim,
                   approvedTeams[i].skor,
                   (approvedTeams[i].skor > 0) ? "Playing" : "Waiting");
        }
    } else {
        printf("Belum ada tim yang approved untuk tournament ini.\n");
    }

    printf("================================================\n");
    pause();
}

void SearchingTim() {
    char cariNama[50];

    clearScreen();
    printf("====================================\n");
    printf("=         SEARCHING TIM            =\n");
    printf("====================================\n\n");

    printf("Masukkan Nama Tim: ");
    getStringInput(cariNama, sizeof(cariNama));

    ftim = fopen("teams.dat", "rb");
    if(ftim == NULL) {
        printf("\nData tim masih kosong.\n");
        pause();
        return;
    }

    printf("\n%-20s %-15s %-10s\n", "Nama Tim", "Ketua", "Status");
    printf("================================================\n");

    struct Tim temp;
    int found = 0;
    while(fread(&temp, sizeof(struct Tim), 1, ftim) == 1) {
        if(strcmp(temp.namaTim, cariNama) == 0) {
            printf("%-20s %-15s %-10s\n",
                   temp.namaTim,
                   temp.idPemain[0],
                   (temp.isApproved == 1) ? "Approved" : "Pending");
            found = 1;
        }
    }

    if(!found) {
        printf("Tim '%s' tidak ditemukan.\n", cariNama);
    }

    fclose(ftim);
    printf("================================================\n");
    pause();
}
