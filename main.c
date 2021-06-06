#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Zajecia_QS_Pawel_Zborowski_ASD_2021
//
// DEKLARACJE FUNKCJI
//
int* generujLosowaTablice(const int* iloscElementow);
int* generujRosnacaTablice(const int* iloscElementow);
int* generujMalejacaTablice(const int* iloscElementow);
void porownajCzasyDlaTablicy(const char* rodzajTablicy, const int* tablica, const int* iloscElementow);
void wypiszTablice(const int* tablica, const int* iloscElementow);
bool weryfikujSortowanie(const int* posortowanaTablica, const int* nieposortowanaTablica, const int* iloscElementow);
void zamienLiczby(int* tablica, int lewyIndeks, int prawyIndeks);
void quickSort(int* tablica, int lewyIndeks, int prawyIndeks);
void heapSort(int* tablica, const int* iloscElementow);
void heapSort_odbudujKopiec(int* tablica, const int iloscElementow, int poczatkowyIndeks);
void selectionSort(int* tablica, const int* iloscElementow);

//
// GŁÓWNA FUNKCJA MAIN
//
int main()
{
    // Inicjalizujemy losowość;
    time_t czas;
    srand((unsigned)time(&czas));

    // Alokujemy pamięć
    int* iloscElementow = (int*)malloc(sizeof(int));
    do {
        printf("Wprowadz rozmiar tablicy lub liczbe niedodatnia by zamknac program.\n");

        // Wczytujemy rozmiar tablicy
        bool wczytano = false;
        while (!wczytano) {
            wczytano = scanf_s("%d", iloscElementow) == 1;

            // Czyścimy bufor - czytamy aż dojdziemy do znaku nowej linii
            while (getchar() != '\n') {
                wczytano = false;
            }

            if (!wczytano) {
                printf("Wprowadzono bledne dane! Sprobuj ponownie.\n");
            }
        }

        if (*iloscElementow <= 0) {
            // Zwalniamy pamięć
            free(iloscElementow);
            // Zamykamy program
            return 0;
        }
        printf("\n");

        // Generujemy losową tablicę
        const int* losowaTablica = generujLosowaTablice(iloscElementow);
        // wypiszTablice(losowaTablica, iloscElementow);
        // Wywołujemy algorytmy sortowania
        porownajCzasyDlaTablicy("Tablica z losowymi wartosciami", losowaTablica, iloscElementow);
        // Zwalniamy pamięć
        free(losowaTablica);

        // Generujemy rosnącą tablicę
        losowaTablica = generujRosnacaTablice(iloscElementow);
        // Wywołujemy algorytmy sortowania
        porownajCzasyDlaTablicy("Tablica z rosnacymi wartosciami", losowaTablica, iloscElementow);
        // Zwalniamy pamięć
        free(losowaTablica);

        // Generujemy losową tablicę
        losowaTablica = generujMalejacaTablice(iloscElementow);
        // Wywołujemy algorytmy sortowania
        porownajCzasyDlaTablicy("Tablica z malejacymi wartosciami", losowaTablica, iloscElementow);
        // Zwalniamy pamięć
        free(losowaTablica);

        printf("\n\n");
    } while (true);
}

//
// DEFINICJE FUNKCJI
//
int* generujLosowaTablice(const int* iloscElementow) {
    // Alokujemy pamięć
    int* tablica = (int*)malloc(*iloscElementow * sizeof(int));
    // Generujemy losową tablicę o zakresie wartości od <0,iloscElementow)
    for (int i = 0; i < *iloscElementow; i++) {
        tablica[i] = rand() % *iloscElementow;
    }
    // Zwracamy tablicę
    return tablica;
}

int* generujRosnacaTablice(const int* iloscElementow) {
    // Alokujemy pamięć
    int* tablica = (int*)malloc(*iloscElementow * sizeof(int));
    // Generujemy rosnącą tablicę
    for (int i = 0; i < *iloscElementow; i++) {
        tablica[i] = i;
    }
    // Zwracamy tablicę
    return tablica;
}

int* generujMalejacaTablice(const int* iloscElementow) {
    // Alokujemy pamięć
    int* tablica = (int*)malloc(*iloscElementow * sizeof(int));
    // Generujemy malejącą tablicę
    for (int i = 0; i < *iloscElementow; i++) {
        tablica[i] = *iloscElementow - i - 1;
    }
    // Zwracamy tablicę
    return tablica;
}

void porownajCzasyDlaTablicy(const char* rodzajTablicy, const int* tablica, const int* iloscElementow) {
    printf("%s z %d elementami.\n", rodzajTablicy, *iloscElementow);
    clock_t czas;

    //
    // QUICKSORT
    //

    // Kopiujemy tablicę
    int* sortowanaTablica = malloc(*iloscElementow * sizeof(int));
    memcpy(sortowanaTablica, tablica, *iloscElementow * sizeof(int));

    // Sortujemy
    czas = clock();
    quickSort(sortowanaTablica, 0, *iloscElementow - 1);
    czas = clock() - czas;
    double czasSortowaniaWSekundach = ((double)czas) / CLOCKS_PER_SEC;

    // Weryfikujemy
    czas = clock();
    bool rezultat = weryfikujSortowanie(sortowanaTablica, tablica, iloscElementow);
    czas = clock() - czas;
    double czasWeryfikacjiWSekundach = ((double)czas) / CLOCKS_PER_SEC;

    if (rezultat) {
        printf("\tQuickSort zakonczony sukcesem w czasie %f sekund. Poprawnosc zweryfikowano w %f sekund.\n", czasSortowaniaWSekundach, czasWeryfikacjiWSekundach);
    } else {
        printf("\tQuickSort zakonczony NIEPOWODZENIEM w czasie %f sekund. Poprawnosc zweryfikowano w %f sekund.\n", czasSortowaniaWSekundach, czasWeryfikacjiWSekundach);
    }

    //
    // HEAPSORT
    //

    // Kopiujemy tablicę
    memcpy(sortowanaTablica, tablica, *iloscElementow * sizeof(int));

    // Sortujemy
    czas = clock();
    heapSort(sortowanaTablica, iloscElementow);
    czas = clock() - czas;
    czasSortowaniaWSekundach = ((double)czas) / CLOCKS_PER_SEC;

    // Weryfikujemy
    czas = clock();
    rezultat = weryfikujSortowanie(sortowanaTablica, tablica, iloscElementow);
    czas = clock() - czas;
    czasWeryfikacjiWSekundach = ((double)czas) / CLOCKS_PER_SEC;

    if (rezultat) {
        printf("\tHeapSort zakonczony sukcesem w czasie %f sekund. Poprawnosc zweryfikowano w %f sekund.\n", czasSortowaniaWSekundach, czasWeryfikacjiWSekundach);
    }
    else {
        printf("\tHeapSort zakonczony NIEPOWODZENIEM w czasie %f sekund. Poprawnosc zweryfikowano w %f sekund.\n", czasSortowaniaWSekundach, czasWeryfikacjiWSekundach);
    }

    //
    // SELECTIONSORT
    //

    // Kopiujemy tablicę
    memcpy(sortowanaTablica, tablica, *iloscElementow * sizeof(int));

    // Sortujemy
    czas = clock();
    selectionSort(sortowanaTablica, iloscElementow);
    czas = clock() - czas;
    czasSortowaniaWSekundach = ((double)czas) / CLOCKS_PER_SEC;

    // Weryfikujemy
    czas = clock();
    rezultat = weryfikujSortowanie(sortowanaTablica, tablica, iloscElementow);
    czas = clock() - czas;
    czasWeryfikacjiWSekundach = ((double)czas) / CLOCKS_PER_SEC;

    if (rezultat) {
        printf("\tSelectionSort zakonczony sukcesem w czasie %f sekund. Poprawnosc zweryfikowano w %f sekund.\n", czasSortowaniaWSekundach, czasWeryfikacjiWSekundach);
    }
    else {
        printf("\tSelectionSort zakonczony NIEPOWODZENIEM w czasie %f sekund. Poprawnosc zweryfikowano w %f sekund.\n", czasSortowaniaWSekundach, czasWeryfikacjiWSekundach);
    }

    // Zwalniamy pamięć
    free(sortowanaTablica);
}

// Raczej nie używałbym, bo tablice mogą być kosmicznie duże.
// Jest inna funkcja, która weryfikuje poprawność sortowania.
void wypiszTablice(const int* tablica, const int* iloscElementow) {
    for (int i = 0; i < *iloscElementow; i++) {
        printf("%d", tablica[i]);
        if (i != *iloscElementow - 1) {
            printf(", ");
        } else {
            printf(".\n");
        }
    }
}

// Weryfikuje poprawność sortowania
bool weryfikujSortowanie(const int* posortowanaTablica, const int* nieposortowanaTablica, const int* iloscElementow) {
    // Sprawdzamy czy liczby rosną. Zaczynamy od -1, bo losujemy liczby od 0.
    // Innymi słowy nie możemy natrafić na mniejszą liczbę niż -1
    int liczba = -1;
    for (int i = 0; i < *iloscElementow; i++) {
        if (posortowanaTablica[i] < liczba) {
            return false;
        }
        liczba = posortowanaTablica[i];
    }

    // Sprawdzamy czy w obu tablicach jest ta sama ilość określonych liczb.
    // Jeśli w źródłowej były 3 liczby "26", to po sortowaniu dalej powinny być.
    for (int i = 0; i < *iloscElementow; i++) {
        int iloscWPosortowanej = 0;
        int iloscWNieposortowanej = 0;

        for (int j = 0; j < *iloscElementow; j++) {
            if (posortowanaTablica[j] == i) {
                iloscWPosortowanej++;
            }

            if (nieposortowanaTablica[j] == i) {
                iloscWNieposortowanej++;
            }
        }
        if (iloscWPosortowanej != iloscWNieposortowanej) {
            return false;
        }
    }

    return true;
}

// Zamienia dwie liczby w tablicy
void zamienLiczby(int* tablica, int lewyIndeks, int prawyIndeks) {
    int tymczasowa = tablica[lewyIndeks];
    tablica[lewyIndeks] = tablica[prawyIndeks];
    tablica[prawyIndeks] = tymczasowa;
}

void quickSort(int* tablica, int lewyIndeks, int prawyIndeks) {
    // Są różne sposoby implementacji, między innymi wartośćPodziału na
    // podstawie najdalej na prawo wysuniętego indeksu, np.:
    //		int* wartoscPodzialu = tablica[prawyIndeks];
    // Jest ona jednak podatna na niekorzystne ułożenie elementów i może
    // dojść do przepełnienia stosu. Lepiej losować indeks.
    int indeks = rand() % (prawyIndeks + 1 - lewyIndeks) + lewyIndeks;
    int wartoscPodzialu = tablica[indeks];

    int nowyLewyIndeks, nowyPrawyIndeks;
    nowyLewyIndeks = lewyIndeks;
    nowyPrawyIndeks = prawyIndeks;

    do {
        // Przesuwamy indeksy z lewej
        while (tablica[nowyLewyIndeks] < wartoscPodzialu) {
            nowyLewyIndeks++;
        }

        // Przesuwamy indeksy z prawej
        while (wartoscPodzialu < tablica[nowyPrawyIndeks]) {
            nowyPrawyIndeks--;
        }

        if (nowyLewyIndeks <= nowyPrawyIndeks) {
            // Konczymy krok, zamieniamy elementy
            zamienLiczby(tablica, nowyLewyIndeks, nowyPrawyIndeks);
            nowyLewyIndeks++;
            nowyPrawyIndeks--;
        }
    } while (nowyLewyIndeks <= nowyPrawyIndeks);

    // Jeśli jest lewa część..
    if (lewyIndeks < nowyPrawyIndeks) {
        // ..sorujemy ją
        quickSort(tablica, lewyIndeks, nowyPrawyIndeks);
    }

    // Jeśli jest prawa część..
    if (nowyLewyIndeks < prawyIndeks) {
        // ..sorujemy ją
        quickSort(tablica, nowyLewyIndeks, prawyIndeks);
    }
}

void heapSort(int* tablica, const int* iloscElementow) {
    // Budujemy kopiec
    for (int i = *iloscElementow / 2 - 1; i >= 0; i--) {
        heapSort_odbudujKopiec(tablica, *iloscElementow, i);
    }

    // Jeden po drugim wyciągamy elementy ze stosu
    for (int i = *iloscElementow - 1; i > 0; i--) {
        // Przenosimy aktualny indeks poczatkowy na koniec
        zamienLiczby(tablica, 0, i);

        // Odbudowujemy zmniejszony kopiec
        heapSort_odbudujKopiec(tablica, i, 0);
    }
}

void heapSort_odbudujKopiec(int* tablica, const int iloscElementow, int poczatkowyIndeks) {
    int indeksNajwyzszejWartosci = poczatkowyIndeks;
    int lewa = 2 * poczatkowyIndeks + 1;
    int prawa = 2 * poczatkowyIndeks + 2;

    // Jeśli lewa wartość jest większa od najwyzszej wartosci
    if (lewa < iloscElementow && tablica[lewa] > tablica[indeksNajwyzszejWartosci]) {
        indeksNajwyzszejWartosci = lewa;
    }

    // Jeśli prawa wartość jest większa od najwyzszej wartosci
    if (prawa < iloscElementow && tablica[prawa] > tablica[indeksNajwyzszejWartosci]) {
        indeksNajwyzszejWartosci = prawa;
    }

    // Jeśli indeks najwyższej wartości nie jest początkowym indeksem
    if (indeksNajwyzszejWartosci != poczatkowyIndeks) {
        zamienLiczby(tablica, poczatkowyIndeks, indeksNajwyzszejWartosci);

        // Recursywnie odbudowujemy kopiec
        heapSort_odbudujKopiec(tablica, iloscElementow, indeksNajwyzszejWartosci);
    }
}

void selectionSort(int* tablica, const int* iloscElementow) {
    int indeksNajnizszejWartosci;

    // Przesuwamy granicę nieposortowanej tablicy
    for (int i = 0; i < *iloscElementow - 1; i++) {
        // Odszukujemy najniższy element w nieposortowanej części tablicy
        indeksNajnizszejWartosci = i;
        for (int j = i + 1; j < *iloscElementow; j++) {
            if (tablica[j] < tablica[indeksNajnizszejWartosci]) {
                indeksNajnizszejWartosci = j;
            }
        }

        // Zamieniamy odszukaną najniższą wartość z pierwszym elementem nieposortowanej części tablicy
        zamienLiczby(tablica, indeksNajnizszejWartosci, i);
    }
}