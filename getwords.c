#include <stdio.h>
#include <stdlib.h>
#include "strFuncs.h"
#include "strFuncs.c"
#define dictfile "./dict"
#define TRUE 1


/**
 *ListNode is used to hold a single node in a linked list of TreeNodes. The list is iterated by following ListNode->next until ListNode->next->next...->next is NULL
 **/
struct ListNode
{
    struct TreeNode *data;
    struct ListNode *next;
};

/**
 *A TreeNode has a letter, and a linked list of TreeNodes which are its children.
 *A TreeNode having a letter as a child means that thisNode.letter can be 
 *followed by thisNode.children->letter. For example, the TreeNode that has 'f'
 *as its letter would have 'a' (fan, etc.) and 'u' (fun, etc) as children, but 
 *say, 'y' would not be a child if the tree were built from a standard English dict.
 */
struct TreeNode
{
    char letter;
    struct ListNode *children;
};

void permute(const char *s, char *out, int *used, int len, int lev, struct TreeNode *tree);

const char *usage = "\"getwords abcde\" will return all possible words in the dictionary that can be made using the letters abcde \n";


/**
 *reads in the dictfile and builds a tree in memory (whole dictionary is 1.7mb so this is ok for most modern systems),
 *then gives the input letters to permute, which prints those combinations that match the dictionary
 **/
int main(int argc, const char * argv[])
{
    //build the prefix tree from the dictionary:
    FILE *dict = fopen(dictfile, "r");
    char line[80];
    struct TreeNode *root = malloc(sizeof(struct TreeNode));
    root->letter = '^';
    //printf("while %d", (int)root->children);
    //fflush(stdout);

    while( fgets(line, sizeof(line), dict) != NULL ) 
    {
        //printf("%s",line);
        add(root, line);  
    }
    /*Test code:
    char *word1 = "won\r\n";
    char *word2 = "wonder\r\n";
    char *word3 = "not_aword\r\n";
    char *word4 = "whatever\r\n";
    char *word5 = "airplane\r\n";
    
    printf("contains %s: %d",word1, contains(root, word1));
    printf("contains %s: %d",word2, contains(root, word2));
    printf("contains %s: %d",word3, contains(root, word3));
    printf("contains %s: %d",word4, contains(root, word4));
    printf("contains %s: %d",word5, contains(root, word5));
    //get a list of letters from the command line:
    /**/
    //one arg: check against default dict
    //two args: second arg is dict
    //no args: read from stdin
    if(argc == 2)
      {
    int len = str_len(argv[1]);
    //printf("%s", argv[0]);
    
    char letters[len+1];
    str_copy(argv[1], letters);
    //letters[len] = '\r';
    letters[len] = '\n';
    //printf("----%s------\n\n", letters);
    //printf("%d\n", contains(root, letters));
    char out[len+1];
    int used[len+1];
    int i = 0;
    while(i< len+1){ used[i] = 0; i++;}
    permute(argv[1], out, used, len, 0, root);
}
    else{printf("%s", usage);}
}

int letter_count(char *list, char c, int len)
{
    int i = 0;
    int ct = 0;
    while(i<len)
    {
        if(list[i] == c) ct++;
        i++;
    }
    return ct;
}



/**
 *takes s, and generates every possible combination of some or all of the letters in s.
 *each combination is then checked against the dictionary to see if it is a
 *word, and printed if it is
 *
 *should be put behind a wrapper that calls with default values. 
 */
void permute(const char *s, char *out, int *used, int len, int lev, struct TreeNode *tree)
{
    if (len == lev) {
        
        //out[lev] = '\0';
        //puts(out);
        return;
    }   
    char this_level[len];

    int i;
    for (i = 0; i < len; i++) {
        if(!((used[i]) || (letter_count(this_level, s[i], len))))
        {

            this_level[i] = s[i];
            used[i] = 1;
            out[lev] = s[i];
            out[lev+1] = '\0';
            char word[lev+4];
            sprintf(word, "%s\n" ,out); 
            if (contains(tree, word)) printf("%s",word);
            //printf("%s", word);
            permute(s, out, used, len, lev + 1, tree);
            used[i] = 0;
        }
    }
    return;
}


int has_child(struct TreeNode *node, char *word)
{
    struct ListNode *childlist = node->children;
    int result = 0;
    while(childlist != NULL && !result)
    {
        result = (childlist->data->letter == *word);
        childlist = childlist->next;
    }
    return result;  
}

/**
 *Returns true if the dict contains the given word.
 *Checks if root has a child matching the first letter in word, then if so
 *checks whether the node matching the first letter has a child matching the
 *secondletter, and so forth.
 *
 *performance is O(length_of_alphabet*length_of_longest_word)
 */
int contains(struct TreeNode *dict, char *word)
{
    if(*word == '\n')//end of the word
    {
        return has_child(dict, word);
    }
    else
    {
        struct ListNode *chirruns = dict->children;
        int found = 0;
        while(chirruns != NULL && !found)
        {
            if(chirruns->data->letter == *word) found = 42;
            else chirruns = chirruns->next;
        }
        if (chirruns == NULL) return 0;
        else return contains(chirruns->data, word+1);
    }
}

int add(struct TreeNode *tree, char *word)
{
    //word is over. stop
    if(*word == 0)
    {
        return 0;
    }
    
    //check if tree has a child that matches *word
    struct ListNode *children = tree->children;
    int found = 0;
    struct ListNode *theChild;
    while(children != NULL && !found)
    {   
        theChild = children;
        if(children->data->letter == *word)
        {
            found = TRUE;
        }
        
        children = children->next;
    }//at this point theChild is either a reference to a child of this node, or null. 
    
    if(found)//tree has a child that matches *word. add word+1 to that child
    {
        add(theChild->data, word+1);
    }
    else//tree does not have a child for current letter. malloc one.
    {
        
        if(tree->children == NULL)//tree has no children, make theChild tree's first child
        {
            tree->children = malloc(sizeof(struct ListNode));
            tree->children->data = malloc(sizeof(struct TreeNode));
            tree->children->data->letter = *word;
            tree->children->data->children = NULL;
        }
        else//tree has at least one child. add to end of list
        {//TODO make this a function:
            theChild->next = malloc(sizeof(struct ListNode));//this is an ugly use of loop side effects
            theChild->next->data = malloc(sizeof(struct TreeNode));
            theChild->next->data->letter = *word;
            theChild->next->data->children = NULL;
        }
        //now tree has a child for the current letter.
        add(tree, word);
    }
}
