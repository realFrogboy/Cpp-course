#include <boost/program_options.hpp>

#include "driver.hpp"

namespace {

namespace po = boost::program_options;

std::string command_line_arg_handler(const int argc, const char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("path", po::value<std::string>(), "A path to the ParaCL source file")
    ;

    po::positional_options_description p;
    p.add("path", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("path")) 
        return vm["path"].as<std::string>();

    std::cout << "./paracl(i) <<file>>" << std::endl;
    return 0;
}

} // namespace

int main(const int argc, const char *argv[]) {
    try {
        std::string path = command_line_arg_handler(argc, argv);
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(path);

        yy::driver_t driver(&file);
        driver.parse();
        driver.traversal_AST();
    } catch (yy::parse_error &err) {
        std::cout << "Parse error: " << err.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
