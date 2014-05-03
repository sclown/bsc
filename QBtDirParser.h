#ifndef INCLUDED_QBtDirParser_h
#define INCLUDED_QBtDirParser_h

/********************************************************************
 * Copyright (C) Piotr Pszczolkowski
 *-------------------------------------------------------------------
 * This file is part of Beesoft Commander.
 *
 * Beesoft Commander is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * Beesoft Commander is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Beesoft Commander; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *-------------------------------------------------------------------
 * Project      : Beesoft Commander
 * File         : QBtDirParser.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 05.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QObject>

/*------- class declaration:
-------------------------------------------------------------------*/
/**
 * @brief Klasa sluzaca do odczytu zawartosci wskazanego katalogu.
 * @author Piotr Pszczolkowski
 * @date 2007.09.05
 *
 * Obiekt klasy odczytuje rekursywnie z dysku wszystkie podkatalogi
 * i znajdujace sie w nich pliki.<br> Istnieje takze mozliwosc 
 * usuwania wszystkich znalezionych podkatalogow i plikow.<br>
 * Informacje o znalezionych podkatalogach i plikach sa
 * w postaci sygnalow przekazywane do uzytkownika.
 */
class QBtDirParser : public QObject
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   /**
   * @brief Konstruktor domyslny inicjujacy zmienne uzywane w procesie.
   * @param in_remove Flaga okreslajaca czy w czasie procesu maja byc usuwane katalogi/pliki,
   * @param in_parent Wskaznik do macierzystego obiektu.
   *
   * Tylko tu, w konstruktorze mozna okreslic czy katalogi/pliki maja byc usuwane.<br>
   * Domyslnie nie maja byc usuwane.
   *
   * @see remove_
   */
   QBtDirParser( const bool in_remove = false, QObject* in_parent = 0 ) : QObject( in_parent )
   , break_       ( false     )
   , remove_      ( in_remove )
   , fname_muster_( QString() )
   , file_filter_ ( false     )
   {}
private:
   /**
   * @brief <b>Zablokowany</b> konstruktor kopiujacy.
   */
   QBtDirParser( const QBtDirParser& );

   /**
   * @brief <b>Zablokowany</b> operator przypisania.
   */
   QBtDirParser& operator=( const QBtDirParser& );

//******* MEMBERS *******
private:
   /**
   * @brief Flaga okreslajaca czy proces parsowania ma byc kontynuowany.
   *
   * Proces parsowania katalogu zostanie przerwany w momencie gdy flaga uzyska wartosc true.<br>
   * Podczas startu procesu flaga ma nadawana wartosc false.
   *
   * @see run(), cancel()
   */
   bool break_;

   /**
   * @brief Flaga okreslajaca czy elementy katalogu (pliki/podkatalogi) maja byc usuniete z dysku.
   *
   * Uzytkownik moze okreslic czy parser ma tylko parsowac (false), czy po sparsowaniu
   * kazdego elementu ma go usunac z dysku (true).<br>
   * Domyslnie flaga ma wartosc false, czyli obiekt tylko i wylacznie parsuje katalog.
   * @see QBtDirParser()
   */
   bool remove_;

   /**
   * @brief Lancuch tekstowy zawierajacy informacje o nazwach plikow, ktore maja byc znalezione.
   *
   * Lancuch ten zawiera maski poszukiwanych plikow oddzielonych znakiem srednika (;).<br>
   * Moga to byc kompletne nazwy plikow lub tak zwane dzikie znaki (wildcards).<br>
   * Lancuch pusty oznacza, ze beda odczytywane wszystkie pliki.<br>
   * Domyslnie lancuch jest pusty.
   *
   * @see run(), file_filter_
   */
   QString fname_muster_;

   /**
   * @brief Flaga okreslajaca czy maja byc odczytywane pliki ukryte (hidden).
   *
   * Jesli flaga ma wartosc true (filtr wlaczony) odczytywane sa tylko pliki
   * nieukryte. Wartosc false (filtr wylaczony) powoduje odczytywanie takze
   * plikow ukrytych.<br>
   * Domyslnie filtr jest wylaczony (false).
   *
   * @see run(), fname_muster_
   */
   bool file_filter_;

//******* METHODS *******
public:
   /**
   * @brief Funkcja inicjujaca i uruchamiajaca proces.
   * @param in_dir_path Pelna sciezka do parsowanego katalogu,
   * @param in_fname_muster Wzorce nazw szukanych plikow,
   * @param in_file_filter Flaga okreslajaca czy uwzgledniac pliki ukryte.
   *
   * Po zakonczeniu calego procesu wysylany jest sygnal <i>finished</i>.
   *
   * @see do_it(), finished(), fname_muster_, file_filter_, break_
   */
   void run ( const QString& in_dir_path,
              const QString& in_fname_muster = QString(),
              bool           in_file_filter  = false );

   /**
   * @brief Funkcja przerywajaca proces.
   *
   * Wywolanie tej funkcji powoduje ustawienie flagi <i>break_</i>,
   * ktora jest sprawdzana w kazdym kolejnym kroku procesu.
   *
   * @see break_
   */
   void cancel();
private:
   /**
   * @brief Pierwsza funkcja rekursywnego parsowania.
   * @param in_path Pelna sciezka do katalogu/pliku.
   *
   * Jesli przyslana sciezka dotyczy pliku wysylany jest sygnal current_item().<br>
   * Jesli przyslana sciezka dotyczy katalogu, wysylany jest sygnal change_dir(),
   * a nastepnie wywolywana jest funkcja odczytujaca jego zawartosc parse_dir().<br>
   * Jesli ustawiona jest flaga <i>remove_</i> kolejnym krokiem jest usuniecie
   * tego katalogu/pliku.<br>
   * Funckja ta wraz parse_dir() tworzy dwuelementowy zespol rekursywnego
   * odczytu drzewa podkatalogow.
   *
   * @see change_dir(), parse_dir(), cant_read_dir(), current_item(), remove_dir(), remove_file(), remove_
   */
   void do_it( const QString& in_path );

   /**
   * @brief Druga funkcja rekursywnego parsowania.
   * @param in_dir_path Pelna sciezka do katalogu.
   *
   * Funkcja odczytujaca zawartosc wskazanego katalogu (podkatalogi i pliki).<br>
   * Dla kazdego znalezionego katalogu/pliku wywolywana jest funkcja do_it().<br>
   *
   * Funkcja ta wraz z funkcja do_it() tworzy dwuelementowy zespol rekursywnego
   * odczytu drzewa katalogow.
   * 
   * @see do_it(), fname_muster_, file_filter_
   */
   void parse_dir( const QString& in_dir_path );

   /**
   * @brief Funkcja usuwajaca z dysku wskazany katalog.
   * @param in_dir_path Pelna sciezka do katalogu, ktory ma byc usuniety.
   *
   * Funkcja jest uzywana tylko i wylacznie gdy ustawiona jest flaga <i>remove_</i>.<br>
   * Poniewaz usuwany katalog musi byc pusty, wczesniej zostana z niego
   * usuniete wszystkie pliki i podkatalogi.<br>
   * W przypadku gdy operacja usuwania katalogu sie nie powiodla wysylany jest
   * sygnal <i>cant_remove_dir</i>.
   *
   * @see remove_, do_it(), cant_remove_dir()
   */
   void remove_dir( const QString& in_dir_path );

   /**
   * @brief Funkcja usuwajaca z dysku wskazany plik.
   * @param in_file_path Pelna sciezka do pliku, ktory ma byc usuniety.
   *
   * Funkcja jest uzywana tylko i wylacznie gdy ustawiona jest flaga <i>remove_</i>.<br>
   * W przypadku gdy operacja usuwania pliku sie niepowiodla wysylany jest
   * jest sygnal <i>cant_remove_file</i>.
   *
   * @see remove_, do_it(), cant_remove_file()
   */
   void remove_file( const QString& in_file_path );
signals:
   /**
   * @brief Sygnal wysylany w momencie wejscia do podkatalogu.
   * @param out_dir_path Pelna sciezka do podkatalogu.
   *
   * Sygnal ma znaczenie czysto informacyjne.<br>
   * Uzytkownik, ktory podlaczy sie pod ten sygnal moze wyswietlic
   * w odpowiednim polu nazwe aktualnie przetwarzanego katalogu.<br>
   * Po przeczytaniu calej zawartosci podkatalogu istnienie tez mozliwosc
   * jego usuniecia.
   *
   * @see remove_, QBtDirParser()
   */
   void change_dir( const QString& out_dir_path );
 
   /**
   * @brief Sygnal wysylany w momencie odczytania informacji o istnieniu pliku.
   * @param out_file_path Pelna sciezka do pliku.
   *
   * Sygnal ma znaczenie zarowno informacyjne jak i praktyczne.<br>
   * Uzytkownik dostajac kompletna sciezke do pliku ma mozliwosc jego przetwarzania,
   * jak rowniez wyswietlenie informacji o tym fakcie.
   */
   void current_item( const QString& out_file_path );

   /**
   * @brief Sygnal wysylany w sytuacji gdy uzytkownik nie ma uprawnien do
   *        odczytu katalogu.
   * @param out_dir_path Sciezka do katalogu, ktorego nie mozna odczytac.
   *
   * Sygnal ma znaczenie czysto informacyjne.<br>
   * Uzytkownik moze (ale nie musi) wyswietlic informacje o tym fakcie.
   *
   * @see do_it()
   */
   void cant_read_dir( const QString& out_dir_path );

   /**
   * @brief Sygnal wysylany po nieudanej probie usuniecia katalogu.
   * @param out_dir_path Pelna sciezka do katalogu.
   *
   * Sytuacja ta moze wystapic wowczas gdy uzytkownik ustawil flage remove_,
   * a nie ma on wystarczajacych uprawnien do usuniecia katalogu.
   *
   * @see remove_, QBtDirParser(), remove_dir().
   */
   void cant_remove_dir( const QString& out_dir_path );

   /**
   * @brief Sygnal wysylany po nieudanej probie usuniecia pliku.
   * @param out_file_path Pelna sciezka do pliku.
   *
   * Sytuacja ta moze wystapic wowczas gdy uzytkownik ustawil flage remove_,
   * a nie ma on wystarczajacych uprawnien do usuniecia pliku.
   *
   * @see remove_, QBtDirParser(), remove_file()
   */
   void cant_remove_file( const QString& out_file_path );

   /**
   * @brief Sygnal wysylany po zakonczeniu procesu parsowania.
   *
   * Sygnal ma znaczenie czysto informacyjne.<br>
   * Uzytkownik ma mozliwosc wykonania czynnosci pooperacyjnych (np. zmiana wygladu kursora).
   *
   * @see run()
   */
   void finished();
};

#endif // INCLUDED_QBtDirParser_h
