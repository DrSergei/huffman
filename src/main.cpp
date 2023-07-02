#include "Application.h"

int main(int argc, char** argv) {
    app::run(app::parse(argc, argv));
    return 0;
}