#include "wezel.hh"

typedef shared_ptr <Node> PTR_NODE;

//KLASA DRZEWA 
class DecisionTree
{
	
	PTR_NODE root;
public:
	
	DecisionTree() {root = NULL;};

	PTR_NODE wez_root() {return root;};
	void ustaw_root(PTR_NODE ptr) {root = ptr;};
};
