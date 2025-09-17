//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.667 Principles of Computer Graphics
//	Instructor:	Brian Russin
//
//	Author:  David W. Nesbitt
//	File:    GeometryTest/main.cpp
//	Purpose: Test the vector library.
//
//============================================================================

#include <stdarg.h>
#include <stdio.h>

namespace cg
{

void vector_test_module1();
void matrix_test_module4();

// Simple logging function
void logmsg(const char *message, ...)
{
    // Open file if not already opened
    static FILE *lfile = NULL;
    if(lfile == NULL) { lfile = fopen("GeometryTest_Module4.log", "w"); }

    va_list arg;
    va_start(arg, message);
    vfprintf(lfile, message, arg);
    putc('\n', lfile);
    fflush(lfile);
    va_end(arg);
}

} // namespace cg

/**
 * Main method. Entry point for application.
 */
int main(int argc, char *argv[])
{
    cg::matrix_test_module4();
    return 1;
}
