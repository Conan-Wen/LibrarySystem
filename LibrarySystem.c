#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1                         // boolean value true
#define FALSE 0                        // boolean value false
#define clear() printf("\033[H\033[J") // clear the screen
#define FILENAME "./bookInfo.dat"

struct Node *list;

// Book information struct
struct bookInfo
{
    int id;
    char title[64];
    char author[64];
    int enable; // can be borrowed? or not?
};

// Linked list node struct
struct Node
{
    struct bookInfo data;
    struct Node *next;
};

// method: create the head of node
struct Node *createHead()
{
    // dynamic application of memory space
    struct Node *headNode = (struct Node *)malloc(sizeof(struct Node));
    // initialization
    headNode->next = NULL;
    return headNode;
};

// method: create the node to save the data
struct Node *createNode(struct bookInfo data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// insert a new node after the first element
void insertNodeByHead(struct Node *headNode, struct bookInfo data)
{
    struct Node *newNode = createNode(data);
    newNode->next = headNode->next;
    headNode->next = newNode;
}

// insert a new node after the last element
void insertNodeByTail(struct Node *headNode, struct bookInfo data)
{
    struct Node *pMove = headNode;
    while (pMove->next)
    {
        pMove = pMove->next;
    }
    struct Node *newNode = createNode(data);
    pMove->next = newNode;
}

// delete the node
int deletaNodeById(struct Node *headNode, int id)
{
    struct Node *desLeftNode = headNode;
    struct Node *desNode = headNode->next;
    while (desNode != NULL && (desNode->data).id != id)
    {
        desLeftNode = desNode;
        desNode = desLeftNode->next;
    }
    if (desNode == NULL)
    {
        return FALSE;
    }
    else
    {
        desLeftNode->next = desNode->next;
        free(desNode);
        desNode = NULL;
        return TRUE;
    }
}

// print the Linked list
void printList(struct Node *headNode)
{
    struct Node *pMove = headNode->next;
    puts("id\t図書\t著者\t貸出可能\n");
    while (pMove)
    {
        printf("%d\t%s\t%s\t%d\t\n", pMove->data.id, pMove->data.title, pMove->data.author, pMove->data.enable);
        pMove = pMove->next;
    }
}

// save the information of books to file
void saveInfoToFile(const char *fileName, struct Node *headNode)
{
    FILE *fp = fopen(fileName, "w");
    struct Node *pMove = headNode->next;
    while (pMove)
    {
        fprintf(fp, "%d\t%s\t%s\t%d\t\n", pMove->data.id, pMove->data.title, pMove->data.author, pMove->data.enable);
        pMove = pMove->next;
    }
    fclose(fp);
}

// read the information of books from file
void readInfoFromFile(const char *fileName, struct Node *headNode)
{
    FILE *fp = fopen(fileName, "r");
    // if the file does not exist, creat one
    if (fp == NULL)
    {
        fp = fopen(fileName, "w+");
    }
    struct bookInfo temp;
    while (fscanf(fp, "%d\t%s\t%s\t%d\t\n", &temp.id, temp.title, temp.author, &temp.enable) != EOF)
    {
        insertNodeByTail(headNode, temp);
    }

    fclose(fp);
}

// find the book by name
struct Node *findByTitle(struct Node *headNode, char *title)
{
    struct Node *desNode = headNode->next;
    while (desNode != NULL && strcmp(desNode->data.title, title))
    {
        desNode = desNode->next;
    }
    return desNode;
}

// find the book by id
struct Node *findById(struct Node *headNode, int id)
{
    struct Node *desNode = headNode->next;
    while (desNode != NULL && desNode->data.id != id)
    {
        desNode = desNode->next;
    }
    return desNode;
}

// 書籍登録メソッド
void bookRegister(struct Node *headNode)
{
    struct bookInfo temp;
    printf("書籍ID(数字): ");
    scanf("%d", &temp.id);
    printf("書名: ");
    scanf("%s", temp.title);
    printf("著者: ");
    scanf("%s", temp.author);
    temp.enable = 1;
    insertNodeByTail(headNode, temp);
    saveInfoToFile(FILENAME, headNode);
    puts("登録完了！");
}

//書籍削除メソッド
void bookDelete(struct Node *headNode)
{
    int id;
    printf("削除の書籍ID: ");
    scanf("%d", &id);
    deletaNodeById(headNode, id);
    saveInfoToFile(FILENAME, headNode);
    puts("削除完了！");
}

//検索メソッド
void findBook(struct Node *headNode)
{
    struct Node *result;
    struct bookInfo temp;
    printf("検索の書名: ");
    scanf("%s", temp.title);
    result = findByTitle(headNode, temp.title);
    if (!result)
    {
        puts("該当の書籍は見つかりませんでした。");
    }
    else
    {
        puts("id\t図書\t著者\t貸出可能\n");
        printf("%d\t%s\t%s\t%d\t\n", result->data.id, result->data.title, result->data.author, result->data.enable);
    }
}

//貸出メソッド
void borrowBook(struct Node *headNode)
{
    struct Node *desBook;
    int id;
    printf("貸出書籍のid: ");
    scanf("%d", &id);
    desBook = findById(headNode, id);
    if (!desBook)
    {
        puts("該当の書籍は見つかりませんでした。");
    }else if(desBook->data.enable == 0) {
        puts("該当の書籍は既に貸出されています。");
    } else {
        desBook->data.enable = 0;
        saveInfoToFile(FILENAME, headNode);
        puts("貸出完了！");
    }
}

//返還メソッド
void returnBook(struct Node *headNode)
{
    struct Node *desBook;
    int id;
    printf("貸出書籍のid: ");
    scanf("%d", &id);
    desBook = findById(headNode, id);
    if (desBook)
    {
        desBook->data.enable = 1;
        saveInfoToFile(FILENAME, headNode);
    }
    
    puts("返還完了！");
}

// Navigation Bar
void makeNav()
{
    puts("---------------------------------\n");
    puts("書籍管理システム\n");
    puts("\t0. システムログアウト\n");
    puts("\t1. 書籍登録\n");
    puts("\t2. 書籍削除\n");
    puts("\t3. 書籍閲覧\n");
    puts("\t4. 検索\n");
    puts("\t5. 貸出\n");
    puts("\t6. 返還\n");
    puts("---------------------------------\n");
    printf("動作番号(0-6)を入力してください: ");
}

// Interaction
void interaction()
{
    int actKey = 0;
    scanf("%d", &actKey);
    clear();
    switch (actKey)
    {
    case 0:
        puts("------「システムログアウト」------");
        puts("ログアウトできました！");
        exit(0);
        break;

    case 1:
        puts("------「書籍登録」------");
        bookRegister(list);
        break;

    case 2:
        puts("------「書籍削除」------");
        bookDelete(list);
        break;

    case 3:
        puts("------「書籍閲覧」------");
        printList(list);
        break;

    case 4:
        puts("------「検索」------");
        findBook(list);
        break;

    case 5:
        puts("------「貸出」------");
        borrowBook(list);
        break;

    case 6:
        puts("------「返還」------");
        returnBook(list);
        break;

    default:
        puts("入力番号エラー: 正しい番号を入力してください\n");
        break;
    }
}

// Pause
void pauseContro(int *FLAGE)
{
    int temp = 0;
    printf("メインメニューに戻りますか？/yes...1 no...0: ");
    scanf("%d", &temp);
    clear();
    *FLAGE = temp;
}

int main()
{
    // create the list of books
    list = createHead();
    // read the info from file
    readInfoFromFile(FILENAME, list);
    int FLAGE = 1;
    while (FLAGE)
    {
        makeNav();
        interaction();
        pauseContro(&FLAGE);
    }

    return 0;
}