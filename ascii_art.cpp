#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cctype>

#define MAX_H 30
#define MAX_L 30
#define MAX_N 200

using namespace std;


class Letter
{
    public:
        int h = MAX_H;
        int l = MAX_L;
        char letter = NULL;
        char box[MAX_H][MAX_L];
        Letter(int h, int l, char letter);
        Letter();
        void add_row(int row, string row_char);
        void set_size(int h, int l);
        void set_letter(char letter);
};

Letter::Letter(int h, int l, char letter)
{
    Letter::h = h;
    Letter::l = l;
    Letter::letter = letter;
};

Letter::Letter() {};

void Letter::add_row(int row, string row_char)
{
    for (int j = 0; j < l; j++) {
        Letter::box[row][j] = row_char[j];
    }
};

void Letter::set_size(int h, int l)
{
    Letter::h = h; Letter::l = l;
};

void Letter::set_letter(char letter)
{
    Letter::letter = letter;
};

int main()
{

    string valid_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZ?";

    int l;
    cin >> l; cin.ignore();
    int h;
    cin >> h; cin.ignore();
    string t;
    getline(cin, t);

    unordered_map<char, Letter> alphabet;

    for (int a=0; a<valid_char.length(); a++) {
        char A = valid_char[a];
        alphabet.insert(pair<char, Letter> {A, Letter()});
        alphabet[A].set_letter(A);
        alphabet[A].set_size(h, l);
    }

    for (int i = 0; i < h; i++) {
        string row;
        getline(cin, row);

        for (int a=0; a<valid_char.length(); a++) {
            alphabet[valid_char[a]].add_row(i, row.substr(a*l, l));
        }
    }

    for (int i = 0; i < h; i++) {
        for (int x=0; x<t.length(); x++) {
            char q = toupper(t[x]);
            if (q != alphabet[q].letter) q = '?';
            for (int j = 0; j < l; j++) cout << alphabet[q].box[i][j];
        }
        cout << endl;
    }

}