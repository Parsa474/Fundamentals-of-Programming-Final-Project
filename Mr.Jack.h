struct info{
	int character;
	int random;
};

struct node{
	struct info character;
	struct node *next;
};

void add_front(struct node *new_node, struct node **list){
     new_node->next = *list;
     *list = new_node;     
}

void add_end(struct node *new_node, struct node *list){
	struct node *current;
	for(current = list; current-> next != NULL; current = current->next);
	current->next = new_node;
	new_node->next = NULL;     
}

void print_list(struct node *list){
	printf("\n");
    struct node * current = list;
    while(current != NULL){
        printf("%d %d\n", current->character.character, current->character.random);
        current = current->next;
    }
    printf("\n");
}

int get(struct node *list, int index){
	if(index<0){
		printf("Wrong index get<0\n");
		return -1;
	}
	struct node *current=list;
	int i;
	for(i=0; i<index; i++){
		current=current->next;
	}
	if(current == NULL){
		printf("Wrong index get: too far\n");
		return -1;
	}
	return current->character.character;
}

int get_rand(struct node *list, int index){
	if(index<0){
		printf("Wrong index<0\n");
		return -1;
	}
	struct node *current=list;
	int i;
	for(i=0; i<index; i++){
		current=current->next;
	}
	if(current == NULL){
		printf("Wrong index too far\n");
		return -1;
	}
	return current->character.random;
}

void swap_in_list(struct node *list, int a, int b){
	int i;
	struct info temp;
	struct node * node1 = list;
	struct node * node2 = list;
	for(i=0; i<a; i++){
		node1 = node1->next;
	}
	for(i=0; i<b; i++){
		node2 = node2->next;
	}
	temp=node2->character;
	node2->character=node1->character;
	node1->character=temp;
	return;
}

void randomize_list(struct node *list){
	struct node *current=list;
	while(current != NULL){
           current->character.random=rand()%100;
           current = current->next;
    }
    return;
}

void shuffle(struct node *list){
	int i, j;
	randomize_list(list);
	for(i=0; i<8; i++){
		for(j=i+1; j<8; j++){
			if(get_rand(list, i)>get_rand(list, j)){
				swap_in_list(list, i, j);
			}
		}
	}
	return;
}

void remove_node(struct node **head, int index){
	struct node *temp = *head, *prev;
	if(temp != NULL && index == 0){
		*head = temp->next;
        free(temp);
        return;
    }
	int i;
	for(i=0; i<index && temp != NULL; i++){
		prev = temp;
		temp = temp->next;
		if(temp == NULL){
			printf("Wrong index for remove_nod\n");
			return;
		}
	}
	prev->next = temp->next;
    free(temp);
    return;
}

struct node * create_node(struct info i){
	struct node * nn;
	nn = (struct node *) malloc(sizeof(struct node));
  	if (nn == NULL)
		return NULL;
	nn->character = i;
	nn->next = NULL;
	return nn;
}

void linked_list_cpy(struct node **list2, struct node * list1){
	struct node *new_node;
	struct node *current=list1;
	int i;
	for(i=0; i<8; i++, current=current->next){
		new_node = (struct node *)malloc(sizeof(struct node));
		if(new_node == NULL){
			printf("memory allocation failed!\n");
            return;                               
        }
        new_node->character = current->character;
        if(*list2 == NULL)
            add_front(new_node, list2);
        else
            add_end(new_node, *list2);
	}
	return;
}

enum blocks_contents{
	SH,//0
	JW,//1
	JS,//2
	IL,//3
	MS,//4
	SG,//5
	WG,//6
	JB,//7
	street,//8
	esc,//9
	light,//10
	building,//11
	extra,//12
};

enum directions{
	up = 8,
	up_left = 7,
	down_left = 1,
	down = 2,
	down_right = 3,
	up_right = 9,
};

struct coordinates{
	int x;
	int y;
};

struct character_info{
	int identifier;
	bool sus;
	bool visible;
	int direction;
};

struct block{
	int identifier;
	bool sus;				//for characters: if 1 it means we are not sure if a character is innocent or not
	bool visible;			//for characters: if 1 it means the character is visible
	bool well;				//if 1 it means if the identifier is character/street it has a well
	bool blocked;			//if 1 it means the ifentifier is character/street and has a well, or if it's esc, it is blocked
	bool on;				//if 1 it means if the idetifier is light, it is on
	int direction;			//only used for JW ability, indicates the direction of his lantern(see enum directions)
	int num;				//for the numbered lights, if 0: unnumbered
	struct character_info temp;
};

struct availability{
	bool u, ur, dr, d, dl, ul;
};

void print_street(struct block block){
	if(block.identifier==JW){
		switch(block.direction){
			case(up): printf(" %c", 24);
			break;
			case(up_left): printf(")%c", 24);
			break;
			case(down_left): printf(")%c", 25);
			break;
			case(down): printf(" %c", 25);
			break;
			case(down_right): printf("%c(", 25);
			break;
			case(up_right): printf("%c(", 24);
			break;
		}
	} else printf("  ");
	switch(block.identifier){
		case SH:
			if(block.visible)
				printf("SH");
			else
				printf("sh");
		break;
		case JW:
			if(block.visible)
				printf("JW");
			else
				printf("jw");
		break;
		case JS:
			if(block.visible)
				printf("JS");
			else
				printf("js");
		break;
		case IL:
			if(block.visible)
				printf("IL");
			else
				printf("il");
		break;
		case MS:
			if(block.visible)
				printf("MS");
			else
				printf("ms");
		break;
		case SG:
			if(block.visible)
				printf("SG");
			else
				printf("sg");
		break;
		case WG:
			if(block.visible)
				printf("WG");
			else
				printf("wg");
		break;
		case JB:
			if(block.visible)
				printf("JB");
			else
				printf("jb");
		break;
		case street:
			printf("  ");
		break;
	}
	if(block.sus)
		printf("!");
	else
		printf(" ");
	if(block.well)
		if(block.blocked)
			printf("%c", 21);
		else
			printf("%c", 18);
	else
		printf(" ");
	return;
}

void print_block(struct block block){
	switch(block.identifier){
		case(esc):
			if(block.blocked)
				printf("Locked");
			else
				printf("Escape");
		break;
		case(light):
			if(block.on)
				printf("   %c ", 15);
			else
				printf("   o ");
			if(block.num)
				printf("%d", block.num);
			else
				printf(" ");
		break;
		case(building):
			printf(" %c%c%c%c ", 127, 127, 127, 127);
		break;
		case(extra):
			printf("xxxxxx");
		break;
	}
	return;
}

void print_map(struct block blocks[9][13]){
	int i, j, index;
	for(i=0; i<=18; i++){
		if(i==0){
			printf("x%c   ", 26);
			for(j=0; j<13; j++){
			printf("%-7d", j+1);
			}
			printf("\n%cy        ", 25);
			for(j=0; j<6; j++){
			printf("______        ");
			}
		}
		printf("\n");
		if(i%2==0 && i!=18) printf("%d ", i/2+1);
		else printf("  ");
		if(i%2==0){
			printf("\\");
		}
		if(i%2==1){	
			for(j=0; j<=12; j+=2){
				printf("/");
				if(blocks[i/2][j].identifier<9) print_street(blocks[i/2][j]);
				else print_block(blocks[i/2][j]);
				//printf("      ");
				printf("\\");
				if(j!=12) printf("______");
			}
		} else{
			printf("______");
			for(j=1; j<=11; j+=2){
				printf("/");
				if(i!=18) 
					if(blocks[i/2][j].identifier<9) print_street(blocks[i/2][j]);
					else print_block(blocks[i/2][j]);
					//printf("      ");
				else printf("      ");
				printf("\\");
				printf("______");
			}
			printf("/");
		}
	}
	printf("\n\n");
}

int read_locations(char name[], int identifier, struct block blocks[9][13]){
	int x, y;
	struct block b;
	b.identifier=identifier;
	FILE *f=fopen(name, "r");
	if(f==NULL){
		printf("Could not open the %s file\n", name);
		close(f); 
		return 1;
	}
	while(1){
		fscanf(f, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &x, &y, &b.sus, &b.visible, &b.well, &b.blocked, &b.on, &b.direction, &b.num);
		if(x==-1) break;
		blocks[y][x]=b;
	}
	close(f);
	return 0;
}

void update_screen(struct block blocks[9][13]){
	system("cls");
	printf("\n");
	print_map(blocks);
}

//from her onwards are the actions' functions

struct coordinates find_character(int identifier, struct block blocks[9][13]){
	int i, j;
	struct coordinates c;
	for(i=0; i<9; i++){
		for(j=0; j<13; j++){
			if(blocks[i][j].identifier==identifier){
				c.x=j;
				c.y=i;
				return c;
			}
		}
	}
	c.x=-1;
	c.y=-1;
	printf("cound not find the character\n");
	return c;
}

bool MS_check(int i, int character){
	bool output;
	if(character==MS){
		output = (i!=extra);
	}else{
		output = (i<light);
	}
	return output;
}
struct availability check_around(int x, int y, struct block blocks[9][13], int character){
	struct availability av;
	av.d=0; av.dl=0; av.dr=0; av.u=0; av.ul=0; av.ur=0;
	int i;
	bool b, r = x%2;
	b = (y!=0);													//check if we can go up
	if(b){
		i = blocks[y-1][x].identifier;
		if(MS_check(i, character)){
			if(blocks[y-1][x].identifier!=esc){
				av.u=1;
			} else if(blocks[y-1][x].blocked==0){
				av.u=1;
			}
		}
	}
	b = (y!=8);													//check if we can go down
	if(b){
		i = blocks[y+1][x].identifier;
		if(MS_check(i, character)){
			if(blocks[y+1][x].identifier!=esc){
				av.d=1;
			} else if(blocks[y+1][x].blocked==0){
				av.d=1;
			}
		}
	}
	b = (x!=12 && y-r>=0 && y-r<9);								//check if we can go up_right
	if(b){
		i = blocks[y-r][x+1].identifier;
		if(MS_check(i, character)){
			if(blocks[y-r][x+1].identifier!=esc){
				av.ur=1;
			} else if(blocks[y-r][x+1].blocked==0){
				av.ur=1;
			}
		}
	}
	b = (x!=0 && y-r>=0 && y-r<9);								//check if we can go up_left
	if(b){
		i = blocks[y-r][x-1].identifier;
		if(MS_check(i, character)){
			if(blocks[y-r][x-1].identifier!=esc){
				av.ul=1;
			} else if(blocks[y-r][x-1].blocked==0){
				av.ul=1;
			}
		}
	}
	b = (x!=0 && y+!r>=0 && y+!r<9);							//check if we can go down_left
	if(b){
		i = blocks[y+!r][x-1].identifier;
		if(MS_check(i, character)){
			if(blocks[y+!r][x-1].identifier!=esc){
				av.dl=1;
			} else if(blocks[y+!r][x-1].blocked==0){
				av.dl=1;
			}
		}
	}
	b = (x!=12 && y+!r>=0 && y+!r<9);							//check if we can go down_right
	if(b){
		i = blocks[y+!r][x+1].identifier;
		if(MS_check(i, character)){
			if(blocks[y+!r][x+1].identifier!=esc){
				av.dr=1;
			} else if(blocks[y+!r][x+1].blocked==0){
				av.dr=1;
			}
		}
	}
	return av;
}

void checker(int a, int b, int *choice){
	while(*choice<a || *choice >b){
		printf("Enter a number from %d to %d!\n", a, b);
		scanf(" %d", choice);
	}
	return;
}

void print_character_name(int identifier){
	switch(identifier){
		case SH:
			printf("SH");
		break;
		case JW:
			printf("JW");
		break;
		case JS:
			printf("JS");
		break;
		case IL:
			printf("IL");
		break;
		case MS:
			printf("MS");
		break;
		case SG:
			printf("SG");
		break;
		case WG:
			printf("WG");
		break;
		case JB:
			printf("JB");
		break;
		default:
			printf("Invalid input for character identifier!");
		break;
	}
	return;
}

void swap_character_info(struct block *b1, struct block *b2){
	
	struct character_info temp;
	
	temp.identifier=b1->identifier;
	temp.sus=b1->sus;
	temp.visible=b1->visible;
	temp.direction=b1->direction;
	
	b1->identifier=b2->identifier;
	b1->sus=b2->sus;
	b1->visible=b2->visible;
	b1->sus=b2->direction;
	
	b2->identifier=temp.identifier;
	b2->sus=temp.sus;
	b2->visible=temp.visible;
	b2->direction=temp.direction;
	
	return;
}

int print_available_moves(struct availability av, int x, int y, bool must, struct block blocks[9][13]){
	int en=0;
	printf("available moves: ");
	if(blocks[y][x].well && blocks[y][x].blocked==0){
		printf("0:Well   ");
		en=1;
	}
	if(av.u){
		printf("%d:%c    ", up, 24);
	}
	if(av.ur){
		printf("%d:%c(   ", up_right, 24);
	}
	if(av.dr){
		printf("%d:%c(   ", down_right, 25);
	}
	if(av.d){
		printf("%d:%c    ", down, 25);
	}
	if(av.dl){
		printf("%d:)%c   ", down_left, 25);
	}
	if(av.ul){
		printf("%d:)%c   ", up_left, 24);
	}
	if(!must) printf("5:I'm done");
	if(!av.u && !av.ur && !av.dr && !av.d && !av.dl && !av.ul && en==0){
		printf("Ops, this caracter cannot move, welp\n");
		return -1;
	}
	return en;
}


int move(struct coordinates c, struct block blocks[9][13], bool must, int *n, int *counter, int Jack, int detective){
	//printf("n=%d counter=%d\n", *n, *counter);
	if(*counter<*n){
	struct availability av;
	if(*counter==0) av = check_around(c.x, c.y, blocks, blocks[c.y][c.x].identifier);
	else av = check_around(c.x, c.y, blocks, blocks[c.y][c.x].temp.identifier);
	int choice, flag=1, x, y, character, en;
	if(*counter==0){
		character=blocks[c.y][c.x].identifier;
	} else{
		character=blocks[c.y][c.x].temp.identifier;
	}
	bool r = c.x % 2;
	struct coordinates next;
	struct character_info info;
	en = print_available_moves(av, c.x, c.y, must, blocks);
	if(en==-1){
		printf("You ran out of moves and cannot remain on this block, these moves are not acceptable!\n");
		return -1;
	}
	while(flag){
		if(*counter==0) printf("\nWhat's your move? Enter the number of the move you want to make: ");
		else printf("You can't remain there, type in your next move: ");
		scanf(" %d", &choice);
		switch(choice){
			case up:
				if(av.u){		//[c.y - 1][c.x]
					next.x=c.x;
					next.y=c.y - 1;
					if(blocks[next.y][next.x].identifier==street){
						if(*counter==0){
							swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
						}
						else{
							blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
							blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
							blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
							blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
						}
						*n-=1;
						flag=0;
					}else{
						if(blocks[next.y][next.x].identifier==esc){
							switch(*counter){
								case 0:
									if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
										printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;
								default:
									if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
										printf("Mr.Jack wins by escaping, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;	
							}
						}
						if(detective && blocks[next.y][next.x].identifier<light){
							printf("do you want to capture this character?  0.no  1.yes\n");
							scanf(" %d", &choice);
							checker(0, 1, &choice);
							if(choice){
								if(blocks[next.y][next.x].identifier==Jack){
									printf("Detective won! Jack was captured");
									exit(EXIT_SUCCESS);
								}else{
									printf("Jack won! Detective captured a poor innocent!");
									exit(EXIT_SUCCESS);
								}
							}
						}
						if(*n - *counter==1){
							printf("You will run out of moves and are not allowed to stay there, try again!\n");
							continue;
						}
						if(*counter==0){
							info.identifier=blocks[c.y][c.x].identifier;
							info.sus=blocks[c.y][c.x].sus;
							info.visible=blocks[c.y][c.x].visible;
							info.direction=blocks[c.y][c.x].direction;
						}else info=blocks[c.y][c.x].temp;
						*counter+=1;
						blocks[next.y][next.x].temp=info;
						if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
							*counter-=1;
							*n-=1;
							flag=0;
							if(*counter==0){
								blocks[c.y][c.x].identifier=street;
								blocks[c.y][c.x].sus=0;
								blocks[c.y][c.x].visible=0;
								blocks[c.y][c.x].direction=0;
							}
						}else{
							*counter-=1;
							continue;
						}
					}
				} else{
					printf("you can't make this move!\n");
				}
			break;
			case up_right:
				if(av.ur){		//[c.y - r][c.x + 1]
					next.x=c.x + 1;
					next.y=c.y - r;
					if(blocks[next.y][next.x].identifier==street){
						if(*counter==0){
							swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
						}
						else{
							blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
							blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
							blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
							blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
						}
						*n-=1;
						flag=0;
					}else{
						if(blocks[next.y][next.x].identifier==esc){
							switch(*counter){
								case 0:
									if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
										printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;
								default:
									if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
										printf("Mr.Jack wins by escaping, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;	
							}
						}
						if(detective && blocks[next.y][next.x].identifier<light){
							printf("do you want to capture this character?  0.no  1.yes\n");
							scanf(" %d", &choice);
							checker(0, 1, &choice);
							if(choice){
								if(blocks[next.y][next.x].identifier==Jack){
									printf("Detective won! Jack was captured");
									exit(EXIT_SUCCESS);
								}else{
									printf("Jack won! Detective captured a poor innocent!");
									exit(EXIT_SUCCESS);
								}
							}
						}
						if(*n - *counter==1){
							printf("You will run out of moves and are not allowed to stay there, try again!\n");
							continue;
						}
						if(*counter==0){
							info.identifier=blocks[c.y][c.x].identifier;
							info.sus=blocks[c.y][c.x].sus;
							info.visible=blocks[c.y][c.x].visible;
							info.direction=blocks[c.y][c.x].direction;
						}else info=blocks[c.y][c.x].temp;
						*counter+=1;
						blocks[next.y][next.x].temp=info;
						if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
							*counter-=1;
							*n-=1;
							flag=0;
							if(*counter==0){
								blocks[c.y][c.x].identifier=street;
								blocks[c.y][c.x].sus=0;
								blocks[c.y][c.x].visible=0;
								blocks[c.y][c.x].direction=0;
							}
						}else{
							*counter-=1;
							continue;
						}
					}
				} else{
					printf("you can't make this move!\n");
				}
			break;
			case down_right:
				if(av.dr){		//[c.y + !r][c.x + 1]
					next.x=c.x + 1;
					next.y=c.y + !r;
					if(blocks[next.y][next.x].identifier==street){
						if(*counter==0){
							swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
						}
						else{
							blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
							blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
							blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
							blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
						}
						*n-=1;
						flag=0;
					}else{
						if(blocks[next.y][next.x].identifier==esc){
							switch(*counter){
								case 0:
									if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
										printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;
								default:
									if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
										printf("Mr.Jack wins by escaping, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;	
							}
						}
						if(detective && blocks[next.y][next.x].identifier<light){
							printf("do you want to capture this character?  0.no  1.yes\n");
							scanf(" %d", &choice);
							checker(0, 1, &choice);
							if(choice){
								if(blocks[next.y][next.x].identifier==Jack){
									printf("Detective won! Jack was captured");
									exit(EXIT_SUCCESS);
								}else{
									printf("Jack won! Detective captured a poor innocent!");
									exit(EXIT_SUCCESS);
								}
							}
						}
						if(*n - *counter==1){
							printf("You will run out of moves and are not allowed to stay there, try again!\n");
							continue;
						}
						if(*counter==0){
							info.identifier=blocks[c.y][c.x].identifier;
							info.sus=blocks[c.y][c.x].sus;
							info.visible=blocks[c.y][c.x].visible;
							info.direction=blocks[c.y][c.x].direction;
						}else info=blocks[c.y][c.x].temp;
						*counter+=1;
						blocks[next.y][next.x].temp=info;
						if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
							*counter-=1;
							*n-=1;
							flag=0;
							if(*counter==0){
								blocks[c.y][c.x].identifier=street;
								blocks[c.y][c.x].sus=0;
								blocks[c.y][c.x].visible=0;
								blocks[c.y][c.x].direction=0;
							}
						}else{
							*counter-=1;
							continue;
						}
					}
				} else{
					printf("you can't make this move!\n");
				}
			break;
			case down:
				if(av.d){		//[c.y + 1][c.x]
					next.x=c.x;
					next.y=c.y + 1;
					if(blocks[next.y][next.x].identifier==street){
						if(*counter==0){
							swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
						}
						else{
							blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
							blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
							blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
							blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
						}
						*n-=1;
						flag=0;
					}else{
						if(blocks[next.y][next.x].identifier==esc){
							switch(*counter){
								case 0:
									if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
										printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;
								default:
									if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
										printf("Mr.Jack wins by escaping, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;	
							}
						}
						if(detective && blocks[next.y][next.x].identifier<light){
							printf("do you want to capture this character?  0.no  1.yes\n");
							scanf(" %d", &choice);
							checker(0, 1, &choice);
							if(choice){
								if(blocks[next.y][next.x].identifier==Jack){
									printf("Detective won! Jack was captured");
									exit(EXIT_SUCCESS);
								}else{
									printf("Jack won! Detective captured a poor innocent!");
									exit(EXIT_SUCCESS);
								}
							}
						}
						if(*n - *counter==1){
							printf("You will run out of moves and are not allowed to stay there, try again!\n");
							continue;
						}
						if(*counter==0){
							info.identifier=blocks[c.y][c.x].identifier;
							info.sus=blocks[c.y][c.x].sus;
							info.visible=blocks[c.y][c.x].visible;
							info.direction=blocks[c.y][c.x].direction;
						}else info=blocks[c.y][c.x].temp;
						*counter+=1;
						blocks[next.y][next.x].temp=info;
						if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
							*counter-=1;
							*n-=1;
							flag=0;
							if(*counter==0){
								blocks[c.y][c.x].identifier=street;
								blocks[c.y][c.x].sus=0;
								blocks[c.y][c.x].visible=0;
								blocks[c.y][c.x].direction=0;
							}
						}else{
							*counter-=1;
							continue;
						}
					}
				} else{
					printf("you can't make this move!\n");
				}
			break;
			case down_left:
				if(av.dl){		//[c.y + !r][c.x - 1]
					next.x=c.x - 1;
					next.y=c.y + !r;
					if(blocks[next.y][next.x].identifier==street){
						if(*counter==0){
							swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
						}
						else{
							blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
							blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
							blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
							blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
						}
						*n-=1;
						flag=0;
					}else{
						if(blocks[next.y][next.x].identifier==esc){
							switch(*counter){
								case 0:
									if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
										printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;
								default:
									if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
										printf("Mr.Jack wins by escaping, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;	
							}
						}
						if(detective && blocks[next.y][next.x].identifier<light){
							printf("do you want to capture this character?  0.no  1.yes\n");
							scanf(" %d", &choice);
							checker(0, 1, &choice);
							if(choice){
								if(blocks[next.y][next.x].identifier==Jack){
									printf("Detective won! Jack was captured");
									exit(EXIT_SUCCESS);
								}else{
									printf("Jack won! Detective captured a poor innocent!");
									exit(EXIT_SUCCESS);
								}
							}
						}
						if(*n - *counter==1){
							printf("You will run out of moves and are not allowed to stay there, try again!\n");
							continue;
						}
						if(*counter==0){
							info.identifier=blocks[c.y][c.x].identifier;
							info.sus=blocks[c.y][c.x].sus;
							info.visible=blocks[c.y][c.x].visible;
							info.direction=blocks[c.y][c.x].direction;
						}else info=blocks[c.y][c.x].temp;
						*counter+=1;
						blocks[next.y][next.x].temp=info;
						if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
							*counter-=1;
							*n-=1;
							flag=0;
							if(*counter==0){
								blocks[c.y][c.x].identifier=street;
								blocks[c.y][c.x].sus=0;
								blocks[c.y][c.x].visible=0;
								blocks[c.y][c.x].direction=0;
							}
						}else{
							*counter-=1;
							continue;
						}
					}
				} else{
					printf("you can't make this move!\n");
				}
			break;
			case up_left:
				if(av.ul){		//[c.y - r][c.x - 1]
					next.x=c.x - 1;
					next.y=c.y - r;
					if(blocks[next.y][next.x].identifier==street){
						if(*counter==0){
							swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
						}
						else{
							blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
							blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
							blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
							blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
						}
						*n-=1;
						flag=0;
					}else{
						if(blocks[next.y][next.x].identifier==esc){
							switch(*counter){
								case 0:
									if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
										printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;
								default:
									if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
										printf("Mr.Jack wins by escaping, try better next time detective :)\n");
										exit(EXIT_SUCCESS);
									}else{
										printf("This character can't escape!\n");
										return 1;
									}
								break;	
							}
						}
						if(detective && blocks[next.y][next.x].identifier<light){
							printf("do you want to capture this character?  0.no  1.yes\n");
							scanf(" %d", &choice);
							checker(0, 1, &choice);
							if(choice){
								if(blocks[next.y][next.x].identifier==Jack){
									printf("Detective won! Jack was captured");
									exit(EXIT_SUCCESS);
								}else{
									printf("Jack won! Detective captured a poor innocent!");
									exit(EXIT_SUCCESS);
								}
							}
						}
						if(*n - *counter==1){
							printf("You will run out of moves and are not allowed to stay there, try again!\n");
							continue;
						}
						if(*counter==0){
							info.identifier=blocks[c.y][c.x].identifier;
							info.sus=blocks[c.y][c.x].sus;
							info.visible=blocks[c.y][c.x].visible;
							info.direction=blocks[c.y][c.x].direction;
						}else info=blocks[c.y][c.x].temp;
						*counter+=1;
						blocks[next.y][next.x].temp=info;
						if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
							*counter-=1;
							*n-=1;
							flag=0;
							if(*counter==0){
								blocks[c.y][c.x].identifier=street;
								blocks[c.y][c.x].sus=0;
								blocks[c.y][c.x].visible=0;
								blocks[c.y][c.x].direction=0;
							}
						}else{
							*counter-=1;
							continue;
						}
					}
				} else{
					printf("you can't make this move!\n");
				}
			break;
			case 0:
				if(en==1){
					do{
						printf("enter the x,y of the well you want to go to: ");
						scanf(" %d %d", &x, &y);
						next.x=x-1;
						next.y=y-1;
						if(!blocks[next.y][next.x].well) printf("Invalid coordinates, try again: ");
						else if(blocks[next.y][next.x].blocked) printf("The destination well is blocked! try again: ");
						else if(blocks[next.y][next.x].identifier==street){
							if(*counter==0){
								swap_character_info(&blocks[c.y][c.x], &blocks[next.y][next.x]);
							} else{
								blocks[next.y][next.x].identifier=blocks[c.y][c.x].temp.identifier;
								blocks[next.y][next.x].sus=blocks[c.y][c.x].temp.sus;
								blocks[next.y][next.x].visible=blocks[c.y][c.x].temp.visible;
								blocks[next.y][next.x].direction=blocks[c.y][c.x].temp.direction;
							}
							*n-=1;
							flag=0;
						} else{
							if(*n - *counter==1){
								printf("You will run out of moves and are not allowed to stay there, try again!\n");
								continue;
							}
							if(*counter==0){
								info.identifier=blocks[c.y][c.x].identifier;
								info.sus=blocks[c.y][c.x].sus;
								info.visible=blocks[c.y][c.x].visible;
								info.direction=blocks[c.y][c.x].direction;
							}else info=blocks[c.y][c.x].temp;
							*counter+=1;
							blocks[next.y][next.x].temp=info;
							if(move(next, blocks, 1, n, counter, Jack, detective)!=-1){
								*counter-=1;
								*n-=1;
								flag=0;
								if(*counter==0){
									blocks[c.y][c.x].identifier=street;
									blocks[c.y][c.x].sus=0;
									blocks[c.y][c.x].visible=0;
									blocks[c.y][c.x].direction=0;
								}
							}else{
								*counter-=1;
								continue;
						}	
						}
					} while(flag);
				} else printf("This block does not have a well!");
			break;
			case 5: if(!must){
						*n=0;
						return 0;
					}
					else printf("You must make a move!\n");
			break;
			default:
				printf("invalid input!\n");
			break;
		}
	}
	update_screen(blocks);
	printf("moving character: ");
	print_character_name(character);
	printf("\n");
	return 1;
	}else{
		printf("You ran out of moves and cannot remain on this block, these moves are not acceptable!\n");
		return -1;
	}
}

void move_character(int character, struct coordinates initial, struct block blocks[9][13], int *n, int *counter, int Jack, int detective){
	struct coordinates c;
	int initial_n=*n;
	c=initial;
	while(initial.x==find_character(character, blocks).x && initial.y==find_character(character, blocks).y){
		move(c, blocks, 1, n, counter, Jack, detective);
		while(*n>0){
			c=find_character(character, blocks);
			move(c, blocks, 0, n, counter, Jack, detective);
		}
		update_screen(blocks);
		if(initial.x==find_character(character, blocks).x && initial.y==find_character(character, blocks).y){
			printf("bruh, you HAVE TO move :)\n");
			*n=initial_n;
		}
	}
	return;
}

bool visible_by_light(int character, struct block blocks[9][13]){
	struct coordinates c = find_character(character, blocks);
	bool visible=0;
	bool r = c.x % 2; 
	if(c.y>0) visible+=(blocks[c.y-1][c.x].identifier==light && blocks[c.y-1][c.x].on);
	if(c.y<8) visible+=(blocks[c.y+1][c.x].identifier==light && blocks[c.y+1][c.x].on);
	if(c.y-r>=0 && c.x>0) visible+=(blocks[c.y-r][c.x-1].identifier==light && blocks[c.y-r][c.x-1].on);
	if(c.y+!r<=9 && c.x>0) visible+=(blocks[c.y+!r][c.x-1].identifier==light && blocks[c.y+!r][c.x-1].on);
	if(c.y-r>=0 && c.x<12) visible+=(blocks[c.y-r][c.x+1].identifier==light && blocks[c.y-r][c.x+1].on);
	if(c.y+!r<=9 && c.x<12) visible+=(blocks[c.y+!r][c.x+1].identifier==light && blocks[c.y+!r][c.x+1].on);
	return visible;
}

bool visible_by_person(int character, struct block blocks[9][13]){
	struct coordinates c = find_character(character, blocks);
	bool visible=0;
	bool r = c.x % 2; 
	if(c.y>0) visible+=(blocks[c.y-1][c.x].identifier<8);
	if(c.y<8) visible+=(blocks[c.y+1][c.x].identifier<8);
	if(c.y-r>=0 && c.x>0) visible+=(blocks[c.y-r][c.x-1].identifier<8);
	if(c.y+!r<=9 && c.x>0) visible+=(blocks[c.y+!r][c.x-1].identifier<8);
	if(c.y-r>=0 && c.x<12) visible+=(blocks[c.y-r][c.x+1].identifier<8);
	if(c.y+!r<=9 && c.x<12) visible+=(blocks[c.y+!r][c.x+1].identifier<8);
	return visible;
}

void update_visibility(struct block blocks[9][13]){
	int i;
	struct coordinates c;
	for(i=0; i<8; i++){
		if(visible_by_light(i, blocks) || visible_by_person(i, blocks)){
			c=find_character(i, blocks);
			blocks[c.y][c.x].visible=1;
		}
	}
	return;
}

struct coordinates find_numbered_light(int no, struct block blocks[9][13]){
	int i, j;
	struct coordinates c;
	for(i=0; i<9; i++){
		for(j=0; j<13; j++){
			if(blocks[i][j].num==no){
				c.x=j;
				c.y=i;
				return c;
			}
		}
	}
	c.x=-1;
	c.y=-1;
	printf("cound not find light numbered %d\n", no);
	return c;
}
