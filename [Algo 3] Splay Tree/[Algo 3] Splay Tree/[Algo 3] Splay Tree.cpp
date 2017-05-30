#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class student;
class group {
public:
	std::string name;
	std::vector<student*> groupList;

	group(std::string a) {
		name = a;
	}
};

class student {
public:
	std::string name;
	std::string homeCountry;
	std::string homeCity;
	double rating;
	group* specialization;

	student(std::string name, std::string country, std::string city, double rating, group* s) {
		this->name = name;
		homeCountry = country;
		homeCity = city;
		this->rating = rating;
		specialization = s;
	}

	void show() const {
		std::cout << "Student Name: " << name << "\nStudent Country: " << homeCountry << "\nStudent City: " << homeCity << "\nStudent grade: " << rating << std::endl;
	}
};

struct Node {
	student* data;
	Node* parent;
	Node* left;
	Node* right;

	Node(student* key) : data(key), parent(nullptr), left(nullptr), right(nullptr) {}
};

class SplayTree {
	
	Node *root;

	void zig(Node*);
	void zig_zig(Node*);
	void zig_zag(Node*);

	void splay(Node*);

public:
	SplayTree() : root(nullptr) {}
	SplayTree(Node* r) : root(r) {}
	Node* find(std::string);
	void insert(student* value);
	void Delete(std::string);
	Node* subtree_max(Node* subRoot);
	Node* subtree_min(Node* subRoot);
};

void SplayTree::zig(Node *x) {
	Node *p = x->parent;
	if (p->left == x) {
		Node *A = x->left;
		Node *B = x->right;
		Node *C = p->right;

		x->parent = nullptr;
		x->right = p;

		p->parent = x;
		p->left = B;

		if (B != nullptr) B->parent = p;
	}
	else {
		Node *A = p->left;
		Node *B = x->left;
		Node *C = x->right;

		x->parent = nullptr;
		x->left = p;

		p->parent = x;
		p->right = B;

		if (B != nullptr) B->parent = p;
	}
}

void SplayTree::zig_zig(Node *x)
{
	Node *p = x->parent;
	Node *g = p->parent;
	if (p->left == x) {
		Node *A = x->left;
		Node *B = x->right;
		Node *C = p->right;
		Node *D = g->right;

		x->parent = g->parent;
		x->right = p;

		p->parent = x;
		p->left = B;
		p->right = g;

		g->parent = p;
		g->left = C;


		if (x->parent != nullptr) {
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != nullptr) B->parent = p;

		if (C != nullptr) C->parent = g;
	}
	else {
		Node *A = g->left;
		Node *B = p->left;
		Node *C = x->left;
		Node *D = x->right;

		x->parent = g->parent;
		x->left = p;

		p->parent = x;
		p->left = g;
		p->right = C;

		g->parent = p;
		g->right = B;

		if (x->parent != nullptr) {
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != nullptr) B->parent = g;

		if (C != nullptr) C->parent = p;
	}
}

void SplayTree::zig_zag(Node *x) {
	Node *p = x->parent;
	Node *g = p->parent;
	if (p->right == x) {
		Node *A = p->left;
		Node *B = x->left;
		Node *C = x->right;
		Node *D = g->right;

		x->parent = g->parent;
		x->left = p;
		x->right = g;

		p->parent = x;
		p->right = B;

		g->parent = x;
		g->left = C;

		if (x->parent != nullptr) {
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != nullptr) B->parent = p;

		if (C != nullptr) C->parent = g;
	}
	else {
		Node *A = g->left;
		Node *B = x->left;
		Node *C = x->right;
		Node *D = p->right;

		x->parent = g->parent;
		x->left = g;
		x->right = p;

		p->parent = x;
		p->left = C;

		g->parent = x;
		g->right = B;

		if (x->parent != nullptr) {
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != nullptr) B->parent = g;

		if (C != nullptr) C->parent = p;
	}
}

void SplayTree::splay(Node *x) {
	while (x->parent != nullptr) {
		Node *p = x->parent;
		Node *g = p->parent;
		if (g == nullptr) zig(x);
		else if (g->left == p && p->left == x) zig_zig(x);
		else if (g->right == p && p->right == x) zig_zig(x);
		else zig_zag(x);
	}
	this->root = x;
}

Node* SplayTree::find(std::string x) {
	Node *ret = nullptr;
	Node *curr = this->root;
	Node *prev = nullptr;
	while (curr != nullptr) {
		prev = curr;
		if (x < curr->data->name) curr = curr->left;
		else if (x > curr->data->name) curr = curr->right;
		else {
			ret = curr;
			break;
		}
	}
	if (ret != nullptr) splay(ret);
	else if (prev != nullptr) splay(prev);
	return ret;
}

void SplayTree::insert(student* x) {
	if (root == nullptr) {
		root = new Node(x);
		return;
	}
	Node *curr = this->root;
	while (curr != nullptr)	{
		if (x->name < curr->data->name)	{
			if (curr->left == nullptr) {
				Node *newNode = new Node(x);
				curr->left = newNode;
				newNode->parent = curr;
				splay(newNode);
				return;
			}
			else curr = curr->left;
		}
		else if (x->name > curr->data->name) {
			if (curr->right == nullptr)	{
				Node *newNode = new Node(x);
				curr->right = newNode;
				newNode->parent = curr;
				splay(newNode);
				return;
			}
			else curr = curr->right;
		}
		else {
			splay(curr);
			return;
		}
	}
}

Node* SplayTree::subtree_max(Node* subRoot) {
	Node *curr = subRoot;
	while (curr->right != nullptr) curr = curr->right;
	return curr;
}

Node* SplayTree::subtree_min(Node* subRoot) {
	Node *curr = subRoot;
	while (curr->left != nullptr) curr = curr->left;
	return curr;
}

void SplayTree::Delete(std::string x) {
	Node *del = find(x);
	if (del == nullptr) {
		return;
	}
	Node *L = del->left;
	Node *R = del->right;
	if (L == nullptr && R == nullptr) {
		this->root = nullptr;
	}
	else if (L == nullptr) {
		Node *M = subtree_min(R);
		splay(M);
	}
	else if (R == nullptr) {
		Node *M = subtree_max(L);
		splay(M);
	}
	else {
		Node *M = subtree_max(L);
		splay(M);
		M->right = R;
		R->parent = M;
	}
	delete del;
}

int main() {
	std::vector<group*> Groups(10);
	std::vector<student*> Students(100);

	SplayTree* t = new SplayTree();

	std::ifstream in("base.txt");
	std::string temp, tempCountry, tempCity, tempSpec, tempR;
	double tempRating;

	for (int i = 0; i < 10; i++) {
		getline(in, temp);
		Groups[i] = new group(temp);
	}

	getline(in, temp);

	for (int i = 0; i < 100; i++) {
		getline(in, temp);
		getline(in, tempR);
		getline(in, tempCity);
		getline(in, tempCountry);
		getline(in, tempSpec);
		tempRating = atof(tempR.c_str());
		Students[i] = new student(temp, tempCountry, tempCity, tempRating, new group(tempSpec));

		t->insert(Students[i]);

		for (int j = 0; j < 10; j++) {
			if (Groups[j]->name == temp) {
				Groups[j]->groupList.push_back(Students[i]);
			}
		}
		getline(in, temp);
	}

	t->find(std::string("Ileana Gordy"))->data->show();
	t->Delete(std::string("Ileana Gordy"));
	t->find(std::string("Ileana Gordy"))->data->show();

	system("pause");
    return 0;
}