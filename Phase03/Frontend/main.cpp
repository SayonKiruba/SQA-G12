#include <iostream>
#include "FrontEnd.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <data-directory> <daily-transaction-output-file>\n";
        return 1;
    }
    FrontEnd app(argv[1], argv[2]);
    app.run();
    return 0;
}
