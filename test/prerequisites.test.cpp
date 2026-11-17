// Basic prerequisites test: DeSiGNAR must compile, link and run with the
// compiler and paths chosen in config.mk.
#include <avltree.hpp>

int main()
{
    Designar::AVLTree<Designar::int_t> tree;
    tree.insert(1);

    return tree.search(1) != nullptr ? 0 : 1;
}
