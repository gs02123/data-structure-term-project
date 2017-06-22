
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

enum color { RED, BLACK };

typedef struct rbnode {
	int key;
	struct rbnode* left;
	struct rbnode* right;
	struct rbnode* parent;
	enum color color;
} *rbnode;

typedef struct rbtree {
	rbnode root;
} *rbtree;

typedef rbnode node;
typedef enum color color;

rbtree createtree();
int compare(int left, int right);
void rbtree_lookup(rbtree t, int key);
void Insert(rbtree t, int key);
void Delete(rbtree t, int key);

node grandparent(node n);
node sibling(node n);
node uncle(node n);
color node_color(node n);
void checkViolation(rbtree t);
void property1(node root);
void property2(node root);
void property4(node root);
void property5(node root);
void property5_implement(node n, int bc, int bcpath);

node createnode(int key, color node_color, node left, node right);
node lookup_node(rbtree t, int key);
node maxnode(node root);
void leftrotate(rbtree t, node n);
void rightrotate(rbtree t, node n);

void transplant(rbtree t, node oldn, node newn);
void insert1(rbtree t, node n);
void insert2(rbtree t, node n);
void insert3(rbtree t, node n);
void insert4(rbtree t, node n);
void insert5(rbtree t, node n);

void delete1(rbtree t, node n);
void delete2(rbtree t, node n);
void delete3(rbtree t, node n);
void delete4(rbtree t, node n);
void delete5(rbtree t, node n);
void delete6(rbtree t, node n);


node grandparent(node n) {
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return n->parent->parent;
}

node sibling(node n) {
	assert(n != NULL);
	assert(n->parent != NULL);
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}

node uncle(node n) {
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return sibling(n->parent);
}

void checkViolation(rbtree t) {
	property1(t->root);
	property2(t->root);
	property4(t->root);
	property5(t->root);
}

void property1(node n) {
	assert(node_color(n) == RED || node_color(n) == BLACK);
	if (n == NULL) return;
	property1(n->left);
	property1(n->right);
}

void property2(node root) {
	assert(node_color(root) == BLACK);
}

color node_color(node n) {
	return n == NULL ? BLACK : n->color;
}

void property4(node n) {
	if (node_color(n) == RED) {
		assert(node_color(n->left) == BLACK);
		assert(node_color(n->right) == BLACK);
		assert(node_color(n->parent) == BLACK);
	}
	if (n == NULL) return;
	property4(n->left);
	property4(n->right);
}

void property5(node root) {
	int black_count_path = -1;
	property5_implement(root, 0, black_count_path);
}

void property5_implement(node n, int bc, int bcpath) {
	if (node_color(n) == BLACK) {
		bc++;
	}
	if (n == NULL) {
		if (bcpath == -1) {
			bcpath = bc;
		}
		else {
			assert(bc == bcpath);
		}
		return;
	}
	property5_implement(n->left, bc, bcpath);
	property5_implement(n->right, bc, bcpath);
}

rbtree createtree() {
	rbtree t = malloc(sizeof(struct rbtree));
	t->root = NULL;
	checkViolation(t);
	return t;
}

node createnode(int key, color node_color, node left, node right) {
	node result = malloc(sizeof(struct rbnode));
	result->key = key;
	result->color = node_color;
	result->left = left;
	result->right = right;
	if (left != NULL)  left->parent = result;
	if (right != NULL) right->parent = result;
	result->parent = NULL;
	return result;
}

node lookup_node(rbtree t, int key) {

	node n = t->root;
	while (n != NULL) {
		int result = compare(key, n->key);
		if (result == 0) {
			return n;
		}
		else if (result < 0) {
			n = n->left;
		}
		else {
			assert(result > 0);
			n = n->right;
		}
	}
	return n;
}

void rbtree_lookup(rbtree t, int key) {
	node n = lookup_node(t, key);
	return n == NULL ? NULL : n->key;
}

void leftrotate(rbtree t, node n) {
	node r = n->right;
	transplant(t, n, r);
	n->right = r->left;
	if (r->left != NULL) {
		r->left->parent = n;
	}
	r->left = n;
	n->parent = r;
}

void rightrotate(rbtree t, node n) {
	node L = n->left;
	transplant(t, n, L);
	n->left = L->right;
	if (L->right != NULL) {
		L->right->parent = n;
	}
	L->right = n;
	n->parent = L;
}

void transplant(rbtree t, node oldn, node newn) {
	if (oldn->parent == NULL) {
		t->root = newn;
	}
	else {
		if (oldn == oldn->parent->left)
			oldn->parent->left = newn;
		else
			oldn->parent->right = newn;
	}
	if (newn != NULL) {
		newn->parent = oldn->parent;
	}
}

void Insert(rbtree t, int key) {
	node newnode = createnode(key, RED, NULL, NULL);
	if (t->root == NULL) {
		t->root = newnode;
	}
	else {
		node n = t->root;
		while (1) {
			int comp_result = compare(key, n->key);
			if (comp_result <= 0) {
				if (n->left == NULL) {
					n->left = newnode;
					break;
				}
				else {
					n = n->left;
				}
			}
			else {
				assert(comp_result > 0);
				if (n->right == NULL) {
					n->right = newnode;
					break;
				}
				else {
					n = n->right;
				}
			}
		}
		newnode->parent = n;
	}
	insert1(t, newnode);
	checkViolation(t);
}

void insert1(rbtree t, node n) {
	if (n->parent == NULL)
		n->color = BLACK;
	else
		insert2(t, n);
}

void insert2(rbtree t, node n) {
	if (node_color(n->parent) == BLACK)
		return;
	else
		insert3(t, n);
}

void insert3(rbtree t, node n) {
	if (node_color(uncle(n)) == RED) {
		n->parent->color = BLACK;
		uncle(n)->color = BLACK;
		grandparent(n)->color = RED;
		insert1(t, grandparent(n));
	}
	else {
		insert4(t, n);
	}
}

void insert4(rbtree t, node n) {
	if (n == n->parent->right && n->parent == grandparent(n)->left) {
		leftrotate(t, n->parent);
		n = n->left;
	}
	else if (n == n->parent->left && n->parent == grandparent(n)->right) {
		rightrotate(t, n->parent);
		n = n->right;
	}
	insert5(t, n);
}

void insert5(rbtree t, node n) {
	n->parent->color = BLACK;
	grandparent(n)->color = RED;
	if (n == n->parent->left && n->parent == grandparent(n)->left) {
		rightrotate(t, grandparent(n));
	}
	else {
		assert(n == n->parent->right && n->parent == grandparent(n)->right);
		leftrotate(t, grandparent(n));
	}
}

void Delete(rbtree t, int key) {
	node child;
	node n = lookup_node(t, key);
	if (n == NULL) return;
	if (n->left != NULL && n->right != NULL) {
		node pred = maxnode(n->left);
		n->key = pred->key;
		n = pred;
	}

	assert(n->left == NULL || n->right == NULL);
	child = n->right == NULL ? n->left : n->right;
	if (node_color(n) == BLACK) {
		n->color = node_color(child);
		delete1(t, n);
	}
	transplant(t, n, child);
	if (n->parent == NULL && child != NULL)
		child->color = BLACK;
	free(n);

	checkViolation(t);
}

node maxnode(node n) {
	assert(n != NULL);
	while (n->right != NULL) {
		n = n->right;
	}
	return n;
}

void delete1(rbtree t, node n) {
	if (n->parent == NULL)
		return;
	else
		delete2(t, n);
}

void delete2(rbtree t, node n) {
	if (node_color(sibling(n)) == RED) {
		n->parent->color = RED;
		sibling(n)->color = BLACK;
		if (n == n->parent->left)
			leftrotate(t, n->parent);
		else
			rightrotate(t, n->parent);
	}
	delete3(t, n);
}

void delete3(rbtree t, node n) {
	if (node_color(n->parent) == BLACK &&
		node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->left) == BLACK &&
		node_color(sibling(n)->right) == BLACK)
	{
		sibling(n)->color = RED;
		delete1(t, n->parent);
	}
	else
		delete4(t, n);
}

void delete4(rbtree t, node n) {
	if (node_color(n->parent) == RED &&
		node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->left) == BLACK &&
		node_color(sibling(n)->right) == BLACK)
	{
		sibling(n)->color = RED;
		n->parent->color = BLACK;
	}
	else
		delete5(t, n);
}

void delete5(rbtree t, node n) {
	if (n == n->parent->left &&
		node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->left) == RED &&
		node_color(sibling(n)->right) == BLACK)
	{
		sibling(n)->color = RED;
		sibling(n)->left->color = BLACK;
		rightrotate(t, sibling(n));
	}
	else if (n == n->parent->right &&
		node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->right) == RED &&
		node_color(sibling(n)->left) == BLACK)
	{
		sibling(n)->color = RED;
		sibling(n)->right->color = BLACK;
		leftrotate(t, sibling(n));
	}
	delete6(t, n);
}

void delete6(rbtree t, node n) {
	sibling(n)->color = node_color(n->parent);
	n->parent->color = BLACK;
	if (n == n->parent->left) {
		assert(node_color(sibling(n)->right) == RED);
		sibling(n)->right->color = BLACK;
		leftrotate(t, n->parent);
	}
	else
	{
		assert(node_color(sibling(n)->left) == RED);
		sibling(n)->left->color = BLACK;
		rightrotate(t, n->parent);
	}
}

int compare(int left, int right) {
	if (left < right)
		return -1;
	else if (left > right)
		return 1;
	else {
		assert(left == right);
		return 0;
	}
}

node searchnode(node root, int key) {
	node n = root;
	while (n != NULL) {
		int comp_result = compare(key, n->key);
		if (comp_result == 0) {
			return n;
		}
		else if (comp_result < 0) {
			n = n->left;
		}
		else {
			assert(comp_result > 0);
			n = n->right;
		}
	}
	return n;
}

int minsearch(node root) {

	if (root == NULL)
	{
		return;
	}
	rbnode temp = root;
	while (temp->left != NULL)
	{
		temp = temp->left;
	}
	int min = temp->key;

	rbnode pre;
	rbnode current = root;

	while (current != NULL) {

		if (current->left != NULL) {

			pre = current->left;while (pre->right != NULL && pre->right != current) {
				pre = pre->right;
			}

			if (pre->right == NULL) {
				pre->right = current;
				current = current->left;
			}
			else if (pre->right != NULL)
			{
				pre->right = NULL;
				if (current->key < min)
				{
					min = current->key;
				}
				current = current->right;
			}

		}
		else if (current->left == NULL)
		{
			if (current->key < min)
			{
				min = current->key;
			}
			current = current->right;
		}
	}

	return min;

}
int maxsearch(node root) {

	if (root == NULL)
	{
		return;
	}
	rbnode temp = root;
	while (temp->left != NULL)
	{
		temp = temp->left;
	}
	int max = temp->key;

	rbnode pre;
	rbnode current = root;

	while (current != NULL) {

		if (current->left != NULL) {

			pre = current->left;
			while (pre->right != NULL && pre->right != current) {
				pre = pre->right;
			}

			if (pre->right == NULL) {
				pre->right = current;
				current = current->left;
			}
			else if (pre->right != NULL)
			{
				pre->right = NULL;
				if (current->key > max)
				{
					max = current->key;
				}
				current = current->right;
			}

		}
		else if (current->left == NULL)
		{
			if (current->key > max)
			{
				max = current->key;
			}
			current = current->right;
		}
	}

	return max;
}
void traversalsearch(rbnode root, int x) {
	int difference = 0;

	printf("x: %d\n", x);
	int mykey = 0;
	if (root == NULL)

	{
		printf("NIL NIL NIL\n");
		return;
	}

	node n = searchnode(root, x);

	rbnode predecessor;
	rbnode current = root;
	rbnode temp = root;

	while (temp->left != NULL)
	{
		temp = temp->left;
	}
	int  smaller = temp->key;
	int bigger = temp->key;
	int min = -9999999;
	int max = 9999999;


	if (n != NULL)
	{
		mykey = x;

		if (n->key == minsearch(root))
		{
			while (current != NULL) {

				if (current->left != NULL) {

					predecessor = current->left;
					while (predecessor->right != NULL && predecessor->right != current) {
						predecessor = predecessor->right;
					}

					if (predecessor->right == NULL) {
						predecessor->right = current;
						current = current->left;
					}
					else if (predecessor->right != NULL) {
						predecessor->right = NULL;

						if (current->key - x != 0)
						{

							difference = current->key - x;



							if (difference > 0 && difference < max)
							{
								max = difference;
								bigger = current->key;
							}

						}


						current = current->right;
					}

				}
				else if (current->left == NULL)
				{

					if (current->key - x != 0)
					{

						difference = current->key - x;



						if (difference > 0 && difference < max)
						{
							max = difference;
							bigger = current->key;
						}

					}


					current = current->right;
				}
			}
		}
		if (n->key == maxsearch(root))
		{

			while (current != NULL) {

				if (current->left != NULL) {

					predecessor = current->left;
					while (predecessor->right != NULL && predecessor->right != current) {
						predecessor = predecessor->right;
					}

					if (predecessor->right == NULL) {
						predecessor->right = current;
						current = current->left;
					}
					else if (predecessor->right != NULL) {
						predecessor->right = NULL;

						if (current->key - x != 0)
						{

							difference = current->key - x;



							if (difference < 0 && difference > min)
							{
								min = difference;
								smaller = current->key;
							}

						}


						current = current->right;
					}

				}
				else if (current->left == NULL)
				{

					if (current->key - x != 0)
					{

						difference = current->key - x;


						if (difference < 0 && difference > min)
						{
							min = difference;
							smaller = current->key;
						}
						
					}


					current = current->right;
				}
			}
		}
		if (n->key != minsearch(root) && n->key != maxsearch(root))
		{
			while (current != NULL) {

				if (current->left != NULL) {

					predecessor = current->left;
					while (predecessor->right != NULL && predecessor->right != current) {
						predecessor = predecessor->right;
					}

					if (predecessor->right == NULL) {     
						predecessor->right = current;
						current = current->left;
					}
					else if (predecessor->right != NULL) { 
						predecessor->right = NULL;
						if (current->key - x != 0)
						{

							difference = current->key - x;
							if (difference <0 && difference>min)
							{
								min = difference;
								smaller = current->key;
							}
							if (difference > 0 && difference < max)
							{
								max = difference;
								bigger = current->key;
							}
						}


						current = current->right;
					}

				}
				else if (current->left == NULL)
				{
					if (current->key - x != 0)
					{
						difference = current->key - x;
						if (difference <0 && difference>min)
						{
							min = difference;
							smaller = current->key;
						}
						if (difference > 0 && difference < max)
						{
							max = difference;
							bigger = current->key;
						}
					}

					current = current->right;
				}
			}
		}

	}

	else
	{
		mykey = -999;
		while (current != NULL) {

			if (current->left != NULL) {

				predecessor = current->left;
				while (predecessor->right != NULL && predecessor->right != current) {
					predecessor = predecessor->right;
				}

				if (predecessor->right == NULL) {
					predecessor->right = current;
					current = current->left;
				}
				else if (predecessor->right != NULL) {
					predecessor->right = NULL;
					if (current->key - x != 0)
					{

						difference = current->key - x;
						if (difference <0 && difference>min)
						{
							min = difference;
							smaller = current->key;
						}
						if (difference > 0 && difference < max)
						{
							max = difference;
							bigger = current->key;
						}
					}


					current = current->right;
				}

			}
			else if (current->left == NULL)
			{
				if (current->key - x != 0)
				{

					difference = current->key - x;
					if (difference <0 && difference>min)
					{
						min = difference;
						smaller = current->key;
					}
					if (difference > 0 && difference < max)
					{
						max = difference;
						bigger = current->key;
					}
				}


				current = current->right;
			}
		}
	}
	FILE* fp;
	fp = fopen("C:/Users/수연/Documents/finaloutput.txt", "a");
	if (fp == NULL)
	{
		printf("FILE OPEN ERROR!!");
		exit(0);

	}


	if (min == -9999999)
	{
		printf("small: NIL ");
		fputs(" NIL ", fp);
	}
	else
	{
		printf("small: %d  ", smaller);
		fprintf(fp, "%d   ", smaller);
	}
	if (mykey == -999)

	{

		printf("key: NIL ");
		fputs(" NIL ", fp);
	}

	else
	{
		printf("key: %d  ", x);
		fprintf(fp, "%d   ", x);
	}
	if (max == 9999999)

	{

		printf("big: NIL ");
		fputs(" NIL ", fp);
	}


	else {
		printf("big: %d  ", bigger);
		fprintf(fp, "%d   ", bigger);
	}


	printf("\n");
	printf("------------\n");
	fputs(" \n ", fp);
	fclose(fp);


}

main()
{
	int  x;
	rbtree t = createtree();
	
	FILE *fp;
	fp = fopen("C:/Users/수연/Documents/test01.txt", "r");
	while (fscanf(fp, "%d", &x) != EOF) {

		if (x > 0)
		{
			Insert(t, x);
		}
		else if (x < 0)

		{
			x = x*(-1);
			Delete(t, x);
		}

		else
		{
			break;
		}

	}

	fp = fopen("C:/Users/수연/Documents/search01.txt", "r");
	while (fscanf(fp, "%d", &x) != EOF) {
		if (x != 0)
		traversalsearch(t->root, x);
	}

	fclose(fp);
	return 0;

}