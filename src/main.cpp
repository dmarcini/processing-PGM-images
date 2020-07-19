#include <string>
#include <vector>
#include <utility>

#include "image.hpp"
#include "image_operations.hpp"
#include "parse_source.hpp"


void show_usage(const std::string &name)
{
    std::cerr << "usage: " << name << '\n'
              << "\t[-h, --help]\n"
              << "\t[--load-image=path_to_image]\n"
              << "\t[--save-image=path_for_image]\n"
              << "\t[--rotate=k (ex. k = 2, rotate 180 degree (2 * 90))]\n"
              << "\t[--reflect=axis (x | y)]\n"
              << "\t[--threshold=threshold_val<0, max_depp>]\n"
              << "\t[--graininess=percent]\n"
              << "\t[--negative]\n"
              << "\t[--histogram=path_for_histogram]\n";
}


int main(int argc, char *argv[])
{
    if (argc == 1) {
        show_usage(argv[0]);

        return 0;
    }

    std::vector<PGM::SourceString> sources {};

    for (int i {1}; i < argc; ++i) {
        sources.emplace_back(argv[i]);
    }

    auto parsed_sources = PGM::parse_sources(sources);

    if (parsed_sources.find(PGM::Option::Help) != parsed_sources.end()) {
        show_usage(sources[0].str());

        return 0;
    }

    if (parsed_sources.find(PGM::Option::Unknow) != parsed_sources.end()) {
        std::cerr << "Unknow option(s).\n\n";

        show_usage(sources[0].str());

        return 1;
    }

    PGM::Image img;

    if (parsed_sources.find(PGM::Option::LoadImage) != parsed_sources.end()) {
        if (!img.load_from_file(parsed_sources[PGM::Option::LoadImage])) {
            return 1;
        }
        parsed_sources.erase(PGM::Option::LoadImage);
    } else {
        std::cerr << "Path to the image must be provided.\n\n";
    }

    std::string histogram_path {""};
    std::string path_to_save {""};

    if (parsed_sources.find(PGM::Option::Histogram) != parsed_sources.end()) {
        histogram_path = parsed_sources[PGM::Option::Histogram];

        parsed_sources.erase(PGM::Option::Histogram);
    }

    if (parsed_sources.find(PGM::Option::SaveImage) != parsed_sources.end()) {
        path_to_save = parsed_sources[PGM::Option::SaveImage];

        parsed_sources.erase(PGM::Option::SaveImage);
    }

    for (auto option : parsed_sources) {
        try {
            switch (std::get<0>(option)) {
                case PGM::Option::Rotate:
                    PGM::rotate(img, std::stoi(std::get<1>(option)));
                    break;
                case PGM::Option::Reflect:
                    PGM::reflect(img, std::get<1>(option).at(0));
                    break;
                case PGM::Option::Threshold:
                    PGM::threshold(img, std::stoi(std::get<1>(option)));
                    break;
                case PGM::Option::Graininess:
                    PGM::graininess(img, std::stoi(std::get<1>(option)));
                    break;
                case PGM::Option::Negative:
                    PGM::negative(img);
                    break;
                default:
                    break;
            }
        } catch (std::invalid_argument &ia) {
            std::cerr << "Parameter '" << std::get<1>(option)
                      << "' must be integer.\n";
        }
    }

    if (!histogram_path.empty()) {
        PGM::create_histogram(img, histogram_path);
    }

    if (!path_to_save.empty()) {
        img.save_to_file(path_to_save);
    }

    return 0;
}
