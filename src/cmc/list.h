/**
 * cmc/list.h
 *
 * Creation Date: 12/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * List
 *
 * A List is a Dynamic Array that store its elements contiguously. It has an
 * internal buffer that is pre-allocated with a greater capacity than necessary
 * in order to foresee new elements being added. The items can be added at both
 * ends of the list and at the middle (with a given index).
 *
 * Insertions at the start are more costly as it is necessary to shift all
 * elements one position to the right to give space to a new element being added
 * at index 0. Insertions at the end are instantaneous as long as there is
 * enough space in the buffer. Insertions at the middle will be based on an
 * index. The index represents where the element being added will be located
 * once the operation ends. This operation might also need to shift elements. It
 * is also possible to add arrays of elements in the list.
 *
 * When the buffer is filled, it is reallocated with a greater capacity, usually
 * being doubled.
 *
 * Removing elements follows the same principle. Removing the front element will
 * require to shift all other elements one position to the left, thus being
 * slower than removing from the end in which is done in constant time. Removing
 * elements in the middle of the list will also require shifting elements to the
 * left. Is is also possible to remove a range of elements or extract them,
 * creating a new list with the removed items.
 *
 * The iterator is a simple structure that is capable of going back and
 * forwards. Any modifications to the target list during iteration is considered
 * undefined behavior. Its sole purpose is to facilitate navigation through a
 * list.
 */

#ifndef CMC_CMC_LIST_H
#define CMC_CMC_LIST_H

/* -------------------------------------------------------------------------
 * Core functionalities of the C Macro Collections Library
 * ------------------------------------------------------------------------- */
#include "../cor/core.h"

/* -------------------------------------------------------------------------
 * List specific
 * ------------------------------------------------------------------------- */
/* to_string format */
static const char *cmc_cmc_string_fmt_list = "struct %s<%s> "
                                             "at %p { "
                                             "buffer:%p, "
                                             "capacity:%" PRIuMAX ", "
                                             "count:%" PRIuMAX ", "
                                             "flag:%d, "
                                             "f_val:%p, "
                                             "alloc:%p, "
                                             "callbacks:%p }";

/**
 * Core List implementation
 */
#define CMC_CMC_LIST_CORE(PARAMS)    \
    CMC_CMC_LIST_CORE_HEADER(PARAMS) \
    CMC_CMC_LIST_CORE_SOURCE(PARAMS)

#define CMC_CMC_LIST_CORE_HEADER(PARAMS)                                      \
    CMC_CMC_LIST_CORE_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), \
                              CMC_PARAM_V(PARAMS))

#define CMC_CMC_LIST_CORE_SOURCE(PARAMS)                                      \
    CMC_CMC_LIST_CORE_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), \
                              CMC_PARAM_V(PARAMS))

/* -------------------------------------------------------------------------
 * Header
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LIST_CORE_HEADER_(PFX, SNAME, V)                              \
                                                                              \
    /* List Structure */                                                      \
    struct SNAME                                                              \
    {                                                                         \
        /* Dynamic array of elements */                                       \
        V *buffer;                                                            \
                                                                              \
        /* Current array capacity */                                          \
        size_t capacity;                                                      \
                                                                              \
        /* Current amount of elements */                                      \
        size_t count;                                                         \
                                                                              \
        /* Flags indicating errors or success */                              \
        int flag;                                                             \
                                                                              \
        /* Value function table */                                            \
        struct CMC_DEF_FVAL(SNAME) * f_val;                                   \
                                                                              \
        /* Custom allocation functions */                                     \
        struct cmc_alloc_node *alloc;                                         \
                                                                              \
        /* Custom callback functions */                                       \
        struct cmc_callbacks *callbacks;                                      \
    };                                                                        \
                                                                              \
    /* Value struct function table */                                         \
    struct CMC_DEF_FVAL(SNAME)                                                \
    {                                                                         \
        /* Comparator function */                                             \
        CMC_DEF_FTAB_CMP(V);                                                  \
                                                                              \
        /* Copy function */                                                   \
        CMC_DEF_FTAB_CPY(V);                                                  \
                                                                              \
        /* To string function */                                              \
        CMC_DEF_FTAB_STR(V);                                                  \
                                                                              \
        /* Free from memory function */                                       \
        CMC_DEF_FTAB_FREE(V);                                                 \
                                                                              \
        /* Hash function */                                                   \
        CMC_DEF_FTAB_HASH(V);                                                 \
                                                                              \
        /* Priority function */                                               \
        CMC_DEF_FTAB_PRI(V);                                                  \
    };                                                                        \
                                                                              \
    /* List Iterator */                                                       \
    struct CMC_DEF_ITER(SNAME)                                                \
    {                                                                         \
        /* Target list */                                                     \
        struct SNAME *target;                                                 \
                                                                              \
        /* Cursor's position (index) */                                       \
        size_t cursor;                                                        \
                                                                              \
        /* If the iterator has reached the start of the iteration */          \
        bool start;                                                           \
                                                                              \
        /* If the iterator has reached the end of the iteration */            \
        bool end;                                                             \
    };                                                                        \
                                                                              \
    /* Collection Functions */                                                \
    /* Collection Allocation and Deallocation */                              \
    struct SNAME *CMC_(PFX, _new)(size_t capacity,                            \
                                  struct CMC_DEF_FVAL(SNAME) * f_val);        \
    struct SNAME *CMC_(PFX, _new_custom)(                                     \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,                  \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks);     \
    void CMC_(PFX, _clear)(struct SNAME * _list_);                            \
    void CMC_(PFX, _free)(struct SNAME * _list_);                             \
    /* Customization of Allocation and Callbacks */                           \
    void CMC_(PFX, _customize)(struct SNAME * _list_,                         \
                               struct cmc_alloc_node * alloc,                 \
                               struct cmc_callbacks * callbacks);             \
    /* Collection Input and Output */                                         \
    bool CMC_(PFX, _push_front)(struct SNAME * _list_, V value);              \
    bool CMC_(PFX, _push_at)(struct SNAME * _list_, V value, size_t index);   \
    bool CMC_(PFX, _push_back)(struct SNAME * _list_, V value);               \
    bool CMC_(PFX, _pop_front)(struct SNAME * _list_);                        \
    bool CMC_(PFX, _pop_at)(struct SNAME * _list_, size_t index);             \
    bool CMC_(PFX, _pop_back)(struct SNAME * _list_);                         \
    /* Collection Sequence Input and Output */                                \
    bool CMC_(PFX, _seq_push_front)(struct SNAME * _list_, V * values,        \
                                    size_t size);                             \
    bool CMC_(PFX, _seq_push_at)(struct SNAME * _list_, V * values,           \
                                 size_t size, size_t index);                  \
    bool CMC_(PFX, _seq_push_back)(struct SNAME * _list_, V * values,         \
                                   size_t size);                              \
    bool CMC_(PFX, _seq_pop_at)(struct SNAME * _list_, size_t from,           \
                                size_t to);                                   \
    struct SNAME *CMC_(PFX, _seq_sublist)(struct SNAME * _list_, size_t from, \
                                          size_t to);                         \
    /* Element Access */                                                      \
    V CMC_(PFX, _front)(struct SNAME * _list_);                               \
    V CMC_(PFX, _get)(struct SNAME * _list_, size_t index);                   \
    V *CMC_(PFX, _get_ref)(struct SNAME * _list_, size_t index);              \
    V CMC_(PFX, _back)(struct SNAME * _list_);                                \
    size_t CMC_(PFX, _index_of)(struct SNAME * _list_, V value,               \
                                bool from_start);                             \
    /* Collection State */                                                    \
    bool CMC_(PFX, _contains)(struct SNAME * _list_, V value);                \
    bool CMC_(PFX, _empty)(struct SNAME * _list_);                            \
    bool CMC_(PFX, _full)(struct SNAME * _list_);                             \
    size_t CMC_(PFX, _count)(struct SNAME * _list_);                          \
    bool CMC_(PFX, _fits)(struct SNAME * _list_, size_t size);                \
    size_t CMC_(PFX, _capacity)(struct SNAME * _list_);                       \
    int CMC_(PFX, _flag)(struct SNAME * _list_);                              \
    /* Collection Utility */                                                  \
    bool CMC_(PFX, _resize)(struct SNAME * _list_, size_t capacity);          \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _list_);                 \
    bool CMC_(PFX, _equals)(struct SNAME * _list1_, struct SNAME * _list2_);  \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _list_);           \
    bool CMC_(PFX, _print)(struct SNAME * _list_, FILE * fptr);               \
                                                                              \
    /* Iterator Functions */                                                  \
    /* Iterator Initialization */                                             \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target); \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target);   \
    /* Iterator State */                                                      \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
    /* Iterator Movement */                                                   \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter);        \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter);          \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter);            \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter);            \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,          \
                                  size_t steps);                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                 size_t steps);                               \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                size_t index);                                \
    /* Iterator Access */                                                     \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter);              \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter);            \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter);

/* -------------------------------------------------------------------------
 * Source
 * ------------------------------------------------------------------------- */
#define CMC_CMC_LIST_CORE_SOURCE_(PFX, SNAME, V)                               \
                                                                               \
    /* Implementation Detail Functions */                                      \
    /* None */                                                                 \
                                                                               \
    struct SNAME *CMC_(PFX, _new)(size_t capacity,                             \
                                  struct CMC_DEF_FVAL(SNAME) * f_val)          \
    {                                                                          \
        struct cmc_alloc_node *alloc = &cmc_alloc_node_default;                \
                                                                               \
        if (capacity < 1)                                                      \
            return NULL;                                                       \
                                                                               \
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                               \
        if (!_list_)                                                           \
            return NULL;                                                       \
                                                                               \
        _list_->buffer = alloc->calloc(capacity, sizeof(V));                   \
                                                                               \
        if (!_list_->buffer)                                                   \
        {                                                                      \
            alloc->free(_list_);                                               \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _list_->capacity = capacity;                                           \
        _list_->count = 0;                                                     \
        _list_->flag = CMC_FLAG_OK;                                            \
        _list_->f_val = f_val;                                                 \
        _list_->alloc = alloc;                                                 \
        _list_->callbacks = NULL;                                              \
                                                                               \
        return _list_;                                                         \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _new_custom)(                                      \
        size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,                   \
        struct cmc_alloc_node * alloc, struct cmc_callbacks * callbacks)       \
    {                                                                          \
        if (capacity < 1)                                                      \
            return NULL;                                                       \
                                                                               \
        if (!f_val)                                                            \
            return NULL;                                                       \
                                                                               \
        if (!alloc)                                                            \
            alloc = &cmc_alloc_node_default;                                   \
                                                                               \
        struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));            \
                                                                               \
        if (!_list_)                                                           \
            return NULL;                                                       \
                                                                               \
        _list_->buffer = alloc->calloc(capacity, sizeof(V));                   \
                                                                               \
        if (!_list_->buffer)                                                   \
        {                                                                      \
            alloc->free(_list_);                                               \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _list_->capacity = capacity;                                           \
        _list_->count = 0;                                                     \
        _list_->flag = CMC_FLAG_OK;                                            \
        _list_->f_val = f_val;                                                 \
        _list_->alloc = alloc;                                                 \
        _list_->callbacks = callbacks;                                         \
                                                                               \
        return _list_;                                                         \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _clear)(struct SNAME * _list_)                              \
    {                                                                          \
        if (_list_->f_val->free)                                               \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
                _list_->f_val->free(_list_->buffer[i]);                        \
        }                                                                      \
                                                                               \
        memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);               \
                                                                               \
        _list_->count = 0;                                                     \
        _list_->flag = CMC_FLAG_OK;                                            \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _free)(struct SNAME * _list_)                               \
    {                                                                          \
        if (_list_->f_val->free)                                               \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
                _list_->f_val->free(_list_->buffer[i]);                        \
        }                                                                      \
                                                                               \
        _list_->alloc->free(_list_->buffer);                                   \
        _list_->alloc->free(_list_);                                           \
    }                                                                          \
                                                                               \
    void CMC_(PFX, _customize)(struct SNAME * _list_,                          \
                               struct cmc_alloc_node * alloc,                  \
                               struct cmc_callbacks * callbacks)               \
    {                                                                          \
        if (!alloc)                                                            \
            _list_->alloc = &cmc_alloc_node_default;                           \
        else                                                                   \
            _list_->alloc = alloc;                                             \
                                                                               \
        _list_->callbacks = callbacks;                                         \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_front)(struct SNAME * _list_, V value)                \
    {                                                                          \
        if (CMC_(PFX, _full)(_list_))                                          \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_list_, _list_->count * 2))                \
                return false;                                                  \
        }                                                                      \
                                                                               \
        if (!CMC_(PFX, _empty)(_list_))                                        \
        {                                                                      \
            memmove(_list_->buffer + 1, _list_->buffer,                        \
                    _list_->count * sizeof(V));                                \
        }                                                                      \
                                                                               \
        _list_->buffer[0] = value;                                             \
                                                                               \
        _list_->count++;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_at)(struct SNAME * _list_, V value, size_t index)     \
    {                                                                          \
        if (index > _list_->count)                                             \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (CMC_(PFX, _full)(_list_))                                          \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_list_, _list_->count * 2))                \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + index + 1, _list_->buffer + index,            \
                (_list_->count - index) * sizeof(V));                          \
                                                                               \
        _list_->buffer[index] = value;                                         \
        _list_->count++;                                                       \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _push_back)(struct SNAME * _list_, V value)                 \
    {                                                                          \
        if (CMC_(PFX, _full)(_list_))                                          \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_list_, _list_->count * 2))                \
                return false;                                                  \
        }                                                                      \
                                                                               \
        _list_->buffer[_list_->count++] = value;                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_front)(struct SNAME * _list_)                          \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer, _list_->buffer + 1,                            \
                (_list_->count - 1) * sizeof(V));                              \
                                                                               \
        _list_->buffer[--_list_->count] = (V){ 0 };                            \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_at)(struct SNAME * _list_, size_t index)               \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + index, _list_->buffer + index + 1,            \
                (_list_->count - index - 1) * sizeof(V));                      \
                                                                               \
        _list_->buffer[--_list_->count] = (V){ 0 };                            \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _pop_back)(struct SNAME * _list_)                           \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
            return false;                                                      \
                                                                               \
        _list_->buffer[--_list_->count] = (V){ 0 };                            \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _seq_push_front)(struct SNAME * _list_, V * values,         \
                                    size_t size)                               \
    {                                                                          \
        if (size == 0)                                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (!CMC_(PFX, _fits)(_list_, size))                                   \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_list_, _list_->count + size))             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + size, _list_->buffer,                         \
                _list_->count * sizeof(V));                                    \
                                                                               \
        memcpy(_list_->buffer, values, size * sizeof(V));                      \
                                                                               \
        _list_->count += size;                                                 \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _seq_push_at)(struct SNAME * _list_, V * values,            \
                                 size_t size, size_t index)                    \
    {                                                                          \
        if (size == 0)                                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index > _list_->count)                                             \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (index == 0)                                                        \
            return CMC_(PFX, _seq_push_front)(_list_, values, size);           \
        else if (index == _list_->count)                                       \
            return CMC_(PFX, _seq_push_back)(_list_, values, size);            \
                                                                               \
        if (!CMC_(PFX, _fits)(_list_, size))                                   \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_list_, _list_->count + size))             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memmove(_list_->buffer + index + size, _list_->buffer + index,         \
                (_list_->count - index) * sizeof(V));                          \
                                                                               \
        memcpy(_list_->buffer + index, values, size * sizeof(V));              \
                                                                               \
        _list_->count += size;                                                 \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _seq_push_back)(struct SNAME * _list_, V * values,          \
                                   size_t size)                                \
    {                                                                          \
        if (size == 0)                                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (!CMC_(PFX, _fits)(_list_, size))                                   \
        {                                                                      \
            if (!CMC_(PFX, _resize)(_list_, _list_->count + size))             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        memcpy(_list_->buffer + _list_->count, values, size * sizeof(V));      \
                                                                               \
        _list_->count += size;                                                 \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->create)                    \
            _list_->callbacks->create();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _seq_pop_at)(struct SNAME * _list_, size_t from, size_t to) \
    {                                                                          \
        if (from > to)                                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (to >= _list_->count)                                               \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        size_t length = (to - from + 1);                                       \
                                                                               \
        memmove(_list_->buffer + from, _list_->buffer + to + 1,                \
                (_list_->count - to - 1) * sizeof(V));                         \
                                                                               \
        memset(_list_->buffer + _list_->count - length, 0,                     \
               length * sizeof(V));                                            \
                                                                               \
        _list_->count -= to - from + 1;                                        \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _seq_sublist)(struct SNAME * _list_, size_t from,  \
                                          size_t to)                           \
    {                                                                          \
        if (from > to)                                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_INVALID;                                   \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (to >= _list_->count)                                               \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        size_t length = to - from + 1;                                         \
                                                                               \
        struct SNAME *result = CMC_(PFX, _new_custom)(                         \
            length, _list_->f_val, _list_->alloc, _list_->callbacks);          \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _list_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        memcpy(result->buffer, _list_->buffer, length * sizeof(V));            \
                                                                               \
        memmove(_list_->buffer + from, _list_->buffer + to + 1,                \
                (_list_->count - to - 1) * sizeof(V));                         \
                                                                               \
        memset(_list_->buffer + _list_->count - length, 0,                     \
               length * sizeof(V));                                            \
                                                                               \
        _list_->count -= length;                                               \
        result->count = length;                                                \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->delete)                    \
            _list_->callbacks->delete ();                                      \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _front)(struct SNAME * _list_)                                 \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->buffer[0];                                              \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _get)(struct SNAME * _list_, size_t index)                     \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->buffer[index];                                          \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _get_ref)(struct SNAME * _list_, size_t index)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (index >= _list_->count)                                            \
        {                                                                      \
            _list_->flag = CMC_FLAG_RANGE;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return &(_list_->buffer[index]);                                       \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _back)(struct SNAME * _list_)                                  \
    {                                                                          \
        if (CMC_(PFX, _empty)(_list_))                                         \
        {                                                                      \
            _list_->flag = CMC_FLAG_EMPTY;                                     \
            return (V){ 0 };                                                   \
        }                                                                      \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return _list_->buffer[_list_->count - 1];                              \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _index_of)(struct SNAME * _list_, V value,                \
                                bool from_start)                               \
    {                                                                          \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        size_t result = _list_->count;                                         \
                                                                               \
        if (from_start)                                                        \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
            {                                                                  \
                if (_list_->f_val->cmp(_list_->buffer[i], value) == 0)         \
                {                                                              \
                    result = i;                                                \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            for (size_t i = _list_->count; i > 0; i--)                         \
            {                                                                  \
                if (_list_->f_val->cmp(_list_->buffer[i - 1], value) == 0)     \
                {                                                              \
                    result = i - 1;                                            \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _contains)(struct SNAME * _list_, V value)                  \
    {                                                                          \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        bool result = false;                                                   \
                                                                               \
        for (size_t i = 0; i < _list_->count; i++)                             \
        {                                                                      \
            if (_list_->f_val->cmp(_list_->buffer[i], value) == 0)             \
            {                                                                  \
                result = true;                                                 \
                break;                                                         \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->read)                      \
            _list_->callbacks->read();                                         \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _empty)(struct SNAME * _list_)                              \
    {                                                                          \
        return _list_->count == 0;                                             \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _full)(struct SNAME * _list_)                               \
    {                                                                          \
        return _list_->count >= _list_->capacity;                              \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _count)(struct SNAME * _list_)                            \
    {                                                                          \
        return _list_->count;                                                  \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _fits)(struct SNAME * _list_, size_t size)                  \
    {                                                                          \
        return _list_->count + size <= _list_->capacity;                       \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _capacity)(struct SNAME * _list_)                         \
    {                                                                          \
        return _list_->capacity;                                               \
    }                                                                          \
                                                                               \
    int CMC_(PFX, _flag)(struct SNAME * _list_)                                \
    {                                                                          \
        return _list_->flag;                                                   \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _resize)(struct SNAME * _list_, size_t capacity)            \
    {                                                                          \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        if (_list_->capacity == capacity)                                      \
            return true;                                                       \
                                                                               \
        if (capacity < _list_->count)                                          \
        {                                                                      \
            _list_->flag = CMC_FLAG_INVALID;                                   \
            return false;                                                      \
        }                                                                      \
                                                                               \
        V *new_buffer =                                                        \
            _list_->alloc->realloc(_list_->buffer, sizeof(V) * capacity);      \
                                                                               \
        if (!new_buffer)                                                       \
        {                                                                      \
            _list_->flag = CMC_FLAG_ALLOC;                                     \
            return false;                                                      \
        }                                                                      \
                                                                               \
        /* TODO zero out new slots */                                          \
                                                                               \
        _list_->buffer = new_buffer;                                           \
        _list_->capacity = capacity;                                           \
                                                                               \
        if (_list_->callbacks && _list_->callbacks->resize)                    \
            _list_->callbacks->resize();                                       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct SNAME *CMC_(PFX, _copy_of)(struct SNAME * _list_)                   \
    {                                                                          \
        struct SNAME *result =                                                 \
            CMC_(PFX, _new_custom)(_list_->capacity, _list_->f_val,            \
                                   _list_->alloc, _list_->callbacks);          \
                                                                               \
        if (!result)                                                           \
        {                                                                      \
            _list_->flag = CMC_FLAG_ALLOC;                                     \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        if (_list_->f_val->cpy)                                                \
        {                                                                      \
            for (size_t i = 0; i < _list_->count; i++)                         \
                result->buffer[i] = _list_->f_val->cpy(_list_->buffer[i]);     \
        }                                                                      \
        else                                                                   \
            memcpy(result->buffer, _list_->buffer, sizeof(V) * _list_->count); \
                                                                               \
        result->count = _list_->count;                                         \
                                                                               \
        _list_->flag = CMC_FLAG_OK;                                            \
                                                                               \
        return result;                                                         \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _equals)(struct SNAME * _list1_, struct SNAME * _list2_)    \
    {                                                                          \
        _list1_->flag = CMC_FLAG_OK;                                           \
        _list2_->flag = CMC_FLAG_OK;                                           \
                                                                               \
        if (_list1_->count != _list2_->count)                                  \
            return false;                                                      \
                                                                               \
        for (size_t i = 0; i < _list1_->count; i++)                            \
        {                                                                      \
            if (_list1_->f_val->cmp(_list1_->buffer[i], _list2_->buffer[i]) != \
                0)                                                             \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct cmc_string CMC_(PFX, _to_string)(struct SNAME * _list_)             \
    {                                                                          \
        struct cmc_string str;                                                 \
        struct SNAME *l_ = _list_;                                             \
                                                                               \
        int n = snprintf(str.s, cmc_string_len, cmc_cmc_string_fmt_list,       \
                         #SNAME, #V, l_, l_->buffer, l_->capacity, l_->count,  \
                         l_->flag, l_->f_val, l_->alloc, l_->callbacks);       \
                                                                               \
        return n >= 0 ? str : (struct cmc_string){ 0 };                        \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _print)(struct SNAME * _list_, FILE * fptr)                 \
    {                                                                          \
        for (size_t i = 0; i < _list_->count; i++)                             \
        {                                                                      \
            if (!_list_->f_val->str(fptr, _list_->buffer[i]))                  \
                return false;                                                  \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME * target)   \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = 0;                                                       \
        iter.start = true;                                                     \
        iter.end = CMC_(PFX, _empty)(target);                                  \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME * target)     \
    {                                                                          \
        struct CMC_DEF_ITER(SNAME) iter;                                       \
                                                                               \
        iter.target = target;                                                  \
        iter.cursor = 0;                                                       \
        iter.start = CMC_(PFX, _empty)(target);                                \
        iter.end = true;                                                       \
                                                                               \
        if (!CMC_(PFX, _empty)(target))                                        \
            iter.cursor = target->count - 1;                                   \
                                                                               \
        return iter;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->start;                 \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        return CMC_(PFX, _empty)(iter->target) || iter->end;                   \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)          \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->cursor = 0;                                                  \
            iter->start = true;                                                \
            iter->end = CMC_(PFX, _empty)(iter->target);                       \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)            \
    {                                                                          \
        if (!CMC_(PFX, _empty)(iter->target))                                  \
        {                                                                      \
            iter->start = CMC_(PFX, _empty)(iter->target);                     \
            iter->cursor = iter->target->count - 1;                            \
            iter->end = true;                                                  \
                                                                               \
            return true;                                                       \
        }                                                                      \
                                                                               \
        return false;                                                          \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->cursor + 1 == iter->target->count)                           \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        iter->cursor++;                                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->cursor == 0)                                                 \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        iter->cursor--;                                                        \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter,           \
                                  size_t steps)                                \
    {                                                                          \
        if (iter->end)                                                         \
            return false;                                                      \
                                                                               \
        if (iter->cursor + 1 == iter->target->count)                           \
        {                                                                      \
            iter->end = true;                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->cursor + steps >= iter->target->count)         \
            return false;                                                      \
                                                                               \
        iter->start = CMC_(PFX, _empty)(iter->target);                         \
                                                                               \
        iter->cursor += steps;                                                 \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator moved */                              \
    bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter,            \
                                 size_t steps)                                 \
    {                                                                          \
        if (iter->start)                                                       \
            return false;                                                      \
                                                                               \
        if (iter->cursor == 0)                                                 \
        {                                                                      \
            iter->start = true;                                                \
            return false;                                                      \
        }                                                                      \
                                                                               \
        if (steps == 0 || iter->cursor < steps)                                \
            return false;                                                      \
                                                                               \
        iter->end = CMC_(PFX, _empty)(iter->target);                           \
                                                                               \
        iter->cursor -= steps;                                                 \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    /* Returns true only if the iterator was able to be positioned at the */   \
    /* given index */                                                          \
    bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter,             \
                                size_t index)                                  \
    {                                                                          \
        if (index >= iter->target->count)                                      \
            return false;                                                      \
                                                                               \
        if (iter->cursor > index)                                              \
            return CMC_(PFX, _iter_rewind)(iter, iter->cursor - index);        \
        else if (iter->cursor < index)                                         \
            return CMC_(PFX, _iter_advance)(iter, index - iter->cursor);       \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)                \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return (V){ 0 };                                                   \
                                                                               \
        return iter->target->buffer[iter->cursor];                             \
    }                                                                          \
                                                                               \
    V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)              \
    {                                                                          \
        if (CMC_(PFX, _empty)(iter->target))                                   \
            return NULL;                                                       \
                                                                               \
        return &(iter->target->buffer[iter->cursor]);                          \
    }                                                                          \
                                                                               \
    size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)           \
    {                                                                          \
        return iter->cursor;                                                   \
    }

#endif /* CMC_CMC_LIST_H */
