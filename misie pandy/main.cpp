#include<iostream>
#include<cmath>
#include<ctime>
#include<fstream>

//Generator losowej liczby rzeczywistej z przedzia³u od [p;q]
double getRandomRealValue(double p, double q) {
	return double((rand()) / double(RAND_MAX)) * (q - p) + p;
}

//Generator losowej liczby rzeczywistej z przedzia³u [0;1]
double getRandomFrom01() {
	return double(rand()) / double(RAND_MAX);
}

// Generator losowej liczby ca³kowitej z przedzia³u [p;q]
int getRandomFromInteger(int p, int q) {
	return rand() % (q - p + 1) + p;
}

//Metoda "Boxa Mullera" do generowania losowej liczby o rozk³adzie normalnym
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

/*		***Etap 1***   $$$Generator liczby o rozk³adzie liniowym$$$

	Etap 1 polega na wylosowaniu iloœæ pand które urodzi³y siê danego roku, liczba pand które siê urodzi³y
	zale¿na jest od iloœæ Pand zarejestrowanych na pocz¹tku ka¿dego roku.
	Etap 1 posiada 3 kana³y

	Kana³ 1: Du¿a iloœæ pand gdy liczba zarejestrowanych pand jest wiêksza od 1300 pand. Ilosæ pand losowana z 
			 przedzia³u od [35;55].
	Kana³ 2: Œrednia iloœæ pand gdy liczba zarejestrowanych pand jest nie wiêksza od 1300 i wiêksza od 700.
			 Iloœæ pand losowana z przedzia³u [20;35].
	Kana³ 3: Ma³a ilosæ pand, gdy liczba zarejestrowanych pand jest nie wiêksza od 700.
			 Iloœæ pand losowana z przedzia³u [5;20];
*/
double etap1(int numberOfBears) {
	//liczba narodzin pand danego roku
	double numberOfBirths = 0.0;

	//Kana³ 1
	if (numberOfBears > 1300) {
		numberOfBirths = getRandomRealValue(35, 55);
	}
	//Kana³ 2
	else if (numberOfBears <= 1300 && numberOfBears > 700) {
		numberOfBirths = getRandomRealValue(20, 35);
	}
	//Kana³ 3
	else if (numberOfBears <= 700) {
		numberOfBirths = getRandomRealValue(5, 20);
	}

	return numberOfBirths;
}

/*		***Etap 2***   $$$Generator o rozk³adzie dyskretnym$$$

	Etap 2 polega na wylosowaniu iloœæ pand, które umar³y z przyczyn naturalnych.
	Etap 2 zawiera 3 kana³y. Ka¿dy kana³ zale¿ny jest od wspó³czynnika umieralnoœci wylosowanego ka¿dego roku.
	Wspó³czynnik umieralnoœæ zale¿ny od Generatora o rozk³adzie dyskretnym
	
	Kana³ 1: Ma³a umieralnoœæ pand, gdy wspó³czynnik jest mniejszy lub równy 0.33 -> umar³o 10 pand
	Kana³ 2: Œrednia umieralnoœæ pand, gdy wspó³czynnik jest mniejszy lub równy 0.66 i wiêkszy od 0.33 -> umar³o 15 pand.
	Kana³ 3: Du¿a umieralnoœæ pand, gdy wspó³czynnik jest mniejszy lub równy 1 i wiêkszy od 0.66 -> umar³o 20 pand.


*/

double etap2() {

	//wspolczynnik umieralnosci na dany rok
	double densityOfDeaths = getRandomFrom01();

	//ilosc pand ktore umarly danego roku 
	double numberOfDeaths = 0.0;
	//Kana³ 1
	if (densityOfDeaths <= 0.33) {
		numberOfDeaths = 10;
	}
	//Kana³ 2
	else if (densityOfDeaths > 0.33 && densityOfDeaths <= 0.66) {
		numberOfDeaths = 15;
	}
	//Kana³ 3
	else if (densityOfDeaths <= 1 && densityOfDeaths > 0.66) {
		numberOfDeaths = 20;
	}

	return numberOfDeaths;
}

/*		***Etap 3***	$$$Generator o rozk³adzie normalnym$$$

	Etap 3 polega na wylosowaniu iloœæ pand, które zginê³y z r¹k osób poluj¹cych na nie.
	Etap 3 zawiera 2 kana³y.
	Iloœæ osób poluj¹cych losowana jest za pomoc¹ generatora liczby ca³kowitej.
	Iloœæ pand które zginê³y losowana za pomoc¹ generatora o rozk³adzie normalnym.
	Natomiast liczba pand które zginê³ z r¹k osób poluj¹cych jest zale¿na od generatora liczby losowej
	o rozk³adzie normalnym

	Kana³ 1: Ma³a liczba pand. Je¿eli poluj¹cych by³o 20 lub mniej ni¿ 20 na rok to liczba pand które zginê³y by³a ma³a
	Liczba pand generowana Boxem-Mullerem -> mi 15, sigma 0.5;
	Kana³ 2: Du¿a liczba pand. Je¿eli poluj¹cych by³o wiêcej ni¿ 20 na rok to liczba pand które zginê³y by³a du¿a.
	Liczba pand generowana Boxem-Mullerem -> mi 25, sigma 0.5;
*/

double etap3() {
	//ilosc polujacych na dany rok
	int numberOfPoachers = getRandomFromInteger(10, 40);

	//ilosc pand ktore umarly na dany rok
	double numberOfDeaths = 0.0;
	//Kana³ 1
	if (numberOfPoachers <= 20) {
		numberOfDeaths = boxMullerMethod(15, 0.5);
	}
	//Kana³ 2
	else if(numberOfPoachers > 20){
		numberOfDeaths = boxMullerMethod(25, 0.5);
	}

	return numberOfDeaths;
}

/*		***Etap 4***	$$$Generator o rozk³adzie normalnym$$$

	Etap 4 polega na wylosowaniu iloœci pand które zginê³y ze wzglêdu na iloœc km^2 wyciêtych lasów.
	Etap 4 sk³ada siê z 2 kana³ów.
	Ka¿dy kana³ zale¿ny jest od km^2 wyciêtych lasów bambusowych.
	Iloœæ kilometrów kwadratowych zawiera siê w przedziale [1;10] generowana za pomoc¹ generatora losowej liczby rzeczywistej.
	Liczba pand które zginê³y losowane s¹ za pomoc¹ generatora o rozk³adzie normalnym.

	Kana³ 1: Ma³a iloœæ pand. Je¿eli iloœæ km^2 wyciêtych lasów bambusowych mniejsza rowna 5tysiêcy km^2.
			 Ma³a iloœc pand losowana Boxem Mullera gdzie mi 15, a sigma 1
	Kana³ 2: Du¿a iloœæ pand. Je¿eli iloœæ km^2 wyciêtych lasów bambusowych wiêksza od 5tysiêcy km^2.
			 Du¿a iloœc pand losowana Boxem Mullera gdzie mi 25, a sigma 1


*/
double etap4() {
	int numberOfKilometers = getRandomRealValue(1.0, 10.0);
	double numberOfDeaths = 0.0;
	//Kana³ 1
	if (numberOfKilometers <= 5.0) {
		numberOfDeaths = boxMullerMethod(15, 1.0);
	}
	//Kana³ 2
	else if (numberOfKilometers > 5.0) {
		numberOfDeaths = boxMullerMethod(25.0, 1.0);
	}

	return numberOfDeaths;
}

/*		***Etap 5***	$$$Generator o rozk³adzie normalnym$$$

	Etap 5 polega na wylosowaniu iloœci pand które zginê³y ze wzglêdu na iloœæ kilogramów bambusa który obumar³.
	Etap 5 sk³ada siê z 2 kana³ów.
	Ka¿dy kana³ zale¿ny jest od kilogramów bambusa który obumar³.
	Iloœæ kiglogramów bambusa zawiera siê w przedziale [0;5] tysiecy kilogramów generowana za pomoc¹ generatora losowej liczby rzeczywistej.
	Liczba pand które zginê³y losowane s¹ za pomoc¹ generatora o rozk³adzie normalnym.

	Kana³ 1: Ma³a iloœæ pand. Je¿eli iloœæ kilogramów bambusa który obumar³ mniejsza rowna 2,5 tysi¹ca kilogramów.
			 Ma³a iloœc pand losowana Boxem Mullera gdzie mi 10, a sigma 0.5
	Kana³ 2: Du¿a iloœæ pand. Je¿eli iloœæ km^2 wyciêtych lasów bambusowych wiêksza od 2,5tysiêcy km^2.
			 Du¿a iloœc pand losowana Boxem Mullera gdzie mi 20, a sigma 0.5


*/

double etap5() {
	
	int numberOfKilograms = getRandomRealValue(0.0, 5.0);
	double numberOfDeaths = 0.0;
	//Kana³ 1
	if (numberOfKilograms <= 2.5) {
			numberOfDeaths = boxMullerMethod(10, 0.5);
		}
	//Kana³ 2
	else if (numberOfKilograms > 2.5) {
		numberOfDeaths = boxMullerMethod(20.0, 0.5);
	}

	return numberOfDeaths;
	
}
int main() {

	//Funkcja inicjalizuj¹ca funkcjê rand(), przy ka¿dym uruchomieniu programu uzyskujemy inn¹ sekwencjê liczb losowych
	srand(time(NULL));

	//otwarcie pliku
	std::fstream p;
	p.open("plik.txt", std::ios::out);

	//Symulowanie 2000 liczb metod¹ Monte Carlo 
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

			//jezeli liczba pand mniejsza od zera nie zakoñcz generowanie
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