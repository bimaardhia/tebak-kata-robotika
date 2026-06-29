/* ============================================================
   TEBAK KATA - Robotika Edition
   Proyek Akhir Praktikum Dasar Pemrograman
   ------------------------------------------------------------
   Nama  : Bima Ardhia Vardhan
   NIM   : 163251025
   Kelas : RK-A1
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_WRONG   6
#define MAX_WORDS   50
#define HOF_SIZE    10
#define NAME_LEN    20
#define WORD_LEN    30

/* ---------- STRUCT ---------- */
struct Date {
    int day, month, year;
};

struct ScoreEntry {
    char name[NAME_LEN];
    int  points;
    struct Date achieved;
};

struct Game {
    char secret[WORD_LEN];
    char display[WORD_LEN];
    char guessed[WORD_LEN];
    int  guessed_count;
    int  wrong;
};

/* ---------- VARIABEL GLOBAL ---------- */
const char *word_bank[MAX_WORDS] = {
    "ALGORITMA", "MOTOR", "NEURON", "ROBOTIKA", "SENSOR"
};
int word_count = 5;

struct ScoreEntry hall_of_fame[HOF_SIZE];
int hof_count = 0;

/* ---------- PROTOTIPE FUNGSI ---------- */
void print_banner(void);
int  show_menu(void);
void play_game(void);
void draw_hangman(int wrong);
int  process_guess(struct Game *g, char letter);
int  linear_search_char(const char *arr, int n, char target);
int  is_won(const struct Game *g);
int  binary_search_word(const char *bank[], int n, const char *target);
void insert_word_sorted(const char *bank[], int *n, const char *new_word);
void bubble_sort_hof(struct ScoreEntry hof[], int n);
void show_hall_of_fame(void);
void add_word(void);
void save_score(const char *name, int points);
void to_upper_str(char *s);

/* ============================================================
   MAIN
   ============================================================ */
int main(void) {
    int choice;
    srand((unsigned) time(NULL));

    do {
        choice = show_menu();
        switch (choice) {
            case 1: play_game();         break;
            case 2: show_hall_of_fame(); break;
            case 3: add_word();          break;
            case 0: printf("\nTerima kasih telah bermain!\n"); break;
            default: printf("\nPilihan tidak valid. Coba lagi.\n");
        }
    } while (choice != 0);

    return 0;
}

void print_banner(void) {
    printf("\n========================================\n");
    printf("   TEBAK KATA - Robotika Edition\n");
    printf("========================================\n");
}

int show_menu(void) {
    int choice;
    print_banner();
    printf("  [1] Main game baru\n");
    printf("  [2] Lihat Hall of Fame\n");
    printf("  [3] Tambah kata ke bank\n");
    printf("  [0] Keluar\n");
    printf("Pilih: ");
    
    if (scanf("%d", &choice) != 1) {
        choice = -1;
    }
    
    /* Input buffer clearing to ensure robustness */
    int c; while ((c = getchar()) != '\n' && c != EOF);
    
    return choice;
}

void draw_hangman(int wrong) {
    printf("\n   +---+\n");
    printf("   |   |\n");
    printf("   %c   |\n", (wrong >= 1) ? 'O' : ' ');
    printf("  %c%c%c  |\n",
           (wrong >= 3) ? '/' : ' ',
           (wrong >= 2) ? '|' : ' ',
           (wrong >= 4) ? '\\' : ' ');
    printf("  %c %c  |\n",
           (wrong >= 5) ? '/' : ' ',
           (wrong >= 6) ? '\\' : ' ');
    printf("  =========\n");
}

/* ============================================================
   IMPLEMENTASI FUNGSI KELENGKAPAN MODUL
   ============================================================ */

void play_game(void) {
    char player_name[NAME_LEN];
    printf("\nMasukkan nama: ");
    fgets(player_name, NAME_LEN, stdin);
    player_name[strcspn(player_name, "\n")] = '\0';
    to_upper_str(player_name);

    struct Game g;
    g.guessed_count = 0;
    g.wrong = 0;
    memset(g.guessed, 0, WORD_LEN);

    /* Pick a random word and initialize the display mask */
    int word_idx = rand() % word_count;
    strncpy(g.secret, word_bank[word_idx], WORD_LEN - 1);
    g.secret[WORD_LEN - 1] = '\0';

    int len = strlen(g.secret);
    for (int i = 0; i < len; i++) {
        g.display[i] = '_';
    }
    g.display[len] = '\0';

    printf("Kata dipilih! Panjang: %d huruf.\n", len);

    /* Main game session loop */
    while (g.wrong < MAX_WRONG && !is_won(&g)) {
        draw_hangman(g.wrong);
        printf("\n========================\n");
        printf("Kata : ");
        for (int i = 0; i < len; i++) {
            printf("%c ", g.display[i]);
        }
        
        printf("\nNyawa: %d   Salah ditebak: ", MAX_WRONG - g.wrong);
        if (g.wrong == 0) {
            printf("(belum ada)");
        } else {
            for (int i = 0; i < g.guessed_count; i++) {
                if (!strchr(g.secret, g.guessed[i])) {
                    printf("%c ", g.guessed[i]);
                }
            }
        }
        
        printf("\nTebak huruf: ");
        char guess;
        scanf(" %c", &guess);
        int c; while ((c = getchar()) != '\n' && c != EOF);
        guess = toupper((unsigned char)guess);

        int status = process_guess(&g, guess);
        if (status == 1) {
            printf("-> Huruf '%c' sudah pernah ditebak! Coba huruf lain.\n", guess);
        } else if (status == 2) {
            printf("-> Benar! Huruf '%c' ada di kata.\n", guess);
        } else {
            printf("-> Salah! '%c' tidak ada. (nyawa berkurang)\n", guess);
        }
    }

    draw_hangman(g.wrong);
    
    if (is_won(&g)) {
        printf("\n*** SELAMAT! Kamu menebak kata: %s ***\n", g.secret);
        int sisa_nyawa = MAX_WRONG - g.wrong;
        int skor = (sisa_nyawa * 100) + (len * 20);
        
        printf("Nyawa tersisa: %d\n", sisa_nyawa);
        printf("Skor = (%d x 100) + (%d x 20) = %d\n", sisa_nyawa, len, skor);

        printf("Apakah skor ini layak masuk Hall of Fame? (Y/T): ");
        char ans;
        scanf(" %c", &ans);
        int c; while ((c = getchar()) != '\n' && c != EOF);
        
        if (toupper((unsigned char)ans) == 'Y') {
            save_score(player_name, skor);
        }
    } else {
        printf("\nGAME OVER! Kata yang benar adalah: %s\n", g.secret);
    }
}

int process_guess(struct Game *g, char letter) {
    if (linear_search_char(g->guessed, g->guessed_count, letter) != -1) {
        return 1; /* Skip turn, already guessed */
    }

    g->guessed[g->guessed_count++] = letter;
    g->guessed[g->guessed_count] = '\0';

    int found = 0;
    const char *p_sec = g->secret;
    char *p_disp = g->display;

    /* Utilizing pointer arithmetic for fast traversal */
    while (*p_sec != '\0') {
        if (*p_sec == letter) {
            *p_disp = letter;
            found = 1;
        }
        p_sec++;
        p_disp++;
    }

    if (!found) {
        g->wrong++;
        return 0; /* Wrong guess */
    }
    
    return 2; /* Correct guess */
}

int linear_search_char(const char *arr, int n, char target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int is_won(const struct Game *g) {
    const char *p = g->display;
    while (*p) {
        if (*p == '_') return 0;
        p++;
    }
    return 1;
}

int binary_search_word(const char *bank[], int n, const char *target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(bank[mid], target);
        if (cmp == 0) return mid;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void insert_word_sorted(const char *bank[], int *n, const char *new_word) {
    if (*n >= MAX_WORDS) return;

    /* Safe dynamic memory allocation for the new persistent string */
    char *allocated_word = (char *)malloc(strlen(new_word) + 1);
    if (!allocated_word) return;
    strcpy(allocated_word, new_word);

    /* Insertion sort logic, moving elements to make space */
    int i = *n - 1;
    while (i >= 0 && strcmp(bank[i], allocated_word) > 0) {
        bank[i + 1] = bank[i];
        i--;
    }
    
    bank[i + 1] = allocated_word;
    (*n)++;
}

void bubble_sort_hof(struct ScoreEntry hof[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (hof[j].points < hof[j + 1].points) {
                struct ScoreEntry temp = hof[j];
                hof[j] = hof[j + 1];
                hof[j + 1] = temp;
            }
        }
    }
}

void show_hall_of_fame(void) {
    printf("\n========================================\n");
    printf("HALL OF FAME\n");
    printf("========================================\n");
    
    if (hof_count == 0) {
        printf("Belum ada data skor.\n");
        return;
    }
    
    printf("Rank  Nama                  Skor   Tanggal\n");
    printf("----------------------------------------\n");
    for (int i = 0; i < hof_count; i++) {
        printf("%-4d  %-20s  %-4d   %02d/%02d/%04d\n",
               i + 1,
               hall_of_fame[i].name,
               hall_of_fame[i].points,
               hall_of_fame[i].achieved.day,
               hall_of_fame[i].achieved.month,
               hall_of_fame[i].achieved.year);
    }
}

void save_score(const char *name, int points) {
    if (hof_count == HOF_SIZE && points <= hall_of_fame[HOF_SIZE - 1].points) {
        printf("Maaf, skor belum cukup tinggi untuk masuk Hall of Fame.\n");
        return;
    }

    /* Integrate exact timestamp extraction */
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    int target_idx = (hof_count < HOF_SIZE) ? hof_count : HOF_SIZE - 1;

    strncpy(hall_of_fame[target_idx].name, name, NAME_LEN - 1);
    hall_of_fame[target_idx].name[NAME_LEN - 1] = '\0';
    hall_of_fame[target_idx].points = points;
    hall_of_fame[target_idx].achieved.day = tm_info->tm_mday;
    hall_of_fame[target_idx].achieved.month = tm_info->tm_mon + 1;
    hall_of_fame[target_idx].achieved.year = tm_info->tm_year + 1900;

    if (hof_count < HOF_SIZE) hof_count++;

    bubble_sort_hof(hall_of_fame, hof_count);

    /* Find where the player was actually sorted to show correct prompt later */
    printf("Skor %s (%d) tersimpan, tanggal %02d/%02d/%04d.\n", 
           name, points, tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

void add_word(void) {
    if (word_count >= MAX_WORDS) {
        printf("Bank kata penuh!\n");
        return;
    }

    char new_word[WORD_LEN];
    printf("\nMasukkan kata baru: ");
    fgets(new_word, WORD_LEN, stdin);
    new_word[strcspn(new_word, "\n")] = '\0';

    if (strlen(new_word) == 0) return;

    to_upper_str(new_word);

    printf("Mengecek bank kata (binary search)...\n");
    if (binary_search_word(word_bank, word_count, new_word) != -1) {
        printf("'%s' sudah ada di bank kata.\n", new_word);
    } else {
        printf("'%s' belum ada. Menyisipkan pada posisi terurut.\n", new_word);
        insert_word_sorted(word_bank, &word_count, new_word);

        printf("Bank kata sekarang: ");
        for (int i = 0; i < word_count; i++) {
            printf("%s ", word_bank[i]);
        }
        printf("\nKata berhasil ditambahkan.\n");
    }
}

void to_upper_str(char *s) {
    while (*s) {
        *s = toupper((unsigned char)*s);
        s++;
    }
}