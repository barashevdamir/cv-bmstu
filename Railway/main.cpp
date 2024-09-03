#include <iostream>

const int boxPos = 51;
const int MAX_STEPS = 20;

bool isBox(int pos) {
    return pos == boxPos;
}

int findBox(const int& start) {
    int step = 1;
    int iterations = 0;

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
        
        iterations++;
        
        if (iterations >= MAX_STEPS) {
            std::cerr << "Превышено максимальное количество шагов. Поиск прерван." << std::endl;
            return -1;
        }
        
        step *= 2;
    }
}

int main() {
    int startPos = 0;

    int boxPosition = findBox(startPos);
    
    if (boxPosition != -1) {
        std::cout << "Груз найден в позиции: " << boxPosition << std::endl;
    } else {
        std::cout << "Поиск груза не удался." << std::endl;
    }

    return 0;
}
