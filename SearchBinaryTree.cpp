#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;

//搜索二叉树的节点结构
template<class K,class V>
struct SearchBinaryNode
{
	SearchBinaryNode* _left;//左子树
	SearchBinaryNode* _right;//右子树
	K _key;//每个节点是(_key,_value)形式
	V _value;

	//搜索二叉树的节点的构造函数
	SearchBinaryNode(const K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		, _key(key)
		, _value(value)
	{}
};


template<class K,class V>
class SearchBinaryTree
{
	typedef SearchBinaryNode<K, V> Node;
public:
	//搜索二叉树的构造函数
	SearchBinaryTree()
		:_root(NULL)
	{}

	//给搜索二叉树插入一个节点
	bool Insert(const K& key, const V& value)
	{
		//搜索二叉树无节点，直接挂上该节点
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}

		//遍历搜索二叉树，要插入的节点比每个根节点小，往左子树走，否则大往右子树走
		Node* cur = _root;
		Node* prev = NULL;//保存节点，便于插入节点链在上面
		while (cur)
		{
			if (key < cur->_key)
			{
				prev = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key)
			{
				prev = cur;
				cur = cur->_right;
			}
			else
			{
				return false;//搜索二叉树要求无重复关键码
			}
		}
		
		//此时不知道链在左子树还是右子树，分情况
		if (prev->_key > key)
		{
			prev->_left = new Node(key, value);
		}
		else if (prev->_key < key)
		{
			prev->_right = new Node(key, value);
		}
		return true;
	}


	//寻找搜索二叉树的一个节点
	bool Find(const K& key,const V& value)
	{
		if (_root == NULL)
		{
			return false;
		}
		Node* cur = _root;
		while (cur)
		{
			if (key < cur->_key)
			{
				cur = cur->_left;
			}
			else if (key > cur->_key)
			{
				cur = cur->_right;
			}
			else
			{
				return true;
			}
		}
		return true;
	}


	//删除搜索二叉树的一个节点
	bool Remove(const K& key,const V& value)
	{
		//无结点：返回
		if (_root == NULL)
		{
			return false;
		}

		//一个节点:直接删
		else if (_root->_left == NULL && _root->_right == NULL)
		{
			if (_root->_key == key)
			{
				delete _root;
				_root = NULL;
				return true;
			}
			else
				return false;
		}

		//多个节点
		else
		{
			Node* cur = _root;
			Node* parent = NULL;
			while (cur)
			{
				//往左子树上走去找
				if (cur->_key > key)
				{
					parent = cur;
					cur = cur->_left;
				}

				//往右子树上走去找
				else if (cur->_key < key)
				{
					parent = cur;
					cur = cur->_right;
				}

				//找到该节点，确定如何去删除该节点
				else
				{
					//要删除的节点无左子树
					if (cur->_left == NULL)
					{
						//问题：这时候要处理一个特殊情况，如果删到只剩下根节点parent:5，以及它的右子树parent->_right:9时，parent为空，
						//访问parent的成员会导致程序崩溃
						//解决：parent为空时，则将parent->right置成根_root,删掉parent节点
						if (parent == NULL)
						{
							_root = cur->_right;
							delete cur;
							cur = NULL;
							return true;
						}
						
						//两种情况：
						//（1）删除的cur在parent的右子树上时，cur->_right链在parent->_right上					
						if (parent->_key<cur->_key)
						{
							parent->_right = cur->_right;
						}

						//（2）否则，cur->_right链在parent->_left
						else if (parent->_key>cur->_key)
						{
							parent->_left = cur->_right;
						}
						delete cur;
						cur = NULL;
						return true;
					}

					//要删除的节点无右子树
					else if (cur->_right == NULL)
					{
						//同cur->_left为空
						if (parent == NULL)
						{
							_root = cur->_left;
							delete cur;
							cur = NULL;
							return true;
						}

						//两种情况：
						//（1）删除的cur在parent的右子树上时，cur->_left链在parent->_right上
						if (parent->_key < cur->_key)
						{
							parent->_right = cur->_left;							
						}

						//（2）否则，cur->_left链在parent->_left
						else if (parent->_key > cur->_key)
						{
							parent->_left = cur->_left;
						}

						//删除释放
						delete cur;
						cur = NULL;
						return true;
					}

					//左右子树都不为空,分两种情况
					else
					{
						//firstLeft是要删除的节点cur的右子树（代码走到这一步，说明左右必然不为空，则肯定不为空）
						Node* firstLeft = cur->_right;

						//往firstLeft的左子树上走，找最左节点
						//（1）左为空，交换，把链在firstLeft的一串节点（仅有右子树）链在cur->_right上
						if (firstLeft->_left == NULL)
						{						
							swap(cur->_key, firstLeft->_key);
							swap(cur->_value, firstLeft->_value);
							cur->_right = firstLeft->_right;
							delete firstLeft;
							firstLeft = NULL;
							return true;
						}

						//左为空，一直往其左子树上走，找到，交换
						else
						{
							Node* tmp = firstLeft;

							//tmp为最左节点
							while (tmp->_left)
							{
								firstLeft = tmp;
								tmp = tmp->_left;
							}
							swap(cur->_key, tmp->_key);
							swap(cur->_value, tmp->_value);

							//把链在tmp的一串节点（仅有右子树）链在firstLeft->_right上
							firstLeft->_left = tmp->_right;
							delete tmp;
							tmp = NULL;
							return true;
						}
					}
				}
			}
		}
	}


	//调用_Insert_R
	void Insert_R(const K& key, const V& value)
	{
		_Insert_R(_root, key, value);
	}


	//调用_Find_R
	Node* Find_R(const K& key, const V& value)
	{
		Node* ret = _Find_R(_root, key, value);
		return ret;
	}
	

	//调用_Remove_R
	void Remove_R(const K& key, const V& value)
	{
		_Remove_R(_root,key, value);
	}
	//调用_InOrder
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

protected:

	//中序遍历打印：便于查看是否构造搜索二叉树是否正确
	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << "  ";
		_InOrder(root->_right);		
	}


	//给搜索二叉树插入一个节点的递归写法
	/*注意：递归写法传引用，创建的节点要放在_root上，而不传引用的话，相当于在_root上拷贝
	的临时形参变量root上建了个节点，对_root本身没有作用*/
	void _Insert_R(Node*& root, const K& key, const V& value)
	{
		if (root == NULL)
		{
			root = new Node(key, value);
			return;
		}
		if (root->_key < key)//往右子树上递归
		{
			_Insert_R(root->_right, key, value);
		}
		else if (root->_key > key)//往左子树上递归
		{
			_Insert_R(root->_left, key, value);
		}
		else//搜索二叉树要求无重复关键码
			return;
	}


	//寻找搜索二叉树的一个节点的递归写法
	Node* _Find_R(Node* root, const K& key, const V& value)
	{
		if (root == NULL)
		{
			return NULL;
		}
		if (root->_key < key)//往右子树上递归
		{
			return _Find_R(root->_right, key, value);
		}
		else if (root->_key > key)//往左子树上递归
		{
			return _Find_R(root->_left, key, value);
		}
		else 
			return root;//找到，返回节点指针		
	}


	//删除搜索二叉树的一个节点的递归写法
	void _Remove_R(Node*& root, const K& key, const V& value)
	{
		//空节点，返回
		if (root == NULL)
			return;	

		//一个节点：若为该节点，删除。否则不处理
		if (root->_left == NULL && root->_right == NULL)
		{
			if (root->_key == key)
			{
				delete root;
				root = NULL;
				return;
			}
			else
				return;		
		}

		//往左子树上递归寻找
		if (root->_key > key)
		{
			_Remove_R(root->_left, key, value);
		}

		//往右子树上递归寻找
		else if (root->_key < key)
		{
			_Remove_R(root->_right, key, value);
		}

		//找到该节点，且该节点的左右子树不空，确定如何去删除该节点
		else
		{
			Node* del = NULL;

			//右子树为空，此时不需和非递归写法一样分两种情况（parent->_right是否）分别去处理，这就是递归的优势所在。
			//parent->_right为空，上一层递归来的是root->_left,即此时的root,链在root->_left上
			//parent->_right不为空时，上一层递归来的root->_right，即此时的root，链在root->_left上
			//所以，当parent->_right为空或者不为空时，均是上一层递归而来，root->_left或者root->_right,都可以用此时的root表示
			if (root->_right == NULL)
			{
				del = root;
				root = root->_left;
				delete del;
				del = NULL;
			}

			//同上，parent->_left为空时，上层递归来的root->_right，即此时的root，链在root->_right上
			//parent->_right不为空时，上层递归来的root->_left,是此时的root,链在root->_right上
			//而这两种情况，都可以用递归解决，无需分别处理
			else if (root->_left == NULL)
			{
				del = root;
				root = root->_right;
				delete del;
				del = NULL;
			}

			//左右子树都不为空，若不递归的话，要分两种情况处理。要删除的root节点要先找到右子树节点firstLeft，
			//再往firstLeft的左子树走，去找最左节点，此时可能左子树为空或者不为空。借用递归的优势，不管是上层的左还是右子树传过来的，
			//都是上一层递归来的，即都是此时的root.一起处理掉。
			//所以，这两种情况合在一起，可以
			else
			{
				Node* firstLeft = root->_right;	
				Node* tmp = firstLeft;

				//找最左节点
				while (tmp->_left)
				{
					firstLeft = tmp;
					tmp = tmp->_left;
				}

				//找到最左边节点，保存，交换，删除
				swap(tmp->_key, root->_key);
				swap(tmp->_value, root->_value);
				del = root;
				root = tmp->_right;
				delete del;
				del = NULL;			
			}
		}
	}

protected :
	Node* _root;
};


void TestSearchBinaryTree()
{
	typedef SearchBinaryNode<int, int> Node;
	SearchBinaryTree<int, int> sbt;
	sbt.Insert(5, 1);
	sbt.Insert(3, 1);
	sbt.Insert(2, 1);
	sbt.Insert(6, 1);
	sbt.Insert(0, 1);
	sbt.Insert(9, 1);
	sbt.Insert(7, 1);
	sbt.Insert(8, 1);
	sbt.Insert(4, 1);
	sbt.Insert(1, 1);
	sbt.InOrder();

	sbt.Find(3,1);

	//删除的完整测试用例：将上序插入的序列删除顺序打乱，并全部删除，所有删除情况基本都可涵盖。
	sbt.Remove(1, 1);
	sbt.Remove(3, 1);
	sbt.Remove(4, 1);	
	sbt.Remove(7, 1);
	sbt.Remove(6, 1);
	sbt.Remove(0, 1);
	sbt.Remove(8, 1);
	sbt.Remove(2, 1);
	sbt.Remove(5, 1);
	sbt.Remove(9, 1);
	sbt.InOrder();

	sbt.Insert_R(5, 1);
	sbt.Insert_R(3, 1);
	sbt.Insert_R(2, 1);
	sbt.Insert_R(6, 1);
	sbt.Insert_R(0, 1);
	sbt.Insert_R(9, 1);
	sbt.Insert_R(7, 1);
	sbt.Insert_R(8, 1);
	sbt.Insert_R(4, 1);
	sbt.Insert_R(1, 1);
	sbt.InOrder();
	
	Node* ret = sbt.Find_R(3, 1);
	if (ret == NULL)
	{
		cout << "not exist!" << endl;
	}
	else
		cout << ret->_key << endl;

	sbt.Remove_R(1, 1);
	sbt.Remove_R(3, 1);
	sbt.Remove_R(4, 1);
	sbt.Remove_R(7, 1);
	sbt.Remove_R(6, 1);
	sbt.Remove_R(0, 1);
	sbt.Remove_R(8, 1);
	sbt.Remove_R(2, 1);
	sbt.Remove_R(5, 1);
	sbt.Remove_R(9, 1);
	sbt.InOrder();
}

int main()
{
	TestSearchBinaryTree();
	system("pause");
	return 0;
}
