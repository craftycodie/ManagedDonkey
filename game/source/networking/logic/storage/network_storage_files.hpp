#pragma once

extern void __cdecl network_storage_files_dispose();
extern void __cdecl network_storage_files_initialize();
extern void __cdecl network_storage_files_update();

extern bool __cdecl create_configuration_file(char const* filename, void const* file_contents, long file_size);

