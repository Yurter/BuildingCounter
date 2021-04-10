#include <iostream>

constexpr auto EmptyPlace    { ' '  };
constexpr auto InvalidPlace  { '\0' };
constexpr auto BuildingPlace { '#'  };

static auto stepCounter { 0 };

struct Map {
    int width;
    int height;
    std::string data;

    char& at(int x, int y) {
        return data[static_cast<std::size_t>(y * width + x)];
    }

    const char& at(int x, int y) const {
        return data[static_cast<std::size_t>(y * width + x)];
    }

    char get(int x, int y) {
        if ((x < 0) || (y < 0) || (x >= width) || (y >= height)) {
            return InvalidPlace;
        }
        const auto pos { static_cast<std::size_t>(y * width + x) };
        return pos < data.size() ? data[pos] : InvalidPlace;
    }
};

Map   generateMap(int width, int height);
void  displayMap(const Map& map);
int   markBuildings(Map& map);

int main() {

    constexpr auto mapSize { 25 };
    auto map { generateMap(mapSize * 2, mapSize) };

    std::cout << "Source random map:\n\n";
    displayMap(map);

    const auto buildingCounter { markBuildings(map) };
    std::cout << "\nMarked map:\n\n";
    displayMap(map);

    std::cout << "\n\n";
    std::cout << ">> Building count: " << buildingCounter << "\n\n";
    std::cout << "Step count: " << stepCounter << '\n';
    std::cout << "Data size: " << map.data.size() << '\n';
    std::cout << "\n\n";

    return 0;
}

float getRandomChance() {
    return (rand() / static_cast<float>(RAND_MAX));
}

Map generateMap(int width, int height) {
    Map map;
    map.width = width;
    map.height = height;
    map.data.resize(static_cast<std::size_t>(width * height));

    for (auto x { 0 }; x < int(width); ++x) {
        for (auto y { 0 }; y < (height); ++y) {
            map.at(x, y) = getRandomChance() < 0.35f ? BuildingPlace : EmptyPlace;
        }
    }

    return map;
}

void displayMap(const Map& map) {
    for (auto y { 0 }; y < map.height; ++y) {
        for (auto x { 0 }; x < map.width; ++x) {
            std::cout << map.at(x, y);
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
}

void markNeighbors(Map& map, int x, int y, char index) {
    ++stepCounter;

    #define TOP_CELL    x,     y - 1
    #define LEFT_CELL   x - 1, y
    #define RIGHT_CELL  x + 1, y
    #define BOTTOM_CELL x,     y + 1

    const auto top    { map.get(TOP_CELL)    };
    const auto left   { map.get(LEFT_CELL)   };
    const auto right  { map.get(RIGHT_CELL)  };
    const auto bottom { map.get(BOTTOM_CELL) };

    if (BuildingPlace == top) {
        map.at(TOP_CELL) = index;
        markNeighbors(map, TOP_CELL, index);
    }
    if (BuildingPlace == left) {
        map.at(LEFT_CELL) = index;
        markNeighbors(map, LEFT_CELL, index);
    }
    if (BuildingPlace == right) {
        map.at(RIGHT_CELL) = index;
        markNeighbors(map, RIGHT_CELL, index);
    }
    if (BuildingPlace == bottom) {
        map.at(BOTTOM_CELL) = index;
        markNeighbors(map, BOTTOM_CELL, index);
    }
}

int markBuildings(Map& map) {
    auto counter { 0 };

    for (auto x { 0 }; x < map.width; ++x) {
        for (auto y { 0 }; y < map.height; ++y) {
            ++stepCounter;

            if (BuildingPlace == map.at(x, y)) {
                const char index { static_cast<char>(counter++ % (212 - 36) + 36) }; // ASCII 36..212
                map.at(x, y) = index;
                markNeighbors(map, x, y, index);
            }
        }
    }

    return counter;
}
