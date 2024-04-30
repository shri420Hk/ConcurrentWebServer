#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        doRead();
    }

private:
    void doRead() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    doWrite(length);
                }
            });
    }

    void doWrite(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    doRead();
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          socket_(io_context) {
        doAccept();
    }

private:
    void doAccept() {
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket_))->start();
                }
                doAccept();
            });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a pool of threads to handle client requests concurrently
        std::vector<std::thread> threads;
        for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            threads.emplace_back([&io_context]() { io_context.run(); });
        }

        // Start the server on port 8080
        Server server(io_context, 8080);

        // Wait for all threads in the pool to complete
        for (auto& thread : threads) {
            thread.join();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
