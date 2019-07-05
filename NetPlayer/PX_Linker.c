#include "PX_Linker.h"

px_bool PX_LinkerInit(PX_Linker *linker, px_void *Init,px_void *Write,px_void *Read,px_void *ioctl,px_void *param)
{
	linker->_PX_LinkerInit=(px_bool (*)(px_void *))Init;
	linker->_PX_LinkerRead=(px_int (*)(px_void *,px_int))Read;
	linker->_PX_LinkerWrite=(px_int (*)(px_void *,px_int))Write;
	linker->_PX_LinkerIOCTL=(px_int (*)(px_int,px_int,px_void *))ioctl;
	return linker->_PX_LinkerInit(param);
}
