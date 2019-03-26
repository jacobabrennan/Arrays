#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
    int capacity;  // How many elements can this array hold?
    int count;  // How many states does the array currently hold?
    char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
    // Allocate memory for the Array struct
    Array *array_new = malloc(sizeof(Array));
    // Set initial values for capacity and count
    array_new->capacity = capacity;
    array_new->count = 0;
    // Allocate memory for elements
    array_new->elements = calloc(capacity, sizeof(char*));
    //
    return array_new;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {
    // Free all elements
    for(int index=0; index < arr->count; index++)
    {
        free(arr->elements[index]);
    }
    // Free array
    free(arr->elements);
    free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {
    // Create a new element storage with double capacity
    int capacity_new = arr->capacity * 2;
    char **elements_new = malloc(sizeof(char *) * capacity_new);
    // Copy elements into the new storage
    for(int index=0; index < arr->count; index++)
    {
        elements_new[index] = arr->elements[index];
    }
    // Free the old elements array (but NOT the strings they point to)
    free(arr->elements);
    // Update the elements and capacity to new values
    arr->elements = elements_new;
    arr->capacity = capacity_new;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {
    // Throw an error if the index is greater than the current count
    if(index >= arr->count)
    {
        // NOTE: Inconsistent specification.
        // The docs specify throwing an error, but the tests require a return
        // value of NULL. I've implemented the behavior specified by the tests.
        //fprintf(stderr, "Array index out of range.\n");
        //exit(1);
        return NULL;
    }
    // Otherwise, return the element at the given index
    return arr->elements[index];
}


/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index) {
    // Throw an error if the index is greater than the current count
    if(index >= arr->count)
    {
        fprintf(stderr, "Array index out of range.\n");
        exit(1);
    }
    // Resize the array if the number of elements is over capacity
    if(arr->count >= arr->capacity)
    {
        resize_array(arr);
    }
    // Move every element after the insert index to the right one position
    for(int index_shift=arr->count; index_shift > index; index_shift--)
    {
        arr->elements[index_shift] = arr->elements[index_shift-1];
    }
    // Copy the element and add it to the array
    arr->elements[index] = strdup(element);
    // Increment count by 1
    arr->count++;
    arr->elements[arr->count] = NULL;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {
    // Resize the array if the number of elements is over capacity
    if(arr->count >= arr->capacity)
    {
        resize_array(arr);
    }
    // Copy the element and add it to the end of the array
    arr->elements[arr->count] = strdup(element);
    // Increment count by 1
    arr->count++;
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {
    // Search for the first occurence of the element and remove it.
    int found = 0;
    for(int index=0; index < arr->count; index++)
    {
        // Shift over every element after the removed element to the left one position
        if(found)
        {
            arr->elements[index-1] = arr->elements[index];
            continue;
        }
        // Don't forget to free its memory!
        if(strcmp(arr->elements[index], element) == 0)
        {
            found = 1;
            free(arr->elements[index]);
        }
    }
    // Throw an error if the value is not found.
    if(!found)
    {
        fprintf(stderr, "Element not in array.\n");
        exit(1);
    }
    // Decrement count by 1
    arr->count--;
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
