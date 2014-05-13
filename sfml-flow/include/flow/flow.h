/////////////////////////////////////////////////////////////////////////
/// sfml-flow
/// Copyright (C) 2014 Jérôme Béchu
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#ifndef SFML_FLOW_FLOW_H
#define SFML_FLOW_FLOW_H

#include "model.h"

namespace flow
{

class Flow
{
public:
    /// a port would be input, output or parameter
    typedef enum
    {
        kInput,
        kOutput,
        kParam
    } Type;
    struct Port
    {
        /// the type of port
        Type type;
        /// name's port
        std::string name;
        /// node index
        int index;
    };
    /// a link between from and to
    struct Link
    {
        Port from;
        Port to;
    };
    /// node position
    struct Position
    {
        Position(float X, float Y)
            : x(X)
            , y(Y)
        {
        }
        float x;
        float y;
    };
    int add(const std::string& name, float x, float y)
    {
        models_name_.push_back(name);
        positions_.push_back(Position(x, y));
        params_.resize(models_name_.size());
        return models_name_.size()-1;
    }
    void set_param(size_t node, const std::string& name, const std::string& value)
    {
        assert(node < params_.size());
        params_[node][name] = value;
    }
    std::map<std::string, std::string> get_params(size_t node) const
    {
        assert(node < params_.size());
        return params_[node];
    }
    void add(const Link& link)
    {
        links_.push_back(link);
    }
    size_t models_size() const
    {
        return models_name_.size();
    }
    size_t links_size() const
    {
        return links_.size();
    }
    std::string getModelName(size_t index) const
    {
        assert(index < models_name_.size());
        return models_name_[index];
    }
    Position getPosition(size_t index) const
    {
        assert(index < positions_.size());
        return positions_[index];
    }
    Link getLink(size_t index) const
    {
        assert(index < links_.size());
        return links_[index];
    }
private:
    std::vector<std::string> models_name_;
    std::vector<Position> positions_;
    std::vector<Link> links_;
    std::vector< std::map<std::string, std::string> > params_;
};

}

#endif // SFML_FLOW_FLOW_H
