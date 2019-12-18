#include<iostream>
using namespace std;

typedef struct
{
	int weight;
	int parent, lchild, rchild;
}HTNode,*HuffmanTree;
int s1, s2;
void Select(HuffmanTree HT, int i, int &s1, int &s2)
{
	s1 = 0, s2 = 0;
	int flag = 0;
	for (int j = 1; j <= i; j++)
		if (HT[j].parent == 0)
		{
			if (flag == 0)
			{
				s1 = j;
				flag += 1;
			}
			else if (flag == 1)
			{
				if (HT[s1].weight > HT[j].weight)
				{
					s2 =s1;
					s1 = j;
				}
				else{ s2 = j; }
				flag += 1;
			}
			else
			{
				if (HT[j].weight < HT[s2].weight&&HT[j].weight > HT[s1].weight)
				{
					s2 = j;
				}
				else if (HT[j].weight <= HT[s1].weight)
				{
					s2 = s1;
					s1 = j;
				}
				
			}
			
		}
}
void CreateHuffmanCode(HuffmanTree &HT, int n)
{
	if (n <= 1) return;
	int m = 2 * n - 1,i;
	HT = new HTNode[m+1];
	for (i = 1; i <= m; i++)
	{
		HT[i].parent = 0;
		HT[i].lchild = 0;
		HT[i].rchild = 0;
	}
	cout << "���������Ȩֵ��" << endl;
	for (i = 1; i <= n; i++)
		cin >> HT[i].weight;
	for (i = n + 1; i < m + 1;i++)
	{
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
}

typedef char **HuffmanCode;
void CreatHuffmanCode(HuffmanTree HT, HuffmanCode HC,int n)
{
	HC = new char*[n+1]; //����洢n���ַ�����ı����ռ�
	char *cd = new char[n]; //������ʱ���ÿ���ַ�����Ķ�̬����ռ�
	cd[n - 1] = '\0';//���������
	int i,start,c,f;
	cout << "������������ֱ�Ϊ��" << endl;
	for (i = 1; i <= n; i++)
	{
		start = n - 1;//��ʼָ�����
		c = i; 
		f = HT[i].parent; //fָ����c��˫�׽��
		while (f!=0) //��Ҷ�ӽ�㿪ʼ���ϻ��ݣ�ֱ�������
		{
			start--;
			if (HT[f].lchild == c) cd[start] = '1';
			else cd[start] = '0';
			c = f;
			f = HT[f].parent;
		}
		for (int p = start; p <n-1; p++)
			cout << cd[p];
		cout << endl;
	}
	delete cd;
}
int main()
{
	cout << "�༶" <<" "<< "ѧ��" << " " << "����" << " " << endl;
	HuffmanTree HT;// ����һ�ù������� 
	HuffmanCode HC;// ��������� 
	HC = NULL;
	int n;//������
	cout << "��������ĸ�����" ;
	cin >> n;
	CreateHuffmanCode(HT,n);
	cout << "����Ĺ��������ṹΪ��" << endl;
	for (int i = 1; i <= 2*n-1; i++)
		cout << HT[i].weight <<" "<< HT[i].parent << " " << HT[i].lchild << " " << HT[i].rchild << " " << endl;
	CreatHuffmanCode(HT, HC, n);
	system("pause");
}