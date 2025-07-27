/*
 * File: stream_buffer.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional> 
#include "uv.h"
#include <vector>



namespace dandelion {


// A stream buffer that can be used to store data from multiple sources
class stream_buffer {
public:
	stream_buffer() noexcept = default;
	stream_buffer(void* buf, size_t size, size_t offset = 0) noexcept;
	~stream_buffer() noexcept;

	void add_stream(void* buf, size_t size) noexcept;

	template<typename T>
	std::optional<T> read() noexcept;

	template<>
	std::optional<uint8_t> read<uint8_t>() noexcept {
		if (!enough(sizeof(uint8_t)))
			return std::nullopt;

		return next();
	}

	template<>
	std::optional<int8_t> read<int8_t>() noexcept {
		return read<uint8_t>();
	}

	template<>
	std::optional<uint16_t> read<uint16_t>() noexcept {
		if (!enough(sizeof(uint16_t)))
			return std::nullopt;

		uint16_t data = 0;
		if (std::endian::little == std::endian::native) {
			data = (next() << 8) | next();
		} else {
			data = next() | (next() << 8);
		}

		return data;
	}

	template<>
	std::optional<int16_t> read<int16_t>() noexcept {
		return read<uint16_t>();
	}

	template<>
	std::optional<uint32_t> read<uint32_t>() noexcept {
		if (!enough(sizeof(uint32_t)))
			return std::nullopt;

		uint32_t data = 0;
		if (std::endian::little == std::endian::native) {
			data = next() | (next() << 8) | (next() << 16) | (next() << 24);
		} else {
			data = (next() << 24) | (next() << 16) | (next() << 8) | next();
		}

		return data;
	}

	template<>
	std::optional<int32_t> read<int32_t>() noexcept {
		return read<uint32_t>();
	}

	template<>
	std::optional<uint64_t> read<uint64_t>() noexcept {
		if (!enough(sizeof(uint64_t)))
			return std::nullopt;

		uint64_t data = 0;
		if (std::endian::little == std::endian::native) {
			data = next() | (next() << 8) | (next() << 16) | (next() << 24) | (next() << 32) | (next() << 40) | (next() << 48) | (next() << 56);
		} else {
			data = (next() << 56) | (next() << 48) | (next() << 40) | (next() << 32) | (next() << 24) | (next() << 16) | (next() << 8) | next();
		}

		return data;
	}

	template<>
	std::optional<int64_t> read<int64_t>() noexcept {
		return read<uint64_t>();
	}

	template<>
	std::optional<std::string> read<std::string>() noexcept {
		auto data = read<std::vector<char>>();
		if (!data)
			return std::nullopt;

		return std::string(data->data(), data->size());
	}

	template<>
	std::optional<std::vector<char>> read<std::vector<char>>() noexcept {
		if (!enough(sizeof(uint64_t)))
			return std::nullopt;

		uint64_t estimate_size = try_next64();
		if (!enough(estimate_size))
			return std::nullopt;

		read<uint64_t>();

		std::vector<char> data(estimate_size);
		for (size_t i = 0; i < estimate_size; ++i)
			data[i] = next();

		return data;
	}

	bool enough(size_t size) const noexcept;

private:
	struct internal_buffer {
		void* m_base;
		size_t m_size;
	};
	internal_buffer& cur_buff() noexcept;

	uint64_t next() noexcept;
	uint64_t try_next64() noexcept;

private:
	std::vector<internal_buffer> m_buffs;
	size_t m_offset;
};


class writable_stream_buffer {
public:
	writable_stream_buffer(uint32_t max_size) noexcept
		: m_buffer_max_size(max_size)
		, m_offset(0)
	{

	}
	~writable_stream_buffer() noexcept = default;

	template<typename T>
	void write(T const& data) noexcept;

	template<>
	void write<uint8_t>(uint8_t const& data) noexcept {
		next(data);
	}

	template<>
	void write<int8_t>(int8_t const& data) noexcept {
		next(data);
	}

	template<>
	void write<uint16_t>(uint16_t const& data) noexcept {
		if (std::endian::native == std::endian::little) {
			next(data & 0xff);
			next(data >> 8);
		} else {
			next(data >> 8);
			next(data & 0xff);
		}
	}

	template<>
	void write<int16_t>(int16_t const& data) noexcept {
		write<uint16_t>(data);
	}

	template<>
	void write<uint32_t>(uint32_t const& data) noexcept {
		if (std::endian::native == std::endian::little) {
			next(data & 0xff);
			next(data >> 8);
			next(data >> 16);
			next(data >> 24);
		} else {
			next(data >> 24);
			next(data >> 16);
			next(data >> 8);
			next(data & 0xff);
		}
	}

	template<>
	void write<int32_t>(int32_t const& data) noexcept {
		write<uint32_t>(data);
	}

	template<>
	void write<uint64_t>(uint64_t const& data) noexcept {
		if (std::endian::native == std::endian::little) {
			next(data & 0xff);
			next(data >> 8);
			next(data >> 16);
			next(data >> 24);
			next(data >> 32);
			next(data >> 40);
			next(data >> 48);
			next(data >> 56);
		} else {
			next(data >> 56);
			next(data >> 48);
			next(data >> 40);
			next(data >> 32);
			next(data >> 24);
			next(data >> 16);
			next(data >> 8);
			next(data & 0xff);
		}
	}

	template<>
	void write<int64_t>(int64_t const& data) noexcept {
		write<uint64_t>(data);
	}

	template<>
	void write<std::string>(std::string const& data) noexcept {
		write<uint64_t>(data.size());
		for (auto const& ch : data)
			next(ch);
	}

	template<>
	void write<std::vector<char>>(std::vector<char> const& data) noexcept {
		write<uint64_t>(data.size());
		for (auto const& ch : data)
			next(ch);
	}

	uint64_t size() const noexcept;
	void finalize() noexcept {
		padding();
	}

	uv_buf_t* data() noexcept {
		return m_buffer.data();
	}

	size_t num() const noexcept {
		return m_buffer.size();
	}

private:
	void padding() noexcept {
		int padding_size = m_offset % 8;
		for (int i = 0; i < padding_size; ++i)
			m_buffer.back().base[m_offset++] = 0;
	}

	void next(uint8_t ch) noexcept;

private:
	uint32_t m_buffer_max_size;
	std::vector<uv_buf_t> m_buffer;
	uint32_t m_offset;
};


} // namespace dandelion
