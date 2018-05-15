#include "genIR.h"


I_NODE* createINode() {
  I_NODE* ret = malloc(sizeof(I_NODE));
  ret->root = NULL;
  ret->lcount = 0;
  ret->next = NULL;
  return ret;
}

I_NODE* gen(D_NODE* roots[]) {
  I_NODE* man = createINode();
  man->express = "        goto Main";


  I_NODE* start = createINode();
  start->root = roots[0];
  man->next = start;





}

char* append(char* str1, char* str2) {
    char* new_str;
    new_str = malloc(strlen(str1)+strlen(str2)+1);
    new_str[0] = '\0';   // ensures the memory is an empty string

    strcat(new_str,str1);
    strcat(new_str,str2);
    return new_str;
}

char* genExp(D_NODE* root) {
  if (strcmp(root->op, "MAIN") == 0) {inMain = true; count++; return "";}
    else if (strcmp(root->op, "FUNCTION") == 0) {inFunc = true; count++; return "";}
    else if (strcmp(root->op, "FOR") == 0) {inFor = true; count++; return "";}
    else if (strcmp(root->op, "WHILE") == 0) {inWhile = true; count++; return "";}
    else if (strcmp(root->op, "SWITCH") == 0) {inSwitch = true; count++; return "";}
    else if (strcmp(root->op, "OTHERWISE") == 0) {scount = 0; inSwitch = false; inOtherWise = true; count++; return "";}
    else if (strcmp(root->op, "IF") == 0) {scount = 0; inIf = true; count++; return "";}
    else if (strcmp(root->op, "THEN") == 0) {inThen = true; count++; return "";}
    else if (strcmp(root->op, "ELSE") == 0) {inElse = true; inThen = false; count++; return "";}
    else if (strcmp(root->op, "WHILE") == 0) {scount = 0; inWhile = true; count++; return "";}
    else if (strcmp(root->op, "END_FUNC") == 0) {inFunc = false; funcc = 0; count++; return append("\treturn ", funcadder);}
    else if (strcmp(root->op, "END_FOR") == 0) {inFor = false; count++; return append(foradder, "\n\tgoto L1\nL2:\n");}
    else if (strcmp(root->op, "END_SWITCH") == 0) {inSwitch = false; inOtherWise = false; count++; return "";}
    else if (strcmp(root->op, "END_IF") == 0) {inIf = false; inElse = false; inThen = false; count++; return "";}
    else if (strcmp(root->op, "END_WHILE") == 0) {inWhile = false; count++; return "L1:\n";}
    else if (strcmp(root->op, "END_MAIN") == 0) {return "exit\n";}
    else if (strcmp(root->op, "reserve") == 0) { count++;
      if (root->left != NULL) {char* x = append("\t", append(root->left->op, " = malloc(16)\n"));
      char* arr = append("\t", root->left->op);
      char* paras = simplify(root->right, "");
      int a = strlen(paras);
      char* arr3 = "";
      char* arr4 = "";
      char* arr5 = "";
      char* pchar = malloc(sizeof(char));
      *pchar = paras[0];
      char* arr2 = append(append(arr, "[0] = "), pchar);
      if (a > 12) {
        arr3 = append("\n", append(append(arr, "[4] = "), pchar));
        if (a > 16) {
          arr4 = append("\n", append(append(arr, "[8] = "), pchar));
        } if (a > 24) {
          arr5 = append("\n", append(append(arr, "[12] = "), pchar));
        }
      }
      return append(append(append(append(x, arr2), arr3), arr4), arr5);
    } else {return append("\t", append(root->right->op, " = malloc(16)"));}

    } else {
      if (inFunc) {
        if (funcc == 0) {
          funcc++;
          count++;
          return append("func ", append(root->op, ":"));
        } else { funcadder = simplify(root, ""); count++; return append("\t", funcadder);}
      } else if (inMain) {
        if (mainco == 0) {
          mainco++; count++; inMain = false; return append("Main:\t", simplify(root, ""));
        } else {
          return append("\t", simplify(root, ""));
        }
      } else if (inFor) {
        char* temp;
        if (forcount == 0) {
          count++;
          forcount++;
          return append("\t", simplify(root, ""));
        } else if (forcount == 1) {
          count++;
          forcount++;
          if (strcmp(root->op, "<") == 0) {
            char* q = append(append(root->left->op, " >= "), root->right->op);
            return append(append("L1:\tif ", q), " goto L2");
          } else {
            char* q = append(append(root->left->op, " <= "), root->right->op);
            return append(append("L1:\tif ", q), " goto L2");
          }
          return append("L1:\tif NOT ", simplify(root, ""));
        } else if (forcount == 2) {
          forcount++;
          foradder = append("\t", simplify(root, ""));
        } else {
          return append("\t", simplify(root, ""));
        }
      }
      else if (inWhile) {
        if (wcount == 0) {
          wcount++;
          return append(append("W0:\n\tif ", simplify(root, "")), " goto W1");
        } else {
          return append("\t", simplify(root, ""));
        }
      } else if (inIf) {
        if (scount == 0) {
          scount++;
          return append(append("\tif  ", simplify(root, "")), " goto F1 else goto F2");
        } else {
          if (inThen) {
            return append("F1: ", append("\t", simplify(root, "")));
          } else if (inElse) {
            return append("F2: ", append("\t", simplify(root, "")));
          }
        }
      } else if (inSwitch) {
        if (scount == 0) {
          scount++;
          return append("\t", simplify(root, ""));
        } else if (scount == 1) {
          scount++;
          return append(append("\tif  ", simplify(root, "")), " goto S1 else got O1");
        } else if (scount == 2) {
          scount++;
          return append("S1: ", append("\t", simplify(root, "")));
        } else {
          return append("\t", simplify(root, ""));
        }
      } else if (inOtherWise) {
        if (scount == 0) {
          scount++;
          return append("O1: ", append("\t", simplify(root, "")));}
          else {return append("\t", simplify(root, ""));}
    }

  }
  return append("\t", simplify(root, ""));
}


char* simplify(D_NODE* root, char* currs) {
  if (root != NULL) {
    if (inco == 3) {
      if (strcmp(currs, "") != 0) {
        char* a = malloc(16);
        snprintf(a, 16, "%d", lcount);
        char* b = append("t", a);
        currs = append(currs, b);
        currs = append(currs, "\n\t");
        currs = append(currs, b);
        currs = append(currs, " = ");
        lcount++;
        inco = 0;
      } else {
        inco = 0;
      }
    }
    inco++;
    currs = simplify(root->left, currs);
    if (strcmp(root->op,":=") == 0) {
      currs = append(currs, "= ");
    } else {
      currs = append(append(currs, root->op), " ");
    }
    currs = simplify(root->right, currs);
  }
  return currs;
}
