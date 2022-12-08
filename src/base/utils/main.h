#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include <fstream>
#include <functional>

#include "../triple/main.h"

#define invalidArgument invalid_argument

using namespace std;


// class Foo {
//     INT foo;
// 
// 
//     public:
//         Foo() {};
// 
//         Foo(INT value) {
//             foo = value;
//         }
// };
// 
// class Bar {
//     Foo foo;
// 
//     Bar(Foo value) {
//         foo = Foo(2);
//     }
// };


namespace meager::main::utils {

    // template <typename T>
    // struct ComputedProperty {
    //     function<void(T*)> compute;
    //     T value;
    //     bool computed;

    //     ComputedProperty() {}

    //     ComputedProperty(function<void(T*)> compute) {
    //         this->compute = compute;
    //         computed = false;
    //     }

    //     public:
    //         operator T () const {
    //             if (computed)
    //                 return value;
    //             compute(&value);
    //             return value;
    //         }
    // };

    enum class SubsetType {
        train,
        test,
        valid
    };

    SubsetType decodeSubsetType(string name);

    class File {
        bool length_is_set;

    public:
        ifstream stream;
        string path;
        INT length;

        File(string path, bool verbose) {
            this->stream.open(path);

            if (!stream.is_open()) {
                cerr << "file " << path << " does not exist" << endl;
                throw invalidArgument("File " + path + " does not exist");
            }

            this->path = path;

            length_is_set = false;
        }

        void close() {
            stream.close();
            // foo = 2;
        }

        INT getLength() {
            if (length_is_set)
                return length;
            stream >> length;
            length_is_set = true;
            return length;
        }

        bool good() {
            return stream.good();
        }

        friend File& operator>>(File& file, long& value);
    };

    // File* readNumberOfElements(std::string path, bool verbose = false);
    // File* readNumberOfTriples(std::string path, bool verbose = false);
    // File* readNumberOfTypeConstrainedRelations(std::string path, bool verbose = false);

}


#endif
