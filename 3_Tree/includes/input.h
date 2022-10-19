
namespace get_commands {

enum class command_codes {
    K,
    Q,
    N,
    M
};

std::pair<std::vector<char>, std::vector<int>> get_commands();

}