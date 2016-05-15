/** 
 * 'Join or Die' model format reader.
 */

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "bdm/BdmLoader.hpp"
#include "bdm/BdmFile.hpp"

template<typename Type, int Count>
void export_(std::ostream &os, const bdm::Array<Type, Count> &array_) {
    for (auto value : array_.values) {
        os << value << " ";
    }
}

void export_(std::ostream &os, const bdm::BdmFile &model) {
    auto &vertices = model.vertices();

    // export vertices
    for (auto &face : model.faces()) {
        for (auto &index : face.values) {
            auto &vertex = vertices[index];

            os << "v ";
            export_(os, vertex);

            os << std::endl;
        }
    }

    // export texture coordinates
    for (auto &texcoord : model.texcoords()) {
        os << "vt ";

        bdm::TexCoord texcoord_ = texcoord;

        texcoord_.values[0] /= 32.0f;
        texcoord_.values[1] /= 64.0f;

        export_(os, texcoord_);

        os << std::endl;
    }

    // export face data
    for (size_t i=0; i<model.faces().size(); i++) {
        bdm::Face face;

        for (size_t j=0; j<3; j++) {
            face.values[j] = static_cast<uint16_t>(i*3) + static_cast<uint16_t>(j) + 1;
        }

        os << "f ";

        export_(os, face);

        os << std::endl;
    }
}

void display( const bdm::BdmFile &file) {
    // output texture filenames
    auto textures = file.textures();
    for (auto texture : textures) {
        std::cout << texture << std::endl;
    }

    // output vertex data
    std::cout << "\nVertices" << std::endl;
    auto vertices = file.vertices();
    for (auto vertex : vertices) {
        std::cout << vertex << std::endl;
    }

    // output face data
    std::cout << "\nFaces" << std::endl;
    auto faces = file.faces();
    for (auto face : faces) {
        std::cout << face << std::endl;
    }

    //  output texture coordinate
    std::cout << "\nTexture Coordinate" << std::endl;
    auto texcoords = file.texcoords();
    for (auto value : texcoords) {
        std::cout << value << std::endl;
    }
}

int main(int argc, char **argv) {
    const std::string filename = argv[1];

    bdm::BdmFile file (filename.c_str());

    export_(std::cout, file);

    return 0;
}
