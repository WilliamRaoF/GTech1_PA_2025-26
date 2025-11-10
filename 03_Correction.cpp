#include <iostream>
#include <iomanip>
using namespace std;

int getR(unsigned int p) { return (p >> 16) & 0xFF; }
int getG(unsigned int p) { return (p >> 8) & 0xFF; }
int getB(unsigned int p) { return p & 0xFF; }

unsigned int makePixel(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

void draw(unsigned int img[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << '(' 
                 << getR(img[i][j]) << ','
                 << getG(img[i][j]) << ','
                 << getB(img[i][j]) << 
                 ")  ";
        }
        cout << endl;
    }
}

void reverse(unsigned int img[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            img[i][j] = makePixel(255 - getR(img[i][j]), 255 - getG(img[i][j]), 255 - getB(img[i][j]));
}

void grey(unsigned int img[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int m = (getR(img[i][j]) + getG(img[i][j]) + getB(img[i][j])) / 3;
            img[i][j] = makePixel(m, m, m);
        }
}

int main() {
    unsigned int img[3][3] = {
        {0xFA5733, 0x47FF51, 0x71A72F},
        {0xF1C40F, 0x8E44AD, 0x1ABC9C},
        {0xE74C3C, 0x2ECC71, 0x3498DB}
    };

    cout << "Matrice initiale :\n";
    draw(img);

    int ligne, col, r, g, b;
    cout << "\nEntrez la ligne et la colonne du pixel a modifier (0-2) : ";
    cin >> ligne >> col;
    cout << "Entrez les nouvelles valeurs R G B (0-255) : ";
    cin >> r >> g >> b;
    img[ligne][col] = makePixel(r, g, b);

    cout << "\nApres modification :\n";
    draw(img);

    int choix;
    cout << "\nChoisir un filtre : 1 = Inversion, 2 = Gris :";
    cin >> choix;

    if (choix == 1) reverse(img);
    else if (choix == 2) grey(img);

    cout << "\nImage finale :\n";
    draw(img);
    return 0;
}
