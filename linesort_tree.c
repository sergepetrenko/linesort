#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FGETS_BUFSIZE 8

typedef struct tree tree;
struct tree{
	char *payload;
	struct tree *left;
	struct tree *right;
};

tree *tree_append(char *str, tree *root) {
	if(str == NULL) {
		return root;
	}
	if(root == NULL) {
		root = (tree *)malloc(1 * sizeof(tree));
		if(root == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		root->payload = str;
		root->left = NULL;
		root->right = NULL;
	} else {
		tree *tmp = root;
        _Bool end_of_tree = 0;
        while(!end_of_tree) {
			if(strcmp(str, tmp->payload) < 0) {
				if(tmp->left != NULL) {
					tmp = tmp->left;
				} else {
					tmp->left = (tree *)malloc(1 * sizeof(tree));
					if(tmp->left == NULL) {
						fprintf(stderr, "%s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
					end_of_tree = 1;
					tmp = tmp->left;
					tmp->payload = str;
					tmp->left = NULL;
					tmp->right = NULL;
				}	
			} else {
				if(tmp->right != NULL) {
					tmp = tmp->right;
				} else {
					tmp->right = (tree *)malloc(1 * sizeof(tree));
					if(tmp->right == NULL) {
						fprintf(stderr, "%s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
					end_of_tree = 1;
					tmp = tmp->right;
					tmp->payload = str;
					tmp->left = NULL;
					tmp->right = NULL;
				}
			}
		}
	}
	return root;
}

void tree_print(tree *root) {
	if(root != NULL) {
		tree_print(root->left);
		printf("%s\n", root->payload);
		tree_print(root->right);
	}
	return;
}

void tree_free(tree *root) {
	if(root != NULL) {
		if(root->payload != NULL) {
			free(root->payload);
		}
		tree_free(root->left);
		tree_free(root->right);
		free(root);
	}
	return;
}

char *getline_unlim(void) {
    char *tmp, *str = NULL;
    int str_size = 0, j;
    _Bool end_of_str = 0;
    str = (char*)malloc(FGETS_BUFSIZE * sizeof(char));
    if(str == NULL) {
        fprintf(stderr, "%s\n" , strerror(errno));
        exit(EXIT_FAILURE);
	}
    do {
        str = (char*)realloc(str, str_size + FGETS_BUFSIZE);
		if(str == NULL) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(EXIT_FAILURE);
		}
        tmp = fgets(str + str_size, FGETS_BUFSIZE, stdin);
        if(tmp != NULL) {
            for(j = 0; j < FGETS_BUFSIZE; j++) {
                if (tmp[j] == '\n') {
                    tmp[j] = '\0';
                    end_of_str = 1;
                    break;
				}
			}
		}
        str_size += FGETS_BUFSIZE - 1;
	} while(tmp != NULL && !end_of_str);
	if(tmp == NULL && !feof(stdin)) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return str;
}

int main(void) {
	char *str;
	tree *root = NULL;
    str = getline_unlim();
	while(!feof(stdin)) {
		root = tree_append(str, root);
        str = getline_unlim();
	}
	tree_print(root);
	tree_free(root);
	return 0;
}
