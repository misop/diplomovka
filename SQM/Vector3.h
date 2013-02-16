#pragma once
#pragma warning(push, 0)
#include <boost\serialization\serialization.hpp>
#include <boost\serialization\variant.hpp>
#include <boost\serialization\vector.hpp>
#include <boost\serialization\nvp.hpp>
#include <boost\serialization\assume_abstract.hpp>
#pragma pop

struct CVector3 {
    friend class boost::serialization::access;
public:
    float x, y, z;
    // A default constructor
    CVector3() : x(0), y(0), z(0) { }

    // This is our constructor that allows us to initialize our data upon creating an instance
    CVector3(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
private:
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(x);
        ar & BOOST_SERIALIZATION_NVP(y);
        ar & BOOST_SERIALIZATION_NVP(z);
    }
};
