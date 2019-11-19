#include<iostream>
#include<fstream>
using namespace std;
#define LH 2 
#define EH 1
#define RH 0
struct AVLNODE
{
	int key;
	int balFactor; // thuộc tính cho biết giá trị cân bằng
	// 0: cân bằng, 1: lệch trái, 2: lệch phải
	AVLNODE* pLeft;
	AVLNODE* pRight;
};
AVLNODE* CreateNode(int Data)
{
	AVLNODE* pNode;
	pNode = new AVLNODE; //Xin cấp phát bộ nhớ động để tạo một phần tử (node) mới
		if (pNode == NULL) {
			return NULL;
		}
	pNode->key = Data;
	pNode->pLeft = NULL;
	pNode->pRight = NULL;
	pNode->balFactor = 0; //Ghi chú: giải thích ý nghĩa của thao tác này
	return pNode;
}
void LeftRotate(AVLNODE*& P)
{
	AVLNODE* Q;
	Q = P->pRight;
	P->pRight = Q->pLeft;
	Q->pLeft = P;
	P = Q;
}
void RightRotate(AVLNODE * &P)
{
	AVLNODE* Q;
	Q = P->pLeft;
	P->pLeft = Q->pRight;
	Q->pRight = P;
	P = Q;
}
void Leftbalance(AVLNODE * &P)
{
	switch (P->pLeft->balFactor) {
	case 1: //mất cân bằng trái trái
		RightRotate(P);
		P->balFactor = 0;
		P->pRight->balFactor = 0;
		break;
	case 2: //Ghi chú: cho biết đây là trường hợp mất cân bằng nào?
		LeftRotate(P->pLeft);
		RightRotate(P);
		switch (P->balFactor) {
		case 0:
			P->pLeft->balFactor = 0;
			P->pRight->balFactor = 0;
			break;
		case 1:
			P->pLeft->balFactor = 0;
			P->pRight->balFactor = 2;
			break;
		case 2:
			P->pLeft->balFactor = 1;
			P->pRight->balFactor = 0;
			break;
		}
		P->balFactor = 0;
		break;
	}
}
void Rightbalance(AVLNODE * &P)
{
	switch (P->pRight->balFactor) {
	case 1: //Ghi chú: cho biết đây là trường hợp mất cân bằng nào?
		RightRotate(P->pRight);
		LeftRotate(P);
		switch (P->balFactor) {
		case 0:
			P->pLeft->balFactor = 0;
			P->pRight->balFactor = 0;
			break;
		case 1:
			P->pLeft->balFactor = 1;
			P->pRight->balFactor = 0;
			break;
		case 2:
			P->pLeft->balFactor = 0;
			P->pRight->balFactor = 2;
			break;
		}
		P->balFactor = 0;
		break;
	case 2: //Ghi chú: cho biết đây là trường hợp mất cân bằng nào?
		LeftRotate(P);
		P->balFactor = 0;
		P->pLeft->balFactor = 0;
		break;
	}
}
int InsertNode(AVLNODE*& tree, int x)
{
	int res;
	if (tree == NULL) { //Ghi chú: cho biết ý nghĩa của câu lệnh này
		tree = CreateNode(x);
		if (tree == NULL) {
			return -1; //thêm ko thành công vì thiếu bộ nhớ
		}
		return 2;//thêm thành công và làm tăng chiều cao cây
	}
	else {
		if (tree->key == x) {
			return 0; //khóa này đã tồn tại trong cây
		}
		else if (tree->key > x) {
			res = InsertNode(tree->pLeft, x);
			if (res < 2) {
				return res;
			}
			switch (tree->balFactor) { //Ghi chú: giải thích ý nghĩa của câu lệnh switch này
			case 0:
				tree->balFactor = 1;
				return 2;
			case 1:
				Leftbalance(tree);
				return 1;
			case 2:
				tree->balFactor = 0;
				return 1;
			}
		}
		else {
			res = InsertNode(tree->pRight, x);
			if (res < 2) {
				return res;
			}
			switch (tree->balFactor) {
			case 0:
				tree->balFactor = 2;
				return 2;
			case 1:
				tree->balFactor = 0;
				return 1;
			case 2:
				Rightbalance(tree);
				return 1;
			}
		}
	}
}
void Traverse(AVLNODE* t)
{
	if (t != NULL)
	{
		Traverse(t->pLeft);
		printf("Khoa: %d, can bang: %d\n", t->key, t->balFactor);
		Traverse(t->pRight);
	}
}
void RemoveAll(AVLNODE*& t)
{
	if (t != NULL) {
		RemoveAll(t->pLeft);
		RemoveAll(t->pRight);
		delete t;
	}
}
int delNode(AVLNODE*& T, int X)

{
	int res;

	if (T == NULL) return 0;

	if (T->key > X) {

		res = delNode(T->pLeft, X);

		if (res < 2) return res;

		switch (T->balFactor) {

		case LH: T->balFactor = EH;

			return 2;

		case EH: T->balFactor = RH;

			return 1;

		case RH: Rightbalance(T);

		}

	}

	if (T->key < X) {

		res = delNode(T->pRight, X);

		if (res < 2) return res;

		switch (T->balFactor) {

		case RH: T->balFactor = EH;

			return 2;

		case EH: T->balFactor = LH;

			return 1;

		case LH: Leftbalance(T);

		}

	}
	else { //T->key == X

		AVLNODE* p = T;

		if (T->pLeft == NULL) {

			T = T->pRight; res = 2;

		}
		else if (T->pRight == NULL) {

			T = T->pLeft; res = 2;

		}
		else { //T có cả 2 con

			res = searchStandFor(p, T->pRight);

			if (res < 2) return res;

			switch (T->balFactor) {

			case RH: T->balFactor = EH;

				return 2;

			case EH: T->balFactor = LH;

				return 1;

			case LH: Leftbalance(T);

			}

		}

		delete p;

		return res;

	}

}

//Tìm phần tử thế mạng

int searchStandFor(AVLNODE*& p, AVLNODE*& q)

{
	int res;
	AVLNODE* T = p;
	if (q->pLeft) {

		res = searchStandFor(p, q->pLeft);

		if (res < 2) return res;

		switch (q->balFactor) {

		case LH: q->balFactor = EH;

			return 2;

		case EH: q->balFactor = RH;

			return 1;

		case RH: Rightbalance(T);

		}

	}
	else {

		p->key = q->key;

		p = q;

		q = q->pRight;

		return 2;

	}

}
int main()
{
	AVLNODE* tree;
	//Ghi chu: Tại sao lại phải thực hiện phép gán phía dưới?
	tree = NULL;
	int Data;
	ifstream outFile;
	outFile.open("AVL.txt");
	while (!outFile.eof()) {
		outFile >> Data;
		InsertNode(tree, Data);
	}
	outFile.close();
	/*do
	{
		printf("Nhap vao du lieu, -1 de ket thuc: ");
		scanf_s("%d", &Data);
		if (Data == -1)
			break;
		InsertNode(tree, Data);
	} while (Data != -1);*/
	printf("\nCay AVL vua tao: \n");
	Traverse(tree);
	RemoveAll(tree);
	return 0;
}