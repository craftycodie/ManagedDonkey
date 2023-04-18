#include "cache/cache_file_tag_resource_runtime.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

void patch_lz_cache_file_decompressor()
{
#ifdef _DEBUG

	patch_pointer({ .address = 0x01690134 }, lz_cache_file_decompressor_begin);
	patch_pointer({ .address = 0x01690138 }, lz_cache_file_decompressor_decompress_buffer);
	patch_pointer({ .address = 0x0169013C }, lz_cache_file_decompressor_finish);

#endif // _DEBUG
}

#ifdef _DEBUG

long begin_ = 0;
long decompress_buffer_ = 0;
long finish_ = 0;
bool __fastcall lz_cache_file_decompressor_begin(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1)
{
	c_console::write("c_lz_cache_file_decompressor::begin %d ", begin_++);
	decompress_buffer_ = 0;

	//return DECLFUNC(0x009E1430, bool, __thiscall, c_lz_cache_file_decompressor*, c_basic_buffer<void>)(_this, a1);

	_this->__buffer4.m_size = a1.m_size;
	_this->__bufferC.m_size = a1.m_size;

	_this->__unknown14 = true;

	_this->__buffer4.m_buffer = a1.m_buffer;
	_this->__bufferC.m_buffer = a1.m_buffer;

	return true;
}

bool __fastcall lz_cache_file_decompressor_decompress_buffer(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
{
	c_console::write("I");
	decompress_buffer_++;

	return DECLFUNC(0x009E14F0, bool, __thiscall, c_lz_cache_file_decompressor*, c_basic_buffer<void>, c_basic_buffer<void>*)(_this, a1, a2);
}

bool __fastcall lz_cache_file_decompressor_finish(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void>* a1)
{
	c_console::write_line("");
	ASSERT(begin_ == ++finish_);

	return DECLFUNC(0x009E1640, bool, __thiscall, c_lz_cache_file_decompressor*, c_basic_buffer<void>*)(_this, a1);
}

#endif // _DEBUG

//HOOK_DECLARE(0x00563E10, tag_resource_get);
//
//void* __cdecl tag_resource_get(s_tag_resource const* resource)
//{
//    FUNCTION_BEGIN(false);
//
//    void* result = nullptr;
//    HOOK_INVOKE(result =, tag_resource_get, resource);
//    return result;
//}

