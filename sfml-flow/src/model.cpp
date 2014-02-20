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

#include "flow/model.h"

namespace flow
{

Model::Data::Type Model::Data::nextTypeId_ = 1;

Model::Data::Data(Model::Data::Type type)
    : type_(type)
{
    raw_.reset();
    raw_ = boost::shared_ptr<void>();
}

Model::Data::~Data()
{
    raw_.reset();
}

Model::Model()
{
}

void Model::input(Model::Data::Type type, const std::string& name)
{
    Model::Data::nextTypeId_ = type+1;
    if(inputs_.find(name) == inputs_.end())
        inputs_[name] = Model::Data(type);
}

void Model::parameter(Data::Type type, const std::string& name)
{
    Model::Data::nextTypeId_ = type+1;
    if(parameters_.find(name) == parameters_.end())
    {
        parameters_[name] = Model::Data(type);
        //parameters_[name].allocate<T>();
    }
}

void Model::output(Data::Type type, const std::string& name)
{
    Model::Data::nextTypeId_ = type+1;
    if(outputs_.find(name) == outputs_.end())
        outputs_[name] = Model::Data(type);
}

}
