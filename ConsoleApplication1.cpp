// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<stdio.h>
#include<uv.h>

int64_t counter = 0;
uv_loop_t* loop = uv_default_loop();

void echo_read(uv_stream_t* stream,
	ssize_t nread,
	const uv_buf_t* buf) {
	if (nread > 0)
		printf("data available");
}

void alloc_buffer(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf) {
	buf->base =(char*) malloc(suggested_size);
	buf->len = suggested_size;
}

void on_accept(uv_stream_t* server, int status) {
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		// error!
		return;
	}

	uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0) {
		uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	}
}

int main() {
	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	sockaddr_in addr;

	uv_ip4_addr("0.0.0.0", 2121, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, 128, on_accept);
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}
