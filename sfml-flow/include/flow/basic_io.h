#ifndef SFML_FLOW_BASIC_IO_H
#define SFML_FLOW_BASIC_IO_H

#include <fstream>
#include <sstream>

#include "io.h"

namespace flow
{

class BasicIO
        : public IO
{
public:
    void save(const std::string& filename,
              const Flow& flow) const
    {
        using namespace flow;

        std::fstream file;
        file.open(filename.c_str(), std::ios_base::out);
        file << flow.models_size() << std::endl;
        for(size_t i=0;i<flow.models_size();++i)
        {
            file << flow.getModelName(i) << ";";
            file << flow.getPosition(i).x << ";";
            file << flow.getPosition(i).y << ";";
            // parameters ...
            std::map<std::string, std::string> params = flow.get_params(i);
            for(std::map<std::string, std::string>::iterator it=params.begin();
                it != params.end(); ++it)
            {
                file << (*it).first << ";" << (*it).second << ";";
            }
            file << std::endl;
        }
        for(size_t i=0;i<flow.links_size();++i)
        {
            Flow::Link link = flow.getLink(i);
            file << link.from.name << ";";
            file << link.from.type << ";";
            file << link.from.index << ";";
            file << link.to.name << ";";
            file << link.to.type << ";";
            file << link.to.index << ";";
            file << std::endl;
        }
    }

    Flow load(const std::string& filename) const
    {
        Flow flow;
        std::fstream file;
        file.open(filename.c_str(), std::ios_base::in);
        int lines = std::count(std::istreambuf_iterator<char>(file),
                               std::istreambuf_iterator<char>(), '\n');

        file.seekg(0);

        if(!lines)
            throw std::string("This file "+filename+" is empty !");

        int count_node =  read<int>(file);
        if(count_node > 100 || count_node < 0) throw std::string("Wrong file !");
        for(int i=0;i<count_node;++i)
        {
            read_node(flow, file);
        }
        for(int i=count_node;i<lines-1;++i)
        {
            read_link(flow, file);
        }
        return flow;
    }

    void read_link(Flow& flow, std::fstream& file) const
    {
        std::string line;
        std::getline(file, line);
        std::vector<std::string> tokens = split(line, ';');
        Flow::Link link;
        link.from.type = static_cast<Flow::Type>(to<int>(tokens[1]));
        link.from.name = tokens[0];
        link.from.index = to<int>(tokens[2]);
        link.to.type = static_cast<Flow::Type>(to<int>(tokens[4]));
        link.to.name = tokens[3];
        link.to.index = to<int>(tokens[5]);
        flow.add(link);
    }

    void read_node(Flow& flow, std::fstream& file) const
    {
        std::string line;
        std::getline(file, line);
        std::vector<std::string> tokens = split(line, ';');
        if(tokens.size() < 3)
            throw std::string("Error detected in the line : " +line);
        int node = flow.add(tokens[0],
                to<float>(tokens[1]),
                to<float>(tokens[2]));
        for(size_t i=3;i<tokens.size()-1;i+=2)
        {
            flow.set_param(node, tokens[i], tokens[i+1]);
        }
    }

    std::vector<std::string> split(const std::string &s, char delim) const
    {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    template <typename T>
    T read(std::fstream& file) const
    {
        std::string temp;
        std::getline(file, temp);
        return to<T>(temp);
    }

    template <typename T>
    T to(const std::string& str) const
    {
        std::stringstream ss(str);
        T u;
        ss >> u;
        return u;
    }
};

}

#endif // SFML_FLOW_BASIC_IO_H
