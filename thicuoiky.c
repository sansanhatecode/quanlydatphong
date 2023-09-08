#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void printMenu(){
    printf("Chuong trinh quan ly dat phong\n");
    printf("[1] Doc du lieu tu file log\n[2] Kiem tra so luong yeu cau khong hop le\n[3] Tra cuu theo ma yeu cau\n[4] Thay doi yeu cau\n[5] Huy yeu cau\n[6] Luu danh sach yeu cau ra file\n[7] Thong ke khach san duoc ua thich nhat\n[8] Ngay khong hop le\n[9] Ngay khong hop le\n");
    printf("Moi chon chuc nang: ");
}

int n;

typedef struct{
    char name[20];//ma yeu cau
    int a;//tong so nguoi
    int b;//ma phong
    char s[20];//ma khach san
    int ngay;
    int thang;
    int nam;
    int d;//so tre em
    struct Profile *next;
}Profile;
Profile* first, *last;

Profile* makeProfile(char* name, int a,int b,char *s,int ngay,int thang,int nam,int d){
    Profile* p = (Profile*)malloc(sizeof(Profile));
    strcpy(p->name,name); 
    p->a = a;
    p->b = b;
    strcpy(p->s,s);
    p->ngay = ngay;
    p->thang = thang;
    p->nam = nam;
    p->d = d;
    p->next = NULL;
    return p;
}

void initList(){
    first  =  NULL;
    last  = NULL;
}

int ListEmpty(){
    return  last == NULL && first == NULL;
}

void insertLast(char* name, int a,int b,char *s,int ngay,int thang,int nam,int d){
    Profile* profile = makeProfile(name, a, b, s, ngay, thang, nam, d);
    if (ListEmpty()){
        last = profile;
        first = profile;
    }else{
        last->next = profile;
        last = profile;
    }
}

typedef struct Node{
    char name[20];//ma yeu cau
    int a;//tong so nguoi
    int b;//ma phong
    char s[20];//ma khach san
    int ngay;
    int thang;
    int nam;
    int d;//so tre em
    struct Node *leftChild;
    struct Node *rightChild;
}Node;
Node *root;

Node* makeNode(char* name, int a,int b,char *s,int ngay,int thang,int nam,int d){
    Node* p = (Node*)malloc(sizeof(Node));
    strcpy(p->name,name); 
    p->a = a;
    p->b = b;
    strcpy(p->s,s);
    p->ngay = ngay;
    p->thang = thang;
    p->nam = nam;
    p->d = d;
    p->leftChild = NULL; 
    p->rightChild = NULL;
    return p;
}

Node* insert(Node* r, char* name, int a,int b,char *s,int ngay,int thang,int nam,int d){
    if(r == NULL) return makeNode(name,a,b,s,ngay,thang,nam,d);
    int c = strcmp(r->name,name);
    if(c == 0){
    printf("Student %s exists, do not insert\n",name); return r;
    }else if(c < 0){
            r->rightChild = insert(r->rightChild,name,a,b,s,ngay,thang,nam,d); return r;
          }else{
        r->leftChild = insert(r->leftChild,name,a,b,s,ngay,thang,nam,d); return r;
    }
}
Node* find(Node* r, char* name){
    if(r == NULL) return NULL;
    int c = strcmp(r->name,name);
    if(c == 0) return r;
    if(c < 0) return find(r->rightChild,name);
    return find(r->leftChild,name);
}

Node* findMin(Node* r){
    if(r == NULL) return NULL;
    Node* lmin = findMin(r->leftChild);
    if(lmin != NULL) return lmin;
    return r;
}
Node* removeStudent(Node* r, char* name){
    if(r == NULL) return NULL;
    int c = strcmp(r->name,name);
    if(c > 0) r->leftChild = removeStudent(r->leftChild,name);
    else if(c < 0) r->rightChild = removeStudent(r->rightChild,name);
    else{
        if(r->leftChild != NULL && r->rightChild != NULL){
            Node* tmp = findMin(r->rightChild);
            strcpy(r->name,tmp->name); 
            r->a = tmp->a;
            r->b = tmp->b;
            strcpy(r->s,tmp->s);
            r->ngay = tmp->ngay;
            r->thang = tmp->thang;
            r->nam = tmp->nam;
            r->d = tmp->d;
            r->rightChild = removeStudent(r->rightChild,tmp->name);
        }else{
            Node* tmp = r;
            if(r->leftChild == NULL) r = r->rightChild; 
            else r = r->leftChild;
            free(tmp);
        }
    }
    return r;
}

void freeTree(Node* r){
    if(r == NULL) return;
    freeTree(r->leftChild);
    freeTree(r->rightChild);
    free(r);
}
void load(){
    FILE *f=fopen("booking_orders.txt","r");
    fscanf(f,"%d",&n);
    while (!feof(f))
    {   
        char name[20];
        int a;
        int b;
        char s[20];
        int ngay;
        int thang;
        int nam;
        int d;
        fscanf(f,"%s%d%d%s%d-%d-%d%d",name,&a,&b,s,&ngay,&thang,&nam,&d);
        insertLast(name,a,b,s,ngay,thang,nam,d);
        root = insert(root,name,a,b,s,ngay,thang,nam,d);
    }
    fclose(f);
}

void inOrder(Node* r){
    if(r == NULL) return;
    inOrder(r->leftChild);
    printf("%s %d %d %s %02d-%02d-%d %d\n",r->name,r->a,r->b,r->s,r->ngay,r->thang,r->nam,r->d);
    inOrder(r->rightChild);
}

void cn2(Profile *p){
    int count=0;
    for (Profile *p=first; p!=NULL; p=p->next){
        if(p->a <= p->d) count++;
    }
    printf("So luong cac yeu cau khong hop le la: %d\n", count);
}

void inOrderF(Node* r, FILE* f){
    if(r == NULL) return;
    inOrderF(r->leftChild,f);
    fprintf(f,"%s %d %d %s %02d-%02d-%d %d\n",r->name,r->a,r->b,r->s,r->ngay,r->thang,r->nam,r->d);
    inOrderF(r->rightChild,f);
}

void processStore(){
    FILE* f = fopen("booking_output.txt","w");
    inOrderF(root,f);
    fclose(f);
}

void cn8(Profile *r){
    int ngayht, thanght, namht;
    printf("Nhap ngay thang nam hien tai: ");
    scanf("%d%d %d", &ngayht, &thanght, &namht);
    printf("Cac yeu cau khong hop le ve thoi gian la: \n");
    for (Profile *r=first; r!=NULL; r=r->next){
        if(r->nam < namht){
            printf("%s %d %d %s %02d-%02d-%d %d\n",r->name,r->a,r->b,r->s,r->ngay,r->thang,r->nam,r->d);
        }
        else if(r->nam == namht && r->thang<thanght){
            printf("%s %d %d %s %02d-%02d-%d %d\n",r->name,r->a,r->b,r->s,r->ngay,r->thang,r->nam,r->d);
        }
        else if(r->nam == namht && r->thang==thanght && r->ngay<ngayht){
            printf("%s %d %d %s %02d-%02d-%d %d\n",r->name,r->a,r->b,r->s,r->ngay,r->thang,r->nam,r->d);
        }
    }
}

int main () { 
    int choice;
    int exit = 0;
    while(exit==0){
        printMenu();
        scanf("%d", &choice);
        switch (choice){
            case 1:
                load();
                printf("So luong yeu cau doc duoc la: %d\n", n);
                inOrder(root);
                break;
            case 2:
                cn2(first);
                break;
            case 3:
                char maTim[20];
                printf("Nhap ma yeu cau: ");
                scanf("%s", maTim);
                Node *timKiem;
                timKiem = find(root, maTim);
                if(timKiem == NULL) printf("Not found\n");
                else printf("%s %d %d %s %02d-%02d-%d %d\n",timKiem->name,timKiem->a,timKiem->b,timKiem->s,timKiem->ngay,timKiem->thang,timKiem->nam,timKiem->d);
                break;
            case 4:
                int newa;//so nguoi
                int newngay;
                int newthang;
                int newnam;
                int newd;//so tre em
                char maTim4[20];
                printf("Nhap ma yeu cau: ");
                scanf("%s", maTim4);
                Node *timKiem4;
                timKiem4 = find(root, maTim4);
                if(timKiem4 == NULL) printf("Not found\n");
                printf("Nhap thong tin cap nhat bao gom ngay thang nam: ");
                scanf("%d-%d-%d",&newngay,&newthang,&newnam);
                printf("Nhap thong tin so nguoi can cap nhat: ");
                scanf("%d", &newa);
                printf("Nhap thong tin so tre em can cap nhat: ");
                scanf("%d", &newd);
                if(newd<newa){
                    timKiem4->ngay=newngay;
                    timKiem4->thang=newthang;
                    timKiem4->nam=newnam;
                    timKiem4->a=newa;
                    timKiem4->d=newd;
                    printf("Update\n");
                }
                else printf("Invalid\n");
                break;
            case 5:
                char maTim5[20];
                Node* xoa;
                printf("Nhap ma yeu cau: ");
                scanf("%s", maTim5);
                Node *timKiem5;
                timKiem5 = find(root, maTim5);
                if(timKiem5 == NULL) printf("Not found\n");
                else {
                    xoa = removeStudent(root, maTim5);
                    printf("Delete\n");
                }
                break;
            case 6:
                processStore();
                break;
            case 7:
                break;
            case 8:
                cn8(first);
                break;
            case 9:
                break;
            default:
                exit = 1;
                break;
        }
    }
    freeTree(root);
    return 0;
}