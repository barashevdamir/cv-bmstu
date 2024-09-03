#include <iostream>

const int boxPos = 51;

bool isBox(int pos) {
    return pos == boxPos;
}

int findBox(const int& start) {
    int step = 1;

    while (true) {
        for (int x = start; x <= start + step; ++x) {
            if (isBox(x)) {
                return x;
            }
        }

        for (int x = start; x >= start - step; --x) {
            if (isBox(x)) {
                return x;
            }
        }

        step *= 2;
    }
}

int main() {
    int startPos = 0;

    int boxPosition = findBox(startPos);

    std::cout << "Груз найден в позиции: " << boxPosition << std::endl;
    return 0;
}
