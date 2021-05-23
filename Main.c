#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Zajecia_QS_Pawel_Zborowski_ASD_2021
//
// DEKLARACJE ZAWARTYCH FUNKCJI
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
// G��WNA FUNKCJA MAIN
//
int main()
{
	// Inicjalizujemy losowo��;
	time_t czas;
	srand((unsigned)time(&czas));

	// Alokujemy pami��
	int* iloscElementow = (int*)malloc(sizeof(int));
	do {
		printf("Wprowadz rozmiar tablicy lub liczbe niedodatnia by zamknac program.\n");

		// Wczytujemy rozmiar tablicy
		bool wczytano = false;
		while (!wczytano) {
			wczytano = scanf_s("%d", iloscElementow) == 1;
			
			// Czy�cimy bufor - czytamy a� dojdziemy do znaku nowej linii
			while (getchar() != '\n') {
				wczytano = false;
			}

			if (!wczytano) {
				printf("Wprowadzono bledne dane! Sprobuj ponownie.\n");
			}
		}

		if (*iloscElementow <= 0) {
			// Zwalniamy pami��
			free(iloscElementow);
			// Zamykamy program
			return 0;
		}
		printf("\n");

		// Generujemy losow� tablic�
		const int* losowaTablica = generujLosowaTablice(iloscElementow);
		// wypiszTablice(losowaTablica, iloscElementow);
		// Wywo�ujemy algorytmy sortowania
		porownajCzasyDlaTablicy("Tablica z losowymi wartosciami", losowaTablica, iloscElementow);
		// Zwalniamy pami��
		free(losowaTablica);

		// Generujemy rosn�c� tablic�
		losowaTablica = generujRosnacaTablice(iloscElementow);
		// Wywo�ujemy algorytmy sortowania
		porownajCzasyDlaTablicy("Tablica z rosnacymi wartosciami", losowaTablica, iloscElementow);
		// Zwalniamy pami��
		free(losowaTablica);

		// Generujemy losow� tablic�
		losowaTablica = generujMalejacaTablice(iloscElementow);
		// Wywo�ujemy algorytmy sortowania
		porownajCzasyDlaTablicy("Tablica z malejacymi wartosciami", losowaTablica, iloscElementow);
		// Zwalniamy pami��
		free(losowaTablica);

		printf("\n\n");
	} while (true);
}

//
// DEFINICJE FUNKCJI
// 
int* generujLosowaTablice(const int* iloscElementow) {
	// Alokujemy pami��
	int* tablica = (int*)malloc(*iloscElementow * sizeof(int));
	// Generujemy losow� tablic� o zakresie warto�ci od <0,iloscElementow)
	for (int i = 0; i < *iloscElementow; i++) {
		tablica[i] = rand() % *iloscElementow;
	}
	// Zwracamy tablic�
	return tablica;
}

int* generujRosnacaTablice(const int* iloscElementow) {
	// Alokujemy pami��
	int* tablica = (int*)malloc(*iloscElementow * sizeof(int));
	// Generujemy rosn�c� tablic�
	for (int i = 0; i < *iloscElementow; i++) {
		tablica[i] = i;
	}
	// Zwracamy tablic�
	return tablica;
}

int* generujMalejacaTablice(const int* iloscElementow) {
	// Alokujemy pami��
	int* tablica = (int*)malloc(*iloscElementow * sizeof(int));
	// Generujemy malej�c� tablic�
	for (int i = 0; i < *iloscElementow; i++) {
		tablica[i] = *iloscElementow - i - 1;
	}
	// Zwracamy tablic�
	return tablica;
}

void porownajCzasyDlaTablicy(const char* rodzajTablicy, const int* tablica, const int* iloscElementow) {
	printf("%s z %d elementami.\n", rodzajTablicy, *iloscElementow);
	clock_t czas;

	//
	// QUICKSORT
	//

	// Kopiujemy tablic�
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

	// Kopiujemy tablic�
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

	// Kopiujemy tablic�
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

	// Zwalniamy pami��
	free(sortowanaTablica);
}

// Raczej nie u�ywa�bym, bo tablice mog� by� kosmicznie du�e. 
// Jest inna funkcja, kt�ra weryfikuje poprawno�� sortowania.
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

// Weryfikuje poprawno�� sortowania
bool weryfikujSortowanie(const int* posortowanaTablica, const int* nieposortowanaTablica, const int* iloscElementow) {
	// Sprawdzamy czy liczby rosn�. Zaczynamy od -1, bo losujemy liczby od 0.
	// Innymi s�owy nie mo�emy natrafi� na mniejsz� liczb� ni� -1
	int liczba = -1;
	for (int i = 0; i < *iloscElementow; i++) {
		if (posortowanaTablica[i] < liczba) {
			return false;
		}
		liczba = posortowanaTablica[i];
	}

	// Sprawdzamy czy w obu tablicach jest ta sama ilo�� okre�lonych liczb.
	// Je�li w �r�d�owej by�y 3 liczby "26", to po sortowaniu dalej powinny by�.
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
	// S� r�ne sposoby implementacji, mi�dzy innymi warto��Podzia�u na 
	// podstawie najdalej na prawo wysuni�tego indeksu, np.:
	//		int* wartoscPodzialu = tablica[prawyIndeks];
	// Jest ona jednak podatna na niekorzystne u�o�enie element�w i mo�e 
	// doj�� do przepe�nienia stosu. Lepiej losowa� indeks.
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

	// Je�li jest lewa cz��..
	if (lewyIndeks < nowyPrawyIndeks) {
		// ..sorujemy j�
		quickSort(tablica, lewyIndeks, nowyPrawyIndeks);
	}

	// Je�li jest prawa cz��..
	if (nowyLewyIndeks < prawyIndeks) {
		// ..sorujemy j�
		quickSort(tablica, nowyLewyIndeks, prawyIndeks);
	}
}

void heapSort(int* tablica, const int* iloscElementow) {
	// Budujemy kopiec
	for (int i = *iloscElementow / 2 - 1; i >= 0; i--) {
		heapSort_odbudujKopiec(tablica, *iloscElementow, i);
	}

	// Jeden po drugim wyci�gamy elementy ze stosu
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

	// Je�li lewa warto�� jest wi�ksza od najwyzszej wartosci
	if (lewa < iloscElementow && tablica[lewa] > tablica[indeksNajwyzszejWartosci]) {
		indeksNajwyzszejWartosci = lewa;
	}

	// Je�li prawa warto�� jest wi�ksza od najwyzszej wartosci
	if (prawa < iloscElementow && tablica[prawa] > tablica[indeksNajwyzszejWartosci]) {
		indeksNajwyzszejWartosci = prawa;
	}

	// Je�li indeks najwy�szej warto�ci nie jest pocz�tkowym indeksem
	if (indeksNajwyzszejWartosci != poczatkowyIndeks) {
		zamienLiczby(tablica, poczatkowyIndeks, indeksNajwyzszejWartosci);

		// Recursywnie odbudowujemy kopiec
		heapSort_odbudujKopiec(tablica, iloscElementow, indeksNajwyzszejWartosci);
	}
}

void selectionSort(int* tablica, const int* iloscElementow) {
	int indeksNajnizszejWartosci;

	// Przesuwamy granic� nieposortowanej tablicy
	for (int i = 0; i < *iloscElementow - 1; i++) {
		// Odszukujemy najni�szy element w nieposortowanej cz�ci tablicy
		indeksNajnizszejWartosci = i;
		for (int j = i + 1; j < *iloscElementow; j++) {
			if (tablica[j] < tablica[indeksNajnizszejWartosci]) {
				indeksNajnizszejWartosci = j;
			}
		}

		// Zamieniamy odszukan� najni�sz� warto�� z pierwszym elementem nieposortowanej cz�ci tablicy
		zamienLiczby(tablica, indeksNajnizszejWartosci, i);
	}
}