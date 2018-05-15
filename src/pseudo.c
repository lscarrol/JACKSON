

typedef struct IF_NODE {
	struct Node* parent;
	struct Node* child;
	R_NODE* paramExp;
	R_NODE** thenExpArr;
	R_NODE** elseExpArr;
	int THEN = 0;
	int ELSE = 1;
	int sizeThen, sizeElse;
} IF_NODE;

/*
pesudocode:

struct IF_NODE {
pointer to previous intruction (parent)
pointer to if parameter exp tree root
pointer array of THEN statement expression tree roots
pointer array of ELSE statement expression tree roots
pointer to next instruction (child)

}

If block {
	if paramExp goto THEN
	else goto ELSE
	THEN:
	3 address code from thenExpArr
	ELSE:
	3 address code from elseExpArr
}
*/


//Generate stack from tree:

traverseTree(R_NODE* root) {
	if (root != NULL) {
	traverseTree(root->left)
	push(root)
	traverseTree(root->right)
}

createStackExp(R_NODE* root) {traverseTree(root);}





/*
qarr1 = malloc(sizeof(S_QUEUE*) * 100);
trees = malloc(sizeof(T_TREE*) * 100);
roots = malloc(sizeof(D_NODE*) * 100);
for (int i = 0; i < 100; i++) {
	qarr1[i] = malloc(sizeof(S_QUEUE*));
	roots[i] = malloc(sizeof(D_NODE*));
	trees[i] = malloc(sizeof(T_TREE*));
}
*/
arrblock: id lparen constant COMMA constant rparen assignOP id {eps++; inexp = true;}


expression: add_expression {}
          | binaryOperator add_expression {push(stack, $<str>1);}
          | binaryOperatorBool add_expression {push(stack, $<str>1);}
          | add_expression binaryOperator expression {push(addexps, $<str>2);}
          | add_expression binaryOperatorBool expression {push(addexps, $<str>2);}
          | add_expression postUnaryOperator {push(addexps, $<str>2);}
          ;
add_expression: constant {push(stack, $<str>1);}
              | preUnaryOperator add_expression {push(stack, $<str>1);}
              | assignable {push(stack, $<str>1);}
              | L_PARENTHESIS add_expression R_PARENTHESIS {push(stack, $<str>1); push(stack, $<str>3);}
              | L_PARENTHESIS add_expression postUnaryOperator R_PARENTHESIS {push(stack, $<str>1); push(stack, $<str>3); push(stack, $<str>4);}
