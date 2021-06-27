#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{
    // is file stream object is initialized with path and two options:
    // reading the data in as binary data and
    // ate, "at-the-end", which seeks at the end of the stream
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;

    // Use tellg() to determine the size of the input stream by getting
    // the position of the current character pointer in the input stream,
    // which is at the end from using std::ios::ate
    auto size = is.tellg();
    std::vector<std::byte> contents(size);

    // seekg(0) sets the position of pointer at the begining of the stream
    is.seekg(0);
    // Read all of the input stream into vector contents
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    // std::move allows the transfer of the contents of the vector to
    // another variable without using pointers or references
    return std::move(contents);
}

int main(int argc, const char **argv)
{
    std::string osm_data_file = "";
    // Parse command line arguments
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }

    std::vector<std::byte> osm_data;

    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            // Read contents of osm datafile into vector osm_data
            osm_data = std::move(*data);
    }

    // DONE 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below in place of 10, 10, 90, 90.
    float start_x, start_y, end_x, end_y;
    std::cout << "Enter start coordinates (ex. 10 10): ";
    std::cin >> start_x >> start_y;
    std::cout << "Enter end coordinates (ex. 90 90): ";
    std::cin >> end_x >> end_y;

    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    // Display utilizing IO2D
    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
