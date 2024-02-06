#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "lib\stb_image.h"

#define MAX_TREE_HEIGHT 100

typedef struct node
{
    int key;
    int freq;
    int code[MAX_TREE_HEIGHT];
    int codeSize;
    struct node *next;
    struct node *right;
    struct node *left;
} Node;

Node *head = NULL;
int list_size = 0;

void print_node(Node *node)
{
    printf("Pixel Value: %5d\tFrequency: %5d\tHuffman Code: ", node->key, node->freq);
    for (int i = 0; i < node->codeSize; i++)
    {
        printf("%d", node->code[i]);
    }
    printf("\n");
}

void print_list()
{
    if (head == NULL)
    {
        printf("Queue is empty!\n");
    }
    else
    {
        Node *temp = head;
        printf("#HEAD OF LIST\n\n");
        while (temp != NULL)
        {
            print_node(temp);
            temp = temp->next;
        }
        printf("\n#END OF LIST\n");
    }
}

Node *create_new_node(int key, int freq)
{
    Node *new = (Node *)malloc(sizeof(Node));
    new->key = key;
    new->freq = freq;
    new->next = NULL;
    new->right = NULL;
    new->left = NULL;

    return new;
}

void insert_node(Node *new)
{
    if (head == NULL || head->freq > new->freq)
    {
        new->next = head;
        head = new;
        list_size++;
    }
    else
    {

        Node *temp = head;
        while (temp->next != NULL && temp->next->freq < new->freq)
        {
            temp = temp->next;
        }
        new->next = temp->next;
        temp->next = new;
        list_size++;
    }
}

Node *pop()
{
    if (head == NULL)
    {
        printf("Queue is empty!\n");
        return NULL;
    }
    else
    {
        Node *temp = (Node *)malloc(sizeof(Node));
        temp = head;
        if (head->next != NULL)
        {
            head = head->next;
        }
        else
        {
            Node *new = create_new_node(-2, 0);
            insert_node(new);
            head->next = NULL;
        }
        list_size--;
        return temp;
    }
}

int is_leaf(Node *node)
{
    return ((node->right == NULL) && (node->left == NULL)) ? 1 : 0;
}

void set_code(Node *node, int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        node->code[i] = arr[i];
    }
    node->codeSize = size;
}

void calc_huffman_codes(Node *root, int *arr, int index)
{
    if (is_leaf(root))
    {
        set_code(root, arr, index);
        insert_node(root);
    }
    else
    {
        arr[index] = 0;
        calc_huffman_codes(root->left, arr, index + 1);
        arr[index] = 1;
        calc_huffman_codes(root->right, arr, index + 1);
    }
}

void create_tree()
{
    int count = 0;
    while (list_size > 1)
    {
        Node *right = pop();
        Node *left = pop();

        Node *root = (Node *)malloc(sizeof(Node));

        root->key = -1;
        root->freq = right->freq + left->freq;
        root->right = right;
        root->left = left;

        insert_node(root);

        if (head->key == -2)
        {
            pop();
        }
        count++;
    }
    printf("%d\n", count);
}

void freqs2list(int *arr, int length)
{
    int i, j;
    int visited = -1;

    int *fr = (int *)malloc(length * sizeof(int));

    for (i = 0; i < length; i++)
    {
        if (fr[i] != visited)
        {
            int count = 1;
            for (j = i + 1; j < length; j++)
            {
                if (arr[i] == arr[j])
                {
                    count++;
                    fr[j] = visited;
                }
            }
            insert_node(create_new_node(arr[i], count));
        }
    }
}

int main()
{
    system("cls");
    char fileName[100];

    printf("Enter file name : ");
    scanf("%s", fileName);

    int width, height, channels;

    unsigned char *img = stbi_load(fileName, &width, &height, &channels, 0);
    if (img == NULL)
    {
        printf("Error while loading image!\n");
        exit(1);
    }

    int *arr = (int *)malloc(width * height * sizeof(int));

    for (int i = 0; i < width * height; i++)
    {
        arr[i] = (int)img[i * channels];
    }

    int codes[MAX_TREE_HEIGHT];

    freqs2list(arr, width * height);

    create_tree();

    calc_huffman_codes(head, codes, 0);

    printf("--------------- --------------- ---------------\n");

    print_list();

    return 0;
}