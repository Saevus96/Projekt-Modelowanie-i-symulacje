#include<iostream>
#include<cmath>
#include<ctime>
#include<fstream>

//Generator losowej liczby rzeczywistej z przedzia�u od [p;q]
double getRandomRealValue(double p, double q) {
	return double((rand()) / double(RAND_MAX)) * (q - p) + p;
}

//Generator losowej liczby rzeczywistej z przedzia�u [0;1]
double getRandomFrom01() {
	return double(rand()) / double(RAND_MAX);
}

// Generator losowej liczby ca�kowitej z przedzia�u [p;q]
int getRandomFromInteger(int p, int q) {
	return rand() % (q - p + 1) + p;
}

//Metoda "Boxa Mullera" do generowania losowej liczby o rozk�adzie normalnym
double boxMullerMethod(double mi, double sigma)
{
	double y1;
	double y2;
	do {
		y1 = getRandomFrom01();
		y2 = getRandomFrom01();
	} while (y1 == 0.0 || y2 == 0.0);

	double r = sqrt(-2. * log(y1));
	double theta = 2. * 3.14*y2;

	double z = r * cos(theta);

	return z * sigma + mi;
}

/*		***Etap 1***   $$$Generator liczby o rozk�adzie liniowym$$$

	Etap 1 polega na wylosowaniu ilo�� pand kt�re urodzi�y si� danego roku, liczba pand kt�re si� urodzi�y
	zale�na jest od ilo�� Pand zarejestrowanych na pocz�tku ka�dego roku.
	Etap 1 posiada 3 kana�y

	Kana� 1: Du�a ilo�� pand gdy liczba zarejestrowanych pand jest wi�ksza od 1300 pand. Ilos� pand losowana z 
			 przedzia�u od [35;55].
	Kana� 2: �rednia ilo�� pand gdy liczba zarejestrowanych pand jest nie wi�ksza od 1300 i wi�ksza od 700.
			 Ilo�� pand losowana z przedzia�u [20;35].
	Kana� 3: Ma�a ilos� pand, gdy liczba zarejestrowanych pand jest nie wi�ksza od 700.
			 Ilo�� pand losowana z przedzia�u [5;20];
*/
double etap1(int numberOfBears) {
	//liczba narodzin pand danego roku
	double numberOfBirths = 0.0;

	//Kana� 1
	if (numberOfBears > 1300) {
		numberOfBirths = getRandomRealValue(35, 55);
	}
	//Kana� 2
	else if (numberOfBears <= 1300 && numberOfBears > 700) {
		numberOfBirths = getRandomRealValue(20, 35);
	}
	//Kana� 3
	else if (numberOfBears <= 700) {
		numberOfBirths = getRandomRealValue(5, 20);
	}

	return numberOfBirths;
}

/*		***Etap 2***   $$$Generator o rozk�adzie dyskretnym$$$

	Etap 2 polega na wylosowaniu ilo�� pand, kt�re umar�y z przyczyn naturalnych.
	Etap 2 zawiera 3 kana�y. Ka�dy kana� zale�ny jest od wsp�czynnika umieralno�ci wylosowanego ka�dego roku.
	Wsp�czynnik umieralno�� zale�ny od Generatora o rozk�adzie dyskretnym
	
	Kana� 1: Ma�a umieralno�� pand, gdy wsp�czynnik jest mniejszy lub r�wny 0.33 -> umar�o 10 pand
	Kana� 2: �rednia umieralno�� pand, gdy wsp�czynnik jest mniejszy lub r�wny 0.66 i wi�kszy od 0.33 -> umar�o 15 pand.
	Kana� 3: Du�a umieralno�� pand, gdy wsp�czynnik jest mniejszy lub r�wny 1 i wi�kszy od 0.66 -> umar�o 20 pand.


*/

double etap2() {

	//wspolczynnik umieralnosci na dany rok
	double densityOfDeaths = getRandomFrom01();

	//ilosc pand ktore umarly danego roku 
	double numberOfDeaths = 0.0;
	//Kana� 1
	if (densityOfDeaths <= 0.33) {
		numberOfDeaths = 10;
	}
	//Kana� 2
	else if (densityOfDeaths > 0.33 && densityOfDeaths <= 0.66) {
		numberOfDeaths = 15;
	}
	//Kana� 3
	else if (densityOfDeaths <= 1 && densityOfDeaths > 0.66) {
		numberOfDeaths = 20;
	}

	return numberOfDeaths;
}

/*		***Etap 3***	$$$Generator o rozk�adzie normalnym$$$

	Etap 3 polega na wylosowaniu ilo�� pand, kt�re zgin�y z r�k os�b poluj�cych na nie.
	Etap 3 zawiera 2 kana�y.
	Ilo�� os�b poluj�cych losowana jest za pomoc� generatora liczby ca�kowitej.
	Ilo�� pand kt�re zgin�y losowana za pomoc� generatora o rozk�adzie normalnym.
	Natomiast liczba pand kt�re zgin� z r�k os�b poluj�cych jest zale�na od generatora liczby losowej
	o rozk�adzie normalnym

	Kana� 1: Ma�a liczba pand. Je�eli poluj�cych by�o 20 lub mniej ni� 20 na rok to liczba pand kt�re zgin�y by�a ma�a
	Liczba pand generowana Boxem-Mullerem -> mi 15, sigma 0.5;
	Kana� 2: Du�a liczba pand. Je�eli poluj�cych by�o wi�cej ni� 20 na rok to liczba pand kt�re zgin�y by�a du�a.
	Liczba pand generowana Boxem-Mullerem -> mi 25, sigma 0.5;
*/

double etap3() {
	//ilosc polujacych na dany rok
	int numberOfPoachers = getRandomFromInteger(10, 40);

	//ilosc pand ktore umarly na dany rok
	double numberOfDeaths = 0.0;
	//Kana� 1
	if (numberOfPoachers <= 20) {
		numberOfDeaths = boxMullerMethod(15, 0.5);
	}
	//Kana� 2
	else if(numberOfPoachers > 20){
		numberOfDeaths = boxMullerMethod(25, 0.5);
	}

	return numberOfDeaths;
}

/*		***Etap 4***	$$$Generator o rozk�adzie normalnym$$$

	Etap 4 polega na wylosowaniu ilo�ci pand kt�re zgin�y ze wzgl�du na ilo�c km^2 wyci�tych las�w.
	Etap 4 sk�ada si� z 2 kana��w.
	Ka�dy kana� zale�ny jest od km^2 wyci�tych las�w bambusowych.
	Ilo�� kilometr�w kwadratowych zawiera si� w przedziale [1;10] generowana za pomoc� generatora losowej liczby rzeczywistej.
	Liczba pand kt�re zgin�y losowane s� za pomoc� generatora o rozk�adzie normalnym.

	Kana� 1: Ma�a ilo�� pand. Je�eli ilo�� km^2 wyci�tych las�w bambusowych mniejsza rowna 5tysi�cy km^2.
			 Ma�a ilo�c pand losowana Boxem Mullera gdzie mi 15, a sigma 1
	Kana� 2: Du�a ilo�� pand. Je�eli ilo�� km^2 wyci�tych las�w bambusowych wi�ksza od 5tysi�cy km^2.
			 Du�a ilo�c pand losowana Boxem Mullera gdzie mi 25, a sigma 1


*/
double etap4() {
	int numberOfKilometers = getRandomRealValue(1.0, 10.0);
	double numberOfDeaths = 0.0;
	//Kana� 1
	if (numberOfKilometers <= 5.0) {
		numberOfDeaths = boxMullerMethod(15, 1.0);
	}
	//Kana� 2
	else if (numberOfKilometers > 5.0) {
		numberOfDeaths = boxMullerMethod(25.0, 1.0);
	}

	return numberOfDeaths;
}

/*		***Etap 5***	$$$Generator o rozk�adzie normalnym$$$

	Etap 5 polega na wylosowaniu ilo�ci pand kt�re zgin�y ze wzgl�du na ilo�� kilogram�w bambusa kt�ry obumar�.
	Etap 5 sk�ada si� z 2 kana��w.
	Ka�dy kana� zale�ny jest od kilogram�w bambusa kt�ry obumar�.
	Ilo�� kiglogram�w bambusa zawiera si� w przedziale [0;5] tysiecy kilogram�w generowana za pomoc� generatora losowej liczby rzeczywistej.
	Liczba pand kt�re zgin�y losowane s� za pomoc� generatora o rozk�adzie normalnym.

	Kana� 1: Ma�a ilo�� pand. Je�eli ilo�� kilogram�w bambusa kt�ry obumar� mniejsza rowna 2,5 tysi�ca kilogram�w.
			 Ma�a ilo�c pand losowana Boxem Mullera gdzie mi 10, a sigma 0.5
	Kana� 2: Du�a ilo�� pand. Je�eli ilo�� km^2 wyci�tych las�w bambusowych wi�ksza od 2,5tysi�cy km^2.
			 Du�a ilo�c pand losowana Boxem Mullera gdzie mi 20, a sigma 0.5


*/

double etap5() {
	
	int numberOfKilograms = getRandomRealValue(0.0, 5.0);
	double numberOfDeaths = 0.0;
	//Kana� 1
	if (numberOfKilograms <= 2.5) {
			numberOfDeaths = boxMullerMethod(10, 0.5);
		}
	//Kana� 2
	else if (numberOfKilograms > 2.5) {
		numberOfDeaths = boxMullerMethod(20.0, 0.5);
	}

	return numberOfDeaths;
	
}
int main() {

	//Funkcja inicjalizuj�ca funkcj� rand(), przy ka�dym uruchomieniu programu uzyskujemy inn� sekwencj� liczb losowych
	srand(time(NULL));

	//otwarcie pliku
	std::fstream p;
	p.open("plik.txt", std::ios::out);

	//Symulowanie 2000 liczb metod� Monte Carlo 
	int numberOfBears = 1500;
	for (int i = 0; i < 2000; i++) {
		int numberOfBears = 1500;
		int years = 0;
		while (numberOfBears > 0) {
			numberOfBears += int(etap1(numberOfBears));
			numberOfBears -= int(etap2());
			numberOfBears -= int(etap3());
			numberOfBears -= int(etap4());
			numberOfBears -= int(etap5());

			//jezeli liczba pand mniejsza od zera nie zako�cz generowanie
			if (numberOfBears < 0) {
				break;
			}
			years++;
		}
		std::cout << years << std::endl;
		p << years << std::endl;
	}


	system("pause");
	return 0;
}