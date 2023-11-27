#include <chrono>
#include <iostream>
#include <random>
#include <string_view>
#include <thread>
#include <vector>
#include <xieite/graphics/color.hpp>
#include <xieite/streams/standard_handle.hpp>

int main() {
	const xieite::streams::StandardHandle terminal = xieite::streams::StandardHandle(std::cin, std::cout);
	const xieite::streams::Position size = terminal.getScreenSize();
	// terminal.setScreenAlternate(true);
	// terminal.setCursorAlternate(true);

	constexpr std::string_view characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+`{}|[]\\;':\",./<>?";

	auto generator = std::mt19937(std::random_device()());
	auto distribution = std::uniform_int_distribution<int>(0, size.row - 1);
	auto character = std::uniform_int_distribution<int>(0, characters.size() - 1);

	std::vector<int> things = std::vector<int>(size.column);
	for (int i = 0; i < size.column; ++i) {
		things[i] = distribution(generator);
	}

	while (true) {
		std::vector<std::vector<xieite::graphics::Color>> canvas = std::vector<std::vector<xieite::graphics::Color>>(size.column, std::vector<xieite::graphics::Color>(size.row, xieite::graphics::Color(0, 0, 0)));
		for (int i = 0; i < size.column; ++i) {
			for (int j = 0; j < 16; ++j) {
				canvas[i][(things[i] - j + size.row) % size.row] = xieite::graphics::Color(0, 255 - j * 16, 0);
			}
		}

		terminal.setCursorPosition(xieite::streams::Position(0, 0));
		for (int y = 0; y < size.row; ++y) {
			for (int x = 0; x < size.column; ++x) {
				terminal.setForegroundColor(canvas[x][y]);
				std::cout << characters[character(generator)];
			}
			terminal.resetForegroundColor();
			std::cout << "\n\r";
		}

		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		for (int i = 0; i < size.column; ++i) {
			things[i] = (things[i] + 1) % size.row;
		}
	}
}
