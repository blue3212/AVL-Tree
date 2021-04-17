#include <string>
#include <iostream>
#include <vector>
#include <iostream>
using std::cout;
using std::vector;
using std::cin;
using std::string;
// A super class was overlooked for data management
// as such, delete will be manually done in the main function.
// A destructor in the node class will delete all nodes of a tree from the root
struct AVLNode
{
	string NAME;
	int ID;
	AVLNode* left;
	AVLNode* right;
	int balanceFactor;
	AVLNode(string name, int id)
	{
		NAME = name;
		ID = id;
		left = nullptr;
		right = nullptr;

		balanceFactor = 0;
	}
	//Slightly slap dash destructor that requires the tree's root
	//to be deleted in main
	~AVLNode()
	{
		if (left != nullptr)
		{
			delete left;
		}
		if (right != nullptr)
		{
			delete right;
		}
	}
	static AVLNode* insertNameID(AVLNode* root, string name, int id);
	static AVLNode* removeID(AVLNode* root, int id);
	static void searchID(AVLNode* root, int id);
	static void searchName(AVLNode* root, string name);
	static void printInorder(AVLNode* root);
	static void printPreorder(AVLNode* root);
	static void printPostorder(AVLNode* root);
	static void printLevelCount(AVLNode* root);
	static AVLNode* removeInorderN(AVLNode* root, int n);
	static bool isAlphanumeric(string name);
private:
	static int populateBalanceFactors(AVLNode* root);
	static void helperInsertNameID(AVLNode* root, string name, int id, vector<AVLNode*>& trace);
	static void helperPrintPreorder(AVLNode* root);
	static void helperPrintInorder(AVLNode* root, string& append);
	static void helperPrintPostorder(AVLNode* root, string& append);
	static AVLNode* leftRotate(AVLNode* unbalNode);
	static AVLNode* rightRotate(AVLNode* unbalNode);
	static AVLNode* helperSearchID(AVLNode* root, int id);
	static AVLNode** helperRemoveID(AVLNode* root, int id, AVLNode* parentNode);
	static AVLNode* helperRemoveInorderN(AVLNode* root, int* n);
	static bool helperSearchName(AVLNode* root, string name);

};

// Left tree rotation, the returned node pointer is the new
// root of the subtree that had "unbalNode" as it's original root
AVLNode* AVLNode::leftRotate(AVLNode* unbalNode)
{
	AVLNode* rightNode = unbalNode->right;
	unbalNode->right = nullptr;
	if (rightNode->left != nullptr)
	{
		unbalNode->right = rightNode->left;
		rightNode->left = nullptr;
	}
	rightNode->left = unbalNode;
	return rightNode;
}

// Right tree rotation, the returned node pointer is the new
// root of the subtree that had "unbalNode" as it's original root
AVLNode* AVLNode::rightRotate(AVLNode* unbalNode)
{
	AVLNode* leftNode = unbalNode->left;
	unbalNode->left = nullptr;
	if (leftNode->right != nullptr)
	{
		unbalNode->left = leftNode->right;
	}
	leftNode->right = unbalNode;
	return leftNode;
}

// Updates the Balance Factors to the current makeup of the Tree
int AVLNode::populateBalanceFactors(AVLNode* root)
{
	if (root == nullptr)
		return 0;
	int leftVal = 0;
	int rightVal = 0;
	if (root->left != nullptr)
	{
		leftVal = populateBalanceFactors(root->left);
	}
	if (root->right != nullptr)
	{
		rightVal = populateBalanceFactors(root->right);
	}
	root->balanceFactor = rightVal - leftVal;
	if (leftVal > rightVal)
	{
		return leftVal + 1;
	}
	else
	{
		return rightVal + 1;
	}
}

// Insert a name, id pair. This traces the nodes passed through to
// find the unbalanced node closest to the newly inserted node
// Returns root
AVLNode* AVLNode::insertNameID(AVLNode* root, string name, int id)
{
	if (id > 99999999 || id < 0)
	{
		return root;
	}
	// Special case where the root is nullptr
	if (root == nullptr)
	{
		root = new AVLNode(name, id);
		cout << "successful" << '\n';
		return root;
	}
	vector<AVLNode*> trace;
	helperInsertNameID(root, name, id, trace);
	populateBalanceFactors(root);

	//Pointer to the unbalanced node closest to the newly inserted node
	AVLNode* unbalNode = nullptr;
	int unbalNodeIndex = 0;
	for (int i = trace.size() - 1; i >= 0; --i)
	{
		AVLNode* temp = trace[i];
		if (temp->balanceFactor == -2 || temp->balanceFactor == 2)
		{
			unbalNode = temp;
			unbalNodeIndex = i;
			break;
		}
	}
	//Tree rotations called by function
	if (unbalNode != nullptr)
	{
		//Special case with the root as there is no parent node to process
		if (unbalNode == root)
		{
			//left heavy tree
			if (unbalNode->balanceFactor == -2)
			{
				//left rotation (left-right tree)
				if (unbalNode->left->balanceFactor == 1)
				{
					unbalNode->left = leftRotate(unbalNode->left);
				}
				//right rotation
				return rightRotate(unbalNode);
			}
			//right heavy tree
			else
			{
				//right rotation (right-left tree)
				if (unbalNode->right->balanceFactor == -1)
				{
					unbalNode->right = rightRotate(unbalNode->right);
				}
				//left rotation
				return leftRotate(unbalNode);
			}
		}
		else
		{
			//Determine if unbalNode is a left or right child of it's parent and
			//sets a pointer to the (parent to unbalNode) pointer
			AVLNode** parentToUnbal = nullptr;
			if (trace[unbalNodeIndex - 1]->ID > unbalNode->ID)
			{
				parentToUnbal = &(trace[unbalNodeIndex - 1]->left);
			}
			else
			{
				parentToUnbal = &(trace[unbalNodeIndex - 1]->right);
			}

			//left heavy tree
			if (unbalNode->balanceFactor == -2)
			{
				//left rotation (left-right tree)
				if (unbalNode->left->balanceFactor == 1)
				{
					unbalNode->left = leftRotate(unbalNode->left);
				}
				//right rotation (Set the parent's pointer to the new subtree root)
				*parentToUnbal = rightRotate(unbalNode);
				return root;
			}
			//right heavy tree
			else
			{
				//right rotation (right-left tree)
				if (unbalNode->right->balanceFactor == -1)
				{
					unbalNode->right = rightRotate(unbalNode->right);
				}
				//left rotation (Set the parent's pointer to the new subtree root)
				*parentToUnbal = leftRotate(unbalNode);
				return root;
			}
		}
	}
	return root;
}

// Actually does the insertion by recursivly finding the location where the id should go.
// Prints "sucessful" if a new node was made or "unsucessful" if a duplicate id was found
// Should be tail-recursive.
// A vector is used to keep track of each node that is passed by during insertion
void AVLNode::helperInsertNameID(AVLNode* root, string name, int id, vector<AVLNode*>& trace)
{
	if (id > root->ID)
	{
		if (root->right == nullptr)
		{
			root->right = new AVLNode(name, id);
			cout << "successful" << '\n';
		}
		else
		{
			trace.push_back(root);
			helperInsertNameID(root->right, name, id, trace);
		}

	}
	else if (id < root->ID)
	{
		if (root->left == nullptr)
		{
			root->left = new AVLNode(name, id);
			cout << "successful" << '\n';
		}
		else
		{
			trace.push_back(root);
			helperInsertNameID(root->left, name, id, trace);
		}
	}
	else
	{
		cout << "unsuccessful" << '\n';
	}
}

// Checks if a name is valid and if so, sends it to the helper function
// to perform an name search and print ID's
void AVLNode::searchName(AVLNode* root, string name)
{
	if (!isAlphanumeric(name))
	{
		cout << "unsuccessful" << '\n';
		return;
	}
	if (root == nullptr)
	{
		cout << "unsuccessful" << '\n';
		return;
	}
	if (!helperSearchName(root, name))
	{
		cout << "unsuccessful" << '\n';
	}
}

// Prints ID's associated with a name in a preorder traversal
bool AVLNode::helperSearchName(AVLNode* root, string name)
{
	bool leftBool = false;
	bool rightBool = false;
	if (name == root->NAME)
	{
		int decimalSizes = 10000000;
		while (root->ID < decimalSizes)
		{
			cout << 0;
			decimalSizes /= 10;
		}
		cout << root->ID << '\n';
		return true;
	}
	if (root->left != nullptr)
	{
		leftBool = helperSearchName(root->left, name);
	}
	if (root->right != nullptr)
	{
		rightBool = helperSearchName(root->right, name);
	}
	return leftBool || rightBool;
}

// Finds if a string is all alphabet character or spaces
bool AVLNode::isAlphanumeric(string name)
{
	for (auto iter = name.begin(); iter != name.end(); ++iter)
	{
		char temp = *iter;
		if (!((65 <= temp && temp <= 90) || (97 <= temp && temp <= 122) || temp == 32))
		{
			return false;
		}
	}
	return true;
}

// Uses the result of findID to print the name of result if the result it a valid AVLNode*
// or "unsucessful" if the result is nullptr
void AVLNode::searchID(AVLNode* root, int id)
{
	if (root == nullptr)
	{
		cout << "unsuccessful" << '\n';
		return;
	}
	AVLNode* result = helperSearchID(root, id);
	if (result == nullptr)
	{
		cout << "unsuccessful" << '\n';
	}
	else
	{
		cout << result->NAME << '\n';
	}
}

//Recursively searches for an ID and returns a pointer to that 
//ID or nullptr if it does not exist
AVLNode* AVLNode::helperSearchID(AVLNode* root, int id)
{
	if (id > root->ID)
	{
		if (root->right == nullptr)
		{
			return nullptr;
		}
		else
		{
			helperSearchID(root->right, id);
		}

	}
	else if (id < root->ID)
	{
		if (root->left == nullptr)
		{
			return nullptr;
		}
		else
		{
			helperSearchID(root->left, id);
		}
	}
	else
	{
		return root;
	}

}

// Removes a valid node and replaces it with inorder sucessor unless the node
// is a leaf
AVLNode* AVLNode::removeID(AVLNode* root, int id)
{
	if (root == nullptr)
	{
		cout << "unsuccessful" << '\n';
		return nullptr;
	}
	AVLNode** result = helperRemoveID(root, id, root);
	if (result == nullptr)
	{
		cout << "unsuccessful" << '\n';
	}
	else if (*result == root)
	{
		cout << "successful" << '\n';
		if (root->right == nullptr)
		{
			AVLNode* newRoot = nullptr;
			if (root->left != nullptr)
			{
				newRoot = root->left;
			}
			delete root;
			delete result;
			return newRoot;
		}
		AVLNode* inorderSucessor = root->right;
		AVLNode* sucessorParent = nullptr;
		while (inorderSucessor->left != nullptr)
		{
			sucessorParent = inorderSucessor;
			inorderSucessor = inorderSucessor->left;
		}

		if (sucessorParent == nullptr)
		{
			root->right = inorderSucessor->right;
		}
		else
		{
			sucessorParent->left = inorderSucessor->right;
		}
		inorderSucessor->right = nullptr;

		root->NAME = inorderSucessor->NAME;
		root->ID = inorderSucessor->ID;
		delete inorderSucessor;
		delete result;
	}
	else
	{
		cout << "successful" << '\n';
		if ((*result)->left == nullptr && (*result)->right == nullptr)
		{
			delete (*result);
			(*result) = nullptr;
			return root;
		}
		AVLNode* inorderSucessor = (*result)->right;
		AVLNode* sucessorParent = nullptr;
		while (inorderSucessor->left != nullptr)
		{
			sucessorParent = inorderSucessor;
			inorderSucessor = inorderSucessor->left;
		}

		if (sucessorParent == nullptr)
		{
			(*result)->right = inorderSucessor->right;
		}
		else
		{
			sucessorParent->left = inorderSucessor->right;
		}
		inorderSucessor->right = nullptr;


		(*result)->NAME = inorderSucessor->NAME;
		(*result)->ID = inorderSucessor->ID;
		delete inorderSucessor;
		(*result) = nullptr;
	}
	return root;
}

// If nullptr is returned, the requested id does not exist
// If the tree's root's pointer's address is returned, the root must be removed
// Otherwise, a valid AVLNode** is the pointer from the parent of the node that must be removed
AVLNode** AVLNode::helperRemoveID(AVLNode* root, int id, AVLNode* parentNode)
{
	if (id > root->ID)
	{
		if (root->right == nullptr)
		{
			return nullptr;
		}
		else
		{
			helperRemoveID(root->right, id, root);
		}

	}
	else if (id < root->ID)
	{
		if (root->left == nullptr)
		{
			return nullptr;
		}
		else
		{
			helperRemoveID(root->left, id, root);
		}
	}
	else
	{
		if (parentNode == root)
		{
			return new AVLNode * (root);
		}
		else
		{
			if (parentNode->ID > id)
			{
				return &(parentNode->left);
			}
			else
			{
				return &(parentNode->right);
			}
		}
	}
}

// Uses removeID to remove a root found from the helper
// Function
AVLNode* AVLNode::removeInorderN(AVLNode* root, int n)
{
	n += 1;
	int* count = &n;
	AVLNode* result = helperRemoveInorderN(root, count);
	if (result == nullptr)
	{
		return root;
	}
	else
	{
		return removeID(root, result->ID);
	}
}

// Finds the root to remove using an inorder search
AVLNode* AVLNode::helperRemoveInorderN(AVLNode* root, int* n)
{
	AVLNode* leftNodes = nullptr;
	AVLNode* rightNodes = nullptr;
	if (root->left != nullptr)
	{
		leftNodes = helperRemoveInorderN(root->left, n);
	}
	--(*n);
	if (*n == 0)
	{
		return root;
	}
	if (root->right != nullptr)
	{
		rightNodes = helperRemoveInorderN(root->right, n);
	}
	if (leftNodes != nullptr)
	{
		return leftNodes;
	}
	if (rightNodes != nullptr)
	{
		return rightNodes;
	}
	return nullptr;
}

// Prints names by preorder traversal
void AVLNode::printPreorder(AVLNode* root)
{
	if (root == nullptr)
	{
		return;
	}
	cout << root->NAME;
	if (root->left != nullptr)
	{
		helperPrintPreorder(root->left);
	}
	if (root->right != nullptr)
	{
		helperPrintPreorder(root->right);
	}
	cout << '\n';
}

// Helper function for printPreorder to circuvent the
// post-fencing problem with the commas
void AVLNode::helperPrintPreorder(AVLNode* root)
{
	cout << ", " << root->NAME;
	if (root->left != nullptr)
	{
		helperPrintPreorder(root->left);
	}
	if (root->right != nullptr)
	{
		helperPrintPreorder(root->right);
	}
}

// Prints names by inorder traversal
void AVLNode::printInorder(AVLNode* root)
{
	string append = "";
	if (root == nullptr)
	{
		return;
	}
	if (root->left != nullptr)
	{
		helperPrintInorder(root->left, append);
	}
	append += root->NAME + ", ";
	if (root->right != nullptr)
	{
		helperPrintInorder(root->right, append);
	}
	append = append.substr(0, append.size() - 2);
	cout << append << '\n';
}

// Helper function for printInorder to circuvent the
// post-fencing problem with the commas
void AVLNode::helperPrintInorder(AVLNode* root, string& append)
{
	if (root->left != nullptr)
	{
		helperPrintInorder(root->left, append);
	}
	append += root->NAME + ", ";
	if (root->right != nullptr)
	{
		helperPrintInorder(root->right, append);
	}
}

// Prints names by postorder traversal
void AVLNode::printPostorder(AVLNode* root)
{
	string append = "";
	if (root == nullptr)
	{
		return;
	}
	if (root->left != nullptr)
	{
		helperPrintPostorder(root->left, append);
	}
	if (root->right != nullptr)
	{
		helperPrintPostorder(root->right, append);
	}
	append += root->NAME;
	cout << append << '\n';
}

// Helper function for printPostorder to circuvent the
// post-fencing problem with the commas
void AVLNode::helperPrintPostorder(AVLNode* root, string& append)
{
	if (root->left != nullptr)
	{
		helperPrintPostorder(root->left, append);
	}
	if (root->right != nullptr)
	{
		helperPrintPostorder(root->right, append);
	}
	append += root->NAME + ", ";
}

// Prints the return value from populateBalanceFactors
void AVLNode::printLevelCount(AVLNode* root)
{
	cout << populateBalanceFactors(root) << '\n';
}

//your AVL tree implementation here


int main()
{
	//your code to invoke the respective commands
	int numberOfCommands = 0;
	cin >> numberOfCommands;
	string command;
	AVLNode* root = nullptr;
	for (int i = 0; i < numberOfCommands; ++i)
	{
		cin >> command;
		if (command == "insert")
		{
			string name;
			int id;
			cin >> name;
			name = name.substr(1, name.size() - 2);
			cin >> id;
			root = AVLNode::insertNameID(root, name, id);
		}
		else if (command == "remove")
		{
			int id;
			cin >> id;
			root = AVLNode::removeID(root, id);
		}
		else if (command == "search")
		{
			string input;
			cin >> input;
			if (input.find("\"") != string::npos)
			{
				input = input.substr(1, input.size() - 2);
				AVLNode::searchName(root, input);
			}
			else
			{
				int id = std::stoi(input);
				AVLNode::searchID(root, id);
			}
		}
		else if (command == "removeInorder")
		{
			int position;
			cin >> position;
			root = AVLNode::removeInorderN(root, position);
		}
		//Print statements
		else
		{
			if (command == "printInorder")
			{
				AVLNode::printInorder(root);
			}
			else if (command == "printPreorder")
			{
				AVLNode::printPreorder(root);
			}
			else if (command == "printPostorder")
			{
				AVLNode::printPostorder(root);
			}
			else
			{
				AVLNode::printLevelCount(root);
			}
		}
	}
	delete root;
}