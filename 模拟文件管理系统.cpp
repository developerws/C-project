#include<iostream>
#include<sstream>
#include<iomanip>
using namespace std;
#define MAX_user 5					//����û����������ļ�Ŀ¼�����Ŀ¼��
#define DiskSpace 10*50			//���̿ռ��СΪ1MB
#define DiskBlockSize 10			//���̿�Ĵ�С1k	
char disk[DiskSpace];				//����һ������
struct diskblock {				//���̿�ṹ��
	int num;					//�̿��
	int max_length = DiskBlockSize;				//���̿�Ĵ�С
	diskblock *next;
};
diskblock *free_block_head;
struct FCB {				//�ļ����ƿ�FCB
	char file_name[10];
	int file_type;			//�ļ����� 0����ͨ�ļ���1��Ŀ¼�ļ�
	diskblock *begin_block;		//��ʼ�̿��
	int size;				//�̿���
	int pro_code;			//������ 1Ϊ��д 0Ϊ�ɶ� 2Ϊ�ɶ�д
	int openflag;		//�򿪱�־ 0Ϊ�رգ�1Ϊ��
};
struct UFD {				//�û��ļ�Ŀ¼
	FCB fcb;					//Ŀ¼��
	UFD* next;
};
struct MFD {				//���ļ�Ŀ¼
	char user[5];				//�û���
	UFD *ufd;			//���û��µ��û��ļ�Ŀ¼�����ļ���ʽ����
};
MFD m[MAX_user];				//Ŀ¼�ļ��Ľṹ��5��Ŀ¼����ɣ�
int login() {											//��¼
	cout << "�����û�����";
	string s;
	char ch;
	int i;
	while ((ch = getchar()) != '\n') {
		s += ch;
	}
	for (i = 0; i < MAX_user; i++) {
		if (s == m[i].user) {
			cout << "��¼�ɹ���" << endl;
			return i;
		}
	}
	if (i == MAX_user) {
		cout << "��¼ʧ�ܣ�" << endl;
		return -1;
	}
}					
void list_dir(int user_id) {
	cout << left << setw(10) << "�ļ���" << left << setw(10) <<"���̿��" << left << setw(10) << "�ļ���С" << left << setw(10) <<"�ļ�ģʽ" << left << setw(10) << "�ļ�״̬" << endl;
	UFD *p;
	p = m[user_id].ufd->next;
	while (p!= NULL) {
		cout << left << setw(10) << p->fcb.file_name << left << setw(10) << p->fcb.begin_block->num << left << setw(10) << p->fcb.size;
		switch (p->fcb.pro_code) {
		case 0:
			cout << left << setw(10) << "�ɶ�";
			break;
		case 1:
			cout << left << setw(10) << "��д";
			break;
		case 2:
			cout << left << setw(10) << "�ɶ���д";
			break;
		}
		switch (p->fcb.openflag) {
		case 0:
			cout << left << setw(10) << "�ر�";
			break;
		case 1:
			cout << left << setw(10) << "��";
			break;
		}
		cout << endl;
		p = p->next;
	}

}
void create_file(int user_id,char *s) {
	int choose;
	cout << "0:read  1:write  2:read&write ? ";
	cin >> choose;
	cin.ignore();				//���Ի��з����������getchar()����
	UFD *q, *p;
	p = new UFD;						//����һ���µ�Ŀ¼��
	p->next = NULL;
	strcpy(p->fcb.file_name, s);
	diskblock *n;					//�ӿ��п���ȡ��һ�����п�
	n = free_block_head->next;
	free_block_head->next = n->next;
	n->next = NULL;
	p->fcb.begin_block = n;
	p->fcb.size = 0;
	p->fcb.pro_code = choose;								//����Ϊ�ɶ��ļ���Ȼ����ʲô����
	p->fcb.openflag = 0;
	q = m[user_id].ufd;
	while (q->next != NULL) {
		q = q->next;
	}
	q->next = p;
}
void delete_file(int user_id, char *s) {
	diskblock *d;
	UFD *p,*q;
	int i;
	q = m[user_id].ufd;
	p = q->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name,s)) {
			d = p->fcb.begin_block;							//ָ���ļ����ݵ�һ�����̿�
			while (d != NULL) {								//�ж��ļ��Ƿ��д��̿�
				d = d->next;
				i = 0;
				while (i < p->fcb.size && i < DiskBlockSize) {								//���մ�������,ע��߽�����
					disk[p->fcb.begin_block->num * DiskBlockSize + i] = NULL;
					i++;
				}
				p->fcb.size -= i;
				p->fcb.begin_block->num = NULL;
				p->fcb.begin_block->next = free_block_head->next;				//�����ͷŵ��̿����������ͷ��
				free_block_head->next = p->fcb.begin_block;
				p->fcb.begin_block = d;
			}
			q->next = p->next;
			delete p;
			break;
		}
		p = p->next;
		q = q->next;
	}
	if (p == NULL) {
		cout << "�ļ������ڣ�" << endl;
	}
}
void open_file(int user_id, char *s) {
	UFD *p;
	p = m[user_id].ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			p->fcb.openflag = 1;
			cout << "�ɹ����ļ���" << endl;
			break;
		}
		p = p->next;
	}
	if (p == NULL) {
		cout << "�ļ������ڣ�" << endl;
	}
}
void close_file(int user_id, char *s) {
	UFD *p;
	p = m[user_id].ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			p->fcb.openflag = 0;
			cout << "�ɹ��ر��ļ���" << endl;
			break;
		}
		p = p->next;
	}
	if (p == NULL) {
		cout << "�ļ������ڣ�" << endl;
	}
}
void read_file(int user_id, char *s) {
	UFD *p;
	diskblock *d;
	
	int i = 0;
	p = m[user_id].ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			break;
		}
		p = p->next;
	}
	if (p) {
		if (p->fcb.openflag) {
			if (p->fcb.pro_code == 0 || p->fcb.pro_code == 2)
			{
				d = p->fcb.begin_block;
				while (i < p->fcb.size) {
					cout << disk[d->num * DiskBlockSize + i % DiskBlockSize];
					i++;
					if (i == DiskBlockSize) {						//��Ҫת����һ����̿�
						d = d->next;
					}
				}
				cout << endl;
			}
			else {
				cout << "�ļ����ɶ���" << endl;
			}
		}
		else {
			cout << "�ļ�δ�򿪣�" << endl;
		}

	}
	else {
		cout << "�ļ������ڣ�" << endl;
	}
}
void write_file(int user_id, char *s) {
	UFD *p;
	diskblock *d,*d_new;
	int block_sum = 1;						//������Ĵ��̿����ܺ�
	p = m[user_id].ufd->next;
	while (p != NULL) {
		if (!strcmp(p->fcb.file_name, s)) {
			break;
		}
		p = p->next;
	}
	if (p) {
		if (p->fcb.openflag) {
			if (p->fcb.pro_code == 1 || p->fcb.pro_code == 2) {
				char ch;
				d = p->fcb.begin_block;
				while ((ch = getchar()) != '\n') {
					disk[d->num *DiskBlockSize + p->fcb.size%DiskBlockSize] = ch;
					p->fcb.size++;						//�Ӹ��жϣ����size����1024��ʹ�ö���Ĵ��̿�
					if (p->fcb.size / DiskBlockSize == block_sum) {
						d_new = free_block_head->next;				//�ӿ��п�����ȡ���¿�
						free_block_head->next = d_new->next;
						d_new->next = NULL;
						d->next = d_new;
						d = d_new;
						block_sum++;

					}
				}
				disk[d->num *DiskBlockSize + p->fcb.size%DiskBlockSize] = '\0';
			}
			else {
				cout << "�ļ�����д��" << endl;
			}
		}
		else {
			cout << "�ļ�δ�򿪣�" << endl;
		}
	}
	else {
		cout << "�ļ������ڣ�" << endl;
	}
}
/*��ʼ�����̿�,Ϊϵͳ��ȡ���ݵĻ�����λ*/
void init_disk(diskblock *free_block_head) {
	diskblock *q, *p;
	q = free_block_head;
	for (int i = 0; i < (DiskSpace / DiskBlockSize); i++) {
		p = new diskblock;
		p->num = i;
		p->next = NULL;
		q->next = p;
		q = p;
	}
}

int main() {
	free_block_head = new diskblock;
	free_block_head->next = NULL;
	init_disk(free_block_head);						//��ʼ������
	for (int i = 0; i < MAX_user; i++) {
		sprintf(m[i].user, "user%d", i + 1);
		m[i].ufd = new UFD;						//�û��ļ�Ŀ¼��ͷָ��
		m[i].ufd->next = NULL;
	}
	int user_ID;
	user_ID = login();							//��ȡ��ǰ��¼���û�
	if(user_ID != -1) {
		cout << "*		�����ʽ		˵��				*" << endl;
		cout << "*		dir				��ʾ�ļ�Ŀ¼		*" << endl;
		cout << "*		create			�����ļ�			*" << endl;
		cout << "*		delete			ɾ���ļ�			*" << endl;
		cout << "*		open			���ļ�			*" << endl;
		cout << "*		close			�ر��ļ�			*" << endl;
		cout << "*		read			���ļ�				*" << endl;
		cout << "*		write			д�ļ�				*" << endl;
		cout << "*		exit			�˳�ϵͳ			*" << endl;
		cout << "************************************************" << endl;
		string command[8];
		command[0] = "dir";
		command[1] = "create";
		command[2] = "delete";
		command[3] = "open";
		command[4] = "close";
		command[5] = "read";
		command[6] = "write";
		command[7] = "exit";
		bool flag = false;							//�ж��˳�ϵͳ
		while (1) {
			cout << "Root:>";
			char ch;
			string c = "";
			while ((ch = getchar()) != '\n') {
				c += ch;
			}
			string str1, str2;
			istringstream is(c);
			is >> str1 >> str2;
			int count;
			for (count = 0; count < 8; count++) {
				if (command[count] == str1) {
					break;
				}
			}
			if (count != 8) {
				switch (count) {
				case 0:
					list_dir(user_ID);
					break;
				case 1:
					create_file(user_ID, (char *)str2.c_str());
					break;
				case 2:
					delete_file(user_ID, (char *)str2.c_str());
					break;
				case 3:
					open_file(user_ID, (char *)str2.c_str());
					break;
				case 4:
					close_file(user_ID, (char *)str2.c_str());
					break;
				case 5:
					read_file(user_ID, (char *)str2.c_str());
					break;
				case 6:
					write_file(user_ID, (char *)str2.c_str());
					break;
				case 7:
					flag = true;
					cout << "�˳�ϵͳ��" << endl;
					break;
				}
			}
			else {
				cout << "������������" << endl;
			}
			if (flag)
				break;
		}
	}
	system("pause");
}