#include "cmc/intervalheap.h"

struct intervalheap
{
    struct intervalheap_node *buffer;
    size_t capacity;
    size_t size;
    size_t count;
    int flag;
    struct intervalheap_fval *f_val;
    struct intervalheap_iter (*it_start)(struct intervalheap *);
    struct intervalheap_iter (*it_end)(struct intervalheap *);
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct intervalheap_node
{
    size_t data[2];
};
struct intervalheap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct intervalheap_iter
{
    struct intervalheap *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct intervalheap *ih_new(size_t capacity, struct intervalheap_fval *f_val);
struct intervalheap *ih_new_custom(size_t capacity,
                                   struct intervalheap_fval *f_val,
                                   struct cmc_alloc_node *alloc,
                                   struct cmc_callbacks *callbacks);
void ih_clear(struct intervalheap *_heap_);
void ih_free(struct intervalheap *_heap_);
void ih_customize(struct intervalheap *_heap_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool ih_insert(struct intervalheap *_heap_, size_t value);
_Bool ih_remove_max(struct intervalheap *_heap_);
_Bool ih_remove_min(struct intervalheap *_heap_);
_Bool ih_update_max(struct intervalheap *_heap_, size_t value);
_Bool ih_update_min(struct intervalheap *_heap_, size_t value);
size_t ih_max(struct intervalheap *_heap_);
size_t ih_min(struct intervalheap *_heap_);
_Bool ih_contains(struct intervalheap *_heap_, size_t value);
_Bool ih_empty(struct intervalheap *_heap_);
_Bool ih_full(struct intervalheap *_heap_);
size_t ih_count(struct intervalheap *_heap_);
size_t ih_capacity(struct intervalheap *_heap_);
int ih_flag(struct intervalheap *_heap_);
_Bool ih_resize(struct intervalheap *_heap_, size_t capacity);
struct intervalheap *ih_copy_of(struct intervalheap *_set_);
_Bool ih_equals(struct intervalheap *_heap1_, struct intervalheap *_heap2_);
struct cmc_string ih_to_string(struct intervalheap *_heap_);
_Bool ih_print(struct intervalheap *_heap_, FILE *fptr);
struct intervalheap_iter *ih_iter_new(struct intervalheap *target);
void ih_iter_free(struct intervalheap_iter *iter);
void ih_iter_init(struct intervalheap_iter *iter, struct intervalheap *target);
_Bool ih_iter_start(struct intervalheap_iter *iter);
_Bool ih_iter_end(struct intervalheap_iter *iter);
void ih_iter_to_start(struct intervalheap_iter *iter);
void ih_iter_to_end(struct intervalheap_iter *iter);
_Bool ih_iter_next(struct intervalheap_iter *iter);
_Bool ih_iter_prev(struct intervalheap_iter *iter);
_Bool ih_iter_advance(struct intervalheap_iter *iter, size_t steps);
_Bool ih_iter_rewind(struct intervalheap_iter *iter, size_t steps);
_Bool ih_iter_go_to(struct intervalheap_iter *iter, size_t index);
size_t ih_iter_value(struct intervalheap_iter *iter);
size_t ih_iter_index(struct intervalheap_iter *iter);
static void ih_impl_float_up_max(struct intervalheap *_heap_);
static void ih_impl_float_up_min(struct intervalheap *_heap_);
static void ih_impl_float_down_max(struct intervalheap *_heap_);
static void ih_impl_float_down_min(struct intervalheap *_heap_);
static struct intervalheap_iter ih_impl_it_start(struct intervalheap *_heap_);
static struct intervalheap_iter ih_impl_it_end(struct intervalheap *_heap_);
struct intervalheap *ih_new(size_t capacity, struct intervalheap_fval *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity == 0 || capacity == 0xffffffffffffffffULL)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    struct intervalheap *_heap_ = alloc->malloc(sizeof(struct intervalheap));
    if (!_heap_)
        return ((void *)0);
    capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;
    _heap_->buffer = alloc->calloc(capacity, sizeof(struct intervalheap_node));
    if (!_heap_->buffer)
    {
        alloc->free(_heap_);
        return ((void *)0);
    }
    _heap_->capacity = capacity;
    _heap_->size = 0;
    _heap_->count = 0;
    _heap_->flag = cmc_flags.OK;
    _heap_->f_val = f_val;
    _heap_->alloc = alloc;
    _heap_->callbacks = ((void *)0);
    _heap_->it_start = ih_impl_it_start;
    _heap_->it_end = ih_impl_it_end;
    return _heap_;
}
struct intervalheap *ih_new_custom(size_t capacity,
                                   struct intervalheap_fval *f_val,
                                   struct cmc_alloc_node *alloc,
                                   struct cmc_callbacks *callbacks)
{
    if (capacity == 0 || capacity == 0xffffffffffffffffULL)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct intervalheap *_heap_ = alloc->malloc(sizeof(struct intervalheap));
    if (!_heap_)
        return ((void *)0);
    capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;
    _heap_->buffer = alloc->calloc(capacity, sizeof(struct intervalheap_node));
    if (!_heap_->buffer)
    {
        alloc->free(_heap_);
        return ((void *)0);
    }
    _heap_->capacity = capacity;
    _heap_->size = 0;
    _heap_->count = 0;
    _heap_->flag = cmc_flags.OK;
    _heap_->f_val = f_val;
    _heap_->alloc = alloc;
    _heap_->callbacks = callbacks;
    _heap_->it_start = ih_impl_it_start;
    _heap_->it_end = ih_impl_it_end;
    return _heap_;
}
void ih_clear(struct intervalheap *_heap_)
{
    if (_heap_->f_val->free)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            _heap_->f_val->free(_heap_->buffer[i / 2].data[i % 2]);
        }
    }
    memset(_heap_->buffer, 0, sizeof(size_t) * _heap_->capacity);
    _heap_->size = 0;
    _heap_->count = 0;
    _heap_->flag = cmc_flags.OK;
}
void ih_free(struct intervalheap *_heap_)
{
    if (_heap_->f_val->free)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            _heap_->f_val->free(_heap_->buffer[i / 2].data[i % 2]);
        }
    }
    _heap_->alloc->free(_heap_->buffer);
    _heap_->alloc->free(_heap_);
}
void ih_customize(struct intervalheap *_heap_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _heap_->alloc = &cmc_alloc_node_default;
    else
        _heap_->alloc = alloc;
    _heap_->callbacks = callbacks;
    _heap_->flag = cmc_flags.OK;
}
_Bool ih_insert(struct intervalheap *_heap_, size_t value)
{
    if (ih_full(_heap_))
    {
        if (!ih_resize(_heap_, _heap_->capacity * 4))
            return 0;
    }
    if (_heap_->count % 2 == 0)
    {
        _heap_->buffer[_heap_->size].data[0] = value;
        _heap_->buffer[_heap_->size].data[1] = (size_t){ 0 };
        _heap_->size++;
    }
    else
    {
        struct intervalheap_node *node = &(_heap_->buffer[_heap_->size - 1]);
        if (_heap_->f_val->cmp(node->data[0], value) > 0)
        {
            node->data[1] = node->data[0];
            node->data[0] = value;
        }
        else
        {
            node->data[1] = value;
        }
    }
    _heap_->count++;
    _heap_->flag = cmc_flags.OK;
    if (_heap_->count > 2)
    {
        struct intervalheap_node *parent =
            &(_heap_->buffer[(_heap_->size - 1) / 2]);
        if (_heap_->f_val->cmp(parent->data[0], value) > 0)
            ih_impl_float_up_min(_heap_);
        else if (_heap_->f_val->cmp(parent->data[1], value) < 0)
            ih_impl_float_up_max(_heap_);
    }
    if (_heap_->callbacks && _heap_->callbacks->create)
        _heap_->callbacks->create();
    return 1;
}
_Bool ih_remove_max(struct intervalheap *_heap_)
{
    if (ih_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return 0;
    }
    if (_heap_->count == 1)
    {
        _heap_->buffer[0].data[0] = (size_t){ 0 };
        _heap_->count--;
        _heap_->size--;
        goto success;
    }
    struct intervalheap_node *last_node = &(_heap_->buffer[_heap_->size - 1]);
    if (_heap_->count % 2 == 1)
    {
        _heap_->buffer[0].data[1] = last_node->data[0];
        last_node->data[0] = (size_t){ 0 };
        _heap_->size--;
    }
    else
    {
        _heap_->buffer[0].data[1] = last_node->data[1];
        last_node->data[1] = (size_t){ 0 };
    }
    _heap_->count--;
    ih_impl_float_down_max(_heap_);
success:
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->delete)
        _heap_->callbacks->delete ();
    return 1;
}
_Bool ih_remove_min(struct intervalheap *_heap_)
{
    if (ih_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return 0;
    }
    if (_heap_->count == 1)
    {
        _heap_->buffer[0].data[0] = (size_t){ 0 };
        _heap_->count--;
        _heap_->size--;
        goto success;
    }
    struct intervalheap_node *last_node = &(_heap_->buffer[_heap_->size - 1]);
    _heap_->buffer[0].data[0] = last_node->data[0];
    if (_heap_->count % 2 == 1)
    {
        last_node->data[0] = (size_t){ 0 };
        _heap_->size--;
    }
    else
    {
        last_node->data[0] = last_node->data[1];
        last_node->data[1] = (size_t){ 0 };
    }
    _heap_->count--;
    ih_impl_float_down_min(_heap_);
success:
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->delete)
        _heap_->callbacks->delete ();
    return 1;
}
_Bool ih_update_max(struct intervalheap *_heap_, size_t value)
{
    if (ih_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return 0;
    }
    if (_heap_->count == 1)
    {
        _heap_->buffer[0].data[0] = value;
    }
    else if (_heap_->f_val->cmp(value, _heap_->buffer[0].data[0]) < 0)
    {
        _heap_->buffer[0].data[1] = _heap_->buffer[0].data[0];
        _heap_->buffer[0].data[0] = value;
        ih_impl_float_down_max(_heap_);
    }
    else
    {
        _heap_->buffer[0].data[1] = value;
        ih_impl_float_down_max(_heap_);
    }
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->update)
        _heap_->callbacks->update();
    return 1;
}
_Bool ih_update_min(struct intervalheap *_heap_, size_t value)
{
    if (ih_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return 0;
    }
    if (_heap_->count == 1)
    {
        _heap_->buffer[0].data[0] = value;
    }
    else if (_heap_->f_val->cmp(value, _heap_->buffer[0].data[1]) > 0)
    {
        _heap_->buffer[0].data[0] = _heap_->buffer[0].data[1];
        _heap_->buffer[0].data[1] = value;
        ih_impl_float_down_min(_heap_);
    }
    else
    {
        _heap_->buffer[0].data[0] = value;
        ih_impl_float_down_min(_heap_);
    }
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->update)
        _heap_->callbacks->update();
    return 1;
}
size_t ih_max(struct intervalheap *_heap_)
{
    if (ih_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->read)
        _heap_->callbacks->read();
    if (_heap_->count == 1)
        return _heap_->buffer[0].data[0];
    return _heap_->buffer[0].data[1];
}
size_t ih_min(struct intervalheap *_heap_)
{
    if (ih_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->read)
        _heap_->callbacks->read();
    return _heap_->buffer[0].data[0];
}
_Bool ih_contains(struct intervalheap *_heap_, size_t value)
{
    _heap_->flag = cmc_flags.OK;
    _Bool result = 0;
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (_heap_->f_val->cmp(_heap_->buffer[i / 2].data[i % 2], value) == 0)
        {
            result = 1;
            break;
        }
    }
    if (_heap_->callbacks && _heap_->callbacks->read)
        _heap_->callbacks->read();
    return result;
}
_Bool ih_empty(struct intervalheap *_heap_)
{
    return _heap_->count == 0;
}
_Bool ih_full(struct intervalheap *_heap_)
{
    return _heap_->size >= _heap_->capacity && _heap_->count % 2 == 0;
}
size_t ih_count(struct intervalheap *_heap_)
{
    return _heap_->count;
}
size_t ih_capacity(struct intervalheap *_heap_)
{
    return _heap_->capacity;
}
int ih_flag(struct intervalheap *_heap_)
{
    return _heap_->flag;
}
_Bool ih_resize(struct intervalheap *_heap_, size_t capacity)
{
    if (_heap_->capacity == capacity)
        goto success;
    if (capacity < _heap_->count)
    {
        _heap_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (capacity == 0xffffffffffffffffULL)
    {
        _heap_->flag = cmc_flags.ERROR;
        return 0;
    }
    capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;
    struct intervalheap_node *new_buffer = _heap_->alloc->realloc(
        _heap_->buffer, sizeof(struct intervalheap_node) * capacity);
    if (!new_buffer)
    {
        _heap_->flag = cmc_flags.ALLOC;
        return 0;
    }
    if (capacity > _heap_->capacity)
    {
        memset(new_buffer + _heap_->capacity, 0,
               sizeof(struct intervalheap_node) *
                   (capacity - _heap_->capacity));
    }
    _heap_->buffer = new_buffer;
    _heap_->capacity = capacity;
success:
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->resize)
        _heap_->callbacks->resize();
    return 1;
}
struct intervalheap *ih_copy_of(struct intervalheap *_heap_)
{
    struct intervalheap *result =
        _heap_->alloc->malloc(sizeof(struct intervalheap));
    if (!result)
    {
        _heap_->flag = cmc_flags.ALLOC;
        return ((void *)0);
    }
    memcpy(result, _heap_, sizeof(struct intervalheap));
    result->buffer = _heap_->alloc->malloc(sizeof(struct intervalheap_node) *
                                           _heap_->capacity);
    if (!result->buffer)
    {
        _heap_->alloc->free(result);
        _heap_->flag = cmc_flags.ALLOC;
        return ((void *)0);
    }
    if (_heap_->f_val->cpy)
    {
        for (size_t i = 0; i < _heap_->count; i++)
            result->buffer[i / 2].data[i % 2] =
                _heap_->f_val->cpy(_heap_->buffer[i / 2].data[i % 2]);
    }
    else
        memcpy(result->buffer, _heap_->buffer,
               sizeof(struct intervalheap_node) * _heap_->capacity);
    result->capacity = _heap_->capacity;
    result->size = _heap_->size;
    result->count = _heap_->count;
    result->flag = cmc_flags.OK;
    result->f_val = _heap_->f_val;
    result->alloc = _heap_->alloc;
    result->callbacks = _heap_->callbacks;
    result->it_start = ih_impl_it_start;
    result->it_end = ih_impl_it_end;
    _heap_->flag = cmc_flags.OK;
    return result;
}
_Bool ih_equals(struct intervalheap *_heap1_, struct intervalheap *_heap2_)
{
    _heap1_->flag = cmc_flags.OK;
    _heap2_->flag = cmc_flags.OK;
    if (_heap1_->count != _heap2_->count)
        return 0;
    for (size_t i = 0; i < _heap1_->count; i++)
    {
        size_t value1 = _heap1_->buffer[i / 2].data[i % 2];
        size_t value2 = _heap2_->buffer[i / 2].data[i % 2];
        if (_heap1_->f_val->cmp(value1, value2) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string ih_to_string(struct intervalheap *_heap_)
{
    struct cmc_string str;
    struct intervalheap *h_ = _heap_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_intervalheap,
                     "intervalheap", "size_t", h_, h_->buffer, h_->capacity,
                     h_->size, h_->count, h_->flag, h_->f_val, h_->alloc,
                     h_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool ih_print(struct intervalheap *_heap_, FILE *fptr)
{
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (!_heap_->f_val->str(fptr, _heap_->buffer[i / 2].data[i % 2]))
            return 0;
    }
    return 1;
}
struct intervalheap_iter *ih_iter_new(struct intervalheap *target)
{
    struct intervalheap_iter *iter =
        target->alloc->malloc(sizeof(struct intervalheap_iter));
    if (!iter)
        return ((void *)0);
    ih_iter_init(iter, target);
    return iter;
}
void ih_iter_free(struct intervalheap_iter *iter)
{
    iter->target->alloc->free(iter);
}
void ih_iter_init(struct intervalheap_iter *iter, struct intervalheap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = ih_empty(target);
}
_Bool ih_iter_start(struct intervalheap_iter *iter)
{
    return ih_empty(iter->target) || iter->start;
}
_Bool ih_iter_end(struct intervalheap_iter *iter)
{
    return ih_empty(iter->target) || iter->end;
}
void ih_iter_to_start(struct intervalheap_iter *iter)
{
    if (!ih_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = ih_empty(iter->target);
    }
}
void ih_iter_to_end(struct intervalheap_iter *iter)
{
    if (!ih_empty(iter->target))
    {
        iter->cursor = iter->target->count - 1;
        iter->start = ih_empty(iter->target);
        iter->end = 1;
    }
}
_Bool ih_iter_next(struct intervalheap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = ih_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool ih_iter_prev(struct intervalheap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = ih_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool ih_iter_advance(struct intervalheap_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= iter->target->count)
        return 0;
    iter->start = ih_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool ih_iter_rewind(struct intervalheap_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor < steps)
        return 0;
    iter->end = ih_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool ih_iter_go_to(struct intervalheap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->cursor > index)
        return ih_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return ih_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t ih_iter_value(struct intervalheap_iter *iter)
{
    if (ih_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor / 2].data[iter->cursor % 2];
}
size_t ih_iter_index(struct intervalheap_iter *iter)
{
    return iter->cursor;
}
static void ih_impl_float_up_max(struct intervalheap *_heap_)
{
    size_t index = _heap_->size - 1;
    struct intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (index > 0)
    {
        size_t P_index = (index - 1) / 2;
        struct intervalheap_node *parent = &(_heap_->buffer[P_index]);
        if (index == _heap_->size - 1 && _heap_->count % 2 != 0)
        {
            if (_heap_->f_val->cmp(curr_node->data[0], parent->data[1]) < 0)
                break;
            size_t tmp = curr_node->data[0];
            curr_node->data[0] = parent->data[1];
            parent->data[1] = tmp;
        }
        else
        {
            if (_heap_->f_val->cmp(curr_node->data[1], parent->data[1]) < 0)
                break;
            size_t tmp = curr_node->data[1];
            curr_node->data[1] = parent->data[1];
            parent->data[1] = tmp;
        }
        index = P_index;
        curr_node = parent;
    }
}
static void ih_impl_float_up_min(struct intervalheap *_heap_)
{
    size_t index = _heap_->size - 1;
    struct intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (index > 0)
    {
        size_t P_index = (index - 1) / 2;
        struct intervalheap_node *parent = &(_heap_->buffer[P_index]);
        if (_heap_->f_val->cmp(curr_node->data[0], parent->data[0]) >= 0)
            break;
        size_t tmp = curr_node->data[0];
        curr_node->data[0] = parent->data[0];
        parent->data[0] = tmp;
        index = P_index;
        curr_node = parent;
    }
}
static void ih_impl_float_down_max(struct intervalheap *_heap_)
{
    size_t index = 0;
    struct intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (1)
    {
        if (2 * index + 1 >= _heap_->size)
            break;
        size_t child;
        size_t L_index = index * 2 + 1;
        size_t R_index = index * 2 + 2;
        if (R_index < _heap_->size)
        {
            struct intervalheap_node *L = &(_heap_->buffer[L_index]);
            struct intervalheap_node *R = &(_heap_->buffer[R_index]);
            if (R_index == _heap_->size - 1 && _heap_->count % 2 != 0)
                child = _heap_->f_val->cmp(L->data[1], R->data[0]) > 0
                            ? L_index
                            : R_index;
            else
                child = _heap_->f_val->cmp(L->data[1], R->data[1]) > 0
                            ? L_index
                            : R_index;
        }
        else
            child = L_index;
        struct intervalheap_node *child_node = &(_heap_->buffer[child]);
        if (child == _heap_->size - 1 && _heap_->count % 2 != 0)
        {
            if (_heap_->f_val->cmp(curr_node->data[1], child_node->data[0]) >=
                0)
                break;
            size_t tmp = child_node->data[0];
            child_node->data[0] = curr_node->data[1];
            curr_node->data[1] = tmp;
        }
        else
        {
            if (_heap_->f_val->cmp(curr_node->data[1], child_node->data[1]) >=
                0)
                break;
            size_t tmp = child_node->data[1];
            child_node->data[1] = curr_node->data[1];
            curr_node->data[1] = tmp;
            if (_heap_->f_val->cmp(child_node->data[0], child_node->data[1]) >
                0)
            {
                tmp = child_node->data[0];
                child_node->data[0] = child_node->data[1];
                child_node->data[1] = tmp;
            }
        }
        index = child;
        curr_node = child_node;
    }
}
static void ih_impl_float_down_min(struct intervalheap *_heap_)
{
    size_t index = 0;
    struct intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (1)
    {
        if (2 * index + 1 >= _heap_->size)
            break;
        size_t child;
        size_t L_index = index * 2 + 1;
        size_t R_index = index * 2 + 2;
        if (R_index < _heap_->size)
        {
            struct intervalheap_node *L = &(_heap_->buffer[L_index]);
            struct intervalheap_node *R = &(_heap_->buffer[R_index]);
            child = _heap_->f_val->cmp(L->data[0], R->data[0]) < 0 ? L_index
                                                                   : R_index;
        }
        else
            child = L_index;
        struct intervalheap_node *child_node = &(_heap_->buffer[child]);
        if (_heap_->f_val->cmp(curr_node->data[0], child_node->data[0]) < 0)
            break;
        size_t tmp = child_node->data[0];
        child_node->data[0] = curr_node->data[0];
        curr_node->data[0] = tmp;
        if (child != _heap_->size - 1 || _heap_->count % 2 == 0)
        {
            if (_heap_->f_val->cmp(child_node->data[0], child_node->data[1]) >
                0)
            {
                tmp = child_node->data[0];
                child_node->data[0] = child_node->data[1];
                child_node->data[1] = tmp;
            }
        }
        index = child;
        curr_node = child_node;
    }
}
static struct intervalheap_iter ih_impl_it_start(struct intervalheap *_heap_)
{
    struct intervalheap_iter iter;
    ih_iter_init(&iter, _heap_);
    ih_iter_to_start(&iter);
    return iter;
}
static struct intervalheap_iter ih_impl_it_end(struct intervalheap *_heap_)
{
    struct intervalheap_iter iter;
    ih_iter_init(&iter, _heap_);
    ih_iter_to_end(&iter);
    return iter;
}
