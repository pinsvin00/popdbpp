#include <iostream>

using namespace std;

const int S_MAX = 100; // rozmiar stosu operatorów

// Zwraca priorytet operatora
//---------------------------

int p ( char c )
{
  switch ( c )
  {
    case '+'     : ;
    case '-'     : return 1;
    case '*'     : ;
    case '/'     : return 2;
    case '^'     : return 3;
  }
  return 0;
}

//------------------------------------
// Tutaj rozpoczyna się program główny
//------------------------------------

int main( )
{
  char S [ S_MAX ];                // stos operatorów
  int sptr = 0;                    // wskaźnik stosu
  char c;                          // kolejny znak wyrażenia

  while( true )
  {
    cin >> c;                      // czytamy znak z wejścia

    if( c == '=' )                 // koniec wyrażenia?
    {
      while( sptr ) cout << S [ --sptr ] << ' ';  // jeśli tak, na wyjście przesyłamy
                                   // wszystkie operatory ze stosu
      break;                       // i przerywamy pętlę
    }

    switch ( c )                   // analizujemy odczytany znak
    {
      case ' ' : break;            // spację ignorujemy
      case '(' : S [ sptr++ ] = '('; // nawias otwierający zawsze na stos
                 break;
      case ')' : while( S [ sptr-1 ] != '(' ) // nawias zamykający
                 cout << S [ --sptr ] << ' '; // ze stosu przesyłamy na wyjście
                 sptr--;           // usuwamy ze stosu nawias otwierający
                 break;
      case '+' : ;                 // operator
      case '-' : ;
      case '*' : ;
      case '/' : ;
      case '^' : while( sptr )
                 {
                   if( ( p ( c ) == 3 ) || ( p ( c ) > p ( S [ sptr - 1 ] ) ) ) break;
                   cout << S [ --sptr ] << ' '; // na wyjście przesyłamy ze stosu wszystkie
                 }                 // operatory o wyższych priorytetach
                 S [ sptr++ ] = c; // operator umieszczamy na stosie
                 break;
      default:   cout << c << ' '; // inaczej znak przesyłamy na wyjście
                 break;
    }
  }

  cout << endl;

  return 0;
} 
