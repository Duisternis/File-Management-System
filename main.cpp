#include <pybind11/pybind11.h>
#include <iostream>
namespace py = pybind11;

int bootstrap(std::string, std::string, std::string);

PYBIND11_MODULE(sortingEngine, handle)
{
    handle.doc() = "This is the module docs. Teehee!!";
    handle.def("startupRequest", &bootstrap);
}