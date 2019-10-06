#ifndef RENDERING_OPTIONS_HPP
#define RENDERING_OPTIONS_HPP

#include <string>
#include <iostream>
#include <yaml-cpp/yaml.h>

#include "vec3.hpp"

struct renderingOptions
{
    // path to the graph to load
    std::string output_path;

    // general options
    int nx = 200;
    int ny = 100;
    int ns = 100;
    bool visualization = false;
    bool verbose = false;

    // camera parameters
    vec3 lookfrom;
    vec3 lookat;
    double dist_to_focus = 10.0;
    double aperture = 0.1;
    double fov = 90;

    void print(){
        std::cout << "list of the parameters:" << std::endl;
        std::cout << std::endl;
        std::cout << "*** IO files: ***" << std::endl;
        std::cout <<  "path_graph_obj: " << output_path << std::endl;
        std::cout << std::endl;
        std::cout << "*** General parameters ***" << std::endl;
        std::cout << "nx: " << nx << std::endl;
        std::cout << "ny: " << ny << std::endl;
        std::cout << "ns: " << ns << std::endl;
        std::cout << "visualization: " << visualization << std::endl;
        std::cout << "verbose: " << verbose << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    bool loadYAML(std::string config_file){
        YAML::Node config = YAML::LoadFile(config_file);
        
        // IO
        output_path     = config["io_files"]["output_path"].as<std::string>();

        // general options
        visualization      = config["general_params"]["visualization"].as<bool>();
        verbose            = config["general_params"]["verbose"].as<bool>();
        nx                 = config["image_params"]["nx"].as<int>();
        ny                 = config["image_params"]["ny"].as<int>();
        ns                 = config["image_params"]["ns"].as<int>();

        // camera params
        lookfrom           = vec3(config["camera_params"]["lookfrom"].as<std::vector<double>>());
        lookat             = vec3(config["camera_params"]["lookat"].as<std::vector<double>>());
        dist_to_focus      = config["camera_params"]["dist_to_focus"].as<double>();
        aperture           = config["camera_params"]["aperture"].as<double>();
        fov                = config["camera_params"]["fov"].as<double>();


        if (verbose)
            print();
    }
};

#endif
