#ifndef PARANTHESES_H_
#define PARANTHESES_H_

int isBalanced(const char *str, int length){

  /* TODO: Cerinta 3
   * Implementation must use a stack.
   * Do NOT forget to deallocate the memory you use.
   */
    int ret = 1; //ret = valoarea de return: 1 - este parantezata bine, 0 - nu este ok
    
    Stack *stack = createStack();

    int len = strlen(str);
    for (int i = 0; i < len; i++) {
      if (strchr("[({", str[i])) {
        push(stack, str[i]);
      }
      else if (strchr("])}", str[i])) {
        if (isStackEmpty(stack)) {
          ret = 0;
          break;
        }
        if ((top(stack) == '(' && str[i] == ')') || 
            (top(stack) == '[' && str[i] == ']') || 
            (top(stack) == '{' && str[i] == '}')) {
            pop(stack);
        }
        else {
          ret = 0;
          break;
        }
      }
    }

    if (!isStackEmpty(stack))
      ret = 0;

    destroyStack(stack);
    return ret;
}

#endif