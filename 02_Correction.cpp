#include <iostream>
#include <string>
#include <iomanip>  // pour std::setw et std::setfill
#include <bitset>   // pour afficher en binaire

using namespace std;

int main() {
    unsigned int hexColor;
    cout << "Entrez une couleur au format hexadécimal (ex: #FF5733 ou FF5733) : ";
    cin >> hex >> hexColor;

    // Extraction avec opérateurs binaires
    unsigned int red = (hexColor >> 16) & 0xFF; // 8 bits de poids fort
    unsigned int green = (hexColor >> 8) & 0xFF; // 8 bits du milieu
    unsigned int blue = hexColor & 0xFF; // 8 bits de poids faible

    // Affichage des résultats
    cout << "\n--- Resultats ---" << endl;
    cout << "Couleur hexadecimale complete : #" << hex << hexColor << dec << endl;

    cout << "\nComposantes : " << endl;
    cout << "Rouge : " << endl;
    cout << "  Decimal : " << red << endl;
    cout << "  Hexadecimal : 0x" << hex << uppercase << setw(2) << setfill('0') << red << nouppercase << dec << endl;
    cout << "  Binaire : " << bitset<8>(red) << endl;

    cout << "\nVert : " << endl;
    cout << "  Decimal : " << green << endl;
    cout << "  Hexadecimal : 0x" << hex << uppercase << setw(2) << setfill('0') << green << nouppercase << dec << endl;
    cout << "  Binaire : " << bitset<8>(green) << endl;

    cout << "\nBleu : " << endl;
    cout << "  Decimal : " << blue << endl;
    cout << "  Hexadecimal : 0x" << hex << uppercase << setw(2) << setfill('0') << blue << nouppercase << dec << endl;
    cout << "  Binaire : " << bitset<8>(blue) << endl;

    return 0;
}
