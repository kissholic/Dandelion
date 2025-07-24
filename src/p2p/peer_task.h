/*
 * File: peer_task.h
 * Author: kissholic
 * Copyright (c) 2025 kissholic. All wrongs reserved.
 */

#pragma once

#include "common/task_system/task.h"
#include "uv.h"



namespace dandelion::p2p {


enum class peer_task_kind : uint32_t {
	REQUEST_FILE_INFO,
	REQUEST_FILE_DATA,

	TOTAL_KIND,
};


class peer_task;
using task_constructor = peer_task*();


class peer_task : public task {
public:
	peer_task() noexcept {}
	virtual ~peer_task() noexcept {}

	virtual void process(char const* buff, ssize_t size) noexcept {}
};


template<typename T>
class peer_task_crtp : public peer_task {
public:
	peer_task_crtp() noexcept : peer_task() {}

	static task_constructor* get_task_constructor() noexcept {
		return []() -> peer_task* {
			return new T();
		};
	}

	static peer_task_kind kind() noexcept {
		return T::get_task_kind();
	}
};


class peer_task_request_file_info : public peer_task_crtp<peer_task_request_file_info> {
public:
	peer_task_request_file_info() noexcept = default;
	virtual ~peer_task_request_file_info() noexcept = default;

	void process(char const* buff, ssize_t size) noexcept override;

	static peer_task_kind get_task_kind() noexcept {
		return peer_task_kind::REQUEST_FILE_INFO;
	}
};


class peer_task_request_file_data : public peer_task_crtp<peer_task_request_file_data> {
public:
	peer_task_request_file_data() noexcept = default;
	virtual ~peer_task_request_file_data() noexcept = default;

	void process(char const* buff, ssize_t size) noexcept override;

	static peer_task_kind get_task_kind() noexcept {
		return peer_task_kind::REQUEST_FILE_DATA;
	}
};



task peer_task_request_file_info(char const* buff, ssize_t size) noexcept;


} // namespace dandelion::p2p
