#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "utils/vector.h"

void set_at(struct _vector* vec, unsigned long long index, void* val) {
    assert(index <= vec->capacity);
    unsigned long long addr = (unsigned long long)vec->data; 
    addr += index; 
    memcpy((void*)addr, val, vec->stride);
}

void*get_at(struct _vector* vec, unsigned long long index) {
    assert(index <= vec->capacity); 
    unsigned long long addr = (unsigned long long)vec->data; 
    return (void*)addr;
}

void push_back(struct _vector* vec, const void* data);
void*pop_back(struct _vector* vec);


void insert_at(struct _vector* vec, const void* data, unsigned long long index);
void*remove_at(struct _vector* vec, unsigned long long index);


void clear(struct _vector* vec);

void resize(struct _vector* vec);

// adds passed length too the current capacity
void reserve(struct _vector* vec, unsigned long long capacity);

vector _vector_create(unsigned long long capacity, unsigned long long stride) {
    vector v;
    v.capacity = capacity;
    v.length = 0;
    v.stride = stride;
    v.data = malloc(capacity * stride);
    v.clear = clear;
    v.insert_at = insert_at;
    v.push_back = push_back;
    v.pop_back = pop_back;
    v.resize = resize;
    v.reserve = reserve;
    v.set_at = set_at;
    v.get_at = get_at;
    v.remove_at = remove_at;
    return v;
}

vector* _vector_create_ptr(unsigned long long capacity, unsigned long long stride) {
    vector* v = malloc(sizeof(vector));
    assert(v != NULL);
    *v = _vector_create(capacity, stride);
    v->is_struct_ptr = true;
    return v;
}

void vector_destroy(vector* vec) {
    free(vec->data);
    vec->data = NULL;
    memset(vec, 0, sizeof(vector));
    if(vec->is_struct_ptr) free(vec);
}

void push_back(struct _vector* vec, const void* data) {
    if (vec->length == vec->capacity) {
        reserve(vec, vec->capacity);
    }
    unsigned long long addr = (unsigned long long)vec->data;
    addr += (vec->length * vec->stride);
    vec->length++;
    memcpy((void*)addr, data, vec->stride);
}

void* pop_back(struct _vector* vec) {
    if (vec->length == 0) {
        return NULL; // Return NULL if the vector is empty
    }
    vec->length--;
    unsigned long long addr = (unsigned long long)vec->data;
    addr += (vec->length * vec->stride);
    void* ret = malloc(vec->stride);
    assert(ret != NULL);
    memcpy(ret, (void*)addr, vec->stride);
    return ret;
}

void insert_at(struct _vector* vec, const void* data, unsigned long long index) {
    assert(index <= vec->length);
    if (vec->length == vec->capacity) {
        reserve(vec, vec->capacity + 1);
    }
    memmove((char*)vec->data + (index + 1) * vec->stride, (char*)vec->data + index * vec->stride, (vec->length - index) * vec->stride);
    memcpy((char*)vec->data + index * vec->stride, data, vec->stride);
    vec->length++;
}

void* remove_at(struct _vector* vec, unsigned long long index) {
    assert(index < vec->length);
    void* ret = malloc(vec->stride);
    assert(ret != NULL);
    memcpy(ret, (char*)vec->data + index * vec->stride, vec->stride);
    memmove((char*)vec->data + index * vec->stride, (char*)vec->data + (index + 1) * vec->stride, (vec->length - index - 1) * vec->stride);
    vec->length--;
    return ret;
}

void clear(struct _vector* vec) {
    memset(vec->data, 0, vec->capacity * vec->stride);
}

void resize(struct _vector* vec) {
    vec->data = realloc(vec->data, vec->length * vec->stride);
    assert(vec->data != NULL);
    vec->capacity = vec->length;
}

// adds passed length to the current capacity
void reserve(struct _vector* vec, unsigned long long capacity) {
    void* temp_data = malloc((vec->capacity + capacity) * vec->stride);
    assert(temp_data != NULL);
    memcpy(temp_data, vec->data, vec->capacity * vec->stride);
    free(vec->data);
    vec->data = temp_data;
    vec->capacity += capacity;
}