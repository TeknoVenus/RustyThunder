#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

extern "C" {

/// * Public functions
/// Generate a greeting
char *greeting_generate(const char *name);

void greeting_free(char *s);

} // extern "C"
