#include <iostream>
#include <fstream>

void przyk_makro5()
{


	// wczytanie danych

	Int_t n = 850, i, j;

	Float_t x1[2000], x2[850], x3[850], x4[850];

	ifstream d;

	d.open("D:\\plik.txt");

	for (i = 0; i <2000; i = i + 1)
	{
		d >> x1[i];

	}
	d.close();

	// deklarowanie histogramu "jednowymiarowego"  .... liczba klas, przedział
	/*TGraph *gr3 = new TGraph(n,x1,x4);
	gr3->Draw("ALP");*/
	
	TH1F *im1 = new TH1F("im1", "Histogram jednowymiarowy", 40, 20, 60);
	TF1 *f1 = new TF1("fit","gaus",20,50);



	for (i = 0; i < 2000; i = i + 1)
	{
		im1->Fill(x1[i]);
	}



	// opcje rysowania histogramu
	im1->SetFillColor(2);
	im1->SetFillStyle(3002);
	im1->Fit(f1);
	im1->Draw();
}
