#pragma once

#include <cstddef>
#include <optional>
#include <string_view>

namespace embedded_assets {
struct AssetView {
	const unsigned char* data;
	std::size_t size;
};

std::optional<AssetView> find(std::string_view path);
}
