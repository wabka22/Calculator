#include "polynomial.h"

int main() {
    try {
        Polynomial v({1,2,3});
        std::cout << v.ToString();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
