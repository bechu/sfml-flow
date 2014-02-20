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

#ifndef SFML_FLOW_MODELS_H
#define SFML_FLOW_MODELS_H

#include "model.h"

namespace flow
{

class Models
        : public std::map<std::string, Model>
{
public:
    typedef std::map<std::string, Model>::iterator iterator;
    void add(const std::string& name, const Model& model);
    Model& add(const std::string& name);
    const Model *getModel(const std::string& name) const;
};

inline void Models::add(const std::string& name, const Model& model)
{
    (*this)[name] = model;
}

inline Model& Models::add(const std::string& name)
{
    (*this)[name] = Model();
    return (*this)[name];
}

}

#endif // SFML_FLOW_MODELS_H
