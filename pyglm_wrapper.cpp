#ifndef _pyglm_wrapper_H_
#define _pyglm_wrapper_H_

#include <base/GeomHelper.h>

#include <wrapper/raw_constructor.hpp>
#include <wrapper/WrapHelper.h>

using namespace boost::python;
using namespace avg;

namespace Vec3Helper
{
    int len(const glm::vec3&) 
    {
        return 3;
    }

    float getX(const glm::vec3& pt)
    {
        return pt.x;
    }

    float getY(const glm::vec3& pt)
    {
        return pt.y;
    }

    float getZ(const glm::vec3& pt)
    {
        return pt.z;
    }

    void setX(glm::vec3& pt, float val)
    {
        pt.x = val;
    }

    void setY(glm::vec3& pt, float val)
    {
        pt.y = val;
    }

    void setZ(glm::vec3& pt, float val)
    {
        pt.z = val;
    }

    void checkItemRange(int i) {
        if (i < 0 || i > 2) {
            throw std::out_of_range("Index out of range for vec3. Must be [0..2].");
        }
    }

    float getItem(const glm::vec3& pt, int i)
    {
        checkItemRange(i);
        switch (i) {
            case 0:
                return pt.x;
            case 1:
                return pt.y;
            case 2:
                return pt.z;
            default:
                AVG_ASSERT(false);
                return 0;
        }
    }

    void setItem(glm::vec3& pt, int i, float val)
    {
        checkItemRange(i);
        switch (i) {
            case 0:
                pt.x = val;
            case 1:
                pt.y = val;
            case 2:
                pt.z = val;
            default:
                AVG_ASSERT(false);
        }
    }

    string str(const glm::vec3& pt)
    {
        stringstream st;
        st << "(" << pt.x << "," << pt.y << "," << pt.z << ")";
        return st.str();
    }

    string repr(const glm::vec3& pt)
    {
        stringstream st;
        st << "pyglm.vec3(" << pt.x << "," << pt.y << "," << pt.z << ")";
        return st.str();
    }

    long getHash(const glm::vec3& pt)
    {
        // Wild guess at what could constitute a good hash function.
        // Will generate very bad hashes if most values are in a range < 0.1,
        // but this is meant for pixel values anyway, right? ;-).
        return long(pt.x*42+pt.y*23+pt.z*17);
    }
    
    glm::vec3 safeGetNormalized(const glm::vec3& pt)
    {
        if (pt.x==0 && pt.y==0 && pt.z==0) {
            throw Exception(AVG_ERR_OUT_OF_RANGE, "Can't normalize (0,0,0).");
        } else {
            float invNorm = 1/sqrt(pt.x*pt.x+pt.y*pt.y+pt.z*pt.z);
            return glm::vec3(pt.x*invNorm, pt.y*invNorm, pt.z*invNorm);
        }
    }

    float getNorm(const glm::vec3& pt)
    {
        return glm::length(pt);
    }
}

glm::vec3* createVec3()
{
    return new glm::vec3(0,0,0);
}


BOOST_PYTHON_MODULE(pyglm)
{
    class_<glm::vec3>("vec3", no_init)
        .def("__init__", make_constructor(createVec3))
        .def(init<float, float, float>())
        .def(init<const glm::vec3&>())
        .def("__setitem__", &Vec3Helper::setItem)
        .add_property("x", &Vec3Helper::getX, &Vec3Helper::setX,"")
        .add_property("y", &Vec3Helper::getY, &Vec3Helper::setY,"")
        .add_property("z", &Vec3Helper::getZ, &Vec3Helper::setZ,"")
        .def("__len__", &Vec3Helper::len)
        .def("__getitem__", &Vec3Helper::getItem)
        .def("__str__", &Vec3Helper::str)
        .def("__repr__", &Vec3Helper::repr)
        .def("__hash__", &Vec3Helper::getHash)
        .def("getNormalized", &Vec3Helper::safeGetNormalized)
        .def("getNorm", &Vec3Helper::getNorm)
//        .def("getRotated", &getRotated)
//        .def("getRotated", &getRotatedPivot)
        .def(self == self)
        .def(self != self)
        .def(-self)
        .def(self + self)
        .def(self - self)
        .def(float() * self)
        .def(self * float())
        .def(self / float())
    ;    
    
}

AVG_PLUGIN_API PyObject* registerPlugin()
{
#if PY_MAJOR_VERSION < 3
    initpyglm();
    PyObject* pyGLMModule = PyImport_ImportModule("pyglm");
#else
    PyObject* pyGLMModule = PyInit_pyglm();
#endif

    return pyGLMModule;

}

#endif
