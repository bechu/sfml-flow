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

#ifndef SFML_FLOW_MODEL_H
#define SFML_FLOW_MODEL_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstring>

namespace flow
{

class Model
{
public:
    Model();

    template <class T>
    void input(const std::string& name);

    template <class T>
    void output(const std::string& name);

    template <class T>
    T& parameter(const std::string& name);

    struct Data
    {
        typedef std::size_t Type;
        static const Type Empty = 0;

        struct Map : std::map<std::string, Data>
        {
        };

        Data(Type type=0);
        ~Data();

        Data(const Data & rhs)
        {
            type_ = rhs.type_;
            raw_ = rhs.raw_;
        }

        Data& operator=(const Data &rhs)
        {
            type_ = rhs.type_;
            raw_ = rhs.raw_;
            return *this;
        }

        template <typename T>
        void setType();

        template <typename T>
        bool is();

        template <typename T>
        void allocate();

        template <typename T>
        T& as();

        template <typename T>
        T get() const;

        template <typename T>
        static Type getType();
        static Type nextTypeId_;
        boost::shared_ptr<void> raw_;
        Type type_;
        static Type getNextType();
    };

    void parameter(Data::Type type, const std::string& name);

    template <class T>
    T& parameter(Data::Type type, const std::string& name);

    void input(Data::Type type, const std::string& name);
    void output(Data::Type type, const std::string& name);

    Data::Map parameters_;
    Data::Map inputs_;
    Data::Map outputs_;
};

template <class T>
T& Model::parameter(Data::Type type, const std::string& name)
{
    parameter(type, name);
    parameters_[name].allocate<T>();
    return parameters_[name].as<T>();
}

template <typename T>
inline T& Model::parameter(const std::string& name)
{
    if(parameters_.find(name) == parameters_.end())
    {
        parameters_[name] = Model::Data(Model::Data::getType<T>());
        parameters_[name].allocate<T>();
    }
    return parameters_[name].as<T>();
}

template <class T>
inline void Model::input(const std::string& name)
{
    if(inputs_.find(name) == inputs_.end())
        inputs_[name] = Model::Data(Model::Data::getType<T>());
}

template <class T>
inline void Model::output(const std::string& name)
{
    if(outputs_.find(name) == outputs_.end())
        outputs_[name] = Model::Data(Model::Data::getType<T>());
}


inline Model::Data::Type Model::Data::getNextType()
{
    return nextTypeId_++;
}

template <typename T>
inline void Model::Data::allocate()
{
    setType<T>();
    raw_ = boost::make_shared<T>();
}

template <typename T>
inline void Model::Data::setType()
{
    type_ = Data::getType<T>();
}

template <typename T>
inline bool Model::Data::is()
{
    return type_ == getType<T>();
}

template <typename T>
inline Model::Data::Type Model::Data::getType()
{
    static const Type id = getNextType();
    return id;
}

template <typename T>
inline T& Model::Data::as()
{
    assert(static_cast<bool>(raw_));
    return *(reinterpret_cast<T*>(raw_.get()));
}

template <typename T>
inline T Model::Data::get() const
{
    assert(static_cast<bool>(raw_));
    return *(reinterpret_cast<T*>(raw_.get()));
}

}

#endif // SFML_FLOW_MODEL_H
