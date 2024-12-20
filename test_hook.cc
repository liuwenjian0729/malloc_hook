#include <iostream>

int main() {
    char* str = (char*)malloc(10);
    if (str != nullptr) {
        str[0] = 'H';
        free(str);
    } else {
        std::cerr << "Failed to allocate memory." << std::endl;
    }
    return 0;
}