#ifndef _SKLC_UTILS_VECTOR_H_
#define _SKLC_UTILS_VECTOR_H_

#include <defines.h>

// Used by the defines, DONT'USE IT BY YOURSELF
typedef struct _vector { 

    void* data;                 // Vector's data

    // WARNING: DO NOT MANUALLY CHANGE ANYTHING BELOW!!!
    u64 stride;  // Size of the element
    u64 length;  // Number of elements currently used
    u64 capacity;// How much items can it hold before reallocating;

    void (*push_back)(struct _vector* vec, const void* data);
    void* (*pop_back)(struct _vector* vec);

    void (*set_at)(struct _vector* vec, u64 index, void* val);
    void*(*get_at)(struct _vector* vec, u64 index);

    void (*insert_at)(struct _vector* vec, const void* data, u64 index);
    void*(*remove_at)(struct _vector* vec, u64 index);

    void (*clear)(struct _vector* vec);

    void (*resize)(struct _vector* vec);

    // adds passed length to the current capacity
    
    void (*reserve)(struct _vector* vec, u64 capacity);

    bool is_struct_ptr;
    
} vector;


vector _vector_create(u64 capacity, u64 stride);

// Use thes ONLY when you want to create an allocated vector struct or an array of vectors
vector* _vector_create_ptr(u64 capacity, u64 stride);
void vector_destroy(vector* vec);

#define vector_create(type) _vector_create(1, sizeof(type))
#define vector_create_ptr(type) _vector_create_ptr(1, sizeof(type))

#endif//_SKLC_UTILS_VECTOR_H_