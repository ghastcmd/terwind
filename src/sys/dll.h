#pragma once

void * dll_load(const char * name);

char * dll_check_erros(void * dll_handle);

void * dll_get_symbol(void * handle, const char * name);

void dll_close(void * handle);
