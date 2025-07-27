/*
 * File: peer_task.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "cdc/cdc_chunk.h"
#include "common/task_system/task.h"
#include "task_kind.h"
#include "uv.h"



namespace dandelion::p2p {


class peer_task;
using task_constructor = peer_task*(uv_handle_t*, uv_buf_t const* buf, ssize_t size);


class peer_task : public task {
public:
	peer_task(uv_handle_t* handle) noexcept : m_handle(handle) {}
	virtual ~peer_task() noexcept;

	virtual void process(char const* buff, ssize_t size) noexcept {}

protected:
	uv_handle_t* m_handle;
};


template<typename T>
class peer_task_crtp : public peer_task {
public:
	peer_task_crtp(uv_handle_t* handle) noexcept : peer_task(handle) {}
	virtual ~peer_task_crtp() noexcept = default;

	static task_constructor* get_task_constructor() noexcept {
		return [](uv_handle_t* handle, uv_buf_t const* buf, ssize_t size) -> peer_task* {
			return new T(handle, buf, size);
		};
	}

	static peer_task_kind kind() noexcept {
		return T::get_task_kind();
	}
};


class peer_task_request_file_info : public peer_task_crtp<peer_task_request_file_info> {
public:
	peer_task_request_file_info(uv_handle_t* handle, uv_buf_t const* buf, ssize_t size) noexcept;
	virtual ~peer_task_request_file_info() noexcept = default;

	void process(char const* buff, ssize_t size) noexcept override;

	static peer_task_kind get_task_kind() noexcept {
		return peer_task_kind::REQUEST_FILE_INFO;
	}

private:
	struct response_file_info {
		std::string file_name;
		std::unique_ptr<std::vector<cdc::cdc_chunk>> chunks;
	};
};


class peer_task_request_file_data : public peer_task_crtp<peer_task_request_file_data> {
public:
	peer_task_request_file_data(uv_handle_t* handle, uv_buf_t const* buf, ssize_t size) noexcept;
	virtual ~peer_task_request_file_data() noexcept = default;

	void process(char const* buff, ssize_t size) noexcept override;

	static peer_task_kind get_task_kind() noexcept {
		return peer_task_kind::REQUEST_FILE_DATA;
	}
};



task peer_task_request_file_info(char const* buff, ssize_t size) noexcept;


} // namespace dandelion::p2p
