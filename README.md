# Concurrent Web Server

ConcurrentWebServer is a C++ project that implements a simple concurrent web server using Boost.Asio, a popular library for network and low-level I/O programming.

## Features

- Concurrent handling of multiple client connections
- Asynchronous I/O for improved performance
- Basic HTTP request handling

## Usage

1. Build the project using a C++ compiler with Boost.Asio library linked.
2. Run the executable file.
3. The server will start listening for incoming connections on port 8080 by default.

## Dependencies

- Boost.Asio: Ensure that Boost.Asio library is installed and linked properly.

## Usage Example

To start the server, simply run the executable file:

```bash
./ConcurrentWebServer
