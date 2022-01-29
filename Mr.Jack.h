//linked list functions:

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

//game functions:

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
	if(block.sus && block.identifier<street)
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
			printf("\n%cy", 25);
			for(j=0; j<6; j++){
			printf("        ______");
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

bool MS_check(int i, int character, bool SG){
	bool output;
	if(character==MS && !SG){
		output = (i!=extra);
	}else{
		output = (i<light);
	}
	return output;
}

struct availability check_around(int x, int y, struct block blocks[9][13], int character, bool SG){
	struct availability av;
	av.d=0; av.dl=0; av.dr=0; av.u=0; av.ul=0; av.ur=0;
	int i;
	bool b, r = x%2;
	b = (y!=0);													//check if we can go up
	if(b){
		i = blocks[y-1][x].identifier;
		if(MS_check(i, character, SG)){
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
		if(MS_check(i, character, SG)){
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
		if(MS_check(i, character, SG)){
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
		if(MS_check(i, character, SG)){
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
		if(MS_check(i, character, SG)){
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
		if(MS_check(i, character, SG)){
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

int print_available_directions(struct availability av, int x, int y, bool must, struct block blocks[9][13], bool SG){
	int en=0;
	if(!SG && blocks[y][x].well && blocks[y][x].blocked==0){
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

int move_sub_function(struct block blocks[9][13], struct coordinates c, struct coordinates next, int *n, int *counter, int Jack, bool detective){
	struct character_info info;
	int choice;
	if(!detective && blocks[next.y][next.x].identifier==esc){
		switch(*counter){
			case 0:
				if(blocks[c.y][c.x].identifier==Jack && blocks[c.y][c.x].visible==0){
					printf("Mr.Jack wins by escaping the city, try better next time detective :)\n");
					getch();
					exit(EXIT_SUCCESS);
				}else{
					printf("This character can't escape!\n");
					return 1;
				}
			break;
			default:
				if(blocks[c.y][c.x].temp.identifier==Jack && blocks[c.y][c.x].temp.visible==0){
					printf("Mr.Jack wins by escaping, try better next time detective :)\n");
					getch();
					exit(EXIT_SUCCESS);
				}else{
					printf("This character can't escape!\n");
					return 1;
				}
			break;	
		}
	}
	if(detective && blocks[next.y][next.x].identifier<street){
		printf("do you want to capture this character?  0.no  1.yes\n");
		scanf(" %d", &choice);
		checker(0, 1, &choice);
		if(choice){
			if(blocks[next.y][next.x].identifier==Jack){
				printf("Detective won! Jack was captured");
				getch();
				exit(EXIT_SUCCESS);
			}else{
				printf("Jack won! Detective captured a poor innocent!");
				getch();
				exit(EXIT_SUCCESS);
			}
		}
	}
	if(*n - *counter==1){
		printf("You will run out of moves and are not allowed to stay there, try again!\n");
		return -1;
	}
	if(*counter==0){
		info.identifier=blocks[c.y][c.x].identifier;
		info.sus=blocks[c.y][c.x].sus;
		info.visible=blocks[c.y][c.x].visible;
		info.direction=blocks[c.y][c.x].direction;
	}else info=blocks[c.y][c.x].temp;
	*counter+=1;
	blocks[next.y][next.x].temp=info;
	return 0;
}

int move(struct coordinates c, struct block blocks[9][13], bool must, int *n, int *counter, int Jack, bool detective, bool SG){
	//printf("n=%d counter=%d\n", *n, *counter);
	if(*counter<*n){
	struct availability av;
	if(*counter==0) av = check_around(c.x, c.y, blocks, blocks[c.y][c.x].identifier, SG);
	else av = check_around(c.x, c.y, blocks, blocks[c.y][c.x].temp.identifier, SG);
	int choice, flag=1, x, y, character, en;
	if(*counter==0){
		character=blocks[c.y][c.x].identifier;
	} else{
		character=blocks[c.y][c.x].temp.identifier;
	}
	printf("moving character: ");
	print_character_name(character);
	printf("\n");
	bool r = c.x % 2;
	struct coordinates next;
	printf("available moves: ");
	en = print_available_directions(av, c.x, c.y, must, blocks, SG);
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
						switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
							case 1: return 1; break;
							case -1: continue; break;
						}
						if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
						switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
							case 1: return 1; break;
							case -1: continue; break;
						}
						if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
						switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
							case 1: return 1; break;
							case -1: continue; break;
						}
						if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
						switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
							case 1: return 1; break;
							case -1: continue; break;
						}
						if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
						switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
							case 1: return 1; break;
							case -1: continue; break;
						}
						if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
						switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
							case 1: return 1; break;
							case -1: continue; break;
						}
						if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
				if(SG){
					printf("You can't use wells when moving characters through SG's ability!\n");
					flag=0;
					getch();
					break;
				}
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
							switch(move_sub_function(blocks, c, next, n, counter, Jack, detective)){
								case 1: return 1; break;
								case -1: continue; break;
							}
							if(move(next, blocks, 1, n, counter, Jack, detective, SG)!=-1){
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
					else printf("You must at least make 1 move!\n");
			break;
			default:
				printf("invalid input!\n");
			break;
		}
	}
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
		move(c, blocks, 1, n, counter, Jack, detective, 0);
		update_screen(blocks);
		while(*n>0){
			c=find_character(character, blocks);
			move(c, blocks, 0, n, counter, Jack, detective, 0);
			update_screen(blocks);
		}
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

void update_visiblity_by_JW_sub_function(int direction, struct coordinates c, struct block blocks[9][13]){
	struct coordinates next;
	bool r = c.x % 2;
	switch(direction){
		case up:
			next.x=c.x;
			next.y=c.y - 1;
			if(blocks[next.y][next.x].identifier>street){
				return;
			}else{
				blocks[next.y][next.x].visible=1;
				update_visiblity_by_JW_sub_function(up, next, blocks);	
			}
		break;
		case up_right:
			next.x=c.x + 1;
			next.y=c.y - r;
			if(blocks[next.y][next.x].identifier>street){
				return;
			}else{
				blocks[next.y][next.x].visible=1;
				update_visiblity_by_JW_sub_function(up_right, next, blocks);	
			}
		break;
		case up_left:
			next.x=c.x - 1;
			next.y=c.y - r;
			if(blocks[next.y][next.x].identifier>street){
				return;
			}else{
				blocks[next.y][next.x].visible=1;
				update_visiblity_by_JW_sub_function(up_left, next, blocks);	
			}
		break;
		case down:
			next.x=c.x;
			next.y=c.y + 1;
			if(blocks[next.y][next.x].identifier>street){
				return;
			}else{
				blocks[next.y][next.x].visible=1;
				update_visiblity_by_JW_sub_function(down, next, blocks);	
			}
		break;
		case down_right:
			next.x=c.x + 1;
			next.y=c.y + !r;
			if(blocks[next.y][next.x].identifier>street){
				return;
			}else{
				blocks[next.y][next.x].visible=1;
				update_visiblity_by_JW_sub_function(down_right, next, blocks);	
			}
		break;
		case down_left:
			next.x=c.x - 1;
			next.y=c.y + !r;
			if(blocks[next.y][next.x].identifier>street){
				return;
			}else{
				blocks[next.y][next.x].visible=1;
				update_visiblity_by_JW_sub_function(down_left, next, blocks);	
			}
		break;
		default:
			printf("bug in update_visiblity_by_JW_sub_function\n");
		break; 
	}
	return;
}

void update_visiblity_by_JW(struct block blocks[9][13]){
	struct coordinates c;
	c=find_character(JW, blocks);
	switch(blocks[c.y][c.x].direction){
		case up:
			update_visiblity_by_JW_sub_function(up, c, blocks);
		break;
		case up_right:
			update_visiblity_by_JW_sub_function(up_right, c, blocks);
		break;
		case up_left:
			update_visiblity_by_JW_sub_function(up_left, c, blocks);
		break;
		case down:
			update_visiblity_by_JW_sub_function(down, c, blocks);
		break;
		case down_right:
			update_visiblity_by_JW_sub_function(down_right, c, blocks);
		break;
		case down_left:
			update_visiblity_by_JW_sub_function(down_left, c, blocks);
		break;
		default:
			printf("bug in update_visiblity_by_JW\n");
		break;
	}
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

void swap_block_abilities(int character, int x1, int y1, int x2, int y2, struct block blocks[9][13]){
	int temp;
	switch(character){
		case JS:
			blocks[y1][x1].on=0;
			blocks[y2][x2].on=1;
			temp=blocks[y1][x1].num;
			blocks[y1][x1].num=blocks[y2][x2].num;
			blocks[y2][x2].num=temp;
		break;
		case IL:
			blocks[y1][x1].blocked=0;
			blocks[y2][x2].blocked=1;
		break;
		case JB:
			blocks[y1][x1].blocked=0;
			blocks[y2][x2].blocked=1;
		break;
		default:
			printf("swap_block_abilities bug\n");
		break;
	}
	return;
}

int convert_name_to_identifier(char name[]){
	int output=-1;
	if(strcmpi("SH", name)==0) output=SH;
	if(strcmpi("JW", name)==0) output=JW;
	if(strcmpi("JS", name)==0) output=JS;
	if(strcmpi("IL", name)==0) output=IL;
	if(strcmpi("MS", name)==0) output=MS;
	if(strcmpi("SG", name)==0) output=SG;
	if(strcmpi("WG", name)==0) output=WG;
	if(strcmpi("JB", name)==0) output=JB;
	return output;
}

void ability(int character, struct block blocks[9][13], int Jack, int detective){
	int x1, y1, x2, y2, flag=1, choice, n=3, counter=0;
	struct coordinates c, c2;
	char name[20];
	while(flag)
	switch(character){
		case JS:
			printf("Enter the x,y of the lit gaslight you want to swap: ");
			scanf(" %d %d", &x1, &y1);
			checker(1, 13, &x1);
			checker(1, 9, &y1);
			if(blocks[y1-1][x1-1].identifier==light && blocks[y1-1][x1-1].on){
				printf("Enter the x,y of the unlit gaslight you want to swap with: ");
				scanf(" %d %d", &x2, &y2);
				checker(1, 13, &x2);
				checker(1, 9, &y2);
				if(blocks[y2-1][x2-1].identifier==light && blocks[y2-1][x2-1].on==0){
					swap_block_abilities(JS, x1-1, y1-1, x2-1, y2-1, blocks);
					flag=0;
				} else{
					("Invalid input! try again\n");
				}
			} else {
				printf("Invalid input! try again\n");
			}
		break;
		case IL:
			printf("Enter the x,y of the cordon you want to move: ");
			scanf(" %d %d", &x1, &y1);
			checker(1, 13, &x1);
			checker(1, 9, &y1);
			if(blocks[y1-1][x1-1].identifier==esc && blocks[y1-1][x1-1].blocked){
				printf("Enter the x,y of the exit you want to block: ");
				scanf(" %d %d", &x2, &y2);
				checker(1, 13, &x2);
				checker(1, 9, &y2);
				if(blocks[y2-1][x2-1].identifier==esc && blocks[y2-1][x2-1].blocked==0){
					printf("exc\n");
					swap_block_abilities(IL, x1-1, y1-1, x2-1, y2-1, blocks);
					flag=0;
				} else{
					printf("Invalid input2! try again\n");
				}
			} else {
				printf("Invalid input1! try again\n");
			}
		break;
		case JB:
			printf("Enter the x,y of the manhole you want to open: ");
			scanf(" %d %d", &x1, &y1);
			checker(1, 13, &x1);
			checker(1, 9, &y1);
			if(blocks[y1-1][x1-1].well && blocks[y1-1][x1-1].blocked){
				printf("Enter the x,y of the manhole you want to close: ");
				scanf(" %d %d", &x2, &y2);
				checker(1, 13, &x2);
				checker(1, 9, &y2);
				if(blocks[y2-1][x2-1].well && blocks[y2-1][x2-1].blocked==0){
					swap_block_abilities(IL, x1-1, y1-1, x2-1, y2-1, blocks);
					flag=0;
				} else{
					("Invalid input! try again\n");
				}
			} else {
				printf("Invalid input! try again\n");
			}
		break;
		case SG:
			while(n>0){
				printf("Who do you want move? ");
				scanf(" %s", name);
				choice=convert_name_to_identifier(name);
				if(choice==-1){
					printf("Invalid input! try again\n");
				}else if(choice==SG){
					printf("You can't move SG himself with his ability!\n");
				}else{
					c=find_character(choice, blocks);
					move(c, blocks, 1, &n, &counter, Jack, detective, 1);
					update_screen(blocks);
				}
			}
			flag=0;
		break;
		case WG:
			printf("Who do you want to swap with? ");
			scanf(" %s", name);
			choice=convert_name_to_identifier(name);
			if(choice==-1){
				printf("Invalid input! try again\n");
			}else if(choice==WG){
				printf("You can't swap WG with himself with his ability!\n");
			}else{
				c=find_character(WG, blocks);
				c2=find_character(choice, blocks);
				
				struct character_info temp;
				temp.identifier=blocks[c.y][c.x].identifier;
				temp.direction=blocks[c.y][c.x].direction;
				temp.sus=blocks[c.y][c.x].sus;
				temp.visible=blocks[c.y][c.x].visible;
				
				blocks[c.y][c.x].identifier=blocks[c2.y][c2.x].identifier;
				blocks[c.y][c.x].direction=blocks[c2.y][c2.x].direction;
				blocks[c.y][c.x].sus=blocks[c2.y][c2.x].sus;
				blocks[c.y][c.x].visible=blocks[c2.y][c2.x].visible;
				
				blocks[c2.y][c2.x].identifier=temp.identifier;
				blocks[c2.y][c2.x].direction=temp.direction;
				blocks[c2.y][c2.x].sus=temp.sus;
				blocks[c2.y][c2.x].visible=temp.visible;
				flag=0;
			}
		break;
		default:
			printf("Error in ability!\n");
			flag=0;
		break;
	}
	update_screen(blocks);
	return;
}

void JW_ability(struct block blocks[9][13]){
	int choice, flag;
	struct coordinates c;
	printf("%d: %c   ", up, 24);
	printf("%d: %c   ", down, 25);
	printf("%d: %c(   ", up_right, 24);
	printf("%d: )%c   ", up_left, 24);
	printf("%d: %c(   ", down_right, 25);
	printf("%d: )%c   ", down_left, 25);
	do{
		scanf(" %d", &choice);
		switch(choice){
			case up:
			c=find_character(JW, blocks);
			blocks[c.y][c.x].direction=up;	
			flag=0;
			break;
			case up_right:
			c=find_character(JW, blocks);
			blocks[c.y][c.x].direction=up_right;
			flag=0;
			break;
			case up_left:
			c=find_character(JW, blocks);
			blocks[c.y][c.x].direction=up_left;
			flag=0;
			break;
			case down:
			c=find_character(JW, blocks);
			blocks[c.y][c.x].direction=down;	
			flag=0;
			break;
			case down_right:
			c=find_character(JW, blocks);
			blocks[c.y][c.x].direction=down_right;	
			flag=0;
			break;
			case down_left:
			c=find_character(JW, blocks);
			blocks[c.y][c.x].direction=down_left;	
			flag=0;
			break;
			default:
				printf("Invalid input! try again:\n");
				flag=1;
			break;
		}
	} while(flag);
	update_screen(blocks);
	return;
}

void save_log(struct block blocks[9][13], struct block logs[][9][13], int *logs_index){
	int i, j;
	for(i=0; i<9; i++){
		for(j=0; j<13; j++){
			logs[*logs_index][i][j]=blocks[i][j];
		}
	}
	*logs_index+=1;
}

void play(int character, struct block blocks[9][13], int *n, int *counter, int Jack, int detective, struct node ** innocent_list, struct block logs[][9][13], int *logs_index){
	struct coordinates c = find_character(character, blocks);
	int innocent_character, choice;
	switch(character){
		case SH:
			move_character(character, c, blocks, n, counter, Jack, detective);
			printf("Ability: draw a card from the innocents' list:\n");
			printf("non_playing player should look away here(press any key)\n");
			getch();
			if(innocent_list!=NULL){
				innocent_character=get(*innocent_list, 0);
				remove_node(innocent_list, 0);
				print_character_name(innocent_character);
				printf(" is innocent\n");
				getch();
				update_screen(blocks);
			} else{
				printf("The innocents have already all been deduced by Sherlock!\n");
				getch();
			}
			
		break;
		case JW:
			move_character(character, c, blocks, n, counter, Jack, detective);
			printf("Set the lantern's direction: ");
			JW_ability(blocks);
		break;
		case JS:
			printf("Do you want to use your ability 1.now 2.after movement?\n");
			scanf(" %d", &choice);
			checker(1, 2, &choice);
			if(choice==1){
				ability(JS, blocks, Jack, detective);
				move_character(character, c, blocks, n, counter, Jack, detective);
			}else{
				move_character(character, c, blocks, n, counter, Jack, detective);
				ability(JS, blocks, Jack, detective);
			}
		break;
		case IL:
			printf("Do you want to use your ability 1.now 2.after movement?\n");
			scanf(" %d", &choice);
			checker(1, 2, &choice);
			if(choice==1){
				ability(IL, blocks, Jack, detective);
				move_character(character, c, blocks, n, counter, Jack, detective);
			}else{
				move_character(character, c, blocks, n, counter, Jack, detective);
				ability(IL, blocks, Jack, detective);
			}
		break;
		case JB:
			printf("Do you want to use your ability 1.now 2.after movement?\n");
			scanf(" %d", &choice);
			checker(1, 2, &choice);
			if(choice==1){
				ability(JB, blocks, Jack, detective);
				move_character(character, c, blocks, n, counter, Jack, detective);
			}else{
				move_character(character, c, blocks, n, counter, Jack, detective);
				ability(JB, blocks, Jack, detective);
			}
		break;
		case SG:
			printf("Do you want to use your ability 1.now 2.after movement?\n");
			scanf(" %d", &choice);
			checker(1, 2, &choice);
			if(choice==1){
				ability(SG, blocks, Jack, detective);
				move_character(character, c, blocks, n, counter, Jack, detective);
			}else{
				move_character(character, c, blocks, n, counter, Jack, detective);
				ability(SG, blocks, Jack, detective);
			}
		break;
		case MS:
			move_character(character, c, blocks, n, counter, Jack, detective);
		break;
		case WG:
			printf("Do you want to 1.use your ability OR 2.do your movement?\n");
			scanf(" %d", &choice);
			checker(1, 2, &choice);
			if(choice==1){
				ability(WG, blocks, Jack, detective);
			}else{
				move_character(character, c, blocks, n, counter, Jack, detective);
			}
		break;
		default:
			printf("Error in play!\n");
		break;
	}
	save_log(blocks, logs, logs_index);
	return;
}
