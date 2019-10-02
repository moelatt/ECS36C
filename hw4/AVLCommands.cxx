#include "AVL.h"

#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: AVLCommands commands_filename_json" << std::endl;
        return 0;
    }

    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cout << "Couldn't open the file " << argv[1] << std::endl;
        return 0;
    }

    nlohmann::json json;
    fin >> json;

    //int num_ops = json.find("metadata").value().begin().value();

    //std::cout << num_ops << std::endl;

    AVLTree tree;

    for (nlohmann::json::iterator it = json.begin(); it != json.end(); ++it)
    {
        if (it.key() == "metadata")
            continue;
        int key;
        std::string operation;
        for (auto ittt = it.value().begin(); ittt != it.value().end(); ++ittt)
        {
            //std::cout << ittt.key() << " : " << ittt.value()<< "\n";
            if (ittt.key() == "key")
                key = ittt.value();
            else if (ittt.key() == "operation")
                operation = ittt.value();
        }
        if (operation == "Insert")
            tree.Insert(key);
        else if (operation == "Delete")
            tree.Delete(key);
        else if (operation == "DeleteMin")
            tree.DeleteMin();
    }
    std::cout << tree.JSON();

    return 0;
}
