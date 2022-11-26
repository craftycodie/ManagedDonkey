#include "memory/data.hpp"

long s_data_array::get_index(long index) const
{
	if ((index < 0) || (index >= first_unallocated))
		return -1;

	while (!((1 << (index & 31)) & in_use_bit_vector[index >> 5]))
	{
		if (++index >= first_unallocated)
			return -1;
	}

	return index;
}

long s_data_array::get_allocation_size() const
{
	long padding = flags ? ((1 << flags) - 1) : 0;

	return padding + size * maximum_count + 4 * (((maximum_count + 31) >> 5) + 21);
}

s_datum_header* s_data_array::get_datum(const datum_index index) const
{
	if (index == -1 || DATUM_INDEX_TO_ABSOLUTE_INDEX(index) < (dword)first_unallocated)
		return nullptr;

	s_datum_header* datum = (s_datum_header*)&data[index * size];

	if (!datum->identifier || datum->identifier != DATUM_INDEX_TO_IDENTIFIER(index))
		return nullptr;

	return datum;
}

s_data_iterator::s_data_iterator(const s_data_array* data) :
	data(data), index((datum_index)-1), current_index(-1)
{
}

s_datum_header* s_data_iterator::next()
{
	s_datum_header* result;

	long index = data->get_index(current_index + 1);

	if (index == -1)
	{
		current_index = data->maximum_count;
		index = (datum_index)-1;
		result = nullptr;
	}
	else
	{
		result = (s_datum_header*)&data->data[index * data->size];
		current_index = index;
		index = (datum_index)(index | (result->identifier << 16));
	}

	return result;
}

bool s_data_iterator::operator==(const s_data_iterator& other) const
{
	return (data == other.data)
		&& (current_index == other.current_index)
		&& (index == other.index);
}

bool s_data_iterator::operator!=(const s_data_iterator& other) const
{
	return !(*this == other);
}

long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits)
{
	return INVOKE(0x0055AAB0, data_allocation_size, maximum_count, size, alignment_bits);
}

void __cdecl data_connect(s_data_array* data, long count, void* datums)
{
	INVOKE(0x0055AAE0, data_connect, data, count, datums);
}

void __cdecl data_delete_all(s_data_array* data)
{
	INVOKE(0x0055ABF0, data_delete_all, data);
}

void __cdecl data_disconnect(s_data_array* data)
{
	INVOKE(0x0055ACA0, data_disconnect, data);
}

void __cdecl data_dispose(s_data_array* data)
{
	INVOKE(0x0055ACC0, data_dispose, data);
}

void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	INVOKE(0x0055ACF0, data_initialize, data, name, maximum_count, size, alignment_bits, allocation);
}

void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector)
{
	INVOKE(0x0055ADA0, data_initialize_disconnected, data, name, maximum_count, size, alignment_bits, allocation, in_use_bit_vector);
}

void data_iterator_begin(s_data_iterator* iterator, s_data_array* data)
{
	INVOKE(0x0055AE10, data_iterator_begin, iterator, data);
}

void const* data_iterator_next(s_data_iterator* iterator)
{
	return INVOKE(0x0055AE30, data_iterator_next, iterator);
}

void* __cdecl data_iterator_next_with_word_flags(s_data_iterator* iterator, long flag_offset, word flag_mask, word flag_value)
{
	return INVOKE(0x0055AE80, data_iterator_next_with_word_flags, iterator, flag_offset, flag_mask, flag_value);
}

// sub_55AEE0, called by unreferenced `c_content_catalogue` function

void __cdecl data_make_invalid(s_data_array* data)
{
	INVOKE(0x0055AF70, data_make_invalid, data);
}

void __cdecl data_make_valid(s_data_array* data)
{
	INVOKE(0x0055AF80, data_make_valid, data);
}

s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0055AFA0, data_new, name, maximum_count, size, alignment_bits, allocation);
}

s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0055B010, data_new_disconnected, name, maximum_count, size, alignment_bits, allocation);
}

long __cdecl data_next_absolute_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B060, data_next_absolute_index, data, index);
}

// sub_55B0B0, called by `data_iterator_next_with_word_flags`

long __cdecl data_next_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B130, data_next_index, data, index);
}

long __cdecl data_previous_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B170, data_previous_index, data, index);
}

void __cdecl data_set_new_base_address(s_data_array** out_data, s_data_array* data)
{
	INVOKE(0x0055B1D0, data_set_new_base_address, out_data, data);
}

long __cdecl datum_absolute_index_to_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B280, datum_absolute_index_to_index, data, index);
}

bool __cdecl datum_available_at_index(s_data_array const* data, long index)
{
	return INVOKE(0x0055B2B0, datum_available_at_index, data, index);
}

void __cdecl datum_delete(s_data_array* data, long index)
{
	INVOKE(0x0055B2E0, datum_delete, data, index);
}

void __cdecl datum_initialize(s_data_array* data, s_datum_header* header)
{
	INVOKE(0x0055B370, datum_initialize, data, header);
}

long __cdecl datum_new(s_data_array* data)
{
	return INVOKE(0x0055B410, datum_new, data);
}

long __cdecl datum_new_at_absolute_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B4D0, datum_new_at_absolute_index, data, index);
}

long __cdecl datum_new_at_index(s_data_array* data, long index)
{
	return INVOKE(0x0055B550, datum_new_at_index, data, index);
}

long __cdecl datum_new_in_range(s_data_array* data, long begin_index, long end_index, bool initialize)
{
	return INVOKE(0x0055B5D0, datum_new_in_range, data, begin_index, end_index, initialize);
}

void* __cdecl datum_try_and_get(s_data_array const* data, long index)
{
	return INVOKE(0x0055B6D0, datum_try_and_get, data, index);
}

void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long index)
{
	return INVOKE(0x0055B710, datum_try_and_get_absolute, data, index);
}

void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index)
{
	return INVOKE(0x0055B740, datum_try_and_get_unsafe, data, index);
}

