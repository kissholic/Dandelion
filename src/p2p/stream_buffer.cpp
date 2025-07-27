/*
 * File: stream_buffer.cpp
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#include <algorithm>
#include <numeric>
#include "stream_buffer.h"



namespace dandelion {

stream_buffer::stream_buffer(void* buf, size_t size, size_t offset) noexcept
	: m_buffs{}, m_offset(offset)
{
	m_buffs.push_back({buf, size});
}

stream_buffer::~stream_buffer() noexcept {
	std::ranges::for_each(m_buffs, [](internal_buffer& buff){
		::free(buff.m_base);
		buff.m_base = nullptr;
		buff.m_size = 0;
	});
}


bool stream_buffer::enough(size_t size) const noexcept {
	size_t total_size = 0;
	std::accumulate(m_buffs.begin(), m_buffs.end(), total_size, [](size_t total, const internal_buffer& buff){
		return total + buff.m_size;
	});

	return total_size >= size + m_offset;
}


stream_buffer::internal_buffer& stream_buffer::cur_buff() noexcept {
	return m_buffs.front();
}


uint64_t stream_buffer::next() noexcept {
	internal_buffer& buff = cur_buff();

	uint64_t data = static_cast<uint8_t*>(buff.m_base)[m_offset];
	m_offset++;

	if (m_offset >= buff.m_size) {
		auto& front = m_buffs.front();
		::free(front.m_base);
		front.m_base = nullptr;
		front.m_size = 0;
		m_buffs.erase(m_buffs.begin());
		m_offset = 0;
	}

	return data;
}


uint64_t stream_buffer::try_next64() noexcept {
	uint64_t data = 0;

	auto ext64 = [](uint8_t b){ return static_cast<uint64_t>(b); };

	if (m_offset + sizeof(uint64_t) > cur_buff().m_size)
		return 0;
	else {
		uint8_t* cur = static_cast<uint8_t*>(cur_buff().m_base) + m_offset;
		if (std::endian::native == std::endian::little)
			data = ext64(cur[0]) | ext64(cur[1]) << 8 | ext64(cur[2]) << 16 | ext64(cur[3]) << 24 |
				   ext64(cur[4]) << 32 | ext64(cur[5]) << 40 | ext64(cur[6]) << 48 | ext64(cur[7]) << 56;
		else
			data = ext64(cur[0]) << 56 | ext64(cur[1]) << 48 | ext64(cur[2]) << 40 | ext64(cur[3]) << 32 |
				   ext64(cur[4]) << 24 | ext64(cur[5]) << 16 | ext64(cur[6]) << 8 | ext64(cur[7]);
	}

	return data;
}


uint64_t writable_stream_buffer::size() const noexcept {
	uint64_t total = m_buffer_max_size * (m_buffer.size() - 1) + m_offset;
	total += total % 8;
	return total;
}


void writable_stream_buffer::next(uint8_t ch) noexcept {
	if (m_buffer.empty()) {
		uv_buf_t buf {.base = (char*)::malloc(m_buffer_max_size), .len = m_buffer_max_size};
		m_buffer.push_back(buf);
	}

	if (m_offset + 1 >= m_buffer_max_size) {
		m_offset = 0;
		m_buffer.push_back({.base = (char*)::malloc(m_buffer_max_size), .len = m_buffer_max_size});
	}

	m_buffer.back().base[m_offset++] = ch;
}


} // namespace dandelion
