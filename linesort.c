#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FGETS_BUFSIZE 8
#define PARAM_MERGE_SORT 'm'
#define PARAM_BUBBLE_SORT 'b'

typedef struct list list;
struct list {
	char *payload;
	struct list *next;
};



list *list_append(char *str, list *str_list) {
	list *elem;
	if(str_list == NULL) {
		str_list = malloc(1 * sizeof(list));
		if(str_list == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		str_list->next = NULL;
		str_list->payload = str;
	} else {
		elem = (list *)malloc(1 * sizeof(list));
		if(elem == NULL) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		elem->next = str_list;
		elem->payload = str;
		str_list = elem;
	}
	return str_list;
}

list *list_addmember(list *lst_member, list *lst) {
	list *new_elem;
	if(lst_member != NULL) {
		if(lst == NULL) {
			lst = malloc(1 * sizeof(list));
			if(lst == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			lst->next = NULL;
			lst->payload = lst_member->payload;
		} else {
			new_elem = (list *)malloc(1 * sizeof(list));
			if(new_elem == NULL) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			new_elem->payload = lst_member->payload;
			new_elem->next = lst;
			lst = new_elem;
		}
	}
	return lst;
}

list *list_swap(list * lst) {
	if(lst != NULL && lst->next != NULL) {
		list *prev = NULL, *next = lst->next, *curr = lst;
		while(next != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
		}
		lst = prev;
	}
	return lst;
}

list *merge_lists(list *lst1, list *lst2) {
	list *lst = NULL, *tmp;
	while(lst1 != NULL && lst2 != NULL) {
		if(strcmp(lst1->payload, lst2->payload) < 0) {
			lst = list_addmember(lst1, lst);
			tmp = lst1;
			lst1 = lst1->next;
			free(tmp);
		} else {
			lst = list_addmember(lst2, lst);
			tmp = lst2;
			lst2 = lst2->next;
			free(tmp);
		}
	}
	while(lst1 != NULL) {
		lst = list_addmember(lst1, lst);
		tmp = lst1;
		lst1 = lst1->next;
		free(tmp);
	}
	while(lst2 != NULL) {
		lst = list_addmember(lst2, lst);
		tmp = lst2;
		lst2 = lst2->next;
		free(tmp);
	}
	lst = list_swap(lst);
	return lst;
}

void print_list(list *lst) {
	list *curr_elem = lst;
	while(curr_elem != NULL) {
		printf("%s\n", curr_elem->payload);
		curr_elem = curr_elem->next;
	}
	return;
}

void free_list(list *lst){
	list *curr_elem, *prev_elem;
	if(lst != NULL) {
		curr_elem = lst;
		while(curr_elem != NULL) {
			prev_elem = curr_elem;
			curr_elem = curr_elem->next;
			free(prev_elem->payload);
			free(prev_elem);
		}
	}
	return;
}

list  *bubble_sort(list *lst) {
	list *curr_elem, *next_elem, *max_elem = lst, *last_sort;
	char *tmp;
	if(lst != NULL) {
		while(max_elem->next != NULL) {
			max_elem = max_elem->next;
		}
		while(max_elem != lst) {
			curr_elem = lst;
			next_elem = lst;
			last_sort = lst;
			while(next_elem != max_elem) {
				next_elem = next_elem->next;
				if(strcmp(curr_elem->payload, next_elem->payload) > 0) {
					tmp = next_elem->payload;
					next_elem->payload = curr_elem->payload;
					curr_elem->payload = tmp;
					last_sort = next_elem;
				}
				curr_elem = curr_elem->next;
			}
			max_elem = last_sort;
		}
	}
	return lst;
}

list *merge_sort(list *lst) {
	list *low = NULL, *high = NULL, *tmp = lst;
	if(lst != NULL) {
		if(lst->next == NULL) {
			return lst;
		} else {
			for(int i = 0;; i++){
				if(i % 2 == 0) {
					low = list_addmember(tmp, low);
				} else {
					high = list_addmember(tmp, high);
				}
				if(tmp->next == NULL) {
					break;
				} else {
					tmp = tmp->next;
				}
			}
			low = merge_sort(low);
			high = merge_sort(high);
			lst = merge_lists(low, high);
		}
	}
	return lst;
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

int main(int argc, char *argv[]) {
	list *string_list = NULL;
	char *curr_str;
	char sort_param;
	if(argc > 2) {
		printf("Too many arguments passed, you may choose type of sorting algorithm using '-b' for bubble sort and '-m' for merge sort\n");
		return 0;
	}
	if(argc == 1) {
		sort_param = PARAM_BUBBLE_SORT;
	}
	if(argc == 2) {
		if (strcmp(argv[1],"-m") == 0) {
			sort_param = PARAM_MERGE_SORT;
		} else if (strcmp(argv[1], "-b") == 0){
			sort_param = PARAM_BUBBLE_SORT;
		} else {
			printf("Wrong arguments passed, you may choose type of sorting algorithm using '-b' for bubble sort and '-m' for merge sort\n");
			return 0;
		}
	}
	curr_str = getline_unlim();
	while(!feof(stdin)) {
		string_list = list_append(curr_str, string_list);
		curr_str = getline_unlim();
	}
	if(string_list == NULL) {
		printf("\t*No lines could be read, nothing to sort.\n\t*Exiting now.\n");
	} else {
		if(sort_param == PARAM_BUBBLE_SORT) {
			string_list = bubble_sort(string_list);
		} else if(sort_param == PARAM_MERGE_SORT) {
			string_list = merge_sort(string_list);
		}
		print_list(string_list);
		free_list(string_list);
	}	
	return 0;
}
