#include "stdio.h"
#include "base_utils.h"

enum tree_type
{
    LEAF,
    STEM,
    FORK
};

struct tree_s
{
    enum tree_type type;
    struct tree_s* left;
    struct tree_s* right;
};

struct tree_s* tree_create(enum tree_type type, struct tree_s* left, struct tree_s* right)
{
    assert(LEAF == type || STEM == type || FORK == type);
    if(LEAF == type) assert(GORNISHT == left && GORNISHT == right);
    if(STEM == type) assert(GORNISHT != left && GORNISHT == right);
    if(FORK == type) assert(GORNISHT != left && GORNISHT != right);

    struct tree_s* ret = malloc(sizeof(*ret));
    //printf("create address %p\n", ret);
    ret->type = type;
    ret->left = left;
    ret->right = right;
    return ret;
}

void tree_destroy(struct tree_s* tree_p)
{
    if(LEAF == tree_p->type) { assert(GORNISHT == tree_p->left && GORNISHT == tree_p->right); }
    if(STEM == tree_p->type) { assert(GORNISHT != tree_p->left && GORNISHT == tree_p->right); tree_destroy(tree_p->left); }
    if(FORK == tree_p->type) { assert(GORNISHT != tree_p->left && GORNISHT != tree_p->right); tree_destroy(tree_p->left); tree_destroy(tree_p->right); }
    free(tree_p);
    //printf("destroy address %p\n", tree_p);
}

struct tree_s* tree_create_by_copying(struct tree_s* tree_p)
{
    if(LEAF == tree_p->type)
        return tree_create(LEAF, GORNISHT, GORNISHT);
    if(STEM == tree_p->type)
        return tree_create(STEM, tree_create_by_copying(tree_p->left), GORNISHT);
    if(FORK == tree_p->type)
        return tree_create(FORK, tree_create_by_copying(tree_p->left), tree_create_by_copying(tree_p->right));
    assert(false);
}

struct tree_s* tree_create_by_application(struct tree_s* tree1_p, struct tree_s* tree2_p)
{
    struct tree_s* tmp1 = GORNISHT;
    struct tree_s* tmp2 = GORNISHT;
    struct tree_s* ret = GORNISHT;

    if(LEAF == tree1_p->type)
    {
        printf("cover A\n");
        return tree_create(STEM, tree_create_by_copying(tree2_p), GORNISHT);
    }
    if(STEM == tree1_p->type)
    {
        printf("cover B\n");
        return tree_create(FORK, tree_create_by_copying(tree1_p->left), tree_create_by_copying(tree2_p));
    }
    assert(FORK == tree1_p->type);
    if(LEAF == tree1_p->left->type)
    {
        printf("cover C\n");
        return tree_create_by_copying(tree1_p->right);
    }
    if(STEM == tree1_p->left->type)
    {
        printf("cover D\n");
        tmp1 = tree_create_by_application(tree1_p->left->left, tree2_p);
        tmp2 = tree_create_by_application(tree1_p->right, tree2_p);
        ret = tree_create_by_application(tmp1, tmp2);
        tree_destroy(tmp1);
        tree_destroy(tmp2);
        return ret;
    }
    assert(FORK == tree1_p->left->type);
    if(LEAF == tree2_p->type)
    {
        printf("cover E\n");
        return tree_create_by_copying(tree1_p->left->left);
    }
    if(STEM == tree2_p->type)
    {
        printf("cover F\n");
        return tree_create_by_application(tree1_p->left->right, tree2_p->left);
    }
    printf("cover G\n");
    assert(FORK == tree2_p->type);
    tmp1 = tree_create_by_application(tree1_p->right, tree2_p->left);
    ret = tree_create_by_application(tmp1, tree2_p->right);
    tree_destroy(tmp1);
    return ret;
}

void tree_print_i(struct tree_s* tree_p)
{
    if(LEAF == tree_p->type) { printf("LEAF "); }
    if(STEM == tree_p->type) { printf("STEM "); tree_print_i(tree_p->left); }
    if(FORK == tree_p->type) { printf("FORK "); tree_print_i(tree_p->left); tree_print_i(tree_p->right); }
}

void tree_print(struct tree_s* tree_p) { tree_print_i(tree_p); printf("\n"); }

int main()
{
    //printf("create F\n");
    struct tree_s* F = tree_create(LEAF, GORNISHT, GORNISHT);
    //printf("create T\n");
    struct tree_s* T = tree_create(STEM, tree_create(LEAF, GORNISHT, GORNISHT), GORNISHT);
    //printf("create NOT\n");
    struct tree_s* NOT = tree_create(FORK, tree_create(FORK, tree_create_by_copying(T), tree_create(FORK, tree_create_by_copying(F), tree_create_by_copying(F))), tree_create_by_copying(F));
    //printf("create NOT_T\n");
    struct tree_s* NOT_T = tree_create_by_application(NOT, T);
    //printf("create NOT_F\n");
    struct tree_s* NOT_F = tree_create_by_application(NOT, F);
    //printf("create NOT NOT\n");
    struct tree_s* NOT_NOT = tree_create_by_application(NOT, NOT);

    printf("Hello, World!\n");
    printf("this is T:\n");
    tree_print(T);
    printf("this is F:\n");
    tree_print(F);
    printf("this is NOT:\n");
    tree_print(NOT);
    printf("this is NOT_T:\n");
    tree_print(NOT_T);
    printf("this is NOT_F:\n");
    tree_print(NOT_F);
    printf("this is NOT_NOT:\n");
    tree_print(NOT_F);

    //printf("destroy F\n");
    tree_destroy(F);
    //printf("destroy T\n");
    tree_destroy(T);
    //printf("destroy NOT\n");
    tree_destroy(NOT);
    //printf("destroy NOT_T\n");
    tree_destroy(NOT_T);
    //printf("destroy NOT_F\n");
    tree_destroy(NOT_F);
    //printf("create NOT_NOT\n");
    tree_destroy(NOT_NOT);

    return 0;
}